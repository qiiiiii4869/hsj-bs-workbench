"""Raw-channel preparation for StripChart, independent of FFT/filter state."""
import numpy as np
from analysis import envelope
from data_reader import open_reader
from sel_reader import SelError,sha256
from interaction import register


def prepare_strip(body,resolve):
    path,name=resolve(body.get('file_id'));reader=open_reader(path)
    condition=body.get('condition',0)
    if isinstance(condition,bool) or not isinstance(condition,int) or not 0<=condition<len(reader.conditions):raise SelError('长图工况不存在')
    channels=body.get('channels',[])
    if not isinstance(channels,list) or not channels:raise SelError('请至少选择一个信号')
    if any(isinstance(c,bool) or not isinstance(c,int) or not 0<=c<reader.channel_count for c in channels):raise SelError('长图通道索引无效')
    start=float(body.get('start',0));end=float(body.get('end',0));width=int(body.get('width',2000))
    if not np.isfinite(start+end) or not 0<=start<end:raise SelError('长图起始时间须非负且小于结束时间')
    if not 300<=width<=22500:raise SelError('绘图宽度超出支持范围')
    digest=sha256(path)
    if body.get('file_sha256') and body['file_sha256']!=digest:raise SelError('原文件内容已改变，请重新创建长图，不能沿用旧配置指向其他数据')
    curves=[]
    for channel in dict.fromkeys(channels):
        raw=reader.read(condition,channel);rate=int(reader.rates[channel]);lo=min(len(raw),int(np.ceil(start*rate)));hi=min(len(raw),int(np.floor(end*rate))+1)
        values=raw[lo:hi];times=np.arange(lo,hi,dtype=np.float64)/rate
        # The legacy graph draws samples through endp inclusively and clips the
        # result to its drawing area. No unit conversion, resampling or filtering.
        snapshot=dict(engine_version='hsj-strip-raw-v1',source=dict(sha256=digest,condition=reader.conditions[condition],channel=reader.channels[channel]),
                      parameters=dict(start_index=lo,end_index_exclusive=hi,window='strip-raw',remove_mean=False,filters=[]))
        if len(values):register(snapshot,(times,values,np.empty(0),np.empty(0)))
        curves.append(dict(channel=channel,name=reader.channels[channel]['name'],unit=reader.channels[channel]['unit'],sample_rate=rate,
                           count=len(values),total_count=len(raw),duration=len(raw)/rate,minimum=float(np.min(raw)),maximum=float(np.max(raw)),
                           points=envelope(times,values,min(12000,max(1200,width*2))) if len(values) else [],
                           analysis_id=snapshot.get('analysis_id'),analysis_signature=snapshot.get('analysis_signature')))
    return dict(file_id=body['file_id'],filename=name,file_sha256=digest,condition=reader.conditions[condition],start=start,end=end,curves=curves)
