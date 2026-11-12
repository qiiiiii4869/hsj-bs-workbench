'use strict';
(() => {
 if(kind!=='orbit')return;
 document.body.classList.add('orbit-layout');
 const main=document.querySelector('.layout>main'),toolbar=$('toolbar');
 const caption=document.createElement('h1');caption.id='orbit-caption';caption.textContent='轴心轨迹';main.prepend(caption);
 document.querySelector('.layout').before(toolbar);
 const create=document.createElement('button');create.type='button';create.id='orbit-new';create.textContent='新建轴心轨迹…';toolbar.prepend(create);
 const more=document.createElement('button');more.type='button';more.id='orbit-details';more.textContent='参数 / 结果明细…';toolbar.append(more);
 function dialog(id,title){const element=document.createElement('dialog');element.id=id;const heading=document.createElement('h2');heading.textContent=title;element.append(heading);document.body.append(element);element.addEventListener('cancel',event=>{if(s.busy)event.preventDefault();});return element;}
 const source=dialog('orbit-source-dialog','新建轴心轨迹'),detail=dialog('orbit-detail-dialog','轴心轨迹：参数与结果');
 source.append($('parameters'));source.querySelector('form>h2').hidden=true;$('compute').textContent='确定';
 const cancel=document.createElement('button');cancel.id='orbit-source-cancel';cancel.type='button';cancel.textContent='取消';$('compute').after(cancel);cancel.addEventListener('click',()=>{if(!s.busy)source.close();});
 const instructions=$('instructions');instructions.textContent='选择文件、试验类型、工况和 X / Y 通道后确定。未选择上一级时，下一级保持灰显。';source.querySelector('h2').after(instructions);
 const resultsTitle=document.createElement('h3');resultsTitle.textContent='分析结果';detail.append(resultsTitle,$('result').closest('.table-scroll'),$('legend'));
 const plotTools=document.createElement('section');plotTools.id='orbit-detailed-tools';detail.append(plotTools);
 const hint=document.createElement('p');hint.textContent='图内可框选缩放、滚轮缩放、平移与右键操作。X、Y 波形时间范围联动更新右侧轨迹。';detail.append(hint);
 const close=document.createElement('button');close.type='button';close.textContent='关闭';close.addEventListener('click',()=>{if(!s.busy)detail.close();});detail.append(close);
 const active=document.createElement('select');active.id='orbit-target';active.setAttribute('aria-label','图表操作对象');for(const [value,text] of [['x','X 波形'],['y','Y 波形'],['main','二维轴心轨迹']])active.append(option(value,text));
 const interactions=document.createElement('div');interactions.id='orbit-interactions';interactions.append(active);toolbar.insertBefore(interactions,more);
 const actions=[['X放大','X 放大'],['Y放大','Y 放大'],['XY放大','XY 放大'],['平移','平移'],['撤销','撤销'],['重绘','重绘']];
 const target=()=>s.plots.find(p=>p.role===active.value);
 for(const [original,title] of actions){const button=document.createElement('button');button.type='button';button.textContent=title;button.addEventListener('click',()=>{if(s.busy)return;const plot=target();if(!plot)return;const header=plot.canvas.closest('.plot').querySelector('header');[...header.querySelectorAll('button')].find(b=>b.textContent===original)?.click();});interactions.append(button);}
 create.addEventListener('click',()=>{if(s.busy)return;$('error').textContent='';source.showModal();});more.addEventListener('click',()=>{if(!s.busy)detail.showModal();});
 const originalSync=sync;sync=function(){originalSync();create.disabled=s.busy||!s.token;cancel.disabled=close.disabled=s.busy;more.disabled=s.busy;for(const control of interactions.querySelectorAll('button,select'))control.disabled=s.busy||!s.result;};
 function niceStep(span){const rough=span/6,power=10**Math.floor(Math.log10(rough||1)),ratio=rough/power;return(ratio<=1?1:ratio<=2?2:ratio<=5?5:10)*power;}
 const originalRange=Plot.prototype.range;
 Plot.prototype.range=function(){const range=originalRange.call(this);if(this.limits)return range;return range.map(([lo,hi])=>{const step=niceStep(hi-lo);return[Math.floor(lo/step)*step,Math.ceil(hi/step)*step];});};
 function ticks(lo,hi){const step=niceStep(hi-lo),values=[];for(let value=Math.ceil(lo/step)*step;value<=hi+step*1e-8&&values.length<100;value+=step)values.push(Math.abs(value)<step*1e-8?0:value);return values;}
 function tick(value){return Math.abs(value)>=1e6||value!==0&&Math.abs(value)<.0001?value.toExponential(2):String(Number(value.toPrecision(6)));}
 // Keep the Plot interaction coordinate transform unchanged: margins are the
 // same as in dataPoint(). Only the framing, grid and plot placement change.
 Plot.prototype.draw=function(){
   if(this.role==='main'){
     if(s.result){caption.textContent=s.result.title;document.title=s.result.title;}
     this.title='二维轴心轨迹';
   }
   const canvas=this.canvas,w=canvas.clientWidth||700,h=canvas.clientHeight||300,ratio=devicePixelRatio||1;canvas.width=Math.round(w*ratio);canvas.height=Math.round(h*ratio);const ctx=canvas.getContext('2d');ctx.scale(ratio,ratio);
   ctx.fillStyle='#dce3eb';ctx.fillRect(0,0,w,h);ctx.fillStyle='#fff';ctx.fillRect(0,0,w,28);const range=this.range(),left=65,right=w-20,top=35,bottom=h-50;
   const x=value=>left+(value-range[0][0])/(range[0][1]-range[0][0])*(right-left),y=value=>bottom-(value-range[1][0])/(range[1][1]-range[1][0])*(bottom-top);
   ctx.fillStyle='#fff';ctx.fillRect(left,top,right-left,bottom-top);ctx.font='12px "Microsoft YaHei",sans-serif';ctx.lineWidth=.65;
   for(const axis of [0,1])for(const value of ticks(...range[axis])){ctx.strokeStyle='#e7edf4';ctx.beginPath();if(axis){ctx.moveTo(left,y(value));ctx.lineTo(right,y(value));}else{ctx.moveTo(x(value),top);ctx.lineTo(x(value),bottom);}ctx.stroke();ctx.strokeStyle='#52687c';ctx.beginPath();if(axis){ctx.moveTo(left-4,y(value));ctx.lineTo(left,y(value));}else{ctx.moveTo(x(value),bottom);ctx.lineTo(x(value),bottom+5);}ctx.stroke();ctx.fillStyle='#52687c';ctx.textAlign=axis?'right':'center';ctx.fillText(tick(value),axis?left-7:x(value),axis?y(value)+4:bottom+20);}
   ctx.strokeStyle='#9bacc0';ctx.lineWidth=1;ctx.strokeRect(left,top,right-left,bottom-top);ctx.save();ctx.beginPath();ctx.rect(left,top,right-left,bottom-top);ctx.clip();
   for(const curve of this.curves){if(curve.visible===false)continue;ctx.strokeStyle=curve.color||'#0000ff';ctx.lineWidth=.75;ctx.beginPath();for(let index=0;index<curve.points.length;index++){const point=curve.points[index];if(index)ctx.lineTo(x(point[0]),y(point[1]));else ctx.moveTo(x(point[0]),y(point[1]));}ctx.stroke();}
   if(this.drag){ctx.fillStyle='#1762b522';ctx.fillRect(this.drag.start[0],this.drag.start[1],this.drag.end[0]-this.drag.start[0],this.drag.end[1]-this.drag.start[1]);}ctx.restore();
   ctx.fillStyle='#263b51';ctx.textAlign='center';ctx.font='13px "Microsoft YaHei",sans-serif';ctx.fillText(this.title,w/2,20,w-20);
   const header=this.canvas.closest('.plot').querySelector('header>span');if(header&&header.textContent!==this.title)header.textContent=this.title;
 };
 function arrange(){
   plotTools.replaceChildren();
   for(const plot of s.plots){const element=plot.canvas.closest('.plot');element.dataset.orbitPlot=plot.role;
     if(plot.role==='main')plot.title='二维轴心轨迹';
     element.querySelector('header>span').textContent=plot.title;
     element.setAttribute('aria-label',plot.title);
     // Detailed buttons proxy the original controls, leaving the engineering
     // plot itself clear while preserving every existing interaction command.
     const section=document.createElement('section'),heading=document.createElement('h3');heading.textContent=plot.title;section.append(heading);
     for(const sourceButton of element.querySelectorAll('header button')){const button=document.createElement('button');button.type='button';button.textContent=sourceButton.textContent;button.addEventListener('click',()=>{if(!s.busy)sourceButton.click();});section.append(button);}plotTools.append(section);
     plot.canvas.addEventListener('pointerdown',()=>active.value=plot.role);
     const observer=new MutationObserver(()=>{if(plot.output.textContent)$('status').textContent=plot.title+'　'+plot.output.textContent;});observer.observe(plot.output,{childList:true,subtree:true,characterData:true});const destroy=plot.destroy.bind(plot);plot.destroy=()=>{observer.disconnect();destroy();};plot.draw();
   }sync();
 }
 const previousShow=showResult;
 showResult=function(result){
   // Source labels come from the data which actually produced the plot, not
   // from an unsubmitted selection currently visible in the new-view dialog.
   if(result.module==='orbit'||result.request?.module==='orbit'){
     const info=result.source;
     if(info){
       if(!s.result||s.result.request?.file_id!==result.request?.file_id||s.result.request?.condition!==result.request?.condition||s.result.request?.x_channel!==result.request?.x_channel||s.result.request?.y_channel!==result.request?.y_channel)
         result.title=`${info.condition.type}_${info.condition.name},[上X]${info.x_channel.name},[下Y]${info.y_channel.name}轴心轨迹`;
       if(result.waves?.[0])result.waves[0].name=info.x_channel.name+' ('+info.x_channel.unit+')';
       if(result.waves?.[1])result.waves[1].name=info.y_channel.name+' ('+info.y_channel.unit+')';
     }
     for(const wave of result.waves||[])wave.color='#0000ff';for(const curve of result.curves||[])curve.color='#0000ff';
   }
   previousShow(result);arrange();if(source.open)source.close();
 };
 composite=function(){
   for(const plot of s.plots)plot.draw();const rectangle=$('plots').getBoundingClientRect(),scale=Math.max(1,1200/Math.max(1,rectangle.width));const canvas=document.createElement('canvas');canvas.width=Math.round(rectangle.width*scale);canvas.height=Math.round(rectangle.height*scale)+42;const ctx=canvas.getContext('2d');ctx.fillStyle='#fff';ctx.fillRect(0,0,canvas.width,canvas.height);ctx.fillStyle='#263b51';ctx.textAlign='center';ctx.font='17px "Microsoft YaHei"';ctx.fillText(s.result?.title||'轴心轨迹',canvas.width/2,26,canvas.width-30);
   for(const plot of s.plots){const rect=plot.canvas.getBoundingClientRect();ctx.drawImage(plot.canvas,(rect.left-rectangle.left)*scale,42+(rect.top-rectangle.top)*scale,rect.width*scale,rect.height*scale);}return canvas;
 };
 const previousAction=action;action=async function(name){if(name==='restore'){for(const plot of s.plots){plot.limits=null;plot.history=[];}for(const plot of s.plots)plot.draw();if(s.activeRequest&&!s.busy){try{setBusy(true);const result=await api(s.activeRequest);const plot=s.plots.find(p=>p.role==='main');plot.curves=result.curves;s.result.curves=result.curves;plot.draw();table(result);}catch(error){status(error.message,true);}finally{setBusy(false);}}return;}return previousAction(name);};
 // Empty views keep the same three-panel layout before the first analysis.
 s.plots=[new Plot($('plots'),'二维轴心轨迹',[],['X','Y'],'main'),new Plot($('plots'),'X 波形',[],['时间(s)','幅值'],'x'),new Plot($('plots'),'Y 波形',[],['时间(s)','幅值'],'y')];arrange();
})();
