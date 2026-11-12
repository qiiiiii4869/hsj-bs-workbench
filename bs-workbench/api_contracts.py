"""Explicit API documentation contracts. These describe, not replace, validation.

Keep request variants, field units, examples and response structures together;
test_api_docs.py checks route coverage and the HTTP contract runner checks examples.
"""
from copy import deepcopy
from api_examples import CAM_TEXT


def obj(properties=None, required=(), **extra):
    return dict(type='object', properties=properties or {}, additionalProperties=True,
                **({'required': list(required)} if required else {}), **extra)


def ref(name):return {'$ref': '#/components/schemas/'+name}
def array(items, **extra):return dict(type='array', items=items, **extra)
def string(description='', **extra):return dict(type='string', **({'description': description} if description else {}), **extra)
def number(description='', **extra):return dict(type='number', **({'description': description} if description else {}), **extra)
def integer(description='', **extra):return dict(type='integer', **({'description': description} if description else {}), **extra)
def boolean(description='', **extra):return dict(type='boolean', **({'description': description} if description else {}), **extra)
def enum(values, **extra):return string(enum=list(values), **extra)
def nullable(schema):return {'anyOf':[schema,{'type':'null'}]}

FILE=string('从 bootstrap 的 samples/imported 获取文件ID；sample-0通常为电厂变负荷.SEL。', examples=['sample-0'])
INDEX=integer('从0开始的索引；以 /api/open 的实际结果为准。', minimum=0)
COLOR=string('CSS十六进制颜色。',pattern='^#[0-9a-fA-F]{6}$',examples=['#0000ff'])
POINT=array(number(),minItems=2,maxItems=2,description='[横坐标,纵坐标]；全览曲线可能经过保留极值的抽样。')
POINTS=array(POINT)
ROW=array({})
ROWS=array(ROW)
FORMAT=enum(['png','jpg','bmp','gif','tiff'],default='png')
OFFICE=enum(['xlsx','docx','xls','doc'],description='XLSX/DOCX不依赖Office；真实二进制XLS/DOC需要本机安装Excel/Word。')
CON={'type':'object','additionalProperties':POINT,'description':'CON：工况index字符串 → [起始秒数,终止秒数]。'}
SCHEMAS={}


filter_common={'type':enum(['lowpass','highpass','bandpass','bandstop']), 'order':integer('原型阶数。高阶及极端截止的可靠性受数值精度限制；不是与NI逐位等价承诺。',minimum=1,maximum=100,default=5),
               'low':number('低通截止或频带下限Hz，严格大于0且小于Fs/2。',exclusiveMinimum=0),
               'high':number('高通截止或频带上限Hz，严格大于0且小于Fs/2。',exclusiveMinimum=0)}
SCHEMAS['Filter']=dict(oneOf=[obj({**filter_common,'type':enum([kind])},['type',*fields]) for kind,fields in [('lowpass',['low']),('highpass',['high']),('bandpass',['low','high']),('bandstop',['low','high'])]],description='滤波按数组顺序累计应用；带通/带阻low<high。原因果单向与前补规则保留。')
SCHEMAS['Overlay']=obj({'channel':INDEX,'color':COLOR,'visible':boolean(default=True)},['channel'])
SCHEMAS['MovingTrend']=obj({'id':string(pattern='^[A-Za-z0-9_-]{1,64}$'),'metric':enum(['mean','median','effective','peak_to_peak'],default='mean'),
    'width':integer('窗口宽度，采样点，不是秒。',minimum=1,default=10),'step':integer('窗口步长，采样点，须小于原通道点数。',minimum=1,default=10),
    'tail':enum(['desktop_strict','complete'],default='desktop_strict',description='desktop_strict遇不足整窗报错；complete仅取完整窗。'),
    'effective_mode':enum(['desktop','window_ac'],default='desktop',description='desktop保留原newData前W点RMS的重复值；window_ac为显式逐窗新口径。'),
    'color':COLOR,'visible':boolean(default=True)},['id'])
SCHEMAS['Annotation']=obj({'plot':enum(['wave','spectrum']),'text':string(minLength=1,maxLength=200),'x':number('图上X坐标。'),'y':number('图上Y坐标。'),
    'dx':number('相对于图宽的文字偏移。',default=.03),'dy':number('相对于图高的文字偏移。',default=-.1),'curve':string(default='main'),
    'unit':string(),'channel_name':string(),'analysis_signature':string('保存分析签名用于来源核对。')},['plot','text','x','y'])
SCHEMAS['ThrowSpec']=obj({'width':integer('均值窗口采样点数。',minimum=1,maximum=10000,default=50),
    'base':number('基准秒数，须>=width/Fs。'),'final':number('终止秒数，须>base且位于完整趋势内。'),'rising':boolean(default=True),'color':COLOR,'visible':boolean(default=True)},['base','final'])
COMMON={'file_id':FILE,'condition':dict(INDEX,default=0),'channel':dict(INDEX,default=0),'start':number('请求起始秒数。',minimum=0,default=0),
    'end':nullable(number('请求终止秒数，null或省略取全通道。')),'window':enum(['rectangular','hann'],default='rectangular'),
    'confidence':integer('离散排序下标定义的峰峰值百分比，不是插值分位数。',enum=[95,97,100],default=97),
    'full_period':boolean('省略时，有两个有效键相点则取整周期；true必须有有效键相。普通固定时段请显式false。'),
    'remove_mean':boolean('仅控制FFT前去均值。',default=False),'filters':array(ref('Filter'),default=[]),'include_original':boolean('滤波时同时返回原始对照。',default=False),
    'title':string('图表标题，未传入则使用通道名。',minLength=1,maxLength=200),'main_color':COLOR}
DECORATIONS={'annotations':array(ref('Annotation'),default=[]),'overlays':array(ref('Overlay'),default=[]),'trends':array(ref('MovingTrend'),default=[]),
             'throw_spec':nullable(ref('ThrowSpec')),'key_visible':boolean('图像/报表是否显示键相点。',default=False)}
IMAGE_FIELDS={'format':FORMAT,'width':integer('像素，400–8000；总像素不超过2400万。',minimum=400,maximum=8000,default=1400),
              'height':integer('像素，300–8000。',minimum=300,maximum=8000,default=950),'plot':enum(['both','wave','spectrum'],default='both'),
              'frequency_range':POINT}
BASE_EXAMPLE=dict(file_id='sample-0',condition=0,channel=0,start=0,end=4,full_period=False,confidence=97,filters=[])
TREND_EXAMPLE=dict(id='mean-1',metric='mean',width=128,step=128,tail='complete')

# Return data; extensible fields are deliberate because plugins and modules add
# their own source/plot metadata. Required fields still describe the real shape.
SCHEMAS['Error']=obj({'error':string()},['error'])
SCHEMAS['FileEntry']=obj({'id':string(),'name':string(),'size':integer(minimum=0)},['id','name'])
SCHEMAS['Condition']=obj({'index':INDEX,'type':string(),'name':string(),'sort_code':INDEX,
    'begin_timestamp':integer('记录起始Unix秒。'),'end_timestamp':integer('记录结束Unix秒。'),'sort_start':integer('原分拣区间起始点字段。'),'sort_end':integer('原分拣区间结束点字段。'),
    'action_start':integer(),'action_end':integer(),'sort_valid':boolean(),'reference_channel_id':integer(),'reference_channel_name':string()},['index','type','name','sort_code'])
SCHEMAS['Channel']=obj({'index':INDEX,'id':INDEX,'name':string(),'unit':string(),'sample_rate':integer('Hz。',minimum=1),'fft_enabled':boolean(),'divisor':integer(),
    'a':nullable(number()),'b':nullable(number()),'count':integer(),'stored_count':integer(),'full_period_available':boolean(),'key_phase_count':integer(),'desktop_start':number(),'desktop_end':number()},['index','name','unit','sample_rate','fft_enabled'])
SCHEMAS['FileMetadata']=obj({'version':{'type':['integer','string']},'exam':string(),'header_size':integer(),'size':integer(),'file_id':string(),'filename':string(),'sha256':string(),'color':COLOR,
    'channels':array(ref('Channel')),'conditions':array(obj({**SCHEMAS['Condition']['properties'],'channels':array(ref('Channel'))},['index','type','name','sort_code','channels'])),
    'properties':obj({'path':string(),'created':string(),'modified':string(),'accessed':string()}),'sampling_configuration':obj({'sample_rate':integer(),'deal_span':integer(),'sort_count':integer(),'sort_length':integer(),'key_index':integer(),'velocity_index':integer(),'set_id':string()}),'ui_states':array(integer())},['version','exam','size','channels','conditions','file_id','filename','sha256'])
SCHEMAS['Source']=obj({'file_id':string(),'filename':string(),'sha256':string(),'exam':string(),'condition':ref('Condition'),'channel':ref('Channel'),'title':string(),'total_count':integer(),'duration':number(),'color':COLOR})
SCHEMAS['Statistics']=obj({k:number(description) for k,description in [('max','最大值'),('min','最小值'),('mean','平均值'),('median','中间值'),('peak_to_peak','选中置信度的峰峰值'),('peak_to_peak_100','最大值减最小值'),('rms','标准RMS'),('ac_rms','去均值RMS'),('desktop_effective','原界面有效值：AC RMS×2×1.41421356')]},['max','min','mean','median','peak_to_peak','peak_to_peak_100','rms','ac_rms','desktop_effective'])
SCHEMAS['Peak']=obj({'frequency':number('Hz。'),'amplitude':number('单边峰值幅度。'),'phase_deg':number('0–360度。')},['frequency','amplitude','phase_deg'])
SCHEMAS['AnalysisParameters']=obj({**{k:v for k,v in COMMON.items() if k not in ('file_id','condition','channel','title','include_original','main_color')},
    'count':integer(),'sample_rate':integer(),'requested_start':number(),'requested_end':nullable(number()),'start_index':integer(),'end_index_exclusive':integer(),
    'key_phase_count':integer(),'delta_frequency':number(),'interval':string(),'fft_mode':string(),'fft_enabled':boolean(),
    'filters':array(obj({**filter_common,'cutoff':array(number()),'prefix_count':integer(),'implementation':string(),'phase':string(),'desktop_equivalence':string()}))},['start','end','count','sample_rate','confidence','filters','start_index','end_index_exclusive','delta_frequency'])
SCHEMAS['AnalysisResult']=obj({'engine_version':string(),'source':ref('Source'),'parameters':ref('AnalysisParameters'),'statistics':ref('Statistics'),'peaks':array(ref('Peak'),maxItems=8),
    'waveform':POINTS,'waveform_full':POINTS,'spectrum':POINTS,'original':obj({'statistics':ref('Statistics'),'parameters':ref('AnalysisParameters'),'waveform':POINTS,'waveform_full':POINTS,'spectrum':POINTS}),
    'analysis_id':string('服务端临时全分辨率快照ID；服务重启、64个快照或128MiB缓存逐出后失效。'),'analysis_signature':string(),'wave_signature':string(),
    'key_phase_points':POINTS,'annotations':array(ref('Annotation')),'overlays':array(obj()),'trends':array(obj()),'throwload':obj()},['engine_version','source','parameters','statistics','peaks','waveform','spectrum'])
SCHEMAS['SnapshotPoint']=obj({'x':number(),'y':number(),'index':integer(),'sample_index':nullable(integer()),'exact':boolean()},['x','y','index','sample_index','exact'])
SCHEMAS['ChartResult']=obj({'analysis_id':string(),'signature':string(),'plot':enum(['wave','spectrum']),'point':ref('SnapshotPoint'),'data':POINTS,'minimum':ref('SnapshotPoint'),'maximum':ref('SnapshotPoint')},['analysis_id','signature','plot'])
SCHEMAS['Curve']=obj({'name':string(),'points':POINTS,'color':COLOR,'visible':boolean()},['name','points'])
SCHEMAS['Plot']=obj({'title':string(),'axes':array(string(),minItems=2,maxItems=2),'curves':array(ref('Curve'))},['title','axes','curves'])
SCHEMAS['SpecialResult']=obj({'title':string(),'axes':array(string()),'curves':array(ref('Curve')),'rows':ROWS,'columns':array(string()),'module':string(),'request':obj(),
    'plots':array(ref('Plot')),'waves':array(ref('Curve')),'details':array(obj({'condition':string(),'reference':number(),'start':number(),'end':number(),'sample_rate':integer(),'count':integer(),'statistics':ref('Statistics'),'peaks':array(ref('Peak'))})),
    'weight':number(),'angle':number(),'amplitude':number(),'phase':number(),'sample_rate':integer(),'start':number(),'end':number(),'periods':integer(),
    'values':array(number()),'key_phases':POINTS,'duration':number(),'minimum':number(),'maximum':number(),'source':ref('Source'),'analysis_id':string(),'analysis_signature':string(),'wave_signature':string(),'engine_version':string(),'parameters':obj(),
    'intervals':obj({'x':POINT,'y':POINT}),'pairing':string(),'wave_axes':array(array(string())),'legacy_text':string('OCA/OAG原格式正文。'),'legacy_extension':enum(['oca','oag'])},['title','axes','curves','rows','columns','module','request'])
SCHEMAS['JobResult']=obj({'id':string(format='uuid'),'status':enum(['queued','running','completed','completed_with_errors','cancelled','failed','interrupted']),
    'done':integer(minimum=0),'total':integer(minimum=1),'current':string(),'errors':array(obj({'item':obj(),'error':string()}))},['id','status','done','total','current','errors'])
SCHEMAS['OverlayCurve']=obj({'key':string(),'channel':ref('Channel'),'color':COLOR,'visible':boolean(),'start':number(),'end':number(),'count':integer(),'processing':string(),'waveform':POINTS,'analysis_id':string(),'analysis_signature':string()})
SCHEMAS['TrendCurve']=obj({**SCHEMAS['OverlayCurve']['properties'],'is_trend':boolean(),'trend':obj(),'is_throwload':boolean(),'trend_request':ref('MovingTrend')})
SCHEMAS['ThrowResult']=obj({'curve':ref('TrendCurve'),'annotations':array(obj()),'base':obj({'x':number(),'y':number()},['x','y']),'extreme':obj({'x':number(),'y':number()},['x','y']),
    'percent':number(),'rising':boolean(),'width':integer()},['curve','annotations','base','extreme','percent','rising','width'])
SCHEMAS['StripResult']=obj({'file_id':string(),'filename':string(),'file_sha256':string(),'condition':ref('Condition'),'start':number(),'end':number(),'curves':array(obj({'channel':INDEX,'name':string(),'unit':string(),'sample_rate':integer(),'count':integer(),'total_count':integer(),'duration':number(),'minimum':number(),'maximum':number(),'points':POINTS,'analysis_id':string(),'analysis_signature':string()}))},['file_id','filename','file_sha256','condition','start','end','curves'])
SCHEMAS['WaterfallResult']=obj({'schema':string(),'mode':enum(['wave','spectrum']),'file_id':string(),'filename':string(),'sha256':string(),'exam':string(),'exam_type':string(),'channel':ref('Channel'),'reference_channel':ref('Channel'),
    'curves':array(obj({'condition':INDEX,'name':string(),'reference':number(),'color':COLOR,'visible':boolean(),'points':POINTS,'minimum':number(),'maximum':number(),'count':integer(),'sample_rate':integer(),'original_count':integer(),'fft_count':integer(),'start_index':integer(),'end_index_exclusive':integer(),'y_max':number()})),
    'title':string(),'axes':array(string(),minItems=3,maxItems=3),'convention':string()},['schema','mode','file_id','filename','sha256','curves','axes'])

SPECIAL_EXAMPLES={}
SPECIAL_NAMES=[]
selection={'file_id':FILE,'condition':INDEX,'channel':INDEX}
pair={'file_id':FILE,'condition':INDEX,'x_channel':INDEX,'y_channel':INDEX,'start':number('秒；correlation在selection_units=samples时为采样点。',default=0),
    'end':number('默认到序列末尾，单位同start。'),'x_filters':array(ref('Filter'),default=[]),'y_filters':array(ref('Filter'),default=[])}


def special(name, properties, required, example, description):
    key='Special_'+name;SPECIAL_NAMES.append(key)
    SCHEMAS[key]=obj({'module':enum([name]),**properties},['module',*required],description=description)
    SPECIAL_EXAMPLES[name]={'summary':description.split('。')[0],'value':dict(module=name,**example)}


special('orbit',{**pair,'orbit_filter_mode':enum(['desktop','corrected'],default='desktop')},['file_id','condition','x_channel','y_channel'],dict(file_id='sample-0',condition=0,x_channel=0,y_channel=1,start=0,end=2),
        '轴心轨迹。按采样序号配对；desktop保留原轴心滤波专用前补及高通/带通参数映射，corrected按填写参数。')
special('correlation',{**pair,'selection_units':enum(['seconds','samples'],default='seconds'),'y_start':number('Y独立起始位置。'),'y_end':number('Y独立结束位置。')},['file_id','condition','x_channel','y_channel'],dict(file_id='sample-0',condition=0,x_channel=0,y_channel=1,selection_units='samples',start=0,end=1024,y_start=0,y_end=768),
        '互相关分析。X和Y区间独立，返回未归一化相关和原NI顺序的采样序号。')
special('trend',{'file_id':FILE,'channel':INDEX,'reference':INDEX,'exam_type':string(),'confidence':COMMON['confidence'],'full_period':boolean(default=False),'con':CON,'report_details':boolean(default=False),'legacy_report_interval':boolean('非整周期报表保留原末尾减1点规则。',default=False)},
        ['file_id','channel','reference','exam_type'],dict(file_id='sample-0',channel=0,reference=13,exam_type='变负荷',confidence=97),'跨工况趋势。参考通道均值作X轴，平均/中间/原有效值/峰峰值作Y轴。')
balance_fields={'before_amplitude':number('配重前单幅值。',minimum=0),'before_phase':number('配重前相位，度。',minimum=0,maximum=360),
 'after_amplitude':number('试重后单幅值。',minimum=0),'after_phase':number('试重后相位，度。',minimum=0,maximum=360),'trial_weight':number('试重重量，须>0，双方单位一致。',exclusiveMinimum=0),'trial_angle':number('试重角度。',minimum=0,maximum=360),
 'clockwise':boolean('仅影响重量位置图的方向。',default=False),'diagram_rotation':number('重量位置图旋转角，度。',default=0),'decompose':boolean(default=False),'angle_1':number('分解方向1，度。'),'angle_2':number('分解方向2，度。')}
special('balance',balance_fields,['before_amplitude','before_phase','after_amplitude','after_phase','trial_weight','trial_angle'],dict(before_amplitude=10,before_phase=20,after_amplitude=15,after_phase=80,trial_weight=100,trial_angle=60),'动平衡配重计算。试重前后矢量须不同，decompose=true需angle_1和angle_2。')
special('balance_sample',{**selection,'start':number('键相选区开始秒数。',default=0),'end':number('选区结束秒数。'),'side':enum(['before','after'],default='after'),'after_sampling_rate':number('仅before侧生效，保留原前后采样率关联。',exclusiveMinimum=0)},
        ['file_id','condition','channel','end'],dict(file_id='sample-0',condition=0,channel=0,start=0,end=1,side='after'),'动平衡周期取参。选区含1–16个完整键相周期且至少17点。')
special('balance_preview',selection,['file_id','condition','channel'],dict(file_id='sample-0',condition=0,channel=0),'动平衡原始预览。返回完整区间波形全览、键相点和可供chart读取的快照ID。')
special('balance_decompose',{'weight':number(minimum=0),'angle':number(minimum=0,maximum=360),'angle_1':number(minimum=0,maximum=360),'angle_2':number(minimum=0,maximum=360)},
        ['weight','angle','angle_1','angle_2'],dict(weight=10,angle=90,angle_1=60,angle_2=120),'独立配重分解。两个方向不能相同/相差180度，目标必须在有效夹角内。')
special('jigger',{**selection,'condition_2':INDEX,'channels':array(INDEX,minItems=16,maxItems=16),'values':array(number(),minItems=16,maxItems=16),'previous_angle':number('保留原偏心角依赖上次角度的行为。',default=0)},[],dict(file_id='sample-0',condition=0,condition_2=1,channels=list(range(8))*2),
        '盘车试验。方式一传16个values；方式二传file_id、condition、condition_2和两组各8个channels。')
efficiency={'file_id':FILE,'exam_type':string(),'density':number('介质密度，单位与压力/功率/流量须配套。',exclusiveMinimum=0,default=1000),
 'gravity':number(exclusiveMinimum=0,default=9.8),'generator_efficiency':number('发电机效率0–1。',exclusiveMinimum=0,maximum=1,default=.98),'elevation':number('高程差。',default=0),'head_factor':number(default=1),'flow_factor':number(default=1),'coefficient':number(default=1),
 'head_method':enum(['pressure','head'],default='pressure'),'flow_method':enum(['meter','differential','high_low'],default='meter'),'dynamic_head':boolean(default=False),'inlet_area':number('计入动水头时必填且>0。',exclusiveMinimum=0),'outlet_area':number('计入动水头时必填且>0。',exclusiveMinimum=0)}
efficiency.update({key:dict(INDEX,description=description) for key,description in [('power_channel','有功通道。'),('inlet_channel','pressure模式进口压力。'),('outlet_channel','pressure模式出口压力。'),('head_channel','head模式静水头通道。'),('flow_channel','meter模式流量通道。'),('diff_channel','differential模式差压通道。'),('high_channel','high_low模式高压通道。'),('low_channel','high_low模式低压通道。')]})
special('efficiency',efficiency,['file_id','exam_type','power_channel'],dict(file_id='sample-0',exam_type='变负荷',power_channel=13,head_method='head',head_channel=0,elevation=50,flow_method='meter',flow_channel=14),
        '效率分析。示例只演示接口映射，真实计算须选实际物理通道和一致单位；其余通道按head/flow模式条件必填。')
special('cam',{'text':string('ICA数值文本：曲线数，各曲线点数与流量/效率对。2–20条，每条3–30点。',maxLength=1048576)},['text'],
        dict(text=CAM_TEXT),
        'ICA协联曲线。示例取自随包“协联曲线_真实电站.ICA”；调用native/cam_math.exe，30秒超时或不能处理时明确返回错误。')
special('airgap',{'values':array(number('径向测点，按均匀角度排列。'),minItems=3,maxItems=1000)},['values'],dict(values=[100,102,103,102,100,98,97,98]),'圆度最小二乘拟合。保留原LONG坐标截断及原偏心角系数，返回OAG正文。')
SCHEMAS['SpecialRequest']={'oneOf':[ref(name) for name in SPECIAL_NAMES],'discriminator':{'propertyName':'module','mapping':{name:ref('Special_'+name)['$ref'] for name in SPECIAL_EXAMPLES}}}

SCHEMAS['ChartRequest']=obj({'analysis_id':string('先分析取得真实ID；示例占位符须替换。'),'plot':enum(['wave','spectrum']),
    'action':enum(['nearest','detail','extrema','index'],default='nearest'),'x':number('nearest目标X，必填。'),'y':number('可选屏幕距离捡取Y；传入时还须y_min/y_max。'),'index':INDEX,
    'x_min':number('范围下界，默认快照首X。'),'x_max':number('范围上界，默认快照末X。'),'y_min':number(),'y_max':number(),'width':number('像素，0–10000。',exclusiveMinimum=0,maximum=10000,default=800),'height':number('像素，0–10000。',exclusiveMinimum=0,maximum=10000,default=250)},['analysis_id','plot'])
SCHEMAS['JobId']=obj({'id':string('任务创建结果中的真实UUID。',format='uuid')},['id'])
SCHEMAS['ResultTable']=obj({'title':string(),'columns':array(string()),'rows':ROWS,'curves':array(ref('Curve')),'axes':array(string()),'plots':array(ref('Plot')),'tables':array(obj({'title':string(),'columns':array(string()),'rows':ROWS})),
    'waves':array(ref('Curve')),'request':obj(),'include_graphs':boolean(default=True)},['columns','rows'])
SCHEMAS['WaveBatchRequest']=obj({**COMMON,**IMAGE_FIELDS,'items':array(obj({'condition':INDEX,'channel':INDEX,**{k:v for k,v in COMMON.items() if k not in ('file_id','condition','channel')}},['condition','channel']),minItems=1),
    'output':enum(['images','data','excel','word'],default='images'),'con':CON,'legacy_office':boolean('excel/word是否转换为XLS/DOC，需Office。',default=False),
    'channel_frequency_ranges':{'type':'object','additionalProperties':POINT,'description':'通道index字符串 → [最小Hz,最大Hz]，用于各通道图像。'}},['file_id','items'])
SCHEMAS['SpecialBatchParameters']={'oneOf':[obj(deepcopy(SCHEMAS[name]['properties']),['module'],description=SCHEMAS[name]['description']) for name in SPECIAL_NAMES],
    'discriminator':{'propertyName':'module'},'description':'部分专项请求，允许必填字段由items覆盖补齐；合并后须为完整SpecialRequest。'}
SCHEMAS['SpecialBatchRequest']=obj({'parameters':ref('SpecialBatchParameters'),'items':array(obj(description='覆盖parameters的逐项字段，如channel、module或区间。'),minItems=1,maxItems=4096),
    'output':enum(['images','xlsx','docx','data']),'format':FORMAT,'include_graphs':boolean(default=True),'width':integer(default=1400),'height':integer(default=950),
    'limits':array(POINT,minItems=2,maxItems=2),'metrics':array(string('仅显示这些名字的曲线。')),'colors':{'type':'object','additionalProperties':COLOR}},['items','output'],description='parameters为基准，items逐项覆盖；每项合并后必须是有效SpecialRequest。parameters可以不含逐项覆盖的必填字段。')
SCHEMAS['SpecialReportRequest']=obj({'source_request':ref('SpecialRequest'),'format':OFFICE,'title':string(),'include_graphs':boolean(default=True),'curve_styles':array(obj({'name':string(),'color':COLOR,'visible':boolean()},['name'])),
    **SCHEMAS['ResultTable']['properties'],'results':array(ref('ResultTable'),minItems=1,maxItems=4096)},description='优先source_request服务端重算；也可传结果表或results列表。有效结果至少应有columns/rows，图像使用curves/plots。')

OPERATIONS={}


def op(path,tag,summary,description,request=None,response=None,example=None,examples=None,downloads=(),method='post'):
    OPERATIONS[path]={'method':method,'tag':tag,'summary':summary,'description':description,'request':request,'response':response,'examples':examples or ({'default':{'summary':'调用示例','value':example}} if example is not None else {}),'downloads':list(downloads)}


JSON='application/json';XLSX='application/vnd.openxmlformats-officedocument.spreadsheetml.sheet';DOCX='application/vnd.openxmlformats-officedocument.wordprocessingml.document'
IMAGES=['image/png','image/jpeg','image/bmp','image/gif','image/tiff'];OFFICE_MIME=[XLSX,DOCX,'application/vnd.ms-excel','application/msword']
op('/api/health','服务','服务版本与滤波引擎','只读；filter_engine显示正在运行的后端实现。',response=obj({'application':string(),'version':string(),'filter_engine':string(),'framework':string(),'server':string()},['application','version','filter_engine','framework','server']),method='get')
op('/api/bootstrap','文件','会话与文件目录','返回当前进程会话token。POST在X-HSJ-Token中传入；重启后重新取得，不要把token写进持久配置。',response=obj({'token':string(),'samples':array(ref('FileEntry')),'imported':array(ref('FileEntry'))},['token','samples','imported']),method='get')
op('/api/coverage','服务','完整功能迁移台账','只读原迁移台账，不代表全功能已验收。',response=obj({'features':array(obj()),'commands':array(obj()),'dialogs':array(obj()),'events':array(obj()),'accepted_count':integer(),'complete_replacement_ready':boolean()},['features','accepted_count','complete_replacement_ready']),method='get')
op('/api/schema','服务','完整OpenAPI定义','文档自身及全部业务接口、请求变体和响应结构。',response=obj({'openapi':string(),'info':obj(),'paths':obj(),'components':obj()},['openapi','info','paths','components']),method='get')
op('/api/open','文件','文件属性、工况与通道','只读取已有文件；包括真实index、逻辑点数、键相可用性、单位与SHA256。',obj({'file_id':FILE},['file_id']),ref('FileMetadata'),{'file_id':'sample-0'})
for path,title in [('/api/upload','导入SEL'),('/api/chd-upload','导入外部CHD')]:
    op(path,'文件',title,'二进制原文件上传，1字节–256MiB；不是multipart/form-data。文件保存在data/UUID，返回file_id。中文文件名/标题/单位需URL编码；CHD为纯数值文本，须显式采样率。',response=ref('FileMetadata'))
op('/api/analyze','分析','波形、频谱与统计','返回显示抽样曲线、完整区间统计及可读取真实点的analysis_id。waveform_full也是显示抽样，不是全部采样点；完整值请用CSV或分段chart读取。',obj(COMMON,['file_id']),ref('AnalysisResult'),BASE_EXAMPLE)
chart_examples={name:{'summary':label,'value':dict(analysis_id='替换为analysis_id',plot='wave',action=name,**fields)} for name,label,fields in [('nearest','按X捡取',{'x':1}),('detail','读取区间细节',{'x_min':0,'x_max':1}),('extrema','读取范围极值',{'x_min':0,'x_max':4}),('index','精确采样序号',{'index':0})]}
chart_examples['nearest_xy']={'summary':'按屏幕二维距离捡取','value':dict(analysis_id='替换为analysis_id',plot='wave',action='nearest',x=1,y=200,x_min=0,x_max=4,y_min=0,y_max=600,width=800,height=250)}
op('/api/chart','分析','精确捡取、细节与极值','analysis_id来自analyze、strip-data、overlays、trends或balance_preview。index相对于快照；sample_index提供原通道采样序号。detail仍可能抽样。',ref('ChartRequest'),ref('ChartResult'),examples=chart_examples)
op('/api/overlays','分析','多通道叠加','不施加主通道滤波；actual_start/end须为主通道实际分析时段。主通道和叠加通道不得重复。',obj({**COMMON,'actual_start':number(),'actual_end':number(),'overlays':array(ref('Overlay'))},['file_id','actual_start','actual_end']),obj({'curves':array(ref('OverlayCurve'))},['curves']),dict(BASE_EXAMPLE,actual_start=0,actual_end=4,overlays=[{'channel':1,'color':'#ff00ff','visible':True}]))
for path,title in [('/api/trends','移动趋势'),('/api/trend-export','导出单条移动趋势CSV')]:
    op(path,'分析' if path=='/api/trends' else '导出',title,'主分析参数同analyze；窗口以采样点为单位。导出必须恰好一条trends。',obj({**COMMON,'trends':array(ref('MovingTrend'))},['file_id','trends']),obj({'curves':array(ref('TrendCurve'))},['curves']) if path=='/api/trends' else None,dict(BASE_EXAMPLE,trends=[TREND_EXAMPLE]),downloads=[] if path=='/api/trends' else ['text/csv'])
op('/api/throwload','分析','甩负荷均值趋势与专用标注','width/Fs<=base<final，final须位于完整窗口趋势内。上升/下降率为相对基准绝对变化百分比。',obj({**COMMON,**SCHEMAS['ThrowSpec']['properties']},['file_id','base','final']),ref('ThrowResult'),dict(BASE_EXAMPLE,width=50,base=1,final=4,rising=True))
op('/api/strip-data','长图','原始长图数据','不附加波形滤波，终点按原长图含终点采样；返回曲线快照。file_sha256可校验旧配置是否对应当前文件。',obj({'file_id':FILE,'condition':dict(INDEX,default=0),'channels':array(INDEX,minItems=1),'start':number(minimum=0,default=0),'end':number(exclusiveMinimum=0),'width':integer(minimum=300,maximum=22500,default=2000),'file_sha256':string()},['file_id','channels','end']),ref('StripResult'),dict(file_id='sample-0',condition=0,channels=[0,1],start=0,end=4,width=2000))
op('/api/waterfall-data','三维瀑布图','三维频谱/波形数据','按试验类型的全部工况及参考通道均值创建；保留原三维共享键相和原N频谱缩放，与波形页FFT选择语义不同。',obj({'file_id':FILE,'channel':dict(INDEX,default=0),'reference':dict(INDEX,default=0),'exam_type':string(),'mode':enum(['spectrum','wave'],default='spectrum')},['file_id']),ref('WaterfallResult'),dict(file_id='sample-0',channel=0,reference=13,exam_type='变负荷',mode='spectrum'))
op('/api/special','专项分析','11类专项计算与预览','从Examples选择模块。各分支参数独立，module是判别字段；返回request回显及各模块专用结果。',ref('SpecialRequest'),ref('SpecialResult'),examples=SPECIAL_EXAMPLES)
op('/api/export','导出','完整CSV / 分析JSON','kind=waveform/spectrum导出完整分析区间；json曲线为显示抽样，保留参数/统计/来源。全时段CSV请显式start=0,end=null,full_period=false。',obj({**COMMON,**DECORATIONS,'kind':enum(['json','waveform','spectrum'],default='json')},['file_id']),ref('AnalysisResult'),dict(BASE_EXAMPLE,kind='json'),downloads=['text/csv'])
op('/api/legacy-data','导出','原格式CHD / 前8谱线表','current为当前计算区间，full为完整滤波后通道，peaks为GB18030谱线表。波形为ASCII六位小数。',obj({**COMMON,'kind':enum(['current','full','peaks'],default='current')},['file_id']),example=dict(BASE_EXAMPLE,kind='current'),downloads=['application/octet-stream'])
op('/api/report','导出','单通道Word / Excel报表','重新计算后导出。XLSX包含完整波形/频谱数据，超过Excel单表上限返回错误；DOC/XLS转换依赖Office。',obj({**COMMON,**DECORATIONS,'format':dict(OFFICE,default='docx')},['file_id']),example=dict(BASE_EXAMPLE,format='xlsx'),downloads=OFFICE_MIME)
op('/api/image','导出','分析图像五格式','由完整计算数据绘制波形/频谱，可带叠加、趋势及标注。尺寸和频率范围详见参数。',obj({**COMMON,**DECORATIONS,**IMAGE_FIELDS},['file_id']),example=dict(BASE_EXAMPLE,format='png',width=1000,height=700,plot='both'),downloads=IMAGES)
op('/api/image-convert','导出','PNG转换为五格式图像','image须为data:image/png;base64,前缀的真实PNG，非远程URL；像素上限2400万，请求体上限32MiB。',obj({'image':string(),'format':FORMAT},['image']),example=dict(image='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mP8/x8AAwMCAO+aDLsAAAAASUVORK5CYII=',format='jpg'),downloads=IMAGES)
op('/api/special-report','专项分析','专项XLSX/DOCX与旧格式报表','source_request用于从原文件重算；也支持结果表或results数组。趋势报表按原报告区间补齐明细与8组谱峰。',ref('SpecialReportRequest'),example=dict(source_request=SPECIAL_EXAMPLES['trend']['value'],format='xlsx',include_graphs=False),downloads=OFFICE_MIME)
op('/api/special-legacy','专项分析','导出OCA/OAG原结果','将special返回的legacy_text和legacy_extension原样提交，按GB18030输出。服务不重新计算此文本。',obj({'legacy_extension':enum(['oca','oag']),'legacy_text':string()},['legacy_extension','legacy_text']),example=dict(legacy_extension='oca',legacy_text='1.000000\t0.900000\r\n'),downloads=['application/octet-stream'])
op('/api/batch','批量任务','创建波形批量任务','按items顺序逐项计算；参数为顶层公共字段加每项覆盖。data输出强制非整周期。进度通过status查询。记录持久化；重启时未完成任务标为interrupted，已完成且ZIP仍存在时可继续下载。',ref('WaveBatchRequest'),ref('JobResult'),dict(file_id='sample-0',items=[{'condition':0,'channel':0}],output='data',start=0,end=4,full_period=False))
op('/api/special-batch','专项批量','创建专项批量任务','parameters与items合并后逐项计算；支持images/xlsx/docx/data。报告趋势按原口径补齐明细。记录持久化，不自动续跑中断任务。',ref('SpecialBatchRequest'),ref('JobResult'),dict(parameters=dict(module='trend',file_id='sample-0',exam_type='变负荷',reference=13),items=[{'channel':0}],output='data',include_graphs=False))
for prefix,tag in [('batch','批量任务'),('special-batch','专项批量')]:
    for suffix,title,description,downloads in [('status','查询进度','返回done/total、当前项目、状态和逐项错误。',[]),('cancel','请求取消','幂等请求；运行中任务在处理下一项前取消，不提供取消后的部分包。已完成任务不会变成取消状态。',[]),('download','下载ZIP','仅completed/completed_with_errors可下载，包含分析清单和错误清单；后者须检查逐项错误。',['application/zip'])]:
        op('/api/'+prefix+'-'+suffix,tag,title,description,ref('JobId'),None if downloads else ref('JobResult'),{'id':'替换为任务创建返回的UUID'},downloads=downloads)
for path,title,response,required in [('/api/export-file-list','列出任务ZIP中的文件',obj({'files':array(obj({'index':INDEX,'name':string(),'size':integer()},['index','name','size']))},['files']),['type','id']),('/api/export-file','下载任务ZIP中的单文件',None,['type','id','index'])]:
    op(path,'导出',title,'type=wave使用波形任务，special使用专项任务；index必须取file-list真实结果，不是文件路径。',obj({'type':enum(['wave','special']),'id':string(format='uuid'),'index':INDEX},required),response,dict(type='wave',id='替换为已完成任务UUID',**({'index':0} if path.endswith('/export-file') else {})),downloads=[] if response else ['application/octet-stream'])
op('/api/shutdown','服务','停止本机服务','会取消任务并停止监听；执行后当前文档也不能再调用接口，须用Start-BS重新启动。仅显式执行。',obj(),obj({'status':enum(['shutting_down'])},['status']),{})

TAGS=[{'name':name,'description':description} for name,description in [('服务','健康、定义、台账与退出'),('文件','先取得会话，再选文件；上传不是multipart'),('分析','波形、峰峰值、FFT、游标、移动趋势与甩负荷'),('长图','原始多通道长图数据'),('三维瀑布图','波形/频谱三维准备'),('专项分析','11种模块及报表'),('导出','数据、图像、Office及逐文件下载'),('批量任务','波形任务状态与取消'),('专项批量','专项后台任务')]]
