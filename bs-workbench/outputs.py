"""Offline image/Office/data exporters. No desktop Office automation."""
import base64
import io
import json
import math
import re
from pathlib import Path
from PIL import Image,ImageDraw,ImageFont
from analysis import envelope
from sel_reader import SelError

FORMATS={'png':('PNG','image/png'),'jpg':('JPEG','image/jpeg'),'bmp':('BMP','image/bmp'),'gif':('GIF','image/gif'),'tiff':('TIFF','image/tiff')}


def font(size):
    path=Path('C:/Windows/Fonts/msyh.ttc')
    return ImageFont.truetype(str(path),size) if path.exists() else ImageFont.load_default()


def dimensions(body):
    width=int(body.get('width',1400));height=int(body.get('height',950))
    if not 400<=width<=8000 or not 300<=height<=8000 or width*height>24_000_000:raise SelError('图片宽高须在 400–8000 / 300–8000 像素内，且总像素不超过 2400 万')
    return width,height


def image_bytes(image,fmt):
    fmt=fmt.lower()
    if fmt not in FORMATS:raise SelError('不支持的图片格式')
    out=io.BytesIO();image.convert('RGB').save(out,FORMATS[fmt][0]);return out.getvalue(),FORMATS[fmt][1]


def convert_image(body):
    url=body.get('image','')
    if not isinstance(url,str) or not url.startswith('data:image/png;base64,'):raise SelError('图像数据无效')
    raw=base64.b64decode(url.split(',',1)[1],validate=True)
    image=Image.open(io.BytesIO(raw))
    if image.width*image.height>24_000_000:raise SelError('图像尺寸超出上限')
    image.load();return image_bytes(image,body.get('format','png'))


def plot_image(result,arrays,body):
    width,height=dimensions(body);im=Image.new('RGB',(width,height),'white');draw=ImageDraw.Draw(im)
    source=result['source'];p=result['parameters'];unit=source['channel']['unit'];title=source['title']
    draw.text((28,18),title,font=font(22),fill='#203247')
    draw.text((28,52),f"{source['filename']} / {source['condition']['name']} / {source['channel']['name']} / {unit} / {p['sample_rate']} Hz",font=font(12),fill='#596d82')
    draw.text((28,76),f"分析区间 [{p['start']:.6f}, {p['end']:.6f}) s；{p['confidence']}% 峰峰值 {result['statistics']['peak_to_peak']:.6f}；有效值 {result['statistics']['desktop_effective']:.6f}",font=font(12),fill='#596d82')
    plots=['wave','spectrum'] if body.get('plot','both')=='both' else [body['plot']]
    if not source['channel'].get('fft_enabled',True):plots=['wave']
    for number,kind in enumerate(plots):
        x,y=arrays[:2] if kind=='wave' else arrays[2:]
        if not len(x):continue
        top=120+number*(height-150)/len(plots);bottom=120+(number+1)*(height-150)/len(plots)-36
        left,right=90,width-30
        xmin=float(x[0]);xmax=float(x[-1]);ymin=float(min(y));ymax=float(max(y));pad=(ymax-ymin)*.1 or 1;ymin-=pad;ymax+=pad
        curves=[c for c in result.get('overlays',[])+result.get('trends',[]) if c.get('visible',True)] if kind=='wave' else []
        for c in curves:
            for a,b in c['waveform']:
                if xmin<=a<=xmax:ymin=min(ymin,b);ymax=max(ymax,b)
        if kind=='spectrum':ymin=0
        limits=body.get('frequency_range') if kind=='spectrum' else None
        if limits:
            xmin,xmax=map(float,limits)
            if not math.isfinite(xmin+xmax) or xmin<0 or xmax<=xmin:raise SelError('频谱坐标范围无效')
        px=lambda a:left+(a-xmin)/(xmax-xmin or 1)*(right-left)
        py=lambda a:bottom-(a-ymin)/(ymax-ymin or 1)*(bottom-top)
        for i in range(6):
            xx=left+i*(right-left)/5;yy=top+i*(bottom-top)/5
            draw.line((xx,top,xx,bottom),fill='#e4ebf2');draw.line((left,yy,right,yy),fill='#e4ebf2')
            draw.text((xx-15,bottom+5),f'{xmin+(xmax-xmin)*i/5:.3g}',font=font(11),fill='#63788d')
            draw.text((6,yy-6),f'{ymax-(ymax-ymin)*i/5:.4g}',font=font(11),fill='#63788d')
        points=[(px(a),py(b)) for a,b in envelope(x,y,max(400,(width-100)*2)) if xmin<=a<=xmax]
        if len(points)>1:draw.line(points,fill=source.get('color','#1762b5') if kind=='wave' else '#0000ff',width=2)
        for c in curves:
            curvepoints=[(px(a),py(b)) for a,b in c['waveform'] if xmin<=a<=xmax]
            if len(curvepoints)>1:draw.line(curvepoints,fill=c['color'],width=2)
        if kind=='wave':
            for a,b in result.get('key_phase_points',[]):
                if xmin<=a<=xmax:draw.ellipse((px(a)-3,py(b)-3,px(a)+3,py(b)+3),fill='#bf4e2b')
        for j,c in enumerate(curves):draw.text((left+10,top+8+j*17),f"{c['channel']['name']} ({c['channel']['unit']})",font=font(11),fill=c['color'])
        draw.text((left,top-23),'时域波形 / s' if kind=='wave' else '幅值频谱 / Hz',font=font(13),fill='#203247')
        for a in result.get('annotations',[]):
            if a.get('curve','main')!='main' and not any(c['key']==a['curve'] for c in curves):continue
            if a['plot']!=kind or not xmin<=a['x']<=xmax or not ymin<=a['y']<=ymax:continue
            ax,ay=px(a['x']),py(a['y']);tx=max(left,min(right-150,ax+a['dx']*(right-left)));ty=max(top,min(bottom-25,ay+a['dy']*(bottom-top)))
            draw.line((ax,ay,tx,ty),fill='#a55f1c');draw.text((tx,ty),a['text'][:100],font=font(12),fill='#a55f1c')
    return im


def scalar_rows(result):
    p=result['parameters'];s=result['statistics']
    return [('开始时间(s)',p['start']),('结束时间(s)',p['end']),('采样频率(Hz)',p['sample_rate']),('点数',p['count']),('最大值',s['max']),('最小值',s['min']),('平均值',s['mean']),('中间值',s['median']),('有效值',s['desktop_effective']),(f"{p['confidence']}%峰峰值",s['peak_to_peak'])]


def excel_report(results,summary_only=False):
    from openpyxl import Workbook
    from openpyxl.styles import Font,PatternFill,Alignment
    from openpyxl.drawing.image import Image as XLImage
    wb=Workbook();summary=wb.active;summary.title='汇总';summary.append(['文件','工况','通道','单位']+[r[0] for r in scalar_rows(results[0][0])]+[f'{i}频率' if j==0 else f'{i}幅值' for i in range(1,9) for j in range(2)])
    for i,(result,arrays) in enumerate(results):
        src=result['source'];row=[src['filename'],src['condition']['name'],src['channel']['name'],src['channel']['unit']]+[v for _,v in scalar_rows(result)]
        for peak in result['peaks']:row.extend([peak['frequency'],peak['amplitude']])
        summary.append(row);ws=wb.create_sheet(f'分析{i+1}');ws.append(['标题',src['title']]);ws.append(['SHA256',src['sha256']]);ws.append(['参数',json.dumps(result['parameters'],ensure_ascii=False)])
        for label,value in scalar_rows(result):ws.append([label,value])
        ws.append(['序号','频率(Hz)','幅值','相位(度)'])
        for j,peak in enumerate(result['peaks'],1):ws.append([j,peak['frequency'],peak['amplitude'],peak['phase_deg']])
        png,_=image_bytes(plot_image(result,arrays,{'width':1100,'height':760}),'png');img=XLImage(io.BytesIO(png));img.width=770;img.height=532;ws.add_image(img,'F2')
        for kind,xa,ya in [('波形',arrays[0],arrays[1]),('频谱',arrays[2],arrays[3])]:
            if summary_only:continue
            data=wb.create_sheet(f'{kind}{i+1}');data.append(['时间(s)' if kind=='波形' else '频率(Hz)',f"数值 ({src['channel']['unit']})"])
            # Excel row cap is explicit; never silently truncate data.
            if len(xa)>1_048_575:raise SelError('数据超过 Excel 单表行数，请改用 CSV/CHD 完整导出')
            for x,y in zip(xa,ya):data.append([float(x),float(y)])
    for ws in wb:
        ws.freeze_panes='A2';ws.auto_filter.ref=ws.dimensions
        for cell in ws[1]:cell.font=Font(bold=True,color='FFFFFF');cell.fill=PatternFill('solid',fgColor='1762B5')
        ws.column_dimensions['A'].width=28;ws.column_dimensions['B'].width=25
    out=io.BytesIO();wb.save(out);return out.getvalue()


def word_report(results):
    from docx import Document
    from docx.shared import Inches,Pt
    from docx.oxml import OxmlElement
    from docx.oxml.ns import qn
    doc=Document();style=doc.styles['Normal'];style.font.name='Microsoft YaHei';style.font.size=Pt(10);style.element.rPr.rFonts.set(qn('w:eastAsia'),'Microsoft YaHei')
    for i,(result,arrays) in enumerate(results):
        if i:doc.add_page_break()
        src=result['source'];doc.add_heading(src['title'],0);doc.add_paragraph(f"{src['filename']} / {src['condition']['name']} / {src['channel']['name']} / {src['channel']['unit']}")
        table=doc.add_table(rows=1,cols=2);table.style='Light Shading Accent 1';table.rows[0].cells[0].text='指标';table.rows[0].cells[1].text='数值'
        for label,value in scalar_rows(result):cells=table.add_row().cells;cells[0].text=label;cells[1].text=f'{value:.6f}' if isinstance(value,float) else str(value)
        png,_=image_bytes(plot_image(result,arrays,{'width':1200,'height':780}),'png');doc.add_picture(io.BytesIO(png),width=Inches(6.1))
        peaks=doc.add_table(rows=1,cols=4);peaks.style='Light Shading Accent 1'
        for c,t in zip(peaks.rows[0].cells,['序号','频率 / Hz','幅值','相位 / 度']):c.text=t
        for j,p in enumerate(result['peaks'],1):
            for c,t in zip(peaks.add_row().cells,[str(j),f"{p['frequency']:.6f}",f"{p['amplitude']:.6f}",f"{p['phase_deg']:.6f}"]):c.text=t
        doc.add_paragraph('输入 SHA256：'+src['sha256']);doc.add_paragraph('分析参数：'+json.dumps(result['parameters'],ensure_ascii=False))
    out=io.BytesIO();doc.save(out);return out.getvalue()


def legacy_data(result,arrays,kind):
    if kind=='chd':return ''.join(f'{y:.6f}\r\n' for y in arrays[1]).encode('ascii')
    p=result['parameters'];text=f"起始时间\t终止时间\r\n{p['start']:.6f}\t{p['end']:.6f}\r\n\r\n序号\t频率\t幅值\t相位\r\n"
    for i,peak in enumerate(result['peaks'],1):text+=f"{i}\t{peak['frequency']:.6f}\t{peak['amplitude']:.6f}\t{peak['phase_deg']:.6f}\r\n"
    return text.encode('gb18030')
