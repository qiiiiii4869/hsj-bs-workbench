'use strict';
// UI state is separate from raw files and isolated by the analysis fingerprint.
const chartMemory=new Map();
try{const saved=JSON.parse(localStorage.getItem('hsj.chartInteractions.v1')||'{}');for(const [k,v] of Object.entries(saved))chartMemory.set(k,v);}catch{}
let annotationEditor=null,contextChart=null;
function classicNiceStep(span,ticks){const rough=Math.max(Number.MIN_VALUE,span/ticks),power=10**Math.floor(Math.log10(rough));return [1,2,2.5,5,10].map(v=>v*power).reduce((best,v)=>Math.abs(v-rough)<Math.abs(best-rough)?v:best);}
function classicTicks(min,max,count){const step=classicNiceStep(max-min,count),start=Math.ceil(min/step)*step,out=[];for(let i=0;i<200;i++){const v=start+i*step;if(v>max+step*1e-8)break;out.push(Math.abs(v)<step*1e-8?0:v);}return out;}

class Chart {
  constructor(id,xunit){
    this.canvas=document.getElementById(id);this.id=id;this.xunit=xunit;this.unit='';
    this.data=[];this.rootData=[];this.comparison=[];this.overlays=[];this.activeCurve='main';this.domain=[0,1];this.range=[0,1];this.yRange=null;this.history=[];
    this.cursor=null;this.mode='pan';this.annotations=[];this.annotationBoxes=[];this.scope='';
    this.drag=null;this.pickSequence=0;this.pickBusy=false;this.pendingPick=null;this.detailSequence=0;
    this.canvas.tabIndex=0;
    new ResizeObserver(()=>this.draw()).observe(this.canvas);
    this.canvas.addEventListener('wheel',e=>{
      if(!this.ready())return;e.preventDefault();if(this.drag)return;
      const previous=this.view(),p=this.coordinates(e),factor=e.deltaY>0?1.25:.8;
      this.detailSequence++;clearTimeout(this.detailTimer);this.data=this.rootData;
      const zoomY=this.mode==='zoom-y'||e.altKey,zoomX=!zoomY||this.mode==='zoom-xy';
      if(zoomX){const span=this.range[1]-this.range[0];this.range=[p.x-p.fx*span*factor,p.x+(1-p.fx)*span*factor];this.clamp();}
      if(zoomY||this.mode==='zoom-xy'){const span=this.transform.ymax-this.transform.ymin;this.yRange=[p.y-(1-p.fy)*span*factor,p.y+p.fy*span*factor];}
      this.commitView(previous);this.loadDetail();
    },{passive:false});
    this.canvas.addEventListener('pointerdown',e=>this.pointerDown(e));
    this.canvas.addEventListener('pointermove',e=>this.pointerMove(e));
    this.canvas.addEventListener('pointerup',()=>this.pointerUp());
    this.canvas.addEventListener('pointercancel',()=>this.cancelGesture());
    this.canvas.addEventListener('lostpointercapture',()=>{if(this.drag)this.cancelGesture();});
    this.canvas.addEventListener('dblclick',e=>{const box=this.hitAnnotation(e);if(box&&this.ready()){openAnnotation(this,box.id);return;}if(['pan','pan-xy'].includes(this.mode))this.reset();});
    this.canvas.addEventListener('contextmenu',e=>{e.preventDefault();if(this.ready())openChartMenu(this,e.clientX,e.clientY);});
    this.canvas.addEventListener('keydown',e=>this.keyDown(e));
    this.createTools();
  }
  ready(){return !!state.result&&!state.busy&&!state.dirty&&!!this.data.length;}
  bind(result){
    const fingerprint=this.id==='wave'?result.wave_signature||result.analysis_signature:result.analysis_signature;
    const key=`${state.activeKey}|${fingerprint}|${this.id}`;
    if(key!==this.scope){
      this.persist();this.scope=key;const saved=chartMemory.get(key)||{};
      this.mode=['pan','pan-xy','zoom-x','zoom-y','zoom-xy','float','nearest','annotation'].includes(saved.mode)?saved.mode:'pan';
      this.annotations=Array.isArray(saved.annotations)?saved.annotations.filter(a=>a&&typeof a.text==='string'&&Number.isFinite(a.x)&&Number.isFinite(a.y)).map(a=>({...a,dx:Number.isFinite(a.dx)?a.dx:.03,dy:Number.isFinite(a.dy)?a.dy:-.12})):[];
      this.cursor=saved.cursor&&Number.isFinite(saved.cursor.x)&&Number.isFinite(saved.cursor.y)?{...saved.cursor}:null;
    }
    this.analysisId=result.analysis_id;this.signature=fingerprint;this.pickSequence++;this.pendingPick=null;
  }
  createTools(){
    const host=document.getElementById(`${this.id}-tools`);
    const label=document.createElement('label');label.textContent='图表操作';
    const select=document.createElement('select');select.setAttribute('aria-label',`${this.id==='wave'?'波形':'频谱'}游标与操作模式`);
    for(const [value,text] of [['pan','X 轴平移'],...(this.id==='wave'?[['pan-xy','XY 轴平移']]:[]),['zoom-x','框选放大 X'],...(this.id==='wave'?[['zoom-y','框选放大 Y']]:[]),['zoom-xy','框选放大 XY'],['float','捡取浮动点'],['nearest','捡取临近点'],['annotation','移动标注位置']]){const option=document.createElement('option');option.value=value;option.textContent=text;select.append(option);}
    select.addEventListener('change',()=>this.setMode(select.value));label.append(select);host.append(label);this.modeSelect=select;
    this.toolButtons={};
    for(const [action,text] of [['undo','撤销缩放 / 平移'],['redraw','重绘全图'],...(this.id==='wave'?[['analyze-view','按当前视区分析']]:[]),['add','添加标注'],['extrema','标注当前范围极值'],['move','移动标注位置'],['clear','清除标注'],['png','保存当前图 PNG']]){
      const button=document.createElement('button');button.type='button';button.textContent=text;button.addEventListener('click',()=>this.action(action));host.append(button);this.toolButtons[action]=button;
    }
    const details=document.createElement('details');details.className='annotation-list';const summary=document.createElement('summary');summary.textContent='标注列表（0）';this.annotationSummary=summary;this.annotationList=document.createElement('ul');this.annotationDetails=details;details.append(summary,this.annotationList);host.after(details);
    this.syncTools();
    this.viewOutput=document.createElement('output');this.viewOutput.className='chart-view-output';host.append(this.viewOutput);
  }
  syncTools(){
    if(!this.modeSelect)return;const ready=this.ready();this.modeSelect.disabled=!ready;this.modeSelect.value=this.mode;
    for(const [key,button] of Object.entries(this.toolButtons))button.disabled=!ready||(key==='add'&&!(this.mode==='nearest'&&this.cursor?.exact))||(key==='clear'&&!this.annotations.length)||(key==='undo'&&!this.history.length);
    this.toolButtons.undo.title=`可撤销 ${this.history.length} 步；仅影响当前图坐标`;
    this.toolButtons.move.setAttribute('aria-pressed',String(this.mode==='annotation'));
    this.annotationSummary.textContent=`标注列表（${this.annotations.length}）`;
    this.annotationDetails.hidden=!this.annotations.length||!this.data.length;
    this.annotationList.replaceChildren(...this.annotations.map(a=>{
      const li=document.createElement('li'),text=document.createElement('span');text.textContent=`${a.channel_name||''} ${a.text} · ${fmt(a.x,6)} ${this.xunit} / ${fmt(a.y,6)} ${a.unit??this.unit}`;
      const edit=document.createElement('button');edit.type='button';edit.textContent='修改文字';edit.disabled=!ready;edit.addEventListener('click',()=>openAnnotation(this,a.id));
      const remove=document.createElement('button');remove.type='button';remove.textContent='删除';remove.disabled=!ready;remove.addEventListener('click',()=>{this.annotations=this.annotations.filter(row=>row.id!==a.id);this.persist();this.draw();this.syncTools();});li.append(text,edit,remove);return li;
    }));
    if(contextChart===this&&!document.getElementById('chart-menu').hidden)syncChartMenu();
    if(typeof classicReady!=='undefined'&&classicReady)syncClassic();
  }
  persist(){
    if(!this.scope)return;
    chartMemory.set(this.scope,{mode:this.mode,cursor:this.cursor?{...this.cursor}:null,annotations:this.annotations.map(a=>({...a}))});
    try{localStorage.setItem('hsj.chartInteractions.v1',JSON.stringify(Object.fromEntries(chartMemory)));}catch{}
  }
  set(data,comparison=[]){
    this.pickSequence++;this.detailSequence++;clearTimeout(this.detailTimer);this.pendingPick=null;
    if(!data.length){this.persist();this.scope='';this.annotations=[];this.cursor=null;this.overlays=[];this.activeCurve='main';this.analysisId=null;this.signature=null;}
    this.data=data;this.rootData=data;this.comparison=comparison;this.domain=data.length?[data[0][0],data.at(-1)[0]]:[0,1];
    this.history=[];this.yRange=null;this.drag=null;this.range=[...this.domain];this.draw();this.syncTools();this.writeCursor();
  }
  view(){return {range:[...this.range],yRange:this.yRange?[...this.yRange]:null};}
  restoreView(view){this.range=[...view.range];this.yRange=view.yRange?[...view.yRange]:null;this.data=this.rootData;}
  commitView(previous){
    if(JSON.stringify(previous)!==JSON.stringify(this.view())){this.history.push(previous);this.pickSequence++;this.pendingPick=null;}
    this.draw();this.syncTools();
    if(this.id==='wave')scheduleViewAnalysis();
  }
  undo(){if(!this.ready()||!this.history.length)return;this.cancelGesture();this.restoreView(this.history.pop());this.pickSequence++;this.pendingPick=null;this.draw();this.syncTools();this.loadDetail();if(this.id==='wave')scheduleViewAnalysis();}
  reset(){this.cancelGesture();this.pickSequence++;this.pendingPick=null;this.detailSequence++;clearTimeout(this.detailTimer);this.data=this.rootData;this.range=[...this.domain];this.yRange=null;this.history=[];this.draw();this.syncTools();if(this.ready()){this.loadDetail();if(this.id==='wave')scheduleViewAnalysis();}}
  setXRange(range,record=true){this.cancelGesture();this.detailSequence++;clearTimeout(this.detailTimer);const previous=this.view();this.range=[...range];this.clamp();this.data=this.rootData;if(record)this.commitView(previous);else {this.draw();this.syncTools();}if(this.ready())this.loadDetail();}
  clamp(){const [min,max]=this.domain,minSpan=(max-min)/10000||1;const span=Math.min(max-min,Math.max(minSpan,this.range[1]-this.range[0]));const start=Math.max(min,Math.min(max-span,this.range[0]));this.range=[start,start+span];}
  cancelGesture(){if(!this.drag)return;const drag=this.drag;this.drag=null;if(drag.previous)this.restoreView(drag.previous);if(drag.kind==='annotation'){drag.annotation.dx=drag.originalDx;drag.annotation.dy=drag.originalDy;}this.draw();this.syncTools();}
  coordinates(e){
    const rect=this.canvas.getBoundingClientRect(),t=this.transform||{x:58,y:20,w:Math.max(1,rect.width-78),h:Math.max(1,rect.height-54),ymin:-1,ymax:1};
    const fx=Math.max(0,Math.min(1,(e.clientX-rect.left-t.x)/t.w)),fy=Math.max(0,Math.min(1,(e.clientY-rect.top-t.y)/t.h));
    return {x:this.range[0]+fx*(this.range[1]-this.range[0]),y:t.ymax-fy*(t.ymax-t.ymin),fx,fy};
  }
  setMode(mode){
    if(!this.ready())return;this.cancelGesture();this.mode=mode;this.pickSequence++;this.pendingPick=null;
    if(!['float','nearest','annotation'].includes(mode))this.cursor=null;
    if(mode==='float'){const t=this.transform;this.cursor={x:(this.range[0]+this.range[1])/2,y:(t.ymin+t.ymax)/2,exact:false};}
    if(mode==='nearest'){this.cursor=null;this.pick({x:(this.range[0]+this.range[1])/2,y:(this.transform.ymin+this.transform.ymax)/2});}
    this.draw();this.syncTools();this.writeCursor();this.persist();
  }
  query(action,extra={}){
    const t=this.transform,target=curveInfo(this);return request('/api/chart',{analysis_id:['nearest','index','extrema'].includes(action)?target.analysis_id:this.analysisId,plot:this.id,action,x_min:this.range[0],x_max:this.range[1],y_min:t.ymin,y_max:t.ymax,width:t.w,height:t.h,...extra});
  }
  async pick(position){
    const seq=++this.pickSequence;
    if(this.pickBusy){this.pendingPick=position;return;}
    this.pickBusy=true;const analysisId=this.analysisId;
    document.getElementById(`${this.id}-cursor`).textContent='正在读取完整计算数据中的临近点…';
    try{
      const response=await this.query('nearest',position);
      if(seq!==this.pickSequence||analysisId!==this.analysisId||this.mode!=='nearest'||!this.ready())return;
      this.cursor={...response.point,curve:curveInfo(this).key};this.writeCursor();this.draw();this.syncTools();
    }catch(error){if(seq===this.pickSequence&&analysisId===this.analysisId)status(error.message,true);}
    finally{this.pickBusy=false;const pending=this.pendingPick;this.pendingPick=null;if(pending&&this.mode==='nearest'&&this.ready())this.pick(pending);else this.persist();}
  }
  pointerDown(e){
    if(e.button!==0||!this.ready())return;this.canvas.focus();this.canvas.setPointerCapture(e.pointerId);
    const box=this.hitAnnotation(e);
    // Annotation labels take precedence over every chart mode, so a newly
    // created label is immediately draggable without changing the cursor tool.
    if(box){
      e.preventDefault();this.pickSequence++;this.pendingPick=null;this.detailSequence++;clearTimeout(this.detailTimer);
      const annotation=this.annotations.find(a=>a.id===box.id),t=this.transform;
      const ax=t.x+(annotation.x-this.range[0])/(this.range[1]-this.range[0])*t.w,ay=t.y+t.h-(annotation.y-t.ymin)/(t.ymax-t.ymin)*t.h;
      this.selectedAnnotation=annotation.id;this.drag={kind:'annotation',annotation,x:e.clientX,y:e.clientY,ax,ay,box:{...box},transform:{...t},originalDx:annotation.dx,originalDy:annotation.dy};this.canvas.style.cursor='grabbing';return;
    }
    if(this.mode==='annotation'){status('在标注文字框上按住鼠标拖动，可调整位置');return;}
    if(['pan','pan-xy'].includes(this.mode)){this.detailSequence++;clearTimeout(this.detailTimer);this.data=this.rootData;this.drag={kind:'pan',x:e.clientX,y:e.clientY,previous:this.view(),transform:{...this.transform},mode:this.mode};return;}
    if(this.mode.startsWith('zoom-')){this.detailSequence++;clearTimeout(this.detailTimer);const point=this.coordinates(e);this.drag={kind:'zoom',start:point,end:point,previous:this.view(),mode:this.mode};this.draw();return;}
    this.drag={kind:'cursor'};const p=this.coordinates(e);if(this.mode==='float'){this.cursor={x:p.x,y:p.y,exact:false};this.writeCursor();this.draw();}else this.pick({x:p.x,y:p.y});
  }
  pointerMove(e){
    if(!this.ready())return;
    if(!this.drag){this.canvas.style.cursor=this.hitAnnotation(e)?'grab':this.mode==='annotation'?'move':this.mode.startsWith('pan')?'grab':'crosshair';return;}
    if(this.drag.kind==='pan'){const d=this.drag,t=d.transform,delta=(e.clientX-d.x)/t.w*(d.previous.range[1]-d.previous.range[0]);this.range=d.previous.range.map(v=>v-delta);this.clamp();if(d.mode==='pan-xy'){const dy=(e.clientY-d.y)/t.h*(t.ymax-t.ymin);this.yRange=[t.ymin+dy,t.ymax+dy];}this.draw();return;}
    if(this.drag.kind==='zoom'){this.drag.end=this.coordinates(e);this.draw();return;}
    if(this.drag.kind==='annotation'){const d=this.drag,t=d.transform;const x=Math.max(t.x,Math.min(t.x+t.w-d.box.w,d.box.x+e.clientX-d.x)),y=Math.max(t.y,Math.min(t.y+t.h-d.box.h,d.box.y+e.clientY-d.y));d.annotation.dx=(x-d.ax)/t.w;d.annotation.dy=(y-d.ay)/t.h;this.draw();this.canvas.style.cursor='grabbing';return;}
    const p=this.coordinates(e);if(this.mode==='float'){this.cursor={x:p.x,y:p.y,exact:false};this.writeCursor();this.draw();}else if(this.mode==='nearest')this.pick({x:p.x,y:p.y});
  }
  pointerUp(){
    const drag=this.drag;this.drag=null;if(!drag)return;
    if(drag.kind==='zoom'){
      const x=drag.mode!=='zoom-y',y=drag.mode!=='zoom-x',a=drag.start,b=drag.end;
      if((x&&Math.abs(a.fx-b.fx)*this.transform.w<5)||(y&&Math.abs(a.fy-b.fy)*this.transform.h<5)){this.draw();status('框选范围太小，请按住鼠标拖出一个范围');return;}
      if(x){this.range=[Math.min(a.x,b.x),Math.max(a.x,b.x)];this.clamp();}
      if(y)this.yRange=[Math.min(a.y,b.y),Math.max(a.y,b.y)];
      this.commitView(drag.previous);this.loadDetail();
    }else if(drag.kind==='pan'){this.commitView(drag.previous);this.loadDetail();}
    this.persist();this.draw();
  }
  hitAnnotation(e){const rect=this.canvas.getBoundingClientRect(),x=e.clientX-rect.left,y=e.clientY-rect.top;return [...this.annotationBoxes].reverse().find(b=>x>=b.x&&x<=b.x+b.w&&y>=b.y&&y<=b.y+b.h);}
  writeCursor(){
    const output=document.getElementById(`${this.id}-cursor`);
    if(!this.cursor){output.textContent=this.mode==='nearest'?'单击或拖动以捡取完整计算数据中的临近点':this.mode==='float'?'单击或拖动以捡取浮动坐标':'请选择捡取模式定位游标';return;}
    const c=this.cursor,target=curveInfo(this);output.textContent=`${target.name} · ${c.exact?'临近点':'浮动坐标'}：${fmt(c.x,6)} ${this.xunit} / ${fmt(c.y,6)} ${target.unit}${c.exact?` · ${this.id==='wave'?'采样点':'频谱序号'} ${c.sample_index??c.index}`:''}`;
  }
  loadDetail(){
    clearTimeout(this.detailTimer);const seq=++this.detailSequence,analysisId=this.analysisId;
    const rangeKey=JSON.stringify(this.range);
    this.detailTimer=setTimeout(async()=>{if(!this.ready())return;try{const r=await this.query('detail');if(seq===this.detailSequence&&analysisId===this.analysisId){this.data=r.data;this.draw();}if(this.id==='wave'){for(const curve of this.overlays.filter(c=>c.visible)){if(seq!==this.detailSequence||analysisId!==this.analysisId)return;try{const response=await this.query('detail',{analysis_id:curve.analysis_id});if(seq===this.detailSequence&&analysisId===this.analysisId){curve.detail=response.data;curve.detailRange=rangeKey;this.draw();}}catch{}}}}catch(error){if(seq===this.detailSequence&&analysisId===this.analysisId)status(error.message,true);}},150);
  }
  async keyDown(e){
    if(!this.ready())return;
    if((e.ctrlKey||e.metaKey)&&e.key.toLowerCase()==='z'){e.preventDefault();this.undo();return;}
    if(e.key==='Escape'){e.preventDefault();this.cancelGesture();return;}
    if(e.key==='Enter'){e.preventDefault();openAnnotation(this);return;}
    if(e.key==='Delete'&&this.mode==='annotation'&&this.selectedAnnotation){this.annotations=this.annotations.filter(a=>a.id!==this.selectedAnnotation);this.persist();this.draw();this.syncTools();return;}
    if(!['ArrowLeft','ArrowRight','ArrowUp','ArrowDown'].includes(e.key)||!this.cursor)return;
    e.preventDefault();
    if(this.mode==='float'){
      const step=e.shiftKey?10:1,t=this.transform;if(e.key==='ArrowLeft')this.cursor.x-=step*(this.range[1]-this.range[0])/t.w;if(e.key==='ArrowRight')this.cursor.x+=step*(this.range[1]-this.range[0])/t.w;if(e.key==='ArrowUp')this.cursor.y+=step*(t.ymax-t.ymin)/t.h;if(e.key==='ArrowDown')this.cursor.y-=step*(t.ymax-t.ymin)/t.h;
      this.cursor.x=Math.max(this.range[0],Math.min(this.range[1],this.cursor.x));this.cursor.y=Math.max(t.ymin,Math.min(t.ymax,this.cursor.y));
    }else if(this.mode==='nearest'&&this.cursor.exact&&['ArrowLeft','ArrowRight'].includes(e.key)){
      const analysisId=this.analysisId,seq=++this.pickSequence;try{const r=await this.query('index',{index:this.cursor.index+(e.key==='ArrowLeft'?-1:1)*(e.shiftKey?10:1)});if(seq!==this.pickSequence||analysisId!==this.analysisId||this.mode!=='nearest')return;this.cursor={...r.point,curve:curveInfo(this).key};}catch(error){status(error.message,true);return;}
    }
    this.writeCursor();this.draw();this.syncTools();this.persist();
  }
  async action(action){
    if(!this.ready())return;
    const pageActions={channels:'channel-open',trend:'trend-open',filter:'filter-open',restore:'filter-restore',span:'span-open',throwload:'throw-open',output:'output-open',batch:'batch-open',title:'title-open'};
    if(pageActions[action])return document.getElementById(pageActions[action]).click();
    if(action==='undo')return this.undo();
    if(action==='redraw'){
      if(this.id==='wave'){await runAnalysis({start:0,end:null},true);return;}
      return this.reset();
    }
    if(action==='analyze-view'){
      const range=[...this.range],old={start:document.getElementById('start').value,end:document.getElementById('end').value};
      // The drawn domain ends on the last included sample; the compute interval
      // uses an exclusive upper endpoint.
      const rate=state.result.parameters.sample_rate;
      const lo=Math.ceil(range[0]*rate),hi=Math.min(state.result.parameters.end_index_exclusive,Math.floor(range[1]*rate)+1);
      if(hi-lo<4){status('当前视区不足 4 个采样点，请扩大范围',true);return;}
      document.getElementById('start').value=String(lo/rate);document.getElementById('end').value=String(hi/rate);
      const success=await runAnalysis();if(!success){document.getElementById('start').value=old.start;document.getElementById('end').value=old.end;}return;
    }
    if(action==='add')return openAnnotation(this);
    if(action==='move')return this.setMode(this.mode==='annotation'?'pan':'annotation');
    if(action==='clear'){this.annotations=[];this.selectedAnnotation=null;this.persist();this.draw();this.syncTools();return;}
    if(action==='png')return this.savePNG();
    if(action==='extrema'){
      const analysisId=this.analysisId,target=curveInfo(this);try{const r=await this.query('extrema');if(analysisId!==this.analysisId||target.key!==curveInfo(this).key||!this.ready())return;for(const [name,p] of [['最大值',r.maximum],['最小值',r.minimum]])this.annotations.push({id:crypto.randomUUID(),text:name,x:p.x,y:p.y,index:p.index,sample_index:p.sample_index,curve:target.key,unit:target.unit,channel_name:target.name,analysis_signature:target.signature,dx:.03,dy:name==='最大值'?-.12:.12});this.persist();this.draw();this.syncTools();}catch(error){status(error.message,true);}
    }
  }
  records(){return this.annotations.map(a=>({plot:this.id,text:a.text,x:a.x,y:a.y,dx:a.dx,dy:a.dy,curve:a.curve||'main',unit:a.unit??this.unit,channel_name:a.channel_name||state.result?.source.channel.name||'',analysis_signature:a.analysis_signature||this.signature}));}
  savePNG(){
    this.draw();const curves=this.overlays.filter(c=>c.visible),header=64+curves.length*20;const out=document.createElement('canvas'),ratio=devicePixelRatio||1;out.width=this.canvas.width;out.height=this.canvas.height+Math.round(header*ratio);const ctx=out.getContext('2d');ctx.fillStyle='#fff';ctx.fillRect(0,0,out.width,out.height);ctx.scale(ratio,ratio);ctx.fillStyle='#203247';ctx.font='15px "Microsoft YaHei",sans-serif';ctx.fillText(document.getElementById('title').textContent,16,24,this.canvas.clientWidth-32);ctx.font='11px "Microsoft YaHei",sans-serif';ctx.fillText(`${this.id==='wave'?'时域波形':'幅值频谱'} · 主通道 ${state.result.source.channel.name} · ${state.result.source.channel.unit}`,16,46,this.canvas.clientWidth-32);curves.forEach((c,i)=>{ctx.fillStyle=c.color;ctx.fillRect(16,60+i*20,20,3);ctx.fillStyle='#203247';ctx.fillText(`${c.channel.name} · ${c.channel.unit} · ${c.is_trend?`宽 ${c.trend.width} / 步 ${c.trend.step} 点 · ${c.processing}`:`${c.channel.sample_rate} Hz · 原始数据`}`,44,64+i*20,this.canvas.clientWidth-60);});ctx.drawImage(this.canvas,0,header,this.canvas.clientWidth,this.canvas.clientHeight);
    out.toBlob(blob=>{if(!blob)return;if(window.hsjSaveBlob){window.hsjSaveBlob(blob,`HSJ-${this.id}-标注.png`);return;}const url=URL.createObjectURL(blob),link=document.createElement('a');link.href=url;link.download=`HSJ-${this.id}-标注.png`;link.click();setTimeout(()=>URL.revokeObjectURL(url),15000);},'image/png');
  }
  draw(){
    const c=this.canvas,w=c.clientWidth,h=c.clientHeight;if(!w||!h)return;
    const scale=devicePixelRatio||1;c.width=Math.round(w*scale);c.height=Math.round(h*scale);const ctx=c.getContext('2d');ctx.scale(scale,scale);ctx.fillStyle='#fff';ctx.fillRect(0,0,w,h);
    const classic=document.body.classList.contains('classic-layout');
    const box=classic?{x:38,y:9,w:Math.max(1,w-46),h:Math.max(1,h-34)}:{x:58,y:20,w:Math.max(1,w-78),h:Math.max(1,h-54)},visible=this.data.filter(p=>p[0]>=this.range[0]&&p[0]<=this.range[1]);let ymin=Infinity,ymax=-Infinity;
    const rangeKey=JSON.stringify(this.range),curveData=c=>c.detailRange===rangeKey?c.detail:c.waveform;
    for(const dataset of [visible,this.comparison,...this.overlays.filter(c=>c.visible).map(curveData)])for(const p of dataset){if(p[0]>=this.range[0]&&p[0]<=this.range[1]){ymin=Math.min(ymin,p[1]);ymax=Math.max(ymax,p[1]);}}
    if(!Number.isFinite(ymin)){ymin=-1;ymax=1;}
    if(classic){const step=classicNiceStep(ymax-ymin||Math.max(1,Math.abs(ymax)),this.id==='wave'?10:12);ymin=this.id==='spectrum'?0:Math.floor(ymin/step)*step;ymax=Math.ceil(ymax/step)*step;if(ymax<=ymin)ymax=ymin+step;}
    else{const pad=(ymax-ymin)*.12||Math.max(Math.abs(ymax)*.1,1);ymin-=pad;ymax+=pad;if(this.id==='spectrum')ymin=0;}
    if(this.yRange){ymin=this.yRange[0];ymax=this.yRange[1];}
    this.transform={...box,ymin,ymax};const px=x=>box.x+(x-this.range[0])/(this.range[1]-this.range[0]||1)*box.w,py=y=>box.y+box.h-(y-ymin)/(ymax-ymin)*box.h;
    this.canvas.style.cursor=this.mode==='annotation'?'move':this.mode.startsWith('pan')?'grab':'crosshair';
    if(this.viewOutput)this.viewOutput.textContent=`视区 X：${fmt(this.range[0],5)}–${fmt(this.range[1],5)} ${this.xunit}　Y：${fmt(ymin,5)}–${fmt(ymax,5)} ${this.axisUnit||this.unit}`;
    ctx.font=classic?'11px "Segoe UI","Microsoft YaHei",sans-serif':'10px "Segoe UI","Microsoft YaHei",sans-serif';ctx.lineWidth=1;
    if(classic){
      for(const value of classicTicks(this.range[0],this.range[1],Math.max(5,Math.floor(box.w/62)))){const x=px(value);ctx.strokeStyle='#cecece';ctx.beginPath();ctx.moveTo(x,box.y);ctx.lineTo(x,box.y+box.h);ctx.stroke();ctx.fillStyle='#4f4f4f';ctx.textAlign='center';ctx.fillText(fmt(value,4),x,h-8);}
      for(const value of classicTicks(ymin,ymax,12)){const y=py(value);ctx.strokeStyle='#cecece';ctx.beginPath();ctx.moveTo(box.x,y);ctx.lineTo(box.x+box.w,y);ctx.stroke();ctx.fillStyle='#4f4f4f';ctx.textAlign='right';ctx.fillText(fmt(value,4),box.x-5,y+4);}
      ctx.strokeStyle='#bcbcbc';ctx.lineWidth=2;ctx.strokeRect(box.x,box.y,box.w,box.h);ctx.lineWidth=1;
    }else for(let i=0;i<=5;i++){const x=box.x+i*box.w/5,y=box.y+i*box.h/5;ctx.strokeStyle='#eaf0f5';ctx.beginPath();ctx.moveTo(x,box.y);ctx.lineTo(x,box.y+box.h);ctx.moveTo(box.x,y);ctx.lineTo(box.x+box.w,y);ctx.stroke();ctx.fillStyle='#77889d';ctx.textAlign='center';ctx.fillText(fmt(this.range[0]+i*(this.range[1]-this.range[0])/5,3),x,h-15);ctx.textAlign='right';ctx.fillText(fmt(ymax-i*(ymax-ymin)/5,3),box.x-8,y+3);}
    ctx.save();ctx.beginPath();ctx.rect(box.x,box.y,box.w,box.h);ctx.clip();
    const line=(data,color,dash)=>{ctx.strokeStyle=color;ctx.lineWidth=classic ? 0.8 : 1.2;ctx.setLineDash(dash);ctx.beginPath();data.forEach((p,i)=>{if(!i)ctx.moveTo(px(p[0]),py(p[1]));else ctx.lineTo(px(p[0]),py(p[1]));});ctx.stroke();ctx.setLineDash([]);};
    line(this.comparison,'#8e9eae',[4,3]);line(this.data,this.id==='wave'?state.mainColor||'#1762b5':classic?'#0000ff':'#258b87',[]);
    if(this.id==='wave'&&document.getElementById('key-visible')?.checked){ctx.fillStyle='#c1472b';for(const p of state.result?.key_phase_points||[]){if(p[0]<this.range[0]||p[0]>this.range[1])continue;ctx.beginPath();ctx.arc(px(p[0]),py(p[1]),3,0,Math.PI*2);ctx.fill();}}
    for(const curve of this.overlays.filter(c=>c.visible))line(curveData(curve),curve.color,[]);
    if(this.cursor&&['float','nearest','annotation'].includes(this.mode)){const {x,y}=this.cursor;ctx.strokeStyle='#398050';ctx.setLineDash([3,3]);ctx.beginPath();ctx.moveTo(px(x),box.y);ctx.lineTo(px(x),box.y+box.h);ctx.moveTo(box.x,py(y));ctx.lineTo(box.x+box.w,py(y));ctx.stroke();ctx.setLineDash([]);ctx.fillStyle='#398050';ctx.beginPath();ctx.arc(px(x),py(y),3,0,2*Math.PI);ctx.fill();}
    this.annotationBoxes=[];ctx.font='12px "Microsoft YaHei",sans-serif';ctx.textAlign='left';
    for(const a of this.data.length?this.annotations:[]){
      if(a.curve&&a.curve!=='main'&&!this.overlays.some(c=>c.key===a.curve&&c.visible))continue;
      if(a.x<this.range[0]||a.x>this.range[1])continue;
      const limit=Math.max(40,Math.min(240,box.w-16)),lines=[];let current='';
      for(const ch of a.text){if(ctx.measureText(current+ch).width>limit){lines.push(current);current=ch;}else current+=ch;}if(current)lines.push(current);
      if(lines.length>3){lines.length=3;lines[2]+='…';}lines.push(`${fmt(a.x,4)} ${this.xunit} / ${fmt(a.y,4)} ${a.unit??this.unit}`);
      const bw=Math.min(box.w,Math.max(...lines.map(v=>ctx.measureText(v).width))+16),bh=lines.length*16+12;
      const x=Math.max(box.x,Math.min(box.x+box.w-bw,px(a.x)+(Number.isFinite(a.dx)?a.dx:.03)*box.w)),y=Math.max(box.y,Math.min(box.y+box.h-bh,py(a.y)+(Number.isFinite(a.dy)?a.dy:-.1)*box.h));
      ctx.strokeStyle='#aa631c';ctx.lineWidth=1;ctx.beginPath();ctx.moveTo(px(a.x),py(a.y));ctx.lineTo(x+bw/2,y+bh/2);ctx.stroke();ctx.fillStyle='rgba(255,255,255,.95)';ctx.fillRect(x,y,bw,bh);ctx.strokeRect(x,y,bw,bh);ctx.fillStyle='#8d4f15';lines.forEach((text,i)=>ctx.fillText(text,x+8,y+17+i*16,bw-16));ctx.fillStyle='#bc5b20';ctx.beginPath();ctx.arc(px(a.x),py(a.y),3,0,2*Math.PI);ctx.fill();this.annotationBoxes.push({id:a.id,x,y,w:bw,h:bh});
    }
    if(this.drag?.kind==='zoom'){const d=this.drag,x1=d.mode==='zoom-y'?box.x:px(Math.min(d.start.x,d.end.x)),x2=d.mode==='zoom-y'?box.x+box.w:px(Math.max(d.start.x,d.end.x)),y1=d.mode==='zoom-x'?box.y:py(Math.max(d.start.y,d.end.y)),y2=d.mode==='zoom-x'?box.y+box.h:py(Math.min(d.start.y,d.end.y));ctx.fillStyle='rgba(23,98,181,.12)';ctx.strokeStyle='#1762b5';ctx.setLineDash([4,3]);ctx.fillRect(x1,y1,x2-x1,y2-y1);ctx.strokeRect(x1,y1,x2-x1,y2-y1);ctx.setLineDash([]);}
    ctx.restore();if(!this.data.length){ctx.fillStyle='#8797aa';ctx.textAlign='center';ctx.font='13px "Microsoft YaHei",sans-serif';ctx.fillText('打开 SEL 文件，选择通道后显示分析结果',w/2,h/2);}ctx.fillStyle='#77889d';ctx.textAlign='right';ctx.font='10px sans-serif';ctx.fillText(this.xunit,w-12,12);
  }
}

function openAnnotation(chart,id=null){
  if(!chart.ready())return;
  const existing=id?chart.annotations.find(a=>a.id===id):null;
  if(!existing&&!(chart.mode==='nearest'&&chart.cursor?.exact)){status('请先选择“捡取临近点”，在曲线上定位后再添加标注；浮动点不绑定采样值',true);return;}
  const point=existing||chart.cursor,target=curveInfo(chart);annotationEditor={chart,scope:chart.scope,id,point:{...point},target};
  document.getElementById('annotation-location').textContent=`${existing?.channel_name||target.name} · ${chart.id==='wave'?'时域波形':'幅值频谱'} · ${fmt(point.x,6)} ${chart.xunit} / ${fmt(point.y,6)} ${existing?.unit??target.unit}`;
  document.getElementById('annotation-text').value=existing?.text||'';document.getElementById('annotation-dialog').showModal();
}
function closeChartMenu(){document.getElementById('chart-menu').hidden=true;}
function openChartMenu(chart,x,y){contextChart=chart;const menu=document.getElementById('chart-menu');menu.hidden=false;menu.style.left=`${Math.max(8,Math.min(x,innerWidth-menu.offsetWidth-8))}px`;menu.style.top=`${Math.max(8,Math.min(y,innerHeight-menu.offsetHeight-8))}px`;syncChartMenu();menu.querySelector('button')?.focus();}
function syncChartMenu(){const chart=contextChart;if(!chart)return;for(const b of document.querySelectorAll('#chart-menu button')){const waveOnly=b.dataset.waveOnly==='true';b.hidden=waveOnly&&chart.id!=='wave';b.disabled=!chart.ready()||(b.dataset.action==='add'&&!(chart.mode==='nearest'&&chart.cursor?.exact))||(b.dataset.action==='clear'&&!chart.annotations.length)||(b.dataset.action==='undo'&&!chart.history.length)||(b.dataset.action==='restore'&&!state.filters.length);if(b.dataset.mode)b.setAttribute('aria-checked',String(chart.mode===b.dataset.mode));}}
function setupChartInteractions(){
  document.getElementById('annotation-cancel').addEventListener('click',()=>document.getElementById('annotation-dialog').close());
  document.getElementById('annotation-form').addEventListener('submit',e=>{
    e.preventDefault();const pending=annotationEditor,text=document.getElementById('annotation-text').value.trim();if(!text||!pending)return;
    const {chart,scope,id,point,target}=pending;if(!chart.ready()||chart.scope!==scope){status('分析内容已改变，请重新定位标注点',true);return;}
    if(id){const a=chart.annotations.find(a=>a.id===id);if(a)a.text=text;}else chart.annotations.push({id:crypto.randomUUID(),text,x:point.x,y:point.y,index:point.index,sample_index:point.sample_index,curve:target.key,unit:target.unit,channel_name:target.name,analysis_signature:target.signature,dx:.03,dy:-.12});chart.persist();chart.draw();chart.syncTools();document.getElementById('annotation-dialog').close();
  });
  document.getElementById('chart-menu').addEventListener('click',e=>{const b=e.target.closest('button');if(!b||b.disabled||!contextChart)return;const chart=contextChart;closeChartMenu();if(b.dataset.mode)chart.setMode(b.dataset.mode);else chart.action(b.dataset.action);});
  document.addEventListener('pointerdown',e=>{if(!document.getElementById('chart-menu').contains(e.target))closeChartMenu();});
  document.addEventListener('keydown',e=>{if(e.key==='Escape')closeChartMenu();});
}
