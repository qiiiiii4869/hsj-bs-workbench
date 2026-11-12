'use strict';
(() => {
 let fileId=null;window.hsjBatchFrequencyOverrides={};
 const dialog=document.getElementById('batch-dialog'),details=document.createElement('details');details.id='batch-channel-frequency';
 const summary=document.createElement('summary');summary.textContent='按通道分别设置频谱 X 范围';details.append(summary);
 const selection=document.createElement('select');selection.id='batch-range-channel';selection.setAttribute('aria-label','频谱坐标设置通道');details.append(selection);
 const fields=document.createElement('div');fields.className='filter-fields';const inputs=[];
 for(const [title,value] of [['最小频率(Hz)',0],['最大频率(Hz)',50]]){const label=document.createElement('label'),input=document.createElement('input');label.textContent=title;input.type='number';input.step='any';input.min=0;input.value=value;label.append(input);fields.append(label);inputs.push(input);}details.append(fields);
 const actions=document.createElement('div');actions.className='page-actions';const apply=document.createElement('button'),clear=document.createElement('button'),note=document.createElement('p');apply.type=clear.type='button';apply.textContent='应用到此通道';clear.textContent='此通道恢复统一范围';note.className='method';actions.append(apply,clear);details.append(actions,note);dialog.querySelector('progress').before(details);
 const display=()=>{const range=window.hsjBatchFrequencyOverrides[selection.value];inputs[0].value=range?.[0]??document.getElementById('batch-freq-min').value;inputs[1].value=range?.[1]??document.getElementById('batch-freq-max').value;note.textContent=range?`此通道全部工况使用 ${range[0]}–${range[1]} Hz`:'此通道使用统一频谱范围或自动范围';};
 apply.addEventListener('click',()=>{const a=Number(inputs[0].value),b=Number(inputs[1].value);if(!Number.isFinite(a+b)||a<0||a>=b){note.textContent='请填写有效频率范围：0 ≤ 最小值 < 最大值';return;}window.hsjBatchFrequencyOverrides[selection.value]=[a,b];display();});clear.addEventListener('click',()=>{delete window.hsjBatchFrequencyOverrides[selection.value];display();});selection.addEventListener('change',display);
 document.getElementById('batch-open').addEventListener('click',()=>{const info=state.batchFile||state.file;if(!info)return;if(fileId!==info.file_id){fileId=info.file_id;window.hsjBatchFrequencyOverrides={};}selection.replaceChildren(...info.channels.filter(c=>c.fft_enabled).map(c=>{const option=document.createElement('option');option.value=c.index;option.textContent=c.name;return option;}));if([...selection.options].some(o=>Number(o.value)===state.channel))selection.value=state.channel;display();});
})();
