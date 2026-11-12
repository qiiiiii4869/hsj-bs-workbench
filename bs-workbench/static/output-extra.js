'use strict';
(() => {
 const kind=document.getElementById('output-kind'),width=document.getElementById('output-width'),height=document.getElementById('output-height');if(!kind||!width||!height)return;
 const label=document.createElement('label'),useDefault=document.createElement('input');label.className='compare-toggle';useDefault.type='checkbox';useDefault.id='output-default-size';useDefault.checked=true;label.append(useDefault,'使用当前图表缺省尺寸');width.closest('.filter-fields')?.before(label);
 const originalValidate=validateImageSize;
 validateImageSize=function(){
   if(!useDefault.checked)return originalValidate();
   const choice=document.getElementById('output-plot').value,charts=state.result?.parameters.fft_enabled===false?[wave]:choice==='wave'?[wave]:choice==='spectrum'?[spectrum]:[wave,spectrum];
   const w=Math.max(400,Math.round(Math.max(...charts.map(c=>c.canvas.clientWidth))));
   const h=Math.max(300,Math.round(charts.reduce((total,c)=>total+c.canvas.clientHeight,0)+76+wave.overlays.filter(c=>c.visible).length*18));
   if(w>8000||h>8000||w*h>24000000)throw new Error('当前图表尺寸超过图像输出范围，请使用自定义尺寸');return{width:w,height:h};
 };
 const sync=()=>{const image=!['docx','doc','xlsx','xls','chd-current','chd-full','chd-peaks'].includes(kind.value);width.disabled=height.disabled=!image||useDefault.checked;useDefault.disabled=!image;document.getElementById('output-format').disabled=!image;document.getElementById('output-plot').disabled=!image;label.hidden=!image;};
 kind.addEventListener('change',sync);useDefault.addEventListener('change',sync);document.getElementById('output-open').addEventListener('click',sync);sync();
})();
