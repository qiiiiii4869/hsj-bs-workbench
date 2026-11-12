'use strict';
const $ = id => document.getElementById(id);
const state = {token:'', file:null, channel:0, busy:false, result:null, params:null, filters:[], overlaySpecs:[], trendSpecs:[], mainColor:'#1762b5', throwSpec:null, activeKey:null, filterDraft:null, dirty:false, sessions:new Map()};
const fmt = (v, digits=4) => Number(v).toLocaleString('zh-CN',{maximumFractionDigits:digits});
const filterNames={lowpass:'低通',highpass:'高通',bandpass:'带通',bandstop:'带阻'};
function hasChannel(){return !!state.file&&state.file.conditions[Number($('condition').value)]?.channels[state.channel]?.count>=4;}
function currentChannel(){return state.file?.conditions[Number($('condition').value)]?.channels[state.channel];}
try{const raw=localStorage.getItem('hsj.filterSessions.v1')||'{}',saved=JSON.parse(raw);if(saved&&typeof saved==='object'&&!Array.isArray(saved)){if(Object.values(saved).some(s=>s?.convention!=='desktop-defaults-v1')&&!localStorage.getItem('hsj.filterSessions.beforeDesktopDefaults'))localStorage.setItem('hsj.filterSessions.beforeDesktopDefaults',raw);state.sessions=new Map(Object.entries(saved).map(([key,s])=>[key,s?.convention==='desktop-defaults-v1'?s:{...s,confidence:97,full_period:true,remove_mean:false,window:'rectangular',convention:'desktop-defaults-v1'}]));}}catch{}
function saveContext(){
  if(!state.activeKey)return;
  const p=state.params;
  const previous=state.sessions.get(state.activeKey)||{};
  state.sessions.set(state.activeKey,{...previous,convention:'desktop-defaults-v1',appearance:'classic-original-v1',mainColor:state.mainColor,throwSpec:state.throwSpec,filters:state.filters.map(f=>({...f})),overlays:overlaySpecs(),trends:trendSpecs(),filterDraft:state.filterDraft?{...state.filterDraft}:null,...(p?{start:p.start,end:p.end,window:p.window,confidence:p.confidence,full_period:p.full_period,remove_mean:p.remove_mean,title:p.title}:{})});
  try{localStorage.setItem('hsj.filterSessions.v1',JSON.stringify(Object.fromEntries(state.sessions)));}catch{}
}
function defaultFilter(){const rate=state.file?.channels[state.channel]?.sample_rate||1024;return {type:'lowpass',order:5,low:Math.min(10,rate/10),high:Math.min(100,rate/3)};}
function renderFilterHistory(){
  const count=state.filters.length;
  $('filter-state').textContent=count?`已滤波 · ${count} 次`:'原始数据';
  $('filter-summary').textContent=count?`滤波记录 · 已连续应用 ${count} 次`:'滤波记录 · 原始数据';
  $('filter-steps').replaceChildren(...state.filters.map((f,i)=>{const li=document.createElement('li');const cut=f.type==='lowpass'?`${fmt(f.low)} Hz`:f.type==='highpass'?`${fmt(f.high)} Hz`:`${fmt(f.low)}–${fmt(f.high)} Hz`;li.textContent=`${i+1}. ${filterNames[f.type]||f.type} · ${f.order} 阶 · ${cut}`;return li;}));
  $('filter-context').textContent=state.file&&state.activeKey?`${state.file.filename} / ${state.file.conditions[Number($('condition').value)].name} / ${state.file.channels[state.channel].name}`:'';
  const original=state.result?.original?.statistics,current=state.result?.statistics;
  $('filter-comparison').textContent=original&&current?`原始 → 当前：峰峰值 ${fmt(original.peak_to_peak)} → ${fmt(current.peak_to_peak)}；AC RMS ${fmt(original.ac_rms)} → ${fmt(current.ac_rms)}`:count?'当前通道已有滤波设置。':'尚未应用滤波。';
}
function renderCharts(preserveView=false){const r=state.result;if(!r)return;const original=$('compare-original').checked?r.original:null;const previous=preserveView?[wave,spectrum].map(chart=>({view:chart.view(),history:[...chart.history]})):null;wave.bind(r);spectrum.bind(r);wave.unit=r.source.channel.unit;spectrum.unit=wave.unit;wave.set(r.waveform_full||r.waveform,original?.waveform_full||original?.waveform||[]);spectrum.set(r.spectrum,original?.spectrum||[]);if(previous){[wave,spectrum].forEach((chart,i)=>{chart.restoreView(previous[i].view);chart.history=previous[i].history;chart.draw();chart.syncTools();chart.loadDetail();});}else{wave.setXRange([r.parameters.start,r.parameters.end],false);applyFrequencyRange();}}
function status(message, error=false) {$('status').textContent=message; $('status').classList.toggle('error',error);logAction((error?'错误：':'')+message);}
function exportsEnabled(enabled) {for(const id of ['export-wave','export-spectrum','export-json','export-full']) $(id).disabled=!enabled;$('export-spectrum').disabled=!enabled||state.result?.parameters.fft_enabled===false;$('export-original').disabled=!enabled||!state.filters.length;}
function busy(value) {
  state.busy=value; document.body.classList.toggle('busy',value);
  for(const id of ['import','open','sample','condition','search','start','end','window','demean']) $(id).disabled=value || (['condition','search','start','end','window','demean'].includes(id)&&!state.file);
  $('analyze').disabled=value||!hasChannel();
  $('full-period').disabled=value||!currentChannel()?.full_period_available;
  for(const id of ['confidence','filter-open','title-open']) $(id).disabled=value||!hasChannel();
  $('filter-restore').disabled=value||!hasChannel()||!state.filters.length;
  $('filter-undo').disabled=value||!hasChannel()||!state.filters.length;
  $('compare-original').disabled=value||state.dirty||!state.result?.original;
  for(const id of ['filter-type','filter-order','filter-cancel','filter-apply']) $(id).disabled=value;
  $('filter-apply').textContent=value?'正在应用…':'应用滤波';
  syncFilterFields();
  document.querySelectorAll('.channel').forEach(b=>b.disabled=value||b.dataset.available==='false');
  exportsEnabled(!value&&!state.dirty&&!!state.result);
  wave.syncTools();spectrum.syncTools();
  syncOverlayControls();for(const id of ['channel-select','channel-color','channel-submit','channel-cancel'])$(id).disabled=value;
  for(const id of ['trend-metric','trend-width','trend-step','trend-color','trend-effective','trend-tail','trend-submit','trend-cancel'])$(id).disabled=value;
  syncTrendControls();
  for(const id of ['span-open','throw-open','output-open','batch-open'])$(id).disabled=value||!wave.ready();
  for(const id of ['span-start','span-end','throw-width','throw-base','throw-end','throw-rise','throw-submit','throw-cancel','output-submit','output-cancel','chd-submit','chd-cancel'])$(id).disabled=value;
  $('key-visible').disabled=value||!state.result?.key_phase_points?.length;
  for(const form of ['output-form','chd-form'])$(form).querySelectorAll('input,select,button').forEach(control=>control.disabled=value);
  if(classicReady){classicTree();syncClassic();}
}
async function request(url, body, extra={}) {
  const response=await fetch(url,{method:'POST',headers:{'X-HSJ-Token':state.token,'Content-Type':'application/json',...(extra.headers||{})},body:extra.raw||JSON.stringify(body)});
  if(!response.ok) {let message=`请求失败 (${response.status})`; try{message=(await response.json()).error||message;}catch{} throw new Error(message);}
  return extra.download?response.blob():response.json();
}
function option(value,text) {const el=document.createElement('option');el.value=value;el.textContent=text;return el;}
function clearResult(){state.result=null;state.params=null;state.dirty=false;exportsEnabled(false);wave.set([]);spectrum.set([]);for(const id of ['metric-pp','metric-effective','metric-freq','metric-count']) $(id).textContent='—';$('sampling').replaceChildren();$('statistics').replaceChildren();$('peaks').replaceChildren();$('wave-cursor').textContent='等待分析数据';$('spectrum-cursor').textContent='等待分析数据';}
function parameters(){return {file_id:state.file.file_id,condition:Number($('condition').value),channel:state.channel,start:Number($('start').value),end:$('end').value===''?null:Number($('end').value),window:$('window').value,remove_mean:$('demean').checked,confidence:Number($('confidence').value),full_period:$('full-period').checked,filters:state.filters.map(f=>({...f})),title:$('title').textContent,main_color:state.mainColor,include_original:true};}
function showChannels(){
  const condition=state.file.conditions[Number($('condition').value)];
  const query=$('search').value.toLocaleLowerCase();
  $('channels').replaceChildren();
  for(const channel of condition.channels){
    if(!channel.name.toLocaleLowerCase().includes(query))continue;
    const b=document.createElement('button');b.type='button';b.className='channel';b.dataset.available=String(channel.count>=4);b.classList.toggle('active',channel.index===state.channel);b.setAttribute('aria-pressed',String(channel.index===state.channel));
    const dot=document.createElement('b'),name=document.createElement('span'),unit=document.createElement('small');name.textContent=channel.name;unit.textContent=channel.unit;b.title=`${channel.name} · ${channel.sample_rate} Hz · ${channel.count} 点`;b.append(dot,name,unit);b.disabled=state.busy||channel.count<4;
    b.addEventListener('click',()=>selectChannel(channel.index));$('channels').append(b);
  }
}
async function selectChannel(index){
  if(state.busy)return;
  saveContext();state.channel=index;
  const channel=state.file.conditions[Number($('condition').value)].channels[index];
  state.activeKey=`${state.file.sha256}|${Number($('condition').value)}|${state.file.conditions[Number($('condition').value)].sort_code}|${index}|${channel.id}`;
  const saved=state.sessions.get(state.activeKey)||{};
  state.filters=Array.isArray(saved.filters)?saved.filters.map(f=>({...f})):[];state.overlaySpecs=Array.isArray(saved.overlays)?saved.overlays.filter(s=>Number.isInteger(s.channel)&&s.channel!==index).map(s=>({...s})):[];state.trendSpecs=Array.isArray(saved.trends)?saved.trends.map(s=>({...s})):[];state.throwSpec=saved.throwSpec||null;$('throw-result').textContent='';state.filterDraft=saved.filterDraft||defaultFilter();clearResult();showChannels();
  $('title').textContent=saved.title||channel.name;
  $('subtitle').textContent=`${state.file.filename} / ${state.file.conditions[Number($('condition').value)].name} / ${channel.sample_rate} Hz`;
  const duration=channel.count/channel.sample_rate;
  $('start').value=String(Number.isFinite(saved.start)&&saved.start>=0&&saved.start<duration?saved.start:0);$('end').value=String(Number.isFinite(saved.end)&&saved.end>Number($('start').value)&&saved.end<=duration?saved.end:duration);
  $('end').max=String(channel.count/channel.sample_rate);
  $('window').value=saved.window==='hann'?'hann':'rectangular';$('confidence').value=String([95,97,100].includes(saved.confidence)?saved.confidence:97);$('demean').checked=saved.remove_mean===true;$('full-period').checked=channel.full_period_available&&saved.full_period!==false;
  document.querySelectorAll('.unit').forEach(el=>el.textContent=channel.unit);
  $('wave-unit').textContent=channel.unit;$('freq-unit').textContent=channel.unit;
  state.mainColor=saved.mainColor||state.file.color||(document.body.classList.contains('classic-layout')?'#0000ff':'#1762b5');if(!saved.appearance&&state.mainColor==='#1762b5')state.mainColor='#0000ff';$('main-color').value=state.mainColor;
  for(const option of $('display-mode').options)option.disabled=!channel.fft_enabled&&option.value!=='wave';$('display-mode').value=channel.fft_enabled?'both':'wave';$('wave').closest('section').hidden=false;$('spectrum').closest('section').hidden=!channel.fft_enabled;
  renderFilterHistory();
  await runAnalysis();
}
async function acceptFile(info){
  saveContext();state.activeKey=null;state.file=info;state.channel=0;state.filters=[];state.overlaySpecs=[];state.trendSpecs=[];state.throwSpec=null;$('throw-result').textContent='';state.overlayError='';state.filterDraft=null;clearResult();$('search').value='';
  $('condition').replaceChildren(...info.conditions.map(c=>option(c.index,`${c.type} / ${c.name}`)));
  $('file-info').textContent=`${info.filename}\n${info.exam} · ${info.conditions.length} 个工况 · ${info.channels.length} 个通道 · ${fmt(info.size/1024/1024,1)} MB`;
  $('file-info').title=`SHA256: ${info.sha256}`;$('file-count').textContent=`${info.channels.length} 通道`;
  renderFilterHistory();busy(false);
  const navigation=state.navigation?.file_id===info.file_id?state.navigation:null;state.navigation=null;const selected=info.conditions.find(c=>c.index===navigation?.condition)||info.conditions[0];$('condition').value=String(selected.index);const first=selected.channels.find(c=>c.index===navigation?.channel&&c.count>=4)||selected.channels.find(c=>c.count>=4&&c.fft_enabled)||selected.channels.find(c=>c.count>=4);
  if(first)await selectChannel(first.index);else {$('title').textContent='当前工况没有有效通道';status('当前工况没有可分析的通道',true);}
}
async function openFile(id){if(!id||state.busy)return;busy(true);status('正在读取 SEL 工况与通道…');try{await acceptFile(await request('/api/open',{file_id:id}));}catch(e){status(e.message,true);busy(false);}}
async function upload(file){
  if(!file||state.busy)return;
  if(file.size>256*1024*1024){status('文件须在 256 MB 以内',true);return;}
  busy(true);status(`正在导入 ${file.name}…`);
  try{const info=await request('/api/upload',null,{raw:file,headers:{'Content-Type':'application/octet-stream','X-Filename':encodeURIComponent(file.name)}});$('sample').append(option(info.file_id,info.filename));$('sample').value=info.file_id;await acceptFile(info);}catch(e){status(e.message,true);busy(false);}finally{$('file').value='';}
}
function fillDL(id, entries){const nodes=[];for(const [label,value] of entries){const dt=document.createElement('dt'),dd=document.createElement('dd');dt.textContent=label;dd.textContent=value;nodes.push(dt,dd);}$(id).replaceChildren(...nodes);}
async function runAnalysis(overrides={},useCommitted=false,preserveNavigation=false){
  if(state.busy||!hasChannel())return;
  if(!useCommitted&&!$('parameters').reportValidity())return false;
  const base=useCommitted&&state.params?{...state.params}:parameters();
  const navigation=preserveNavigation?{view:wave.view(),history:[...wave.history]}:null;
  busy(true);status('正在使用完整区间数据计算统计与 FFT…');
  try{
    const params={...base,...overrides},r=await request('/api/analyze',params);state.result=r;state.params=params;state.filters=params.filters.map(f=>({...f}));state.dirty=false;
    if(useCommitted){$('start').value=String(params.start);$('end').value=params.end===null?'':String(params.end);$('window').value=params.window;$('confidence').value=String(params.confidence);$('demean').checked=params.remove_mean;$('full-period').checked=params.full_period;}
    const s=r.statistics,p=r.parameters,unit=r.source.channel.unit;
    $('metric-pp').textContent=fmt(s.peak_to_peak,2);$('metric-effective').textContent=fmt(s.desktop_effective,2);$('metric-freq').textContent=p.fft_enabled===false?'—':fmt(r.peaks[0]?.frequency||0,3);$('metric-count').textContent=fmt(p.count,0);
    $('pp-label').textContent=`${p.confidence}% 峰峰值`;
    fillDL('sampling',[['采样频率',`${fmt(p.sample_rate)} Hz`],['分析模式',p.full_period?'键相整周期':'非整周期'],['分析时长',`${fmt(p.end-p.start,3)} s`],['实际开始时间',`${fmt(p.start,3)} s`],['实际结束时间',`${fmt(p.end,3)} s`],['计算点数',fmt(p.count,0)],['起始点序号',fmt(p.start_index,0)],['终止点（不含）',fmt(p.end_index_exclusive,0)],['有效总点数',fmt(r.source.total_count,0)]]);
    fillDL('statistics',[['最大值',fmt(s.max,2)],['最小值',fmt(s.min,2)],['平均值',fmt(s.mean,2)],['中位数',fmt(s.median,2)],['标准 RMS',fmt(s.rms)],['AC RMS',fmt(s.ac_rms)],['有效值 · 原版',fmt(s.desktop_effective,2)],[`${p.confidence}% 峰峰值`,fmt(s.peak_to_peak,2)]]);
    $('statistics').title=`单位：${unit}`;
    $('peaks').replaceChildren(...r.peaks.map((v,i)=>{const tr=document.createElement('tr');for(const text of [i+1,Number(v.frequency).toFixed(3),Number(v.amplitude).toFixed(3)]){const td=document.createElement('td');td.textContent=text;tr.append(td);}return tr;}));
    renderCharts();await reloadOverlays();renderFilterHistory();saveContext();
    if(navigation){wave.restoreView(navigation.view);if(r.parameters.full_period)wave.range=[r.parameters.start,r.parameters.end];wave.history=navigation.history;wave.draw();wave.syncTools();}
    $('resolution').textContent=`频率分辨率 ${fmt(p.delta_frequency,6)} Hz · 单边峰值幅度`;
    $('footer-state').textContent=`${r.engine_version} · ${p.count.toLocaleString()} 点 · ${p.window==='hann'?'Hann 窗':'矩形窗'}`;
    status(`分析完成 · ${r.source.channel.name} · [${fmt(p.start)}, ${fmt(p.end)}) s${state.overlayError?' · '+state.overlayError:''}`,!!state.overlayError);
    logAction(`分析 ${r.source.filename} / ${r.source.condition.name} / ${r.source.channel.name} [${p.start},${p.end})；${p.confidence}% 峰峰值；滤波 ${state.filters.length} 次`);
    return true;
  }catch(e){if(!state.result)clearResult();status(e.message,true);return false;}finally{busy(false);}
}
async function download(kind){
  if(!state.result||state.busy||state.dirty)return;
  busy(true);status('正在生成完整数据导出…');
  try{const full=kind==='full',original=kind==='original';const blob=await request('/api/export',{...state.params,...(full?{start:0,end:null,full_period:false}:{}),...(original?{filters:[]}:{}),include_original:kind==='json',overlays:kind==='json'?overlaySpecs():[],trends:kind==='json'?trendSpecs():[],throw_spec:kind==='json'?state.throwSpec:null,annotations:[...wave.records(),...spectrum.records()],kind:full||original?'waveform':kind},{download:true});if(window.hsjSaveBlob){await window.hsjSaveBlob(blob,`HSJ-${kind}.${kind==='json'?'json':'csv'}`);status('导出已完成');return;}const url=URL.createObjectURL(blob),a=document.createElement('a');a.href=url;a.download=`HSJ-${kind}.${kind==='json'?'json':'csv'}`;a.click();setTimeout(()=>URL.revokeObjectURL(url),15000);status('导出完成 · 已交给浏览器下载');}catch(e){status(e.message,true);}finally{busy(false);}
}
const wave=new Chart('wave','s'),spectrum=new Chart('spectrum','Hz');
setupChartInteractions();
setupChannels();
setupTrends();
setupPage();setupExports();
function applyFrequencyRange(record=false){if(!spectrum.data.length)return;const maximum=$('freq-range').value==='full'?spectrum.domain[1]:Math.min(Number($('freq-range').value),spectrum.domain[1]);spectrum.setXRange([spectrum.domain[0],maximum],record);}
$('open').addEventListener('click',()=>openFile($('sample').value));
$('import').addEventListener('click',()=>$('file').click());$('file').addEventListener('change',e=>upload(e.target.files[0]));
$('condition').addEventListener('change',()=>{const channels=state.file.conditions[Number($('condition').value)].channels;const current=channels.find(c=>c.index===state.channel&&c.count>=4)||channels.find(c=>c.count>=4);if(current)selectChannel(current.index);else {saveContext();state.activeKey=null;state.filters=[];state.overlaySpecs=[];state.trendSpecs=[];state.throwSpec=null;$('throw-result').textContent='';state.overlayError='';state.filterDraft=null;clearResult();showChannels();renderFilterHistory();$('title').textContent='当前工况没有有效通道';busy(false);status('当前工况没有可分析数据',true);}});
$('search').addEventListener('input',showChannels);
$('parameters').addEventListener('submit',e=>{e.preventDefault();runAnalysis();});
$('parameters').addEventListener('input',()=>{state.dirty=true;exportsEnabled(false);$('compare-original').disabled=true;wave.syncTools();spectrum.syncTools();syncOverlayControls();syncTrendControls();if(state.file)status('参数已修改 · 图表保留上次结果，请点击“开始分析”更新');});
$('reset-wave').addEventListener('click',()=>wave.reset());$('freq-range').addEventListener('change',()=>applyFrequencyRange(true));
$('export-wave').addEventListener('click',()=>download('waveform'));$('export-spectrum').addEventListener('click',()=>download('spectrum'));$('export-json').addEventListener('click',()=>download('json'));
$('coverage').addEventListener('click',async()=>{try{const response=await fetch('/api/coverage');if(!response.ok)throw new Error('迁移对照表加载失败');const ledger=await response.json();$('coverage-summary').textContent=`${ledger.features.length} 项功能要求 / 139 个命令 / 45 个窗口 / 650 个控件 / 276 个事件`;$('coverage-body').replaceChildren(...ledger.features.map(row=>{const tr=document.createElement('tr');for(const text of [row.id,row.name,row.web_entry,`${row.status}：${row.note}`]){const td=document.createElement('td');td.textContent=text;tr.append(td);}return tr;}));$('coverage-dialog').showModal();}catch(e){status(e.message,true);}});
$('coverage-close').addEventListener('click',()=>$('coverage-dialog').close());
function syncFilterFields(){const type=$('filter-type').value;$('filter-low').disabled=state.busy||type==='highpass';$('filter-high').disabled=state.busy||type==='lowpass';$('filter-low-label').textContent=type==='highpass'?'低截止频率 / Hz（不使用）':'低截止频率 / Hz';$('filter-high-label').textContent=type==='lowpass'?'高截止频率 / Hz（不使用）':'高截止频率 / Hz';}
$('filter-type').addEventListener('change',syncFilterFields);
$('filter-open').addEventListener('click',()=>{
  $('filter-error').textContent='';const draft={...defaultFilter(),...state.filterDraft};$('filter-type').value=draft.type;$('filter-order').value=draft.order;$('filter-low').value=draft.low;$('filter-high').value=draft.high;syncFilterFields();
  const channel=state.file.channels[state.channel];$('filter-sampling').textContent=`${channel.name} · 采样频率 ${channel.sample_rate} Hz · 截止频率须小于 ${channel.sample_rate/2} Hz`;
  $('filter-operation').textContent=state.filters.length?`将在现有 ${state.filters.length} 次滤波结果上继续应用。`:'将在原始数据上应用滤波。';
  $('filter-dialog').showModal();
});
$('filter-cancel').addEventListener('click',()=>$('filter-dialog').close());
$('filter-dialog').addEventListener('cancel',e=>{if(state.busy)e.preventDefault();});
$('filter-form').addEventListener('submit',async e=>{
  e.preventDefault();if(state.busy)return;
  const filter={type:$('filter-type').value,order:Number($('filter-order').value),low:Number($('filter-low').value),high:Number($('filter-high').value)};
  const nyquist=state.file.channels[state.channel].sample_rate/2;
  let error='';if(!Number.isInteger(filter.order)||filter.order<1||filter.order>100)error='阶数须为 1–100 的整数';
  else if(filter.type!=='highpass'&&(!Number.isFinite(filter.low)||filter.low<=0||filter.low>=nyquist))error=`低截止频率须大于 0 且小于 ${nyquist} Hz`;
  else if(filter.type!=='lowpass'&&(!Number.isFinite(filter.high)||filter.high<=0||filter.high>=nyquist))error=`高截止频率须大于 0 且小于 ${nyquist} Hz`;
  else if(['bandpass','bandstop'].includes(filter.type)&&filter.low>=filter.high)error='低截止频率必须小于高截止频率';
  if(error){$('filter-error').textContent=error;return;}
  if(!$('parameters').reportValidity()){$('filter-error').textContent='请先取消弹窗，修正分析起止时间后再应用滤波';return;}
  if(await runAnalysis({filters:[...state.filters,filter]})){state.filterDraft=filter;saveContext();$('filter-history').open=true;$('filter-dialog').close();}else $('filter-error').textContent=$('status').textContent;
});
$('filter-restore').addEventListener('click',async()=>{if(await runAnalysis({filters:[]},true))status('已恢复原始文件数据 · 全部滤波步骤已清除');});
$('filter-undo').addEventListener('click',async()=>{if(await runAnalysis({filters:state.filters.slice(0,-1)},true))status(state.filters.length?`已撤销上一次滤波 · 保留 ${state.filters.length} 步`:'已撤销最后一次滤波 · 当前为原始数据');});
$('compare-original').addEventListener('change',()=>renderCharts(true));
$('export-original').addEventListener('click',()=>download('original'));
$('confidence').addEventListener('change',async()=>{const previous=state.params?.confidence??97;if(!await runAnalysis())$('confidence').value=String(previous);});
$('display-mode').addEventListener('change',()=>{const mode=$('display-mode').value;$('wave').closest('section').hidden=mode==='spectrum';$('spectrum').closest('section').hidden=mode==='wave';wave.draw();spectrum.draw();});
$('title-open').addEventListener('click',()=>{$('title-value').value=$('title').textContent;$('title-dialog').showModal();});
$('title-cancel').addEventListener('click',()=>$('title-dialog').close());
$('title-form').addEventListener('submit',e=>{e.preventDefault();const title=$('title-value').value.trim();if(!title)return;$('title').textContent=title;if(state.result){state.result.source.title=title;state.params.title=title;}saveContext();status('标题已更新 · 分析记录与数据导出使用此标题');$('title-dialog').close();});
$('export-full').addEventListener('click',()=>download('full'));
async function init(){try{
 const r=await fetch('/api/bootstrap');if(!r.ok)throw new Error('本机服务连接失败');const boot=await r.json();state.token=boot.token;
 const files=[...boot.samples,...boot.imported];$('sample').replaceChildren(...files.map(f=>option(f.id,f.name)));
 const query=new URLSearchParams(location.search),requested=files.find(f=>f.id===query.get('file_id')),initial=requested||boot.samples[0];
 if(requested)state.navigation={file_id:requested.id,condition:Number(query.get('condition')),channel:Number(query.get('channel'))};
 if(initial){$('sample').value=initial.id;await openFile(initial.id);}
 }catch(e){status(e.message,true);}}

setupClassic();
document.documentElement.dataset.workbenchReady='true';
init();
