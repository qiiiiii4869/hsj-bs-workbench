'use strict';
let viewTimer=null;
function scheduleViewAnalysis(){
  clearTimeout(viewTimer);if(!document.getElementById('auto-view')?.checked)return;
  const scope=state.activeKey;
  viewTimer=setTimeout(async()=>{if(!wave.ready()||state.activeKey!==scope||wave.drag)return;const range=[...wave.range],old=wave.history.at(-1);const success=await runAnalysis({start:Math.max(0,range[0]),end:Math.min(state.result.source.duration,range[1])},true,true);if(!success&&old){wave.restoreView(old);wave.history.pop();wave.draw();wave.syncTools();}},220);
}
function openSpan(){if(!wave.ready())return;$('span-error').textContent='';$('span-start').value=$('start').value;$('span-end').value=$('end').value||state.result.source.duration;$('span-dialog').showModal();}
function pagePayload(){return {...state.params,key_visible:$('key-visible').checked,overlays:overlaySpecs(),trends:trendSpecs(),throw_spec:state.throwSpec,annotations:[...wave.records(),...spectrum.records()]};}
async function fileDownload(route,body,name){const blob=await request(route,body,{download:true});if(window.hsjSaveBlob){await window.hsjSaveBlob(blob,name);return;}const url=URL.createObjectURL(blob),a=document.createElement('a');a.href=url;a.download=name;a.click();setTimeout(()=>URL.revokeObjectURL(url),15000);}
function setupPage(){
  $('strip-link').addEventListener('click',()=>{if(state.file)try{sessionStorage.setItem('hsj.strip-source',JSON.stringify({file_id:state.file.file_id,condition:Number($('condition').value)}));}catch{}});
  $('main-color').addEventListener('change',()=>{state.mainColor=$('main-color').value;if(state.params)state.params.main_color=state.mainColor;if(state.result)state.result.source.color=state.mainColor;saveContext();wave.draw();});
  $('key-visible').addEventListener('change',()=>wave.draw());
  $('auto-view').addEventListener('change',()=>{if($('auto-view').checked)scheduleViewAnalysis();});
  $('span-open').addEventListener('click',openSpan);$('span-cancel').addEventListener('click',()=>$('span-dialog').close());
  $('span-form').addEventListener('submit',async e=>{e.preventDefault();if(!wave.ready())return;const start=Number($('span-start').value),end=Number($('span-end').value);if(await runAnalysis({start,end,full_period:false},true)){ $('span-dialog').close();}else $('span-error').textContent=$('status').textContent;});
  $('throw-open').addEventListener('click',()=>{if(!wave.ready())return;$('throw-base').value=String(Math.max(wave.range[0],Number($('throw-width').value)/state.result.parameters.sample_rate));$('throw-end').value=String(wave.range[1]);$('throw-error').textContent='';$('throw-dialog').showModal();});
  $('throw-cancel').addEventListener('click',()=>$('throw-dialog').close());
  $('throw-form').addEventListener('submit',async e=>{e.preventDefault();if(!wave.ready())return;busy(true);try{const spec={width:Number($('throw-width').value),base:Number($('throw-base').value),final:Number($('throw-end').value),rising:$('throw-rise').checked};const r=await request('/api/throwload',{...pagePayload(),...spec});state.throwSpec=spec;wave.overlays=wave.overlays.filter(c=>c.key!=='throwload');wave.overlays.push(r.curve);wave.annotations=wave.annotations.filter(a=>a.curve!=='throwload');for(const a of r.annotations)wave.annotations.push({id:crypto.randomUUID(),...a});wave.persist();saveContext();wave.draw();$('throw-result').textContent=`基准值 ${fmt(r.base.y,4)}；${r.rising?'最大':'最小'}值 ${fmt(r.extreme.y,4)}；${r.rising?'上升':'下降'}率 ${fmt(r.percent,4)}%`;$('throw-dialog').close();status('已生成甩负荷趋势和两条专用标注');}catch(error){$('throw-error').textContent=error.message;}finally{busy(false);}});
}
