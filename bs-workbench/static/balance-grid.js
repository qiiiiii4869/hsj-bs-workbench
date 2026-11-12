'use strict';
(() => {
 if(kind!=='balance')return;
 const columns=['状态','转速(转/分)','峰峰值(双幅)','转频幅值(双幅)','转频相位(°)','重量','角度(°)'];
 const labels=['原始波动','试重波动','配重方案','分解1','分解2'];
 const host=document.createElement('section');host.id='balance-matrix';const heading=document.createElement('h2');heading.textContent='动平衡参数表';const table=document.createElement('table');table.tabIndex=0;table.addEventListener('keydown',event=>{if(event.target===table&&event.code==='Space')event.preventDefault();});host.append(heading,table);$('result').closest('.table-scroll').before(host);
 s.balanceGrid=Array.from({length:5},(_,row)=>[labels[row],...Array(6).fill(null)]);
 const cells=new Map(),overrides=new Map();const header=document.createElement('tr');for(const title of columns){const th=document.createElement('th');th.textContent=title;header.append(th);}table.append(header);
 function editable(row,col){return row<2&&col>=1&&(row===1||col<5)||row>=3&&col===6;}
 function inputBinding(row,col){if(row<2&&col===3)return row?'after_amplitude':'before_amplitude';if(row<2&&col===4)return row?'after_phase':'before_phase';if(row===1&&col===5)return'trial_weight';if(row===1&&col===6)return'trial_angle';if(row===3&&col===6)return'angle_1';if(row===4&&col===6)return'angle_2';return null;}
 function commit(input,row,col){const value=input.value===''?null:Number(input.value);if(value!==null&&!Number.isFinite(value))throw new Error('请输入有效数值');s.balanceGrid[row][col]=value;overrides.set(row+':'+col,value);const binding=inputBinding(row,col);if(binding&&value!==null){$(binding).value=col===3?value/2:value;if(row>=3)$('decompose').checked=true;}}
 for(let row=0;row<5;row++){
   const tr=document.createElement('tr');
   for(let col=0;col<7;col++){
     const td=document.createElement('td');
     if(col===0)td.textContent=labels[row];else if(editable(row,col)){
       const input=document.createElement('input');input.type='number';input.step='any';input.setAttribute('aria-label',labels[row]+' '+columns[col]);input.dataset.row=row;input.dataset.col=col;let old='';
       input.addEventListener('focus',()=>old=input.value);
       input.addEventListener('change',()=>{try{commit(input,row,col);sync();}catch(e){input.value=old;status(e.message,true);}});
       input.addEventListener('keydown',event=>{
         if(event.key==='Escape'){event.preventDefault();input.value=old;input.blur();return;}
         if(event.key==='Enter'){event.preventDefault();input.dispatchEvent(new Event('change'));input.blur();if(Number($('trial_weight').value)>0&&!s.busy)compute();return;}
         if(event.key==='ArrowUp'||event.key==='ArrowDown'){event.preventDefault();input.dispatchEvent(new Event('change'));const direction=event.key==='ArrowUp'?-1:1;for(let next=row+direction;next>=0&&next<5;next+=direction){const target=cells.get(next+':'+col)?.querySelector('input');if(target){target.focus();break;}}}
       });td.append(input);
     }else td.className='readonly';
     cells.set(row+':'+col,td);tr.append(td);
   }table.append(tr);
 }
 function fill(){
   for(const [row,side] of [[0,'before'],[1,'after']]){
     const data=s[side];if(data){s.balanceGrid[row][1]=data.rows.find(r=>r[0]==='转速(r/min)')?.[1]??null;s.balanceGrid[row][2]=data.rows.find(r=>r[0]==='峰峰值')?.[1]??null;}
     s.balanceGrid[row][3]=Number($(row?'after_amplitude':'before_amplitude').value)*2;s.balanceGrid[row][4]=Number($(row?'after_phase':'before_phase').value);
   }
   s.balanceGrid[1][5]=Number($('trial_weight').value);s.balanceGrid[1][6]=Number($('trial_angle').value);
   if(s.result?.weight!==undefined){s.balanceGrid[2][5]=s.result.weight;s.balanceGrid[2][6]=s.result.angle;}
   for(const [row,label,key] of [[3,'分解重量1','angle_1'],[4,'分解重量2','angle_2']]){s.balanceGrid[row][5]=s.result?.rows?.find(r=>r[0]===label)?.[1]??null;s.balanceGrid[row][6]=Number($(key).value);}
   for(let row=0;row<5;row++)for(let col=1;col<7;col++){const cell=cells.get(row+':'+col),value=s.balanceGrid[row][col],input=cell.querySelector('input');if(input){if(document.activeElement!==input)input.value=value??'';}else cell.textContent=value===null?'—':fmt(value);}
   for(const [key,value] of overrides){const [row,col]=key.split(':').map(Number);if(col<3){s.balanceGrid[row][col]=value;const input=cells.get(key).querySelector('input');if(input&&document.activeElement!==input)input.value=value??'';}}
   if(s.result)s.result.tables=[{title:'动平衡参数表',columns,rows:clone(s.balanceGrid)}];
 }
 const render=showResult;showResult=function(result){if(result.module==='balance_sample'){const row=result.request?.side==='after'?1:0;for(let col=1;col<5;col++)overrides.delete(row+':'+col);}render(result);fill();};
 const params=body;body=function(){return {...params(),balance_grid:clone(s.balanceGrid)};};
 const previousComposite=composite;composite=function(){
   const base=previousComposite(),out=document.createElement('canvas');out.width=base.width;out.height=base.height+240;const ctx=out.getContext('2d');ctx.fillStyle='#fff';ctx.fillRect(0,0,out.width,out.height);ctx.drawImage(base,0,0);ctx.font='14px "Microsoft YaHei"';
   const width=(out.width-40)/7,top=base.height+15;
   for(let row=0;row<6;row++)for(let col=0;col<7;col++){const x=20+col*width,y=top+row*32;ctx.strokeStyle='#c3ccd5';ctx.strokeRect(x,y,width,32);ctx.fillStyle='#273440';const value=row? s.balanceGrid[row-1][col]:columns[col];ctx.fillText(value===null?'—':String(typeof value==='number'?Number(value.toFixed(6)):value),x+6,y+21,width-12);}return out;
 };
 $('numbers').addEventListener('change',fill);fill();
})();
