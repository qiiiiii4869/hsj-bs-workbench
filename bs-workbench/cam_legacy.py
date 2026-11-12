"""ICA parsing and isolated original CAM smoothing/interpolation executable."""
from pathlib import Path
import subprocess
import numpy as np
from sel_reader import SelError


def compute(body):
    text=body.get('text')
    if not isinstance(text,str) or len(text)>1024*1024:raise SelError('请加载有效 ICA 文件')
    tokens=text.split();position=0
    try:
        nb=int(tokens[position]);position+=1
        if not 2<=nb<=20:raise ValueError()
        for i in range(nb):
            n=int(tokens[position]);position+=1
            if not 3<=n<=30:raise ValueError()
            for j in range(n):
                q,e=map(float,tokens[position:position+2]);position+=2
                if not np.isfinite(q+e):raise ValueError()
        if position!=len(tokens):raise ValueError()
    except (IndexError,ValueError):raise SelError('ICA 格式须为曲线数、各曲线点数及流量/效率数对，2–20条曲线，每条3–30点')
    exe=Path(__file__).parent/'native/cam_math.exe'
    if not exe.is_file():raise SelError('协联数值程序尚未构建，请运行 native/build_cam.py')
    try:result=subprocess.run([str(exe)],input=text.encode('ascii'),capture_output=True,timeout=30)
    except subprocess.TimeoutExpired:raise SelError('原协联迭代未在30秒内收敛，请核对数据')
    if result.returncode:raise SelError('原协联算法无法处理此数据：检查曲线顺序、导数根与收敛条件')
    values=np.fromstring(result.stdout.decode('ascii'),sep=' ')
    if len(values)!=(nb+1)*200 or not np.all(np.isfinite(values)) or np.any(np.abs(values)>1e90):raise SelError('协联数值结果无效或插值超出范围')
    points=values.reshape(nb+1,100,2)
    return dict(title='协联工况',axes=['流量','效率'],curves=[dict(name=f'定桨曲线{i+1}' if i<nb else '协联曲线',points=p.tolist(),color='#888888' if i<nb else '#0000ff',visible=True) for i,p in enumerate(points)],rows=points[-1].tolist(),columns=['流量','效率'],legacy_text=''.join(f'{x:.6f}\t{y:.6f}\r\n' for x,y in points[-1]),legacy_extension='oca')
