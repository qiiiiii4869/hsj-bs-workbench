"""AddMobTrend/AddTrendWave migration with explicit legacy semantics."""
import re
import hashlib
import numpy as np
from analysis import ENGINE_VERSION, envelope, filter_signal
from interaction import register
from sel_reader import SelError

NAMES={'mean':'平均值','median':'中间值','effective':'有效值','peak_to_peak':'峰峰值'}


def create_trend(raw, sample_rate, analysis_values, filters, spec, source, keep_snapshot=True):
    if not isinstance(spec,dict):raise SelError('移动趋势参数无效')
    kind=spec.get('metric','mean');width=spec.get('width',10);step=spec.get('step',10)
    color=spec.get('color','#ff00ff');tail=spec.get('tail','desktop_strict');effective=spec.get('effective_mode','desktop')
    identifier=spec.get('id')
    if not isinstance(identifier,str) or not re.fullmatch(r'[A-Za-z0-9_-]{1,64}',identifier):raise SelError('移动趋势标识无效')
    if kind not in NAMES:raise SelError('移动趋势计算依据无效')
    if any(isinstance(v,bool) or not isinstance(v,int) or v<=0 for v in (width,step)):
        raise SelError('窗口宽度、步长须为正整数，单位为采样点')
    if not isinstance(color,str) or not re.fullmatch(r'#[0-9a-fA-F]{6}',color):raise SelError('趋势颜色无效')
    if tail not in ('desktop_strict','complete') or effective not in ('desktop','window_ac'):
        raise SelError('移动趋势计算口径无效')
    if width>len(raw) or step>=len(raw):raise SelError('窗口宽度不得超过通道点数，步长须小于通道点数')
    values,filter_records=filter_signal(raw,sample_rate,filters)
    # Original for(k=0; k<N-winS; k+=winS), then PlotY at winW/Fs.
    starts=np.arange(0,len(values)-step,step,dtype=np.int64)
    if not len(starts):raise SelError('当前参数没有可计算的移动窗口')
    incomplete=starts+width>len(values)
    if np.any(incomplete):
        if tail=='desktop_strict':
            raise SelError('原版循环会产生不足一窗的尾部数据；请调整宽度/步长，或明确选择“仅完整窗口”，不自动补零')
        starts=starts[~incomplete]
    if not len(starts):raise SelError('当前参数没有完整移动窗口')
    if kind=='effective' and effective=='desktop':
        if width>len(analysis_values):raise SelError('原版有效值读取当前分析数据开头，窗口宽度不能超过当前分析点数')
        # AddTrendWave sets m_mean_temp=0, then uses newData[i], not TrendData[i].
        value=float(np.sqrt(np.mean(analysis_values[:width]**2))*2*1.41421356)
        output=np.full(len(starts),value)
        method='原版源码：当前分析区间前 W 点标准 RMS × 2√2，各移动点使用同一值'
    elif kind in ('mean','effective'):
        sums=np.r_[0.,np.cumsum(values,dtype=np.float64)]
        means=(sums[starts+width]-sums[starts])/width
        if kind=='mean':output=means;method='每窗口平均值'
        else:
            squares=np.r_[0.,np.cumsum(values*values,dtype=np.float64)]
            output=np.sqrt(np.maximum(0,(squares[starts+width]-squares[starts])/width-means**2))*2*1.41421356
            method='显式新口径：每窗口去均值 RMS × 2√2'
    else:
        windows=np.lib.stride_tricks.sliding_window_view(values,width)
        output=np.empty(len(starts));batch=max(1,1_000_000//width)
        for lo in range(0,len(starts),batch):
            hi=min(len(starts),lo+batch);chunk=windows[starts[lo:hi]]
            output[lo:hi]=np.median(chunk,axis=1) if kind=='median' else np.ptp(chunk,axis=1)
        method='每窗口中间值' if kind=='median' else '每窗口最大值减最小值（100%）'
    if not np.all(np.isfinite(output)):raise SelError('移动趋势计算超出数值范围')
    times=(starts+width)/sample_rate
    settings=dict(metric=kind,width=width,step=step,tail=tail,effective_mode=effective if kind=='effective' else None,
                  method=method,sample_rate=sample_rate,time_origin='(window_start+width)/Fs',filters=filter_records)
    if kind=='effective' and effective=='desktop':
        settings['reference_sha256']=hashlib.sha256(analysis_values[:width].tobytes()).hexdigest()
    snapshot=dict(engine_version=ENGINE_VERSION,source=source,parameters=dict(start_index=0,end_index_exclusive=len(values),
                  sample_index_stride=step,window='moving-trend',remove_mean=False,filters=filter_records,trend=settings))
    if keep_snapshot:register(snapshot,(times,output,np.empty(0),np.empty(0)))
    curve=dict(key='trend-'+identifier,channel=dict(source['channel'],name=f"{source['channel']['name']} · {NAMES[kind]}趋势"),
               color=color,visible=spec.get('visible',True),is_trend=True,start=float(times[0]),end=float(times[-1]),count=len(output),
               processing=method,trend=settings,waveform=envelope(times,output),analysis_id=snapshot.get('analysis_id'),analysis_signature=snapshot.get('analysis_signature'))
    if not isinstance(curve['visible'],bool):raise SelError('趋势显隐参数无效')
    return curve,(times,output)
