'use strict';
function overlaySpecs(){return state.overlaySpecs.map(s=>({...s}));}
function curveInfo(chart){
  if(chart.id!=='wave'||chart.activeCurve==='main')return {key:'main',unit:chart.unit,name:state.result?.source.channel.name||'',analysis_id:chart.analysisId,signature:chart.signature};
  const c=chart.overlays.find(c=>c.key===chart.activeCurve&&c.visible);
  return c?{key:c.key,unit:c.channel.unit,name:c.channel.name,analysis_id:c.analysis_id,signature:c.analysis_signature}:{key:'main',unit:chart.unit,name:state.result?.source.channel.name||'',analysis_id:chart.analysisId,signature:chart.signature};
}
function syncOverlayControls(){
  const ready=wave.ready();$('channel-open').disabled=!ready;
  const select=$('curve-select'),selected=wave.activeCurve||'main';
  select.replaceChildren(option('main',`主通道：${state.file?.channels[state.channel]?.name||'—'}`),...wave.overlays.filter(c=>c.visible).map(c=>option(c.key,`${c.channel.name} (${c.channel.unit})`)));
  select.value=[...select.options].some(o=>o.value===selected)?selected:'main';wave.activeCurve=select.value;select.disabled=!ready;
  $('overlay-list').replaceChildren(...wave.overlays.map(c=>{
    const row=document.createElement('div');row.className='overlay-row';
    const visible=document.createElement('input');visible.type='checkbox';visible.checked=c.visible;visible.disabled=!ready;visible.setAttribute('aria-label',`显示 ${c.channel.name}`);
    const color=document.createElement('input');color.type='color';color.value=c.color;color.disabled=!ready;color.setAttribute('aria-label',`${c.channel.name} 曲线颜色`);
    const label=document.createElement('span');label.textContent=`${c.channel.name} · ${c.channel.unit||'无单位'} · ${c.channel.sample_rate} Hz · ${c.is_trend?`窗宽 ${c.trend.width} 点 / 步长 ${c.trend.step} 点 · ${c.processing}`:'原始值'}`;
    const remove=document.createElement('button');remove.type='button';remove.textContent='移除';remove.disabled=!ready;
    const exportButton=document.createElement('button');exportButton.type='button';exportButton.textContent='导出 CSV';exportButton.disabled=!ready;
    exportButton.addEventListener('click',async()=>{if(!wave.ready())return;busy(true);try{const params=c.is_trend?{...state.params,trends:c.is_throwload?[c.trend_request]:trendSpecs().filter(s=>'trend-'+s.id===c.key),include_original:false}:{...state.params,channel:c.channel.index,start:c.start,end:c.end,full_period:false,filters:[],overlays:[],include_original:false,title:c.channel.name,annotations:wave.records().filter(a=>a.curve===c.key),kind:'waveform'};const blob=await request(c.is_trend?'/api/trend-export':'/api/export',params,{download:true});const url=URL.createObjectURL(blob),link=document.createElement('a');link.href=url;link.download=`HSJ-${c.channel.name.replace(/[\\/:*?"<>|]/g,'_')}.csv`;link.click();setTimeout(()=>URL.revokeObjectURL(url),15000);status(`已导出 ${c.channel.name} 的完整趋势/采样数据`);}catch(error){status(error.message,true);}finally{busy(false);}});
    const save=()=>{if(c.is_throwload)state.throwSpec={...state.throwSpec,color:c.color,visible:c.visible};else if(c.is_trend)state.trendSpecs=state.trendSpecs.map(s=>'trend-'+s.id===c.key?{...s,color:c.color,visible:c.visible}:s);else state.overlaySpecs=state.overlaySpecs.map(s=>s.channel===c.channel.index?{...s,color:c.color,visible:c.visible}:s);wave.cursor=null;wave.pickSequence++;wave.pendingPick=null;saveContext();wave.draw();wave.syncTools();syncOverlayControls();};
    visible.addEventListener('change',()=>{c.visible=visible.checked;if(!c.visible&&wave.activeCurve===c.key)wave.activeCurve='main';save();});
    color.addEventListener('change',()=>{c.color=color.value;save();});
    remove.addEventListener('click',()=>{if(c.is_throwload){state.throwSpec=null;$('throw-result').textContent='';}else if(c.is_trend)state.trendSpecs=state.trendSpecs.filter(s=>'trend-'+s.id!==c.key);else state.overlaySpecs=state.overlaySpecs.filter(s=>s.channel!==c.channel.index);wave.overlays=wave.overlays.filter(v=>v!==c);if(wave.activeCurve===c.key)wave.activeCurve='main';wave.cursor=null;wave.pickSequence++;saveContext();wave.draw();wave.syncTools();syncOverlayControls();});
    row.append(visible,color,label,exportButton,remove);return row;
  }));
  const units=new Set([state.result?.source.channel.unit,...wave.overlays.filter(c=>c.visible).map(c=>c.channel.unit)]);
  $('overlay-note').textContent=state.overlayError|| (wave.overlays.length?(units.size>1?'多种单位共用数值轴，未作单位换算；请按图例识别单位。':'曲线共用原始数值轴与时间轴。')+' 统计和频谱属于主通道；移动趋势使用主通道当前滤波数据，其他叠加通道保持原始值。':'');
  $('wave-unit').textContent=units.size>1?'混合单位 · 见通道列表':state.result?.source.channel.unit||'';
  wave.axisUnit=units.size>1?'混合单位':wave.unit;
  $('overlay-panel').hidden=!state.file;
}
async function fetchOverlays(specs){
  if(!state.params||!state.result)return [];
  const r=await request('/api/overlays',{...state.params,actual_start:state.result.parameters.start,actual_end:state.result.parameters.end,overlays:specs});return r.curves;
}
async function reloadOverlays(){
  wave.overlays=[];wave.activeCurve='main';wave.cursor=null;wave.pickSequence++;state.overlayError='';
  if(state.overlaySpecs.length)try{wave.overlays=await fetchOverlays(overlaySpecs());}catch(error){state.overlayError=`叠加通道尚未加载：${error.message}`;}
  if(state.trendSpecs.length)try{wave.overlays.push(...await fetchTrends(trendSpecs()));}catch(error){state.overlayError+=" 移动趋势尚未加载："+error.message;}
  if(state.throwSpec)try{const r=await request('/api/throwload',{...state.params,...state.throwSpec,include_original:false});wave.overlays.push(r.curve);wave.annotations=wave.annotations.filter(a=>a.curve!=='throwload');wave.annotations.push(...r.annotations.map(a=>({id:crypto.randomUUID(),...a})));$('throw-result').textContent=`甩负荷基准 ${fmt(r.base.y,4)}；极值 ${fmt(r.extreme.y,4)}；变化率 ${fmt(r.percent,4)}%`;wave.persist();}catch(error){state.overlayError+=' 甩负荷尚未加载：'+error.message;}
  wave.draw();
}
function openChannelDialog(){
  if(!wave.ready())return;const channels=state.file.conditions[Number($('condition').value)].channels;
  const used=new Set([state.channel,...state.overlaySpecs.map(s=>s.channel)]);
  const available=channels.filter(c=>!used.has(c.index)&&c.count>=4);
  $('channel-select').replaceChildren(...available.map(c=>option(c.index,`${c.name} · ${c.unit||'无单位'} · ${c.sample_rate} Hz`)));
  $('channel-submit').disabled=!available.length;$('channel-error').textContent=available.length?'':'没有可继续添加的通道';$('channel-context').textContent=`${state.file.filename} / ${state.file.conditions[Number($('condition').value)].name}`;
  $('channel-color').value='#ff00ff';
  $('channel-dialog').showModal();
}
function setupChannels(){
  $('channel-open').addEventListener('click',openChannelDialog);$('channel-cancel').addEventListener('click',()=>$('channel-dialog').close());
  $('channel-dialog').addEventListener('cancel',e=>{if(state.busy)e.preventDefault();});
  $('curve-select').addEventListener('change',()=>{wave.activeCurve=$('curve-select').value;wave.pickSequence++;wave.pendingPick=null;wave.cursor=null;wave.writeCursor();wave.syncTools();wave.draw();});
  $('channel-form').addEventListener('submit',async e=>{
    e.preventDefault();if(!wave.ready())return;const spec={channel:Number($('channel-select').value),color:$('channel-color').value,visible:true};
    const next=[...overlaySpecs(),spec];busy(true);$('channel-error').textContent='正在读取通道数据…';
    try{const curves=await fetchOverlays(next);state.overlaySpecs=next;state.overlayError='';wave.overlays=[...curves,...wave.overlays.filter(c=>c.is_trend)];wave.yRange=null;saveContext();wave.draw();$('channel-dialog').close();status('已添加通道 · 曲线按各自采样率对齐时间，主通道分析保持不变');}catch(error){$('channel-error').textContent=error.message;}finally{busy(false);}
  });
}
