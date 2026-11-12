"""Read extra raw waveforms without changing the primary channel's analysis."""
import re
import numpy as np
from analysis import ENGINE_VERSION, envelope
from interaction import register
from sel_reader import SelError


def read_overlays(reader, condition, main_channel, start, end, specs, source, keep_snapshot=True):
    if not isinstance(specs, list):
        raise SelError('叠加通道列表无效')
    used = {main_channel}
    curves = []
    for spec in specs:
        if not isinstance(spec, dict):
            raise SelError('叠加通道参数无效')
        index = spec.get('channel')
        color = spec.get('color', '#d13e89')
        visible = spec.get('visible', True)
        if isinstance(index, bool) or not isinstance(index, int) or not 0<=index<reader.channel_count:
            raise SelError('叠加通道不存在')
        if index in used:
            raise SelError('通道已存在，请勿重复添加')
        if not isinstance(color, str) or not re.fullmatch(r'#[0-9a-fA-F]{6}', color):
            raise SelError('曲线颜色无效')
        if not isinstance(visible, bool):
            raise SelError('曲线显隐参数无效')
        used.add(index)
        raw = reader.read(condition, index)
        channel = reader.channels[index]
        rate = channel['sample_rate']
        lo = max(0,int(np.ceil(start*rate)))
        hi = min(len(raw),int(np.ceil(end*rate)))
        if hi<=lo:
            raise SelError(f"{channel['name']} 在当前分析时段内没有数据")
        values = raw[lo:hi]
        times = np.arange(lo,hi,dtype=np.float64)/rate
        result = dict(engine_version=ENGINE_VERSION, source=dict(source, channel=channel),
                      parameters=dict(start_index=lo,end_index_exclusive=hi,window='rectangular',remove_mean=False,filters=[]))
        if keep_snapshot:
            register(result,(times,values,np.empty(0),np.empty(0)))
        curves.append(dict(key=f'channel-{index}', channel=channel, color=color,visible=visible,
                           start=lo/rate,end=hi/rate,count=hi-lo,processing='原始通道，未施加主通道滤波',
                           waveform=envelope(times,values),analysis_id=result.get('analysis_id'),
                           analysis_signature=result.get('analysis_signature')))
    return curves
