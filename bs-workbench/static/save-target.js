'use strict';
(() => {
 let selected=null,browserOnly=false;const buttons=new Set();
 const parentTarget=()=>{try{return parent!==window?parent.hsjSaveDirectory:null;}catch{return null;}};
 const current=()=>browserOnly?null:selected||parentTarget();
 const safe=name=>String(name).replace(/[\\/:*?"<>|\x00-\x1f]/g,'_').replace(/[. ]+$/,'').slice(0,180)||'HSJ结果';
 const notify=text=>{for(const id of ['status','strip-status']){const el=document.getElementById(id);if(el)el.textContent=text;}};
 function update(){const handle=current();for(const b of buttons)b.textContent=handle?'保存目录：'+handle.name:'选择保存目录';}function informChildren(){for(const frame of document.querySelectorAll('iframe'))frame.contentWindow?.postMessage({type:'hsj-save-target-changed'},location.origin);}window.addEventListener('message',event=>{if(event.origin===location.origin&&event.data?.type==='hsj-save-target-changed')update();});
 window.hsjChooseDirectory=async()=>{
   if(typeof window.showDirectoryPicker!=='function')throw new Error('当前浏览器不支持目录选择，请使用浏览器下载保存');
   const chosen=await window.showDirectoryPicker({mode:'readwrite',id:'hsj-analysis-output'});browserOnly=false;selected=chosen;window.hsjSaveDirectory=selected;update();informChildren();return selected;
 };
 const browserDownload=(blob,name)=>{const url=URL.createObjectURL(blob),a=document.createElement('a');a.href=url;a.download=name;a.click();setTimeout(()=>URL.revokeObjectURL(url),60000);};
 async function write(folder,blob,name){const handle=await folder.getFileHandle(safe(name),{create:true});const writer=await handle.createWritable();try{await writer.write(blob);await writer.close();}catch(error){try{await writer.abort();}catch{}throw error;}}
 window.hsjSaveBlob=async(blob,name)=>{
   const folder=current();if(!folder){browserDownload(blob,name);return;}
   try{await write(folder,blob,name);notify('已保存：'+folder.name+' / '+safe(name));}
   catch(error){notify('目录写入失败，改为浏览器下载：'+error.message);browserDownload(blob,name);}
 };
 window.hsjSaveFiles=async(files,title='HSJ批量结果',onProgress=()=>{})=>{
   const root=current();if(!root)return false;
   const stamp=new Date().toISOString().replace(/[:.]/g,'-');const folder=await root.getDirectoryHandle(safe(title+'-'+stamp),{create:true});
   let done=0;for(const file of files){await write(folder,file.blob,file.name);onProgress(++done,files.length);}notify(`已保存 ${done} 个文件到 ${root.name} / ${folder.name}`);return true;
 };
 window.hsjSaveExportJob=async(type,id)=>{
   let folder=current();if(!folder)folder=await window.hsjChooseDirectory();
   const bootstrap=await(await fetch('/api/bootstrap')).json();const headers={'Content-Type':'application/json','X-HSJ-Token':bootstrap.token};
   const listing=await fetch('/api/export-file-list',{method:'POST',headers,body:JSON.stringify({type,id})});if(!listing.ok)throw new Error((await listing.json()).error);const data=await listing.json();
   const output=await folder.getDirectoryHandle(safe('HSJ-'+type+'-'+id.slice(0,8)),{create:true});
   for(let i=0;i<data.files.length;i++){
     const file=data.files[i],response=await fetch('/api/export-file',{method:'POST',headers,body:JSON.stringify({type,id,index:file.index})});if(!response.ok)throw new Error((await response.json()).error);
     await write(output,await response.blob(),file.name);notify(`保存到目录 ${i+1}/${data.files.length}：${file.name}`);
   }notify(`已保存 ${data.files.length} 个文件到 ${folder.name} / ${output.name}`);
 };
 document.addEventListener('DOMContentLoaded',()=>{
   const bar=document.createElement('div');bar.className='hsj-save-target';const choose=document.createElement('button'),reset=document.createElement('button');choose.type=reset.type='button';buttons.add(choose);choose.textContent='选择保存目录';reset.textContent='使用浏览器下载';choose.addEventListener('click',async()=>{try{await window.hsjChooseDirectory();}catch(e){if(e.name!=='AbortError')notify(e.message);}});reset.addEventListener('click',()=>{browserOnly=true;selected=null;window.hsjSaveDirectory=null;update();informChildren();});bar.append(choose,reset);
   const host=document.getElementById('hsj-workspace')?.querySelector('.hsj-workspace-body')||document.querySelector('body>header')||document.body;host.append(bar);update();
   const wave=document.getElementById('batch-download');
   if(wave){const save=document.createElement('button');save.type='button';save.textContent='保存批量结果到目录';wave.after(save);const type=location.pathname==='/specialty'?'special':'wave';save.addEventListener('click',async()=>{try{const id=type==='special'?sessionStorage.getItem('hsj.special-batch.'+kind):batchId;if(!id)throw new Error('请先生成批量结果');await window.hsjSaveExportJob(type,id);}catch(e){if(e.name!=='AbortError')notify(e.message);}});}
 });
})();
