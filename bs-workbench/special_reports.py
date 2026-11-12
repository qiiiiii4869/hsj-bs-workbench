"""Complete specialty tables, source records, curves and graphics exports."""
import io
import json
import math
from PIL import Image, ImageDraw
from outputs import font, image_bytes
from sel_reader import SelError


def safe_cell(value):
    if isinstance(value,str) and value.startswith(('=','+','-','@')):return "'"+value
    return value


def plots(result):
    yield dict(title=result.get('title','分析结果'),axes=result.get('axes',['X','Y']),curves=result.get('curves',[]),equal_axes=result.get('equal_axes',False))
    yield from result.get('plots',[])
    for wave in result.get('waves',[]):
        yield dict(title=wave['name'],axes=['时间(s)','幅值'],curves=[wave])


def plot_picture(plot,width=1300,height=760,limits=None):
    if not 400<=width<=8000 or not 300<=height<=8000 or width*height>24000000:raise SelError('图像尺寸超出支持范围')
    image=Image.new('RGB',(width,height),'white');draw=ImageDraw.Draw(image)
    curves=[c for c in plot.get('curves',[]) if c.get('visible',True)]
    points=[p for c in curves for p in c.get('points',[])]
    if limits is None:
        limits=[]
        for axis in range(2):
            vals=[float(p[axis]) for p in points if len(p)==2 and math.isfinite(float(p[axis]))]
            lo,hi=(min(vals),max(vals)) if vals else (-1,1);pad=(hi-lo)*.06 or 1
            limits.append([lo-pad,hi+pad])
    if len(limits)!=2 or any(len(v)!=2 or not all(math.isfinite(float(x)) for x in v) or v[0]>=v[1] for v in limits):raise SelError('报表坐标范围无效')
    left,top,right,bottom=95,85,width-35,height-85
    if plot.get('equal_axes'):
        scale=min((right-left)/(limits[0][1]-limits[0][0]),(bottom-top)/(limits[1][1]-limits[1][0]))
        pw=scale*(limits[0][1]-limits[0][0]);ph=scale*(limits[1][1]-limits[1][0])
        left=int(left+(right-left-pw)/2);top=int(top+(bottom-top-ph)/2);right=left+max(1,int(pw));bottom=top+max(1,int(ph))
    x=lambda v:left+(v-limits[0][0])/(limits[0][1]-limits[0][0])*(right-left)
    y=lambda v:bottom-(v-limits[1][0])/(limits[1][1]-limits[1][0])*(bottom-top)
    draw.text((width/2,25),plot.get('title',''),font=font(20),fill='#273440',anchor='mt')
    for i in range(6):
        px=left+(right-left)*i/5;py=top+(bottom-top)*i/5
        draw.line([(px,top),(px,bottom)],fill='#d0d0d0');draw.line([(left,py),(right,py)],fill='#d0d0d0')
        draw.text((px,bottom+12),f'{limits[0][0]+(limits[0][1]-limits[0][0])*i/5:.5g}',font=font(12),fill='#555555',anchor='mt')
        draw.text((left-10,py),f'{limits[1][1]-(limits[1][1]-limits[1][0])*i/5:.5g}',font=font(12),fill='#555555',anchor='rm')
    layer=Image.new('RGB',(right-left,bottom-top),'white');pen=ImageDraw.Draw(layer)
    # Grid on the clipped plot layer, preserving curves outside custom limits.
    for i in range(6):pen.line([(0,(bottom-top)*i/5),(right-left,(bottom-top)*i/5)],fill='#d0d0d0');pen.line([((right-left)*i/5,0),((right-left)*i/5,bottom-top)],fill='#d0d0d0')
    for c in curves:
        coordinates=[(x(float(p[0]))-left,y(float(p[1]))-top) for p in c.get('points',[]) if len(p)==2 and all(math.isfinite(float(v)) for v in p)]
        if len(coordinates)>1:pen.line(coordinates,fill=c.get('color','#0000ff'),width=2)
        elif coordinates:pen.ellipse([coordinates[0][0]-2,coordinates[0][1]-2,coordinates[0][0]+2,coordinates[0][1]+2],fill=c.get('color','#0000ff'))
    image.paste(layer,(left,top));draw.rectangle((left,top,right,bottom),outline='#777777',width=1)
    axes=plot.get('axes',['X','Y']);draw.text(((left+right)/2,height-28),axes[0],font=font(14),fill='#333333',anchor='mm');draw.text((12,52),axes[1],font=font(14),fill='#333333')
    offset=left
    for c in curves:
        draw.line([(offset,62),(offset+18,62)],fill=c.get('color','#0000ff'),width=2);draw.text((offset+24,53),c['name'],font=font(12),fill='#333333');offset+=max(130,len(c['name'])*14+45)
    return image


def workbook(results):
    from openpyxl import Workbook
    from openpyxl.styles import Font,PatternFill,Alignment
    from openpyxl.drawing.image import Image as XLImage
    book=Workbook();summary=book.active;summary.title='汇总';all_columns=[]
    for r in results:
        for name in r.get('columns',[]):
            if name not in all_columns:all_columns.append(name)
    summary.append(['分析标题',*all_columns])
    for index,r in enumerate(results,1):
        sheet=book.create_sheet(f'结果{index}');sheet.append(['标题',r.get('title','')]);sheet.append(['输入参数',json.dumps(r.get('request',{}),ensure_ascii=False)]);sheet.append([safe_cell(v) for v in r.get('columns',[])])
        columns=r.get('columns',[])
        for row in r.get('rows',[]):
            sheet.append([safe_cell(v) for v in row]);mapped=dict(zip(columns,row));summary.append([safe_cell(r.get('title','')),*[safe_cell(mapped.get(k)) for k in all_columns]])
        for extra_index,table in enumerate(r.get('tables',[]),1):
            extra=book.create_sheet(f'参数表{index}-{extra_index}');extra.append([safe_cell(v) for v in table.get('columns',[])])
            for row in table.get('rows',[]):extra.append([safe_cell(v) for v in row])
        for j,plot in enumerate(plots(r),1):
            graphic=book.create_sheet(f'图{index}-{j}');graphic.append(['标题',plot.get('title','')]);graphic.append(['X轴',plot.get('axes',['X','Y'])[0]]);graphic.append(['Y轴',plot.get('axes',['X','Y'])[1]])
            if r.get('include_graphs',True):
                png,_=image_bytes(plot_picture(plot),'png');picture=XLImage(io.BytesIO(png));picture.width=910;picture.height=532;graphic.add_image(picture,'E2')
            graphic.append(['曲线','X','Y','颜色','可见'])
            for c in plot.get('curves',[]):
                for p in c.get('points',[]):graphic.append([safe_cell(c['name']),*p,c.get('color'),c.get('visible',True)])
        if r.get('details'):
            detail=book.create_sheet(f'工况明细{index}')
            legacy=book.create_sheet(f'原版趋势表{index}')
            legacy.append(['序号','记录名称','参考通道均值','最小值','最大值','平均值','中间值','有效值','峰峰值']+[f'{j}{name}' for j in range(1,9) for name in ('频率','幅值','相位')])
            for sequence,record in enumerate(r['details'],1):
                stats=record['statistics'];line=[sequence,record['condition'],record['reference'],stats['min'],stats['max'],stats['mean'],stats['median'],stats['desktop_effective'],stats['peak_to_peak']]
                for peak in record.get('peaks',[]):line.extend([peak['frequency'],peak['amplitude'],peak['phase_deg']])
                legacy.append(line)
                detail.append(['工况',record['condition']]);detail.append(['参考通道均值',record['reference']]);detail.append(['开始(s)',record['start'],'结束(s)',record['end'],'采样率',record['sample_rate'],'点数',record['count']])
                for name,value in record['statistics'].items():detail.append([name,value])
                detail.append(['序号','频率(Hz)','幅值','相位'])
                for k,peak in enumerate(record.get('peaks',[]),1):detail.append([k,peak['frequency'],peak['amplitude'],peak['phase_deg']])
                detail.append([])
            if r.get('include_graphs',True) and legacy.max_row>1:
                from openpyxl.chart import ScatterChart,Series,Reference
                chart=ScatterChart();chart.title=r.get('title','趋势分析');chart.x_axis.title='参考通道均值';chart.y_axis.title='通道统计值';chart.scatterStyle='lineMarker'
                xs=Reference(legacy,min_col=3,min_row=2,max_row=legacy.max_row)
                for col in range(4,10):chart.series.append(Series(Reference(legacy,min_col=col,min_row=1,max_row=legacy.max_row),xs,title_from_data=True))
                legacy.add_chart(chart,f'A{legacy.max_row+3}')
    for sheet in book:
        sheet.freeze_panes='A2';sheet.column_dimensions['A'].width=32;sheet.column_dimensions['B'].width=28
        for cell in sheet[1]:cell.font=Font(bold=True,color='FFFFFF');cell.fill=PatternFill('solid',fgColor='305D8A')
        sheet.sheet_properties.pageSetUpPr.fitToPage=True
    stream=io.BytesIO();book.save(stream);return stream.getvalue()


def word_report(results):
    from docx import Document
    from docx.shared import Inches,Pt
    from docx.oxml.ns import qn
    doc=Document();style=doc.styles['Normal'];style.font.name='Microsoft YaHei';style.font.size=Pt(10);style.element.rPr.rFonts.set(qn('w:eastAsia'),'Microsoft YaHei')
    for i,r in enumerate(results):
        if i:doc.add_page_break()
        doc.add_heading(r.get('title','分析结果'),0)
        columns=r.get('columns',[])
        if columns:
            table=doc.add_table(rows=1,cols=len(columns));table.style='Light Shading Accent 1'
            for cell,value in zip(table.rows[0].cells,columns):cell.text=str(value)
            for row in r.get('rows',[]):
                for cell,value in zip(table.add_row().cells,row):cell.text=str(value)
        for extra in r.get('tables',[]):
            doc.add_heading(extra.get('title','参数表'),2);columns=extra.get('columns',[])
            if columns:
                table=doc.add_table(rows=1,cols=len(columns));table.style='Light Shading Accent 1'
                for cell,value in zip(table.rows[0].cells,columns):cell.text=str(value)
                for row in extra.get('rows',[]):
                    for cell,value in zip(table.add_row().cells,row):cell.text='—' if value is None else str(value)
        if r.get('include_graphs',True):
            for plot in plots(r):
                png,_=image_bytes(plot_picture(plot),'png');doc.add_picture(io.BytesIO(png),width=Inches(6.2))
        doc.add_paragraph('输入参数：'+json.dumps(r.get('request',{}),ensure_ascii=False))
    stream=io.BytesIO();doc.save(stream);return stream.getvalue()
