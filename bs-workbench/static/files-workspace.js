'use strict';
(() => {
 state.openFiles??=new Map();const accept=acceptFile,select=selectChannel;let restoring=false;
 function reveal(){if(!state.file)return;const condition=state.file.conditions.find(c=>c.index===Number(document.getElementById('condition').value));state.revealSelection=[state.file.file_id,state.file.file_id+'|'+condition?.type,state.file.file_id+'|'+condition?.type+'|'+condition?.index];classicTree();}
 function locationContext(){if(!state.file)return;const url=new URL(location.href);url.searchParams.set('file_id',state.file.file_id);url.searchParams.set('condition',document.getElementById('condition').value);url.searchParams.set('channel',state.channel);history.replaceState(null,'',url.pathname+url.search);if(parent!==window&&parent.hsjDesktop)parent.hsjDesktop.rename(window,document.getElementById('title').textContent);}
 async function restore(){if(restoring||!state.token)return;restoring=true;try{const ids=JSON.parse(sessionStorage.getItem('hsj.open-file-ids')||'[]');for(const id of ids){if(state.openFiles.has(id))continue;try{state.openFiles.set(id,await request('/api/open',{file_id:id}));}catch{}}classicTree();}catch{}finally{restoring=false;}}
 acceptFile=async function(info){state.openFiles.set(info.file_id,info);await accept(info);try{const ids=JSON.parse(sessionStorage.getItem('hsj.open-file-ids')||'[]');sessionStorage.setItem('hsj.open-file-ids',JSON.stringify([...new Set([...ids,...state.openFiles.keys()])]));}catch{}reveal();locationContext();restore();};
 selectChannel=async function(index){await select(index);locationContext();reveal();};
 window.addEventListener('storage',event=>{if(event.key==='hsj.open-file-ids')restore();});
 document.getElementById('batch-open').addEventListener('click',()=>{const info=state.batchFile||state.file;if(info)document.getElementById('batch-heading').textContent='批量图像、数据与汇总报表 — '+info.filename;});
 if(state.file){state.openFiles.set(state.file.file_id,state.file);reveal();restore();}
})();
