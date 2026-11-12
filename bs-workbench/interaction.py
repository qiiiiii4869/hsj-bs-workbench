"""Immutable full-resolution analysis snapshots for cursor and annotation reads."""
from collections import OrderedDict
import hashlib
import json
import secrets
import threading
import numpy as np
from analysis import envelope
from sel_reader import SelError

_snapshots = OrderedDict()
_lock = threading.Lock()
_limit = 128 * 1024 * 1024


def register(result, arrays):
    p, source = result['parameters'], result['source']
    identity = dict(engine=result['engine_version'], sha256=source['sha256'],
                    condition=source['condition']['sort_code'], channel=source['channel']['index'],
                    start=p['start_index'], end=p['end_index_exclusive'],
                    window=p['window'], remove_mean=p['remove_mean'], filters=p['filters'])
    if 'trend' in p:identity['trend']=p['trend']
    signature = hashlib.sha256(json.dumps(identity, sort_keys=True, ensure_ascii=False).encode()).hexdigest()
    identifier = secrets.token_urlsafe(24)
    # Copy interval data, so a short selected interval does not retain a huge parent array.
    copied = tuple(np.array(v, dtype=np.float64, copy=True) for v in arrays)
    size = sum(v.nbytes for v in copied)
    with _lock:
        while _snapshots and (len(_snapshots)>=64 or sum(v['size'] for v in _snapshots.values())+size>_limit):
            _snapshots.popitem(last=False)
        _snapshots[identifier] = dict(arrays=copied, size=size, signature=signature,
                                    start_index=p.get('wave_start_index',p['start_index']),stride=p.get('sample_index_stride',1))
    result['analysis_id'] = identifier
    result['analysis_signature'] = signature
    wave_identity={k:v for k,v in identity.items() if k not in ('start','end','window','remove_mean')}
    result['wave_signature']=hashlib.sha256(json.dumps(wave_identity,sort_keys=True,ensure_ascii=False).encode()).hexdigest()


def _number(body, key, default=None):
    value = float(body.get(key, default))
    if not np.isfinite(value):
        raise SelError('图表坐标必须是有限数值')
    return value


def read_chart(body):
    with _lock:
        identifier = body.get('analysis_id')
        if not isinstance(identifier, str) or identifier not in _snapshots:
            raise SelError('当前图表的数据快照已释放，请点击“开始分析”重新读取')
        snapshot = _snapshots[identifier]
        _snapshots.move_to_end(identifier)
    plot = body.get('plot')
    if plot not in ('wave', 'spectrum'):
        raise SelError('图表类型无效')
    x, y = snapshot['arrays'][:2] if plot=='wave' else snapshot['arrays'][2:]
    if not len(x):
        raise SelError('当前图表没有可读取的数据')
    action = body.get('action', 'nearest')

    def point(i):
        return dict(x=float(x[i]), y=float(y[i]), index=int(i),
                    sample_index=int(snapshot['start_index']+i*snapshot['stride']) if plot=='wave' else None,
                    exact=True)

    response = dict(analysis_id=identifier, signature=snapshot['signature'], plot=plot)
    if action=='index':
        index = body.get('index')
        if isinstance(index, bool) or not isinstance(index, int) or not 0<=index<len(x):
            raise SelError('采样点或频谱序号超出当前分析范围')
        return dict(response, point=point(index))
    lo = _number(body, 'x_min', x[0])
    hi = _number(body, 'x_max', x[-1])
    if hi<lo:
        raise SelError('图表坐标范围无效')
    begin, end = int(np.searchsorted(x, lo, side='left')), int(np.searchsorted(x, hi, side='right'))
    if begin>=end:
        raise SelError('当前图表范围没有采样点')
    if action=='detail':
        return dict(response, data=envelope(x[begin:end], y[begin:end]))
    if action=='extrema':
        return dict(response, minimum=point(begin+int(np.argmin(y[begin:end]))),
                    maximum=point(begin+int(np.argmax(y[begin:end]))))
    if action!='nearest':
        raise SelError('图表读取操作无效')
    target = _number(body, 'x')
    if 'y' not in body:
        index = int(np.searchsorted(x[begin:end], target))+begin
        index = min(end-1, index)
        if index>begin and abs(x[index-1]-target)<abs(x[index]-target):
            index -= 1
    else:
        target_y = _number(body, 'y')
        y_min, y_max = _number(body, 'y_min'), _number(body, 'y_max')
        width, height = _number(body, 'width', 800), _number(body, 'height', 250)
        if hi<=lo or y_max<=y_min or not 0<width<=10000 or not 0<height<=10000:
            raise SelError('图表尺寸或坐标范围无效')
        distance = ((x[begin:end]-target)*width/(hi-lo))**2 + ((y[begin:end]-target_y)*height/(y_max-y_min))**2
        index = begin+int(np.argmin(distance))
    return dict(response, point=point(index))


def annotation_records(body):
    records = body.get('annotations', [])
    if not isinstance(records, list):
        raise SelError('标注记录格式无效')
    cleaned = []
    for row in records:
        if not isinstance(row, dict) or row.get('plot') not in ('wave','spectrum'):
            raise SelError('标注图表无效')
        text = row.get('text')
        if not isinstance(text, str) or not text.strip() or len(text)>200:
            raise SelError('标注文字须为 1–200 个字符')
        cleaned.append(dict(plot=row['plot'], text=text.strip(), x=_number(row,'x'), y=_number(row,'y'),
                            dx=_number(row,'dx',.03), dy=_number(row,'dy',-.1),
                            curve=str(row.get('curve','main'))[:80],unit=str(row.get('unit',''))[:64],
                            channel_name=str(row.get('channel_name',''))[:200],
                            analysis_signature=str(row.get('analysis_signature',''))[:128]))
    return cleaned
