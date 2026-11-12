"""Specialized analyses. Source equations are recorded in 专项模块迁移说明.md."""
import io
import math
import json
import numpy as np
from scipy.signal import correlate
from stable_filter import apply_butterworth
from openpyxl import Workbook
from analysis import envelope, filter_signal, confidence_pp, analyze
from data_reader import open_reader
from sel_reader import SelError


def number(body, key, default=None):
    try:
        value=float(body.get(key,default))
    except (ValueError,TypeError):
        raise SelError(f'{key} 必须为数值')
    if not math.isfinite(value):raise SelError(f'{key} 必须为有限数值')
    return value


def vector(value, count=None):
    try: v=np.asarray(value,dtype=np.float64)
    except (ValueError,TypeError):raise SelError('数据表含非数值')
    if v.ndim!=1 or not len(v) or not np.all(np.isfinite(v)) or count is not None and len(v)!=count:raise SelError('数据数量或数值无效')
    return v


def channel(reader, body, key='channel'):
    value=body.get(key)
    if isinstance(value,bool) or not isinstance(value,int) or not 0<=value<reader.channel_count:raise SelError(f'请选择有效的 {key} 通道')
    return value


def selection(body,resolve):
    path,name=resolve(body.get('file_id'));reader=open_reader(path)
    condition=body.get('condition')
    if isinstance(condition,bool) or not isinstance(condition,int) or not 0<=condition<len(reader.conditions):raise SelError('请选择有效工况')
    return reader,condition,name


def curve(name,points,color='#0000ff',**extra):
    return dict(name=name,points=points,color=color,visible=True,**extra)


def orbit_filter(values,rate,chain,legacy=True):
    """AxisOrbit.cpp uses its own prefix and legacy NI argument mapping."""
    if not isinstance(chain,list):raise SelError('轴心轨迹滤波记录无效')
    data=np.asarray(values,dtype=np.float64)
    for spec in chain:
        if not isinstance(spec,dict):raise SelError('轴心轨迹滤波参数无效')
        kind=spec.get('type');order=spec.get('order',5)
        if kind not in ('lowpass','highpass','bandpass','bandstop') or isinstance(order,bool) or not isinstance(order,int) or not 1<=order<=100:raise SelError('轴心滤波类型或阶数无效')
        low=number(spec,'low',10);high=number(spec,'high',100)
        actual_order=5 if legacy and kind in ('bandpass','bandstop') else order
        cutoff=[low] if kind=='lowpass' else [low if legacy else high] if kind=='highpass' else [low,float(order) if legacy else high]
        if any(not 0<f<rate/2 for f in cutoff) or len(cutoff)==2 and cutoff[0]>=cutoff[1]:
            raise SelError('原版轴心滤波调用得到的截止频率无效；可改用“按填写参数”口径' if legacy else '截止频率须位于(0,Fs/2)，且下限小于上限')
        data=apply_butterworth(data,rate,kind,actual_order,cutoff,min(len(data),3072))
    return data


def pair(body,resolve):
    reader,condition,name=selection(body,resolve)
    cx,cy=channel(reader,body,'x_channel'),channel(reader,body,'y_channel')
    rx,ry=int(reader.rates[cx]),int(reader.rates[cy])
    rawx,rawy=reader.read(condition,cx),reader.read(condition,cy)
    orbit=body.get('module')=='orbit'
    if orbit:
        if body.get('orbit_filter_mode','desktop') not in ('desktop','corrected'):raise SelError('轴心滤波口径无效')
        legacy=body.get('orbit_filter_mode','desktop')=='desktop'
        x=orbit_filter(rawx,ry if legacy else rx,body.get('x_filters',[]),legacy)
        y=orbit_filter(rawy,ry,body.get('y_filters',[]),legacy)
    else:
        x,_=filter_signal(rawx,rx,body.get('x_filters',[]));y,_=filter_signal(rawy,ry,body.get('y_filters',[]))
    samples=not orbit and body.get('selection_units')=='samples'
    duration=min(len(x),len(y))/rx if orbit else len(x)/rx
    start=number(body,'start',0)/(rx if samples else 1);end=number(body,'end',duration*rx if samples else duration)/(rx if samples else 1)
    if not 0<=start<end<=duration:raise SelError('分析时间超出通道范围')
    ystart=number(body,'y_start',start*ry if samples else start)/(ry if samples else 1) if body.get('module')=='correlation' else start
    yend=number(body,'y_end',len(y) if samples else len(y)/ry)/(ry if samples else 1) if body.get('module')=='correlation' else end
    yr=rx if orbit else ry
    if not 0<=ystart<yend<=len(y)/yr:raise SelError('Y 通道分析时间超出范围')
    xlo=int(number(body,'start',0)) if samples else int(np.ceil(start*rx))
    xhi=int(number(body,'end',len(x))) if samples else int(np.floor(end*rx))
    ylo=int(number(body,'y_start',xlo)) if samples else int(np.ceil(ystart*yr))
    yhi=int(number(body,'y_end',len(y))) if samples else int(np.floor(yend*yr))
    x=x[xlo:xhi];y=y[ylo:yhi]
    if min(len(x),len(y))<2:raise SelError('选取范围数据不足')
    wave=[curve(reader.channels[cx]['name'],envelope(np.arange(len(x))+xlo if samples else np.arange(len(x))/rx+start,x)),curve(reader.channels[cy]['name'],envelope(np.arange(len(y))+ylo if samples else np.arange(len(y))/yr+ystart,y),'#d12c68')]
    if body['module']=='orbit':
        n=min(len(x),len(y))
        # Preserve every paired sample; uniform thinning can lose orbit spikes.
        points=np.column_stack((x[:n],y[:n])).tolist()
        main=curve('轴心轨迹',points)
        axes=[reader.channels[cx]['name']+' ('+reader.channels[cx]['unit']+')',reader.channels[cy]['name']+' ('+reader.channels[cy]['unit']+')']
    else:
        # NI Advanced Analysis Correlate: R[i] = sum_k x[k+n-1-i] y[k].
        # SciPy full output uses increasing lags; NI uses the reverse order.
        corr=correlate(x,y,mode='full',method='fft')[::-1]
        main=curve('互相关',envelope(np.arange(len(corr)),corr));axes=['相关序号','互相关值']
    return dict(title=name+' / '+reader.conditions[condition]['name'],axes=axes,curves=[main],waves=wave,
                rows=[['X 点数',len(x)],['Y 点数',len(y)],['X 采样率',rx],['Y 采样率',ry]],columns=['项目','数值'],intervals=dict(x=[xlo,xhi] if samples else [start,end],y=[ylo,yhi] if samples else [ystart,yend]),pairing='sample-index' if orbit else 'independent-sequences',
                wave_axes=[['采样序号' if samples else '时间(s)',reader.channels[cx]['unit']],['采样序号' if samples else '时间(s)',reader.channels[cy]['unit']]],
                source=dict(filename=name,condition=reader.conditions[condition],x_channel=reader.channels[cx],y_channel=reader.channels[cy]))


def trend(body,resolve):
    path,name=resolve(body.get('file_id'));reader=open_reader(path)
    ch,ref=channel(reader,body),channel(reader,body,'reference')
    conditions=[c for c in reader.conditions if c['type']==body.get('exam_type')]
    if not conditions:raise SelError('请选择试验类型')
    rows=[];details=[]
    for c in sorted(conditions,key=lambda c:c['name']):
        raw=reader.read(c['index'],ch);reference=reader.read(c['index'],ref);rate=int(reader.rates[ch])
        lo_index=0
        span=body.get('con',{}).get(str(c['index']))
        if span:
            lo,hi=map(float,span)
            if 0<=lo<hi:
                lo_index=int(np.ceil(lo*rate));raw=raw[lo_index:min(len(raw),int(np.floor(hi*rate)))]
        reported_end=lo_index+len(raw)
        if body.get('full_period'):
            keys=reader.key_phase_points(c['index'],ch)
            keys=keys[(keys>=lo_index)&(keys<lo_index+len(raw))]
            if len(keys)>=2:
                reported_end=int(keys[-1]);raw=raw[int(keys[0])-lo_index:int(keys[-1])-1-lo_index];lo_index=int(keys[0])
        elif body.get('legacy_report_interval'):
            raw=raw[:-1]
        if not len(raw):raise SelError('趋势区间无数据')
        mean=float(np.mean(raw))
        rows.append([c['name'],float(np.mean(reference)),mean,float(np.median(raw)),float(np.sqrt(np.mean((raw-mean)**2))*2*1.41421356),confidence_pp(raw,body.get('confidence',97))])
        if body.get('report_details'):
            analysis,_=analyze(raw,rate,confidence=body.get('confidence',97))
            details.append(dict(condition=c['name'],reference=float(np.mean(reference)),start=lo_index/rate,end=reported_end/rate,sample_rate=rate,count=len(raw),statistics=analysis['statistics'],peaks=analysis['peaks']))
    colors=['#0000ff','#008000','#ff00ff','#e03020'];names=['平均值','中间值','有效值',str(body.get('confidence',97))+'%峰峰值']
    return dict(title=name+' / '+reader.channels[ch]['name']+'趋势',axes=[reader.channels[ref]['name']+' ('+reader.channels[ref]['unit']+')',reader.channels[ch]['unit']],
                curves=[curve(n,[[r[1],r[i+2]] for r in rows],colors[i]) for i,n in enumerate(names)],rows=rows,columns=['工况','参考通道均值',*names],details=details)


def balance(body):
    a,p,b,q,w,t=[number(body,k) for k in ['before_amplitude','before_phase','after_amplitude','after_phase','trial_weight','trial_angle']]
    if a<0 or b<0 or w<=0:raise SelError('幅值须非负，试重须大于零')
    if any(not 0<=v<=360 for v in [p,q,t]):raise SelError('相位和试重角度须为 0–360 度')
    v0=a*np.exp(1j*np.deg2rad(p));v1=b*np.exp(1j*np.deg2rad(q));delta=v1-v0
    if abs(delta)<1e-15:raise SelError('试重前后矢量相同，无法计算影响系数')
    correction=-v0/(delta/(w*np.exp(1j*np.deg2rad(t))))
    weight,angle=float(abs(correction)),float(np.angle(correction,deg=True)%360)
    rows=[['配重重量',weight],['配重角度',angle],['影响系数幅值',float(abs(delta)/w)]]
    curves=[curve('原始振动',[[0,0],[float(v0.real),float(v0.imag)]]),curve('试重后振动',[[0,0],[float(v1.real),float(v1.imag)]],'#d12c68'),curve('配重矢量',[[0,0],[float(correction.real),float(correction.imag)]],'#008000')]
    if body.get('decompose'):
        d0,d1=number(body,'angle_1'),number(body,'angle_2')
        matrix=np.array([[math.cos(math.radians(d0)),math.cos(math.radians(d1))],[math.sin(math.radians(d0)),math.sin(math.radians(d1))]])
        if abs(np.linalg.det(matrix))<1e-12:raise SelError('两个分解角不能相同或相差180度')
        masses=np.linalg.solve(matrix,[correction.real,correction.imag])
        if np.any(masses< -1e-9):raise SelError('配重角度不在两个分解角的有效夹角内')
        rows.extend([['分解重量1',float(max(0,masses[0]))],['分解重量2',float(max(0,masses[1]))]])
    curves=curves[:2]+[curve('试重引起的振动变化',[[0,0],[float(delta.real),float(delta.imag)]],'#008000')]
    direction=-1 if body.get('clockwise') else 1
    rotation=number(body,'diagram_rotation',0)
    def weight_vector(mass,degrees):
        radians=math.radians(direction*degrees+rotation)
        return [[0,0],[mass*math.cos(radians),mass*math.sin(radians)]]
    return dict(title='动平衡计算',axes=['振动实部','振动虚部'],rows=rows,columns=['项目','结果'],curves=curves,weight=weight,angle=angle,
                plots=[dict(title='试重与配重位置',axes=['重量 X','重量 Y'],curves=[curve('试重块',weight_vector(w,t),'#d12c68'),curve('配重块',weight_vector(weight,angle),'#008000')])])


def balance_sample(body,resolve):
    reader,c,name=selection(body,resolve);ch=channel(reader,body);raw=reader.read(c,ch);rate=int(reader.rates[ch])
    keys=reader.key_phase_points(c,ch)
    start=number(body,'start',0);end=number(body,'end',len(raw)/rate)
    selected=keys[(keys>=start*rate)&(keys<=end*rate)]
    if len(selected)<2 or not 1<=len(selected)-1<=16:raise SelError('请选择含1至16个完整周期的键相区间')
    lo,hi=int(selected[0]),int(selected[-1]);data=raw[lo:hi-1]
    if len(data)<17:raise SelError('周期区间至少需要17点')
    n=2**int(math.log2(len(data)))
    interpolation_rate=number(body,'after_sampling_rate',rate) if body.get('side')=='before' else rate
    if interpolation_rate<=0:raise SelError('试重后采样率无效')
    values=np.interp(np.arange(n)*len(data)/n*interpolation_rate/rate,np.arange(len(data)),data,left=0,right=0)
    fft=np.fft.rfft(values);frequency=rate*(len(selected)-1)/(hi-lo);df=rate/len(data);idx=int(np.floor(frequency/df+.5));idx=min(idx,len(fft)-1)
    amp=float(abs(fft[idx])*2/n);phase=float(np.angle(fft[idx],deg=True)%360)
    spectrum=np.abs(fft[:-1])*2/n;spectrum[0]=0
    return dict(title=name+' / '+reader.channels[ch]['name'],axes=['时间(s)','幅值'],curves=[curve('选定周期',envelope(np.arange(len(data))/rate+lo/rate,data))],
                columns=['项目','结果'],rows=[['转速(r/min)',frequency*60],['峰峰值',float(np.ptp(data))],['转频双幅值',amp*2],['相位',phase]],amplitude=amp,phase=phase,
                sample_rate=rate,start=lo/rate,end=hi/rate,periods=len(selected)-1,
                plots=[dict(title='选定周期幅值频谱',axes=['频率(Hz)','幅值'],curves=[curve('频谱',envelope(np.arange(len(fft)-1)*df,spectrum))])])


def balance_preview(body,resolve):
    reader,c,name=selection(body,resolve);ch=channel(reader,body);values=reader.read(c,ch);rate=int(reader.rates[ch]);keys=reader.key_phase_points(c,ch)
    valid=keys[(keys>=0)&(keys<len(values))]
    from interaction import register
    from sel_reader import sha256
    result=dict(title=name+' / '+reader.channels[ch]['name'],axes=['时间(s)',reader.channels[ch]['unit']],columns=['项目','数值'],rows=[['采样率',rate],['点数',len(values)],['键相点数',len(valid)]],
                curves=[curve('原始波形',envelope(np.arange(len(values))/rate,values))],key_phases=[[float(k/rate),float(values[int(k)])] for k in valid],duration=len(values)/rate,sample_rate=rate,
                source=dict(filename=name,exam=reader.exam,condition=reader.conditions[c],channel=reader.channels[ch],sha256=sha256(reader.path)),
                minimum=float(np.min(values)),maximum=float(np.max(values)),
                engine_version='hsj-balance-preview-v1',parameters=dict(start_index=0,end_index_exclusive=len(values),window='raw',remove_mean=False,filters=[]))
    register(result,(np.arange(len(values))/rate,values,np.empty(0),np.empty(0)))
    return result


def balance_decompose(body):
    weight,angle,d0,d1=[number(body,key) for key in ['weight','angle','angle_1','angle_2']]
    if weight<0 or any(not 0<=v<=360 for v in [angle,d0,d1]):raise SelError('重量须非负，角度须在0–360度范围')
    a0,a1=map(math.radians,[d0,d1]);matrix=np.array([[math.cos(a0),math.cos(a1)],[math.sin(a0),math.sin(a1)]])
    if abs(np.linalg.det(matrix))<1e-12:raise SelError('分解角不能相同或相差180度')
    target=weight*np.array([math.cos(math.radians(angle)),math.sin(math.radians(angle))]);masses=np.linalg.solve(matrix,target)
    if np.any(masses<-1e-9):raise SelError('输入重量方向不在有效夹角内')
    return dict(title='独立配重分解',axes=['重量X','重量Y'],columns=['项目','重量','角度'],rows=[['原配重',weight,angle],['分解1',float(max(0,masses[0])),d0],['分解2',float(max(0,masses[1])),d1]],curves=[curve('原配重',[[0,0],target.tolist()]),curve('分解1',[[0,0],[float(masses[0]*math.cos(a0)),float(masses[0]*math.sin(a0))]],'#d12c68'),curve('分解2',[[0,0],[float(masses[1]*math.cos(a1)),float(masses[1]*math.sin(a1))]],'#008000')])


def jigger(body,resolve):
    if body.get('channels'):
        reader,c,name=selection(body,resolve);channels=body['channels']
        if not isinstance(channels,list) or len(channels)!=16:raise SelError('请选择两组各8个通道')
        second=body.get('condition_2')
        if isinstance(second,bool) or not isinstance(second,int) or not 0<=second<len(reader.conditions):raise SelError('请选择第二组工况')
        values=[float(np.mean(reader.read(c if index<8 else second,channel(reader,{'channel':ch})))) for index,ch in enumerate(channels)]
    else:values=vector(body.get('values'),16)
    first,second=np.asarray(values[:8]),np.asarray(values[8:]);alpha=np.arange(8)*np.pi/4
    matrix=np.zeros((16,12));matrix[:8,:8]=np.eye(8);matrix[8:,:8]=np.eye(8)
    matrix[:8,8]=-np.cos(alpha);matrix[:8,9]=-np.sin(alpha)
    matrix[8:,10]=-np.cos(alpha);matrix[8:,11]=-np.sin(alpha)
    matrix[8:10,10]=-np.cos(alpha[2:4]);matrix[8:10,11]=-np.sin(alpha[2:4])
    h=np.r_[first,second];h[8:10]=h[10:12]
    result=np.linalg.lstsq(matrix,h,rcond=None)[0];x,y=map(float,result[10:12])
    return dict(title='盘车试验',axes=['方位序号','间隙值'],curves=[curve('第一组',[[i+1,float(v)] for i,v in enumerate(first)]),curve('第二组',[[i+1,float(v)] for i,v in enumerate(second)],'#d12c68')],
                columns=['项目','数值'],rows=[['偏心X',x],['偏心Y',y],['偏心距',math.hypot(x,y)],['偏心角',math.degrees(math.atan2(y,x))+(0 if number(body,'previous_angle',0)>0 else 360)]],values=list(map(float,values)))


def airgap(body):
    radii=vector(body.get('values'))
    if not 3<=len(radii)<=1000:raise SelError('圆度拟合至少3点，最多1000点')
    angles=np.arange(len(radii))*2*3.1415926/len(radii)
    # Original m_points is LONG: preserve truncation before the fit.
    x=np.trunc(radii*np.cos(angles));y=np.trunc(radii*np.sin(angles))
    design=np.column_stack((x,y,np.ones(len(x))))
    if np.linalg.matrix_rank(design)<3:raise SelError('点位退化，不能拟合圆')
    a,b,c=np.linalg.lstsq(design,-x*x-y*y,rcond=None)[0];cx,cy=-a/2,-b/2;r=math.sqrt(max(0,(a*a+b*b)/4-c))
    t=np.linspace(0,2*np.pi,361)
    angle=float((360*math.atan2(cy,cx)/3.1415926+360) if math.atan2(cy,cx)<0 else 360*math.atan2(cy,cx)/3.1415926)
    radial=np.hypot(x,y)
    values=[('圆心x坐标',float(cx)),('圆心y坐标',float(cy)),('半径R',r),('偏心距',math.hypot(cx,cy)),('偏心角',angle)]
    text=''.join(f'{label} =\t{value:.6f};\r\n' for label,value in values)
    for title,data in [('原始数据',radii),('半径R',radial)]:
        text+='\r\n'+title+'\r\n----------------------\r\n'+''.join(f'{label} =\t{value:.6f};\r\n' for label,value in [('最大值',float(np.max(data))),('最小值',float(np.min(data))),('平均值',float(np.mean(data)))])
    return dict(title='最小二乘圆度拟合',axes=['X','Y'],curves=[curve('测点',np.column_stack((np.r_[x,x[0]],np.r_[y,y[0]])).tolist()),curve('拟合圆',np.column_stack((cx+r*np.cos(t),cy+r*np.sin(t))).tolist(),'#d12c68')],
                columns=['项目','结果'],rows=[['圆心X',float(cx)],['圆心Y',float(cy)],['半径',r],['偏心距',math.hypot(cx,cy)],['偏心角（原版360系数）',angle],['原始数据最大值',float(np.max(radii))],['原始数据最小值',float(np.min(radii))],['原始数据平均值',float(np.mean(radii))],['投影半径最大值',float(np.max(radial))],['投影半径最小值',float(np.min(radial))],['投影半径平均值',float(np.mean(radial))]],values=radii.tolist(),legacy_text=text,legacy_extension='oag')


def efficiency(body,resolve):
    path,name=resolve(body.get('file_id'));reader=open_reader(path)
    density,gravity,eta=[number(body,k,d) for k,d in [('density',1000),('gravity',9.8),('generator_efficiency',.98)]]
    if density<=0 or gravity<=0 or not 0<eta<=1:raise SelError('密度/重力加速度须为正数，发电机效率取0–1')
    method=body.get('flow_method','meter');head=body.get('head_method','pressure')
    if method not in ('meter','differential','high_low') or head not in ('pressure','head'):raise SelError('请选择有效测量方式')
    z=number(body,'elevation',0);fs=number(body,'head_factor',1);fq=number(body,'flow_factor',1);coefficient=number(body,'coefficient',1)
    conditions=[c for c in reader.conditions if c['type']==body.get('exam_type')]
    if not conditions:raise SelError('请选择试验类型')
    def mean(c,key):return float(np.mean(reader.read(c,channel(reader,body,key))))
    rows=[]
    for c in conditions:
        idx=c['index'];power=mean(idx,'power_channel')
        static=(mean(idx,'head_channel')+z)*fs if head=='head' else (abs(mean(idx,'inlet_channel')-mean(idx,'outlet_channel'))/(density*gravity)+z)*fs
        flow=mean(idx,'flow_channel') if method=='meter' else coefficient*math.sqrt(abs(mean(idx,'diff_channel') if method=='differential' else mean(idx,'high_channel')-mean(idx,'low_channel')))*fq
        dynamic=0
        if body.get('dynamic_head'):
            ain,aout=number(body,'inlet_area'),number(body,'outlet_area')
            if min(ain,aout)<=0:raise SelError('进出口面积须大于零')
            dynamic=(flow*flow/(ain*ain)-flow*flow/(aout*aout))/(2*gravity)
        total=static+dynamic
        if flow==0 or total==0:raise SelError(f"{c['name']} 流量或水头为零，无法计算效率")
        rows.append([c['name'],power,power/eta,static,dynamic,total,flow,abs((power/eta)/(density*gravity*flow*total))])
    return dict(title=name+'效率分析',axes=['有功功率','效率'],curves=[curve('效率',[[r[1],r[-1]] for r in rows])],columns=['工况','有功','水轮机出力','静水头','动水头','总水头','流量','效率'],rows=rows)


def compute_special(body,resolve):
    module=body.get('module')
    if module in ('orbit','correlation'):return pair(body,resolve)
    if module=='trend':return trend(body,resolve)
    if module=='balance':return balance(body)
    if module=='balance_sample':return balance_sample(body,resolve)
    if module=='balance_preview':return balance_preview(body,resolve)
    if module=='balance_decompose':return balance_decompose(body)
    if module=='jigger':return jigger(body,resolve)
    if module=='airgap':return airgap(body)
    if module=='efficiency':return efficiency(body,resolve)
    if module=='cam':
        from cam_legacy import compute
        return compute(body)
    raise SelError('专项模块不存在')


def dispatch(body,resolve):
    result=compute_special(body,resolve)
    result['request']=json.loads(json.dumps(body,ensure_ascii=False))
    result['module']=body.get('module')
    return result


def workbook(body):
    rows=body.get('rows');columns=body.get('columns')
    if not isinstance(rows,list) or not isinstance(columns,list) or len(rows)>100000:raise SelError('报表数据无效')
    book=Workbook();sheet=book.active;sheet.title='分析结果'
    for row in [columns,*rows]:
        if not isinstance(row,list):raise SelError('报表行数据无效')
        sheet.append([("'"+v if v.startswith(('=','+','-','@')) else v) if isinstance(v,str) else v for v in row])
    sheet.freeze_panes='A2'
    for col in sheet.columns:sheet.column_dimensions[col[0].column_letter].width=22
    out=io.BytesIO();book.save(out);return out.getvalue()
