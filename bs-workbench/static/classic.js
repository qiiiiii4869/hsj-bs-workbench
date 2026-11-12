'use strict';
let classicMenuButton=null,classicReady=false;
let classicSubmenuButton=null;
function classicAction(text,action,enabled=()=>true){return {text,action,enabled};}
function closeClassicSubmenu(){if($('classic-submenu'))$('classic-submenu').hidden=true;if(classicSubmenuButton)classicSubmenuButton.setAttribute('aria-expanded','false');classicSubmenuButton=null;}
function closeClassicMenu(){closeClassicSubmenu();$('classic-popup').hidden=true;if(classicMenuButton)classicMenuButton.setAttribute('aria-expanded','false');classicMenuButton=null;}
function fillClassicMenu(menu,items){
  menu.replaceChildren(...items.map(item=>{
    const b=document.createElement('button');b.type='button';b.role=item.checked?'menuitemradio':'menuitem';b.disabled=!item.enabled();
    if(item.checked){const check=document.createElement('span');check.className='classic-menu-check';check.setAttribute('aria-hidden','true');const checked=!!item.checked();check.textContent=checked?'✓':'';b.setAttribute('aria-checked',String(checked));b.append(check);}
    const label=document.createElement('span');label.textContent=item.text;b.append(label);
    if(item.children){
      const arrow=document.createElement('span');arrow.className='classic-menu-arrow';arrow.textContent='▶';arrow.setAttribute('aria-hidden','true');b.append(arrow);b.setAttribute('aria-haspopup','menu');b.setAttribute('aria-expanded','false');b.setAttribute('aria-controls','classic-submenu');
      b.addEventListener('pointerenter',()=>{if(!b.disabled)showClassicSubmenu(b,item.children);});
      b.addEventListener('click',()=>showClassicSubmenu(b,item.children,true));
    }else{
      b.addEventListener('pointerenter',()=>{if(menu.id==='classic-popup')closeClassicSubmenu();});
      b.addEventListener('click',()=>{closeClassicMenu();item.action();});
    }
    return b;
  }));
}
function showClassicSubmenu(button,items,focus=false){
  closeClassicSubmenu();classicSubmenuButton=button;button.setAttribute('aria-expanded','true');
  const menu=$('classic-submenu');menu.setAttribute('aria-label',button.textContent.replace('▶',''));fillClassicMenu(menu,items);menu.hidden=false;
  const box=button.getBoundingClientRect();menu.style.left=`${box.right+menu.offsetWidth<=innerWidth?box.right:Math.max(0,box.left-menu.offsetWidth)}px`;menu.style.top=`${Math.max(0,Math.min(box.top,innerHeight-menu.offsetHeight))}px`;
  if(focus)(menu.querySelector('[aria-checked="true"]:not(:disabled)')||menu.querySelector('button:not(:disabled)'))?.focus();
}
function showClassicMenu(button,items){
  const menu=$('classic-popup');if(classicMenuButton===button&&!menu.hidden){closeClassicMenu();return;}closeClassicMenu();classicMenuButton=button;button.setAttribute('aria-expanded','true');
  fillClassicMenu(menu,items);menu.hidden=false;
  const box=button.getBoundingClientRect();menu.style.left=`${Math.max(0,Math.min(box.left,innerWidth-menu.offsetWidth))}px`;menu.style.top=`${box.bottom}px`;menu.querySelector('button:not(:disabled)')?.focus();
}
async function classicSetConfidence(confidence){
  if(state.busy||!state.result||state.dirty||![95,97,100].includes(confidence))return false;
  const previous=state.params?.confidence??97;
  const ok=await runAnalysis({confidence},true,true);
  if(!ok)$('confidence').value=String(previous);
  syncClassic();return !!ok;
}
function classicOpenSettings(){if(state.busy)return;$('classic-settings').showModal();}
function classicOpenData(){if(state.busy)return;$('classic-files').showModal();}
function classicOpenCurves(){if(state.busy)return;$('classic-curves').showModal();}
function syncClassic(){
  if(!document.body.classList.contains('classic-layout'))return;
  const r=state.result;
  $('classic-caption').textContent=r?`${state.file.exam}，${r.source.condition.type}，${r.source.condition.name}，${r.source.title===r.source.channel.name?r.source.channel.name+'通道波形频谱图':r.source.title} （单位：${r.source.channel.unit||'—'}）`:'波形与频谱';
  const rows=[];if(r){const p=r.parameters,s=r.statistics;rows.push(['起始时刻(秒)：',p.start.toFixed(3)],['终止时刻(秒)：',p.end.toFixed(3)],['采样频率(赫兹)：',String(p.sample_rate)],['最大值：',s.max.toFixed(2)],['最小值：',s.min.toFixed(2)],['平均值：',s.mean.toFixed(2)],['中间值：',s.median.toFixed(2)],['有效值：',s.desktop_effective.toFixed(2)],[`${p.confidence}%峰峰值：`,s.peak_to_peak.toFixed(2)]);}
  $('classic-stats').replaceChildren(...rows.flatMap(([label,value])=>{const dt=document.createElement('dt'),dd=document.createElement('dd');dt.textContent=label;dd.textContent=value;return [dt,dd];}));
  const mode=$('display-mode').value;document.body.dataset.graphMode=mode;
  $('classic-status').textContent=$('status').textContent+(state.filters.length?` · 已滤波 ${state.filters.length} 次`:'')+(wave.overlays.length?` · 叠加 ${wave.overlays.length} 条曲线`:'');
  for(const button of document.querySelectorAll('[data-source-button]')){const original=$(button.dataset.sourceButton);button.disabled=original?.disabled??false;}
  for(const button of document.querySelectorAll('[data-chart-action]')){const chart=button.dataset.chart==='spectrum'?spectrum:wave;const action=button.dataset.chartAction;button.disabled=!chart.ready()||(action==='undo'&&!chart.history.length)||(action==='add'&&!(chart.mode==='nearest'&&chart.cursor?.exact));const current=button.dataset.chartMode;button.setAttribute('aria-pressed',String(current?chart.mode===current:false));}
  for(const button of document.querySelectorAll('[data-classic-close]'))button.disabled=state.busy;
}
function classicTree(){
  const root=$('classic-tree');state.treeExpansion??=new Map();
  for(const node of root.querySelectorAll('details[data-tree-key]'))state.treeExpansion.set(node.dataset.treeKey,node.open);
  root.replaceChildren();const files=state.openFiles?.size?[...state.openFiles.values()]:state.file?[state.file]:[];
  const branch=(key,label,opened)=>{const d=document.createElement('details');d.dataset.treeKey=key;d.open=state.revealSelection?.includes(key)?true:state.treeExpansion.has(key)?state.treeExpansion.get(key):opened;const summary=document.createElement('summary');summary.textContent=label;d.append(summary);return d;};
  files.forEach((info,fi)=>{
    const activeFile=state.file?.file_id===info.file_id;const file=branch(info.file_id,`[${fi+1}] ${info.exam||info.filename}`,activeFile);file.dataset.fileId=info.file_id;file.firstElementChild.title=info.filename;
    [...new Set(info.conditions.map(c=>c.type))].forEach((type,ti)=>{
      const typeNode=branch(info.file_id+'|'+type,`[${ti+1}] ${type}`,activeFile);typeNode.dataset.examType=type;
      info.conditions.filter(c=>c.type===type).forEach((condition,ci)=>{
        const node=branch(info.file_id+'|'+type+'|'+condition.index,`[${ci+1}] ${condition.name}`,activeFile&&condition.index===Number($('condition').value));node.dataset.condition=condition.index;
        for(const c of condition.channels){
          const button=document.createElement('button');button.type='button';button.dataset.fileId=info.file_id;button.dataset.condition=condition.index;button.dataset.channel=c.index;button.textContent=`[${c.index+1}] ${c.name}`;button.title=`${c.name} · ${c.unit} · ${c.sample_rate} Hz · 双击或 Enter 打开`;button.disabled=state.busy||c.count<4;
          const active=activeFile&&condition.index===Number($('condition').value)&&c.index===state.channel;button.classList.toggle('selected',active);button.setAttribute('aria-pressed',String(active));
          const open=async()=>{
            if(state.busy)return;
            if(parent!==window&&new URLSearchParams(location.search).get('embedded')==='1'){hsjOpenView('/?file_id='+encodeURIComponent(info.file_id)+'&condition='+condition.index+'&channel='+c.index,c.name);return;}
            if(!activeFile){state.navigation={file_id:info.file_id,condition:condition.index,channel:c.index};await acceptFile(info);}
            else{$('condition').value=String(condition.index);await selectChannel(c.index);}classicTree();
          };
          button.addEventListener('click',()=>{root.querySelectorAll('button.selected').forEach(other=>{other.classList.remove('selected');other.setAttribute('aria-pressed','false');});button.classList.add('selected');button.setAttribute('aria-pressed','true');});
          button.addEventListener('dblclick',open);button.addEventListener('keydown',e=>{if(e.key==='Enter'){e.preventDefault();open();}});node.append(button);
        }typeNode.append(node);
      });file.append(typeNode);
    });root.append(file);
  });state.revealSelection=null;
}

const classicIcons={
  folder:'M2 5h7l2 2h11v14H2z M2 9h20',table:'M3 3h18v18H3z M3 9h18 M9 3v18 M15 9v12 M3 15h18',save:'M4 3h14l3 3v15H3V3z M7 3v6h10V3 M7 21v-8h10v8',
  wave:'M2 12h3l2-8 3 16 3-14 3 12 2-6h4',cross:'M12 2v20 M2 12h20 M8 8l8 8 M16 8l-8 8',nearest:'M3 18l5-10 5 6 7-11 M8 4v17 M3 8h16',zoom:'M10 3a7 7 0 1 0 0 14 7 7 0 0 0 0-14 M15 15l7 7 M6 10h8 M10 6v8',
  move:'M12 2v20 M2 12h20 M8 6l4-4 4 4 M8 18l4 4 4-4 M6 8l-4 4 4 4 M18 8l4 4-4 4',undo:'M8 4L2 10l6 6 M3 10h11a6 6 0 0 1 0 12',reset:'M20 8a9 9 0 1 0 1 9 M20 2v7h-7',pen:'M4 17l12-13 4 4L8 21H3z M13 7l4 4',
  filter:'M2 3h20l-8 9v8l-4 2V12z',restore:'M4 7h16 M8 7V4h8v3 M6 7l1 14h10l1-14 M10 10v7 M14 10v7',chart:'M3 2v20h19 M7 17V9 M12 17V5 M17 17v-6',export:'M4 3h10v6h6v12H4z M14 3l6 6 M8 15h8 M13 12l3 3-3 3',
  key:'M8 3a5 5 0 1 0 0 10 5 5 0 0 0 0-10 M12 11l9 9 M16 15l3-3 M19 18l3-3',clock:'M12 2a10 10 0 1 0 0 20 10 10 0 0 0 0-20 M12 5v7l5 3',layers:'M12 2L2 8l10 6 10-6z M2 12l10 6 10-6 M2 16l10 6 10-6',
  bars:'M3 21V11h4v10 M10 21V3h4v18 M17 21V7h4v14',title:'M3 4h18 M12 4v17 M8 21h8',gear:'M3 5h18 M3 12h18 M3 19h18 M8 2v6 M16 9v6 M7 16v6'
};
function iconButton(label,icon,action,meta={}){
  const button=document.createElement('button');button.type='button';button.className='classic-icon';button.title=label;button.setAttribute('aria-label',label);for(const [key,value] of Object.entries(meta))button.dataset[key]=value;
  const svg=document.createElementNS('http://www.w3.org/2000/svg','svg');svg.setAttribute('viewBox','0 0 24 24');svg.setAttribute('aria-hidden','true');const path=document.createElementNS(svg.namespaceURI,'path');path.setAttribute('d',classicIcons[icon]||classicIcons.chart);svg.append(path);button.append(svg);button.addEventListener('click',action);return button;
}
function setupClassic(){
  // Move the existing controls; preserve their IDs, state and attached handlers.
  const settings=$('classic-settings-content');
  for(const selector of ['#workspace .page-options','#parameters','#workspace .analysis-tools','#filter-history','#workspace .metrics','#workspace>.method']){const el=document.querySelector(selector);if(el)settings.append(el);}
  for(const note of document.querySelectorAll('#workspace>.method'))settings.append(note);
  const fileControls=$('classic-files-content'),library=document.querySelector('.library');
  for(const child of [...library.children])fileControls.append(child);
  const heading=document.createElement('div');heading.className='classic-data-heading';heading.textContent='数据';const picker=document.createElement('button');picker.type='button';picker.textContent='…';picker.title='文件列表与通道搜索';picker.addEventListener('click',classicOpenData);heading.append(picker);const tree=document.createElement('div');tree.id='classic-tree';tree.setAttribute('role','tree');tree.setAttribute('aria-label','试验数据树');library.append(heading,tree);
  $('classic-curves-content').append($('overlay-panel'));
  const actions=document.querySelector('#workspace .page-actions');$('classic-settings-content').append(actions);
  const main=$('workspace'),plots=[...main.querySelectorAll('section.plot')];plots[0].classList.add('classic-wave');plots[1].classList.add('classic-spectrum');
  for(const [index,chart] of [wave,spectrum].entries()){$('classic-settings-content').append(plots[index].querySelector('.plot-heading'));$('classic-settings-content').append($(chart.id+'-tools'));$(chart.id+'-tools').classList.add('classic-chart-tools');$('classic-settings-content').append(chart.annotationDetails);}
  const result=document.querySelector('.results');const top=document.createElement('section');top.className='classic-result-top';const dl=document.createElement('dl');dl.id='classic-stats';top.append(dl);const bottom=document.createElement('section');bottom.className='classic-result-bottom';bottom.append(result.querySelector('table'));
  for(const child of [...result.children])$('classic-settings-content').append(child);result.append(top,bottom);
  const caption=document.createElement('div');caption.id='classic-caption';main.prepend(caption);
  const click=id=>()=>{if(!$(id).disabled)$(id).click();},ready=()=>wave.ready(),chartAction=(chart,action)=>()=>chart.action(action),mode=(chart,value)=>()=>chart.setMode(value);
  const fileItems=[classicAction('打开分拣文件…',click('import')),classicAction('已打开文件 / 搜索通道…',classicOpenData),classicAction('文件属性…',click('properties-open'),()=>!!state.file),classicAction('载入数据 FFT…',click('chd-open')),classicAction('保存运行日志',click('log-save')),classicAction('完整功能迁移对照…',click('coverage')),classicAction('接口文档 / 调试',()=>window.open('/docs','_blank','noopener')),classicAction('刷新当前视图',()=>location.reload()),classicAction('退出程序 / 停止服务',()=>hsjExit().catch(e=>status(e.message,true)))];
  const viewItems=[classicAction('导航 / 消息 / 状态栏 / 工具栏设置',()=>{document.getElementById('hsj-workspace').open=true;}),classicAction('波形与频谱',()=>setGraphMode('both'),ready),classicAction('仅波形',()=>setGraphMode('wave'),ready),classicAction('仅频谱',()=>setGraphMode('spectrum'),()=>ready()&&state.result.parameters.fft_enabled!==false),classicAction('分析参数 / 详细指标…',classicOpenSettings),classicAction('叠加通道 / 移动趋势管理…',classicOpenCurves,ready)];
  const waveItems=[classicAction('捡取浮动点',mode(wave,'float'),ready),classicAction('捡取临近点',mode(wave,'nearest'),ready),classicAction('平移 XY',mode(wave,'pan-xy'),ready),classicAction('放大 X',mode(wave,'zoom-x'),ready),classicAction('放大 Y',mode(wave,'zoom-y'),ready),classicAction('放大 XY',mode(wave,'zoom-xy'),ready),classicAction('撤销',()=>wave.undo(),()=>ready()&&wave.history.length>0),classicAction('重绘',chartAction(wave,'redraw'),ready),classicAction('添加标注',chartAction(wave,'add'),()=>ready()&&wave.cursor?.exact),classicAction('移动标注位置',chartAction(wave,'move'),ready),classicAction('取消标注',chartAction(wave,'clear'),ready)];
  const signalItems=[classicAction('添加通道数据…',click('channel-open'),ready),classicAction('添加移动趋势…',click('trend-open'),ready),classicAction('波形滤波…',click('filter-open'),ready),classicAction('恢复滤波前数据',click('filter-restore'),()=>ready()&&!!state.filters.length),classicAction('整周期 / 置信度 / 分析参数…',classicOpenSettings,ready),classicAction('设置分析时段（非整周期）…',click('span-open'),ready),classicAction('甩负荷窗口与专用标注…',click('throw-open'),ready),classicAction('修改标题…',click('title-open'),ready),classicAction('数据 / 图像 / Word / Excel 输出…',click('output-open'),ready),classicAction('批量输出…',click('batch-open'),ready)];
  const confidenceItems=[97,95,100].map(confidence=>({...classicAction(`置信度${confidence}%`,()=>classicSetConfidence(confidence),ready),checked:()=>Number(state.params?.confidence??$('confidence').value)===confidence}));
  const menus=[
    ['文件(F)',fileItems],
    ['视图(V)',viewItems],
    ['波形频谱(W)',[{text:'峰峰值设定',children:confidenceItems,enabled:ready},...waveItems,...signalItems]],
    ['波形长图(L)',[classicAction('打开波形长图',()=>{$('strip-link').click();})]],
    ['三维瀑布图(D)',[classicAction('新建三维频谱瀑布图',()=>hsjOpenView('/waterfall')),classicAction('新建三维波形瀑布图',()=>hsjOpenView('/waterfall?mode=wave'))]],
    ['轴心轨迹(A)',[classicAction('新建轴心轨迹',()=>hsjOpenView('/specialty?module=orbit'))]],
    ['趋势分析(T)',[classicAction('新建趋势分析',()=>hsjOpenView('/specialty?module=trend'))]],
    ['相关分析(C)',[classicAction('新建相关分析',()=>hsjOpenView('/specialty?module=correlation'))]],
    ['动平衡分析(B)',[classicAction('新建动平衡分析',()=>hsjOpenView('/specialty?module=balance'))]],
    ['盘车试验(J)',[classicAction('新建盘车试验',()=>hsjOpenView('/specialty?module=jigger'))]],
    ['效率分析(O)',[classicAction('新建效率分析',()=>hsjOpenView('/specialty?module=efficiency')),classicAction('协联工况 ICA',()=>hsjOpenView('/specialty?module=cam')),classicAction('最小二乘圆度拟合 IAG',()=>hsjOpenView('/specialty?module=airgap'))]],
    ['窗口(W)',[classicAction('多视图工作区',()=>{if(parent!==window&&parent.hsjDesktop)return;location.assign('/desktop');}),classicAction('关闭当前视图',()=>window.close()),classicAction('全部关闭',()=>{if(typeof hsjCloseAllViews==='function')hsjCloseAllViews();else location.assign('/?closed=1');}),classicAction('当前分析参数',classicOpenSettings),classicAction('功能迁移对照',click('coverage'))]],
    ['版权及联系方式(H)',[classicAction('版权及联系方式',()=>hsjShowAbout())]],
  ];
  const menubar=$('classic-menubar');for(const [label,items] of menus){const b=document.createElement('button');b.type='button';b.textContent=label;b.setAttribute('aria-haspopup','menu');b.setAttribute('aria-expanded','false');b.disabled=!items.length;if(!items.length)b.title='此页面尚未迁移';b.addEventListener('click',()=>showClassicMenu(b,items));menubar.append(b);}
  const group=(title,items)=>{const g=document.createElement('div');g.className='classic-tool-group';const caption=document.createElement('div');caption.className='classic-tool-title';caption.textContent=title;const row=document.createElement('div');row.className='classic-tool-buttons';for(const item of items)row.append(iconButton(...item));g.append(caption,row);$('classic-ribbon').append(g);};
  group('文件',[['打开分拣文件','folder',click('import'),{sourceButton:'import'}],['文件列表 / 搜索','table',classicOpenData],['文件属性','table',click('properties-open'),{sourceButton:'properties-open'}],['保存运行日志','save',click('log-save')],['分析参数','gear',classicOpenSettings]]);
  const tools=chart=>[['捡取浮动点','cross',mode(chart,'float'),{chart:chart.id,chartAction:'mode',chartMode:'float'}],['捡取临近点','nearest',mode(chart,'nearest'),{chart:chart.id,chartAction:'mode',chartMode:'nearest'}],['平移','move',mode(chart,chart.id==='wave'?'pan-xy':'pan'),{chart:chart.id,chartAction:'mode',chartMode:chart.id==='wave'?'pan-xy':'pan'}],['放大 X','zoom',mode(chart,'zoom-x'),{chart:chart.id,chartAction:'mode',chartMode:'zoom-x'}],...(chart.id==='wave'?[['放大 Y','zoom',mode(chart,'zoom-y'),{chart:chart.id,chartAction:'mode',chartMode:'zoom-y'}]]:[]),['放大 XY','zoom',mode(chart,'zoom-xy'),{chart:chart.id,chartAction:'mode',chartMode:'zoom-xy'}],['撤销','undo',()=>chart.undo(),{chart:chart.id,chartAction:'undo'}],['重绘','reset',chartAction(chart,'redraw'),{chart:chart.id,chartAction:'redraw'}],['添加标注','pen',chartAction(chart,'add'),{chart:chart.id,chartAction:'add'}]];
  group('波形图',tools(wave));group('频谱图',tools(spectrum));
  group('通道数据处理',[['添加通道数据','layers',click('channel-open'),{sourceButton:'channel-open'}],['添加移动趋势','wave',click('trend-open'),{sourceButton:'trend-open'}],['波形滤波','filter',click('filter-open'),{sourceButton:'filter-open'}],['恢复原始数据','restore',click('filter-restore'),{sourceButton:'filter-restore'}],['键相与置信度','key',classicOpenSettings],['分析时段','clock',click('span-open'),{sourceButton:'span-open'}],['甩负荷分析','bars',click('throw-open'),{sourceButton:'throw-open'}]]);
  group('通道数据存储',[['数据 / 图像 / Word / Excel','export',click('output-open'),{sourceButton:'output-open'}],['批量输出','table',click('batch-open'),{sourceButton:'batch-open'}],['分析记录 JSON','save',click('export-json'),{sourceButton:'export-json'}]]);
  group('试验图谱',[['波形长图','chart',()=>{$('strip-link').click();}],['叠加与趋势曲线管理','layers',classicOpenCurves]]);
  for(const id of ['classic-settings','classic-files','classic-curves'])$(id).addEventListener('cancel',e=>{if(state.busy)e.preventDefault();});
  document.querySelectorAll('[data-classic-close]').forEach(b=>b.addEventListener('click',()=>{if(!state.busy)$(b.dataset.classicClose).close();}));
  const submenu=document.createElement('div');submenu.id='classic-submenu';submenu.className='classic-popup classic-submenu';submenu.role='menu';submenu.hidden=true;document.body.append(submenu);
  $('classic-popup').addEventListener('scroll',closeClassicSubmenu);
  document.addEventListener('pointerdown',e=>{if(!$('classic-popup').contains(e.target)&&!submenu.contains(e.target)&&!menubar.contains(e.target))closeClassicMenu();});
  document.addEventListener('keydown',e=>{
    if($('classic-popup').hidden)return;
    if(e.key==='Escape'){const button=classicMenuButton;closeClassicMenu();button?.focus();}
    else if(e.key==='ArrowRight'&&document.activeElement?.getAttribute('aria-haspopup')==='menu'){e.preventDefault();document.activeElement.click();}
    else if(e.key==='ArrowLeft'&&!submenu.hidden){e.preventDefault();const button=classicSubmenuButton;closeClassicSubmenu();button?.focus();}
    else if(['ArrowDown','ArrowUp'].includes(e.key)){e.preventDefault();const menu=submenu.contains(document.activeElement)?submenu:$('classic-popup'),buttons=[...menu.querySelectorAll('button:not(:disabled)')],index=buttons.indexOf(document.activeElement);buttons[(index+(e.key==='ArrowDown'?1:-1)+buttons.length)%buttons.length]?.focus();}
  });
  $('display-mode').addEventListener('change',syncClassic);
  const observer=new MutationObserver(()=>{syncClassic();});observer.observe($('status'),{childList:true,subtree:true,characterData:true});
  classicTree();document.body.classList.add('classic-layout');classicReady=true;syncClassic();if(typeof setupWorkspace==='function')setupWorkspace();
}
function setGraphMode(mode){const option=[...$('display-mode').options].find(o=>o.value===mode);if(option?.disabled)return;$('display-mode').value=mode;$('display-mode').dispatchEvent(new Event('change'));}
