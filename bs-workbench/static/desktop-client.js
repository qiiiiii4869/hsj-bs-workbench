'use strict';
(() => {
 const embedded=window.parent!==window&&new URLSearchParams(location.search).get('embedded')==='1';
 let host=null;try{if(embedded&&parent.location.origin===location.origin)host=parent.hsjDesktop;}catch{}
 window.hsjOpenView=(url,title)=>host?host.open(url,title):location.assign(url);
 window.hsjExit=async()=>{
   const initial=await fetch('/api/bootstrap');if(!initial.ok)throw new Error('无法连接本机服务');const data=await initial.json();
   const response=await fetch('/api/shutdown',{method:'POST',headers:{'Content-Type':'application/json','X-HSJ-Token':data.token},body:'{}'});
   if(!response.ok)throw new Error((await response.json()).error);
   if(host)host.closeAll();else{document.body.replaceChildren();const note=document.createElement('p');note.textContent='分析视图已关闭，正在停止服务。重新使用时请运行启动脚本。';document.body.append(note);}
 };
 if(!host)return;
 const originalOpen=window.open.bind(window);
 window.open=(url,target,features)=>{try{const u=new URL(url,location.href);if(u.origin===location.origin&&['/','/strip','/waterfall','/specialty','/balance','/docs'].includes(u.pathname)){host.open(u.pathname+u.search);return null;}}catch{}return originalOpen(url,target,features);};
 window.close=()=>host.close(window);
 document.addEventListener('click',event=>{const a=event.target.closest('a[href]');if(!a||a.download||event.defaultPrevented)return;const url=new URL(a.href);if(url.origin===location.origin&&['/','/strip','/waterfall','/specialty','/balance','/docs'].includes(url.pathname)){event.preventDefault();host.open(url.pathname+url.search,a.textContent.trim());}},true);
 document.addEventListener('pointerdown',()=>host.activate(window),true);
 document.addEventListener('keydown',event=>{if(event.ctrlKey&&event.key==='F4'){event.preventDefault();host.close(window);}if(event.ctrlKey&&event.key==='Tab'){event.preventDefault();host.next(window,event.shiftKey?-1:1);}});
 window.hsjCloseAllViews=()=>host.closeAll();
 document.addEventListener('DOMContentLoaded',()=>{
   const update=()=>{const title=document.querySelector('#classic-caption')?.textContent||document.querySelector('#orbit-caption')?.textContent||document.querySelector('#plots .plot header span')?.textContent||document.title;if(title)host.rename(window,title);};
   const observer=new MutationObserver(update);
   for(const target of [document.querySelector('title'),document.getElementById('classic-caption'),document.getElementById('orbit-caption'),document.getElementById('plots')])if(target)observer.observe(target,{childList:true,subtree:true,characterData:true});
   update();
 },{once:true});
})();
