'use strict';
(() => {
 const renderBase=showResult,computeBase=compute,syncBase=sync,actionBase=action,bodyBase=body,loadChannelBase=loadChannel,clearDownBase=clearDown,loadTypeBase=loadType;
 if(kind==='jigger'){
   clearDown=function(id){
     if(['file','type'].includes(id))return clearDownBase(id);
     const group=id==='condition'?0:id==='condition_2'?1:Number(id.split('_')[1])>=8?1:0;
     const first=group*8,stop=first+8,start=id.startsWith('jigger_')?Number(id.split('_')[1])+1:first;
     for(let index=start;index<stop;index++)placeholder('jigger_'+index,'请先选择上一级');
     sync();
   };
   const onType=()=>{loadTypeBase();placeholder('condition_2','请选择第二组工况',(s.info?.conditions||[]).filter(c=>c.type===$('type').value).map(c=>option(c.index,c.name)));sync();};
   $('type').removeEventListener('change',loadTypeBase);$('type').addEventListener('change',onType);
   loadChannel=function(key){clearDown(key);const index=Number(key.split('_')[1]);if(index!==7&&index!==15&&$(key).value!=='')placeholder('jigger_'+(index+1),'请选择通道',jiggerChannels('jigger_'+(index+1)).map(c=>option(c.index,c.name)));sync();};
 }
 if(kind!=='jigger')loadChannel=function(key){loadChannelBase(key);if(['orbit','correlation'].includes(kind)){const x=channels().find(c=>String(c.index)===$('x_channel').value),y=channels().find(c=>String(c.index)===$('y_channel').value);if(x&&y){$('end').value=kind==='orbit'?Math.min(x.count,y.count)/x.sample_rate:x.count;if($('y_end'))$('y_end').value=y.count;}}};
 if(kind==='efficiency'){
   let sensorScope='',choices={};
   const changeMethod=()=>{
     const scope=$('file').value+'|'+$('condition').value;if(scope!==sensorScope){sensorScope=scope;choices={};}
     for(const [key] of fields())if($(key).value!=='')choices[key]=$(key).value;
     for(const [key] of fields()){
       const remembered=choices[key];placeholder(key,'请选择通道',channels().map(c=>option(c.index,c.name)));
       if(remembered!==undefined&&channels().some(c=>String(c.index)===remembered))$(key).value=remembered;
     }sync();
   };
   for(const key of ['head_method','flow_method']){const previous=$(key),replacement=previous.cloneNode(true);replacement.value=previous.value;previous.replaceWith(replacement);replacement.addEventListener('change',changeMethod);}
 }
 let jobId=null,pollTimer=null,pendingSide=null,sessions={};
 const sessionKey='hsj.specialty.sessions.v2.'+kind;
 try{sessions=JSON.parse(localStorage.getItem(sessionKey)||'{}');}catch{}
 const cloneValue=v=>JSON.parse(JSON.stringify(v));
 previewPeriod=async function(){
   if(!s.sampleSide){status('请先选择配重前或试重后数据',true);return;}
   if(!condition()||$('channel').value===''){status('请完成文件、工况和通道选择',true);return;}
   try{
     setBusy(true);const result=await api({...bodyBase(),module:'balance_preview'});s.sourceSnapshots??={};s.sourceSnapshots[s.sampleSide]={request:cloneValue(result.request),sample_rate:result.sample_rate};showResult(result);s.periodDuration=result.duration;const plot=s.plots[0];plot.role='period';plot.mode='zoom-x';const rawDraw=plot.draw.bind(plot);
     plot.draw=()=>{rawDraw();const c=plot.canvas,ctx=c.getContext('2d'),ratio=devicePixelRatio||1,limits=plot.range(),w=c.clientWidth,h=c.clientHeight;ctx.save();ctx.scale(ratio,ratio);ctx.beginPath();ctx.rect(65,35,w-85,h-85);ctx.clip();ctx.fillStyle='#d24830';for(const [time,value] of result.key_phases){if(time<limits[0][0]||time>limits[0][1])continue;const x=65+(time-limits[0][0])/(limits[0][1]-limits[0][0])*(w-85),y=h-50-(value-limits[1][0])/(limits[1][1]-limits[1][0])*(h-85);ctx.beginPath();ctx.arc(x,y,3,0,Math.PI*2);ctx.fill();}ctx.restore();};
     plot.canvas.addEventListener('pointerup',()=>{const range=plot.range()[0],keys=result.key_phases.filter(k=>k[0]>=Math.max(0,range[0])&&k[0]<=Math.min(result.duration,range[1]));if(keys.length<2){status('选区内不足两个键相点，请重新框选',true);return;}$('start').value=keys[0][0];$('end').value=keys.at(-1)[0];s.cycleSelections??={};s.cycleSelections[s.sampleSide]={...bodyBase(),start:keys[0][0],end:keys.at(-1)[0]};status(`已选择 ${keys.length-1} 个完整周期${keys.length>17?'，请缩短至16周期以内':''}`);plot.draw();});plot.draw();status('红点为键相位置；框选1至16个完整周期后点击计算');
   }catch(error){status(error.message,true);}finally{setBusy(false);}
 };
 function sourceKey(params){return [params.module,params.file_id,params.condition,params.x_channel,params.y_channel,params.channel,params.reference,params.exam_type].join('|');}
 function persist(){if(!s.result)return;const record={schema:'hsj-specialty-session-v2',module:kind,request:cloneValue(s.result.request||body()),title:s.result.title,curves:s.result.curves.map(c=>({name:c.name,color:c.color,visible:c.visible!==false})),ranges:s.plots.map(p=>cloneValue(p.limits)),filters:cloneValue(s.filters),before:s.before,after:s.after,cycleSelections:s.cycleSelections||{}};sessions[sourceKey(record.request)]=record;try{localStorage.setItem(sessionKey,JSON.stringify(sessions));}catch{}return record;}
 function styleResult(result,record){if(!record)return result;if(record.title)result.title=record.title;for(const c of result.curves){const style=record.curves?.find(v=>v.name===c.name);if(style){c.color=style.color;c.visible=style.visible;}}return result;}
 showResult=function(result){
   if(result.request){const record=sessions[sourceKey(result.request)];if(record)styleResult(result,record);}
   if(kind==='balance'&&result.module==='balance_sample'){
     const side=pendingSide||'before';s[side]=result;s.cycleSelections??={};s.cycleSelections[side]=cloneValue(result.request);
     const analyses=[['before','配重前'],['after','试重后']].filter(([key])=>s[key]);
     const combined={...result,title:'动平衡周期分析',curves:[],plots:[],rows:[],columns:['归属','文件 / 通道','项目','结果']};
     for(const [key,label] of analyses){const r=s[key];combined.plots.push({title:label+'：'+r.title,curves:r.curves,axes:r.axes});for(const plot of r.plots||[])combined.plots.push({...plot,title:label+'：'+plot.title});for(const row of r.rows)combined.rows.push([label,r.title,...row]);}
     const first=combined.plots.shift();combined.curves=first.curves;combined.axes=first.axes;combined.title=first.title;
     renderBase(combined);
   }else renderBase(result);
   if(result.request){s.activeRequest=cloneValue(result.request);const record=sessions[sourceKey(result.request)];if(record?.ranges)s.plots.forEach((p,i)=>{if(record.ranges[i]){p.limits=record.ranges[i];p.draw();}});}
 };
 body=function(){const params=bodyBase();if(kind==='balance'&&s.sampleSide){params.side=s.sampleSide;if(s.sourceSnapshots?.after?.sample_rate)params.after_sampling_rate=s.sourceSnapshots.after.sample_rate;}return params;};
 compute=async function(event){pendingSide=s.sampleSide;await computeBase(event);if(!$('error').textContent&&s.result)persist();pendingSide=null;};
 $('parameters').removeEventListener('submit',computeBase);$('parameters').addEventListener('submit',compute);
 sync=function(){syncBase();
   if(kind==='jigger'&&!isExternal()){
     const typeReady=!!s.info&&s.info.file_id===$('file').value&&$('type').value!=='';let complete=typeReady;
     $('condition_2').disabled=s.busy||!typeReady;
     for(let group=0;group<2;group++){
       const selected=group?$('condition_2').value:$('condition').value;
       let previous=typeReady&&selected!==''&&(s.info?.conditions||[]).some(c=>String(c.index)===selected&&c.type===$('type').value);
       for(let index=group*8;index<group*8+8;index++){
         const input=$('jigger_'+index);input.disabled=s.busy||!previous;
         previous=previous&&input.value!==''&&jiggerChannels('jigger_'+index).some(c=>String(c.index)===input.value);
       }complete=complete&&previous;
     }$('compute').disabled=s.busy||!complete;
   }
   $('batch-download').disabled=s.busy||!s.specialBatchFinished;const detached=['session-load','before','after','period-preview','decompose-only'];for(const button of $('toolbar').querySelectorAll('button'))if(detached.includes(button.dataset.action))button.disabled=s.busy;
 };
 function reportPayload(){if(s.activeRequest?.module&&kind!=='balance')return {source_request:cloneValue(s.activeRequest),title:s.result.title,include_graphs:$('report-graphs').checked,curve_styles:s.result.curves.map(c=>({name:c.name,color:c.color,visible:c.visible!==false}))};const report=cloneValue(reportResult(s.result));report.request=cloneValue(s.activeRequest||report.request||{});report.plots=s.plots.slice(1).map(p=>({title:p.title,axes:p.axes,curves:cloneValue(p.curves)}));report.waves=[];report.curves=cloneValue(s.plots[0].curves);report.title=s.result.title;report.include_graphs=$('report-graphs').checked;return report;}
 action=async function(act){
   if(s.busy)return;
   if(act==='decompose-only'){edit('独立配重分解',[['weight','配重重量',s.result?.weight||0],['angle','配重角度',s.result?.angle||0],['angle_1','分解角1',0],['angle_2','分解角2',90]],async values=>{setBusy(true);try{showResult(await api({module:'balance_decompose',...values}));persist();}finally{setBusy(false);}});return;}
   if(act==='session-load'){$('session-file').click();return;}
   if(act==='legacy-data'){if(!s.result?.legacy_text)return;const response=await fetch('/api/special-legacy',{method:'POST',headers:{'Content-Type':'application/json','X-HSJ-Token':s.token},body:JSON.stringify(s.result)});if(!response.ok)throw new Error((await response.json()).error);download(await response.blob(),s.result.title+'.'+s.result.legacy_extension);return;}
   if(act==='legacy-office'){
     if(!s.result)return;
     edit('原版 Office 报表',[['format','输出格式','xls','text',[['xls','XLS（本机 Excel）'],['doc','DOC（本机 Word）']]]],async value=>{
       setBusy(true);try{download(await api({...reportPayload(),format:value.format},true),s.result.title+'.'+value.format);}finally{setBusy(false);}
     });return;
   }
   if(act==='session-save'){const record=persist();if(record)download(new Blob([JSON.stringify(record,null,2)],{type:'application/json'}),'HSJ-'+kind+'-分析配置.json');return;}
   if(act==='batch'){
     if(!s.activeRequest?.file_id)return;
     try{
       const info=await post('/api/open',{file_id:s.activeRequest.file_id});
       const condition=info.conditions.find(c=>c.type===s.activeRequest.exam_type);
       $('batch-channels').replaceChildren(...(condition?.channels||[]).filter(c=>c.count>=4).map(c=>{const label=document.createElement('label'),input=document.createElement('input');input.type='checkbox';input.value=c.index;input.checked=c.index===s.activeRequest.channel;label.append(input,c.name);return label;}));
       $('batch-dialog').showModal();jobId=jobId||sessionStorage.getItem('hsj.special-batch.'+kind);if(jobId)poll();
     }catch(error){status(error.message,true);}return;
   }
   if(kind==='orbit'&&['filter-x','filter-y','restore-filter'].includes(act)){
     if(!s.activeRequest)return;
     const apply=async(axis,spec)=>{
       const params=cloneValue(s.activeRequest);params.orbit_filter_mode=$('orbit_filter_mode').value;params.x_filters??=[];params.y_filters??=[];
       if(axis===null){params.x_filters=[];params.y_filters=[];}else params[axis+'_filters'].push(spec);
       setBusy(true);try{const result=await api(params);s.filters={x:params.x_filters,y:params.y_filters};showResult(result);persist();$('filter-summary').textContent=`X滤波 ${params.x_filters.length} 步 / Y滤波 ${params.y_filters.length} 步`;status('当前轴心轨迹对应通道已更新');}finally{setBusy(false);}
     };
     if(act==='restore-filter'){try{await apply(null);}catch(error){status(error.message,true);}return;}
     const axis=act==='filter-x'?'x':'y';
     edit(axis.toUpperCase()+' 通道滤波',[['type','滤波类型','lowpass','text',[['lowpass','低通'],['highpass','高通'],['bandpass','带通'],['bandstop','带阻']]],['order','阶数',5],['low','低通截止 / 频带下限(Hz)',10],['high','高通截止 / 频带上限(Hz)',100]],async params=>{
       await apply(axis,params);
     });
     const syncCutoff=()=>{$('edit-low').disabled=$('edit-type').value==='highpass';$('edit-high').disabled=$('edit-type').value==='lowpass';};$('edit-type').addEventListener('change',syncCutoff);syncCutoff();return;
   }
   if(act==='xlsx'||act==='docx'){
     if(!s.result)return;setBusy(true);
     try{const payload=reportPayload();payload.format=act;download(await api(payload,true),s.result.title+'.'+act);status('已生成含图表、曲线及参数的报表');}catch(error){status(error.message,true);}finally{setBusy(false);}return;
   }
   if(act==='clear-period'&&kind==='balance'){
     edit('取消周期选择',[['side','需要取消的一侧','before','text',[['before','配重前'],['after','试重后']]]],value=>{delete s.cycleSelections?.[value.side];s.sampleSide=null;for(const p of s.plots)if(p.role==='period'){p.limits=null;p.history=[];p.draw();}status('该侧周期框选已取消，已计算的数据保留');sync();});return;
   }
   await actionBase(act);
 };
 $('session-file').addEventListener('change',async()=>{
   const file=$('session-file').files[0];if(!file)return;
   try{const record=JSON.parse(await file.text());if(record.schema!=='hsj-specialty-session-v2'||record.module!==kind||!record.request)throw new Error('配置与当前分析模块不匹配');setBusy(true);const result=await api(record.request);sessions[sourceKey(record.request)]=record;s.filters=record.filters||{x:[],y:[]};s.before=record.before;s.after=record.after;s.cycleSelections=record.cycleSelections||{};s.activeRequest=record.request;showResult(styleResult(result,record));persist();status('配置与真实数据已恢复');}catch(error){status(error.message,true);}finally{setBusy(false);$('session-file').value='';}
 });
 $('legend').addEventListener('change',persist);
 $('edit-form').addEventListener('submit',()=>setTimeout(persist,0));
 function batchState(record){$('progress').max=record.total;$('progress').value=record.done;$('batch-log').textContent=`${record.status} · ${record.done}/${record.total} · ${record.current}\n`+record.errors.map(e=>JSON.stringify(e)).join('\n');const running=['running','queued'].includes(record.status);s.specialBatchFinished=['completed','completed_with_errors'].includes(record.status);$('batch-go').disabled=running;$('batch-cancel').disabled=!running;$('batch-download').disabled=!s.specialBatchFinished;if(running)pollTimer=setTimeout(poll,700);}
 async function poll(){if(!jobId)return;try{batchState(await post('/api/special-batch-status',{id:jobId}));}catch(error){$('batch-log').textContent=error.message;$('batch-go').disabled=false;}}
 $('batch-form').addEventListener('submit',async event=>{
   event.preventDefault();if(s.busy)return;
   const items=[...$('batch-channels').querySelectorAll('input:checked')].map(c=>({channel:Number(c.value)}));if(!items.length){$('batch-log').textContent='请勾选通道';return;}
   try{
     const parameters={...cloneValue(s.activeRequest||body()),con:cloneValue(s.con),report_details:true};parameters.module=kind;
     const payload={parameters,items,include_graphs:$('batch-graphs').checked,output:$('batch-kind').value,format:$('batch-image-format').value,width:Number($('batch-width').value),height:Number($('batch-height').value),metrics:s.result.curves.filter(c=>c.visible!==false).map(c=>c.name),colors:Object.fromEntries(s.result.curves.map(c=>[c.name,c.color]))};
     if($('batch-custom-limits').checked)payload.limits=s.plots[0].range();
     const record=await post('/api/special-batch',payload);jobId=record.id;sessionStorage.setItem('hsj.special-batch.'+kind,jobId);clearTimeout(pollTimer);batchState(record);
   }catch(error){$('batch-log').textContent=error.message;}
 });
 $('batch-cancel').addEventListener('click',async()=>{if(jobId)try{batchState(await post('/api/special-batch-cancel',{id:jobId}));}catch(e){$('batch-log').textContent=e.message;}});
 $('batch-download').addEventListener('click',async()=>{if(!jobId)return;try{const r=await fetch('/api/special-batch-download',{method:'POST',headers:{'Content-Type':'application/json','X-HSJ-Token':s.token},body:JSON.stringify({id:jobId})});if(!r.ok)throw new Error((await r.json()).error);download(await r.blob(),'HSJ专项批量结果.zip');}catch(e){$('batch-log').textContent=e.message;}});
 // A previously submitted job remains discoverable after closing the dialog or refreshing.
 $('toolbar').addEventListener('click',event=>{if(event.target.dataset.action==='batch'){jobId=jobId||sessionStorage.getItem('hsj.special-batch.'+kind);if(jobId)poll();}});
 window.addEventListener('pagehide',()=>{clearTimeout(pollTimer);persist();});
})();
