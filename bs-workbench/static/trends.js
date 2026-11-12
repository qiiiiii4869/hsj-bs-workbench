'use strict';
function trendSpecs(){return state.trendSpecs.map(s=>({...s}));}
async function fetchTrends(specs){if(!state.params||!state.result)return [];return (await request('/api/trends',{...state.params,include_original:false,trends:specs})).curves;}
function syncTrendControls(){
  $('trend-open').disabled=!wave.ready();
  const legacy=$('trend-metric').value==='effective';$('trend-effective').disabled=state.busy||!legacy;
  $('trend-method').textContent=legacy?($('trend-effective').value==='desktop'?'原版有效值代码读取当前分析区间开头的窗口数据，各趋势点可能为同一值；不自动修正此行为。':'逐窗去均值 RMS × 2√2：这是显式选择的新口径。'):'平均/中间/峰峰值按完整通道上的移动窗口计算，趋势时间为（窗口起点＋窗口宽度）÷采样率。';
}
function openTrendDialog(){if(!wave.ready())return;$('trend-error').textContent='';$('trend-context').textContent=`${state.result.source.channel.name} · ${state.result.parameters.sample_rate} Hz · 窗口宽度、步长均为采样点`;$('trend-dialog').showModal();syncTrendControls();}
function setupTrends(){
  $('trend-open').addEventListener('click',openTrendDialog);$('trend-cancel').addEventListener('click',()=>$('trend-dialog').close());
  $('trend-dialog').addEventListener('cancel',e=>{if(state.busy)e.preventDefault();});
  $('trend-metric').addEventListener('change',syncTrendControls);$('trend-effective').addEventListener('change',syncTrendControls);
  $('trend-form').addEventListener('submit',async e=>{
    e.preventDefault();if(!wave.ready())return;
    const width=Number($('trend-width').value),step=Number($('trend-step').value);
    if(!Number.isInteger(width)||!Number.isInteger(step)||width<1||step<1){$('trend-error').textContent='窗口宽度、步长须为正整数采样点';return;}
    const spec={id:crypto.randomUUID(),metric:$('trend-metric').value,width,step,color:$('trend-color').value,visible:true,tail:$('trend-tail').value,effective_mode:$('trend-effective').value};
    busy(true);$('trend-error').textContent='正在计算移动趋势…';
    try{const curves=await fetchTrends([spec]);state.trendSpecs.push(spec);wave.overlays.push(...curves);wave.yRange=null;saveContext();wave.draw();$('trend-dialog').close();status('已添加移动趋势 · 图例记录计算依据、宽度和步长');}catch(error){$('trend-error').textContent=error.message;}finally{busy(false);}
  });
}
