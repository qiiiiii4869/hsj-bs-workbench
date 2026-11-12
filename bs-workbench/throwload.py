"""Separate throw-load trend and two-annotation workflow (W-14/ID_Annotations)."""
import numpy as np
from analysis import filter_signal
from moving_trend import create_trend
from sel_reader import SelError


def compute_throw(raw, rate, filters, spec, source):
    width=spec.get('width',50);base=float(spec.get('base',0));final=float(spec.get('final',0));rising=spec.get('rising',True)
    if isinstance(width,bool) or not isinstance(width,int) or not 1<=width<=10000:raise SelError('甩负荷窗口宽度须为 1–10000 点的整数')
    if not isinstance(rising,bool) or not np.isfinite(base) or not np.isfinite(final):raise SelError('甩负荷参数无效')
    if not width/rate<=base<final<=len(raw)/rate:raise SelError('基准时刻须不小于窗口宽度/Fs，且基准时刻小于最终时刻、不超出通道范围')
    trend_spec=dict(id='throwload',metric='mean',width=width,step=1,color='#ff00ff',visible=True,tail='complete')
    curve,(times,values)=create_trend(raw,rate,np.empty(0),filters,trend_spec,source)
    begin=int(np.ceil(base*rate))-width;end=int(np.floor(final*rate))-width
    if not 0<=begin<end<=len(values):raise SelError('该时段超出完整甩负荷趋势范围，请缩短最终时刻或调整窗口')
    offset=int(np.argmax(values[begin:end]) if rising else np.argmin(values[begin:end]))
    index=begin+offset;baseline=float(values[begin]);value=float(values[index]);percent=100*abs((value-baseline)/baseline) if baseline else 0.
    curve['key']='throwload';curve['is_throwload']=True;curve['trend_request']=trend_spec
    curve['color']=spec.get('color','#ff00ff');curve['visible']=spec.get('visible',True)
    notes=[]
    for text,i in [(f'基准值：{baseline:.2f}',begin),(f"{'最大值' if rising else '最小值'}：{value:.2f}；{'上升率' if rising else '下降率'}：{percent:.2f}%",index)]:
        notes.append(dict(text=text,x=float(times[i]),y=float(values[i]),index=i,sample_index=i,curve='throwload',unit=source['channel']['unit'],channel_name='甩负荷趋势',analysis_signature=curve['analysis_signature'],dx=.03,dy=-.12))
    return dict(curve=curve,annotations=notes,base=dict(x=float(times[begin]),y=baseline),extreme=dict(x=float(times[index]),y=value),percent=percent,rising=rising,width=width)
