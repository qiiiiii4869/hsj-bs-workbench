"""Legacy Ni3DWaveView data preparation, independent of waveform-page FFT."""
import numpy as np
from analysis import envelope
from data_reader import open_reader
from sel_reader import SelError, sha256

COLORS=['#ff0000','#004000','#0000ff','#ff00ff','#808000','#ff8000','#0040ff','#00ff00']


def prepare_waterfall(body, resolve):
    path, name = resolve(body.get('file_id'))
    reader = open_reader(path)
    mode = body.get('mode', 'spectrum')
    if mode not in ('spectrum', 'wave'):
        raise SelError('瀑布图类型无效')
    channel, reference = body.get('channel', 0), body.get('reference', 0)
    if any(isinstance(v, bool) or not isinstance(v, int) or not 0 <= v < reader.channel_count for v in (channel, reference)):
        raise SelError('瀑布图通道索引无效')
    conditions = [c for c in reader.conditions if c['type'] == body.get('exam_type', reader.conditions[0]['type'])]
    if not conditions:
        raise SelError('试验类型不存在')
    # CT3DFreqDataSet visits all parameters, retaining the first parameter's
    # key-phase vector in its shared member. std::map then orders curve names.
    shared_keys = reader.key_phase_points(conditions[0]['index'], channel) if mode == 'spectrum' else []
    curves = []
    for number, condition in enumerate(sorted(conditions, key=lambda c: c['name'])):
        raw = reader.read(condition['index'], channel)
        ref = reader.read(condition['index'], reference)
        if len(raw) < 4 or not len(ref):
            raise SelError(f"{condition['name']} 数据不足")
        n = len(raw)
        rate = int(reader.rates[channel])
        start, stop = 0, n
        if mode == 'spectrum':
            if len(shared_keys):
                keys = np.asarray(shared_keys)
                keys = keys[(keys >= 0) & (keys <= n-1)]
                if len(keys) < 2:
                    raise SelError(f"{condition['name']} 无法按原版共享键相取整周期")
                start, stop = int(keys[0]), int(keys[-1])-1
            values = raw[start:stop]
            if len(values) < n//2 or len(values) < 2:
                raise SelError(f"{condition['name']} 原版 FFT 索引超出整周期数据，不能生成有效瀑布谱")
            # Original SetFFTData uses original N for frequency and amplitude,
            # despite GetFullPeriodData truncating the FFT input to M samples.
            transformed = np.fft.fft(values)
            z = np.abs(transformed[:n//2]) * (2/n)
            z[0] = 0
            y = np.arange(len(z), dtype=np.float64) * rate/n
        else:
            z = raw
            y = np.arange(n, dtype=np.float64)/rate
        curves.append(dict(condition=condition['index'], name=condition['name'], reference=float(np.mean(ref)),
                           color=COLORS[number % len(COLORS)], visible=True,
                           points=envelope(y,z,6000), minimum=float(np.min(z)), maximum=float(np.max(z)),
                           count=len(z), sample_rate=rate, original_count=n, fft_count=stop-start,
                           start_index=start, end_index_exclusive=stop, y_max=float(y[-1])))
    return dict(schema='hsj-waterfall-v1', mode=mode, file_id=body['file_id'], filename=name, sha256=sha256(path),
                exam=reader.exam, exam_type=conditions[0]['type'], channel=reader.channels[channel],
                reference_channel=reader.channels[reference], curves=curves,
                title=f"{reader.exam}，{conditions[0]['type']}，{reader.channels[channel]['name']}三维{'频谱' if mode=='spectrum' else '波形'}瀑布图",
                axes=[reader.channels[reference]['name']+' ('+reader.channels[reference]['unit']+')',
                      '频率 (Hz)' if mode=='spectrum' else '时间 (s)',
                      '幅值 ('+reader.channels[channel]['unit']+')'],
                convention='Ni3DWaveView-v1: shared first-condition keys; [int(first),int(last)-1); original-N FFT scaling; no resampling')
