'use strict';
    window.addEventListener('error',event=>{
      if(document.documentElement.dataset.workbenchReady==='true')return;
      const show=()=>{
        const banner=document.getElementById('startup-error');
        const detail=event.message||(event.target?.src?'无法加载 '+new URL(event.target.src).pathname:'页面资源加载失败');
        banner.textContent='页面初始化失败，请刷新页面。错误信息：'+detail;
        banner.hidden=false;
      };
      if(document.readyState==='loading')document.addEventListener('DOMContentLoaded',show,{once:true});else show();
    },true);
