"""Generate new flat toolbar strips; retain original slot sizes and command order."""
from pathlib import Path
import re
from PIL import Image, ImageDraw, ImageFont

SRC = Path(__file__).resolve().parent.parent / 'ScanAnalysis201204021-20231225'
text = (SRC / 'ScanAnalysis.rc').read_bytes().decode('gbk')
resources = dict(re.findall(r'^(\w+)\s+BITMAP\s+"([^"]+)"', text, re.M))
S = 4
BLUE = (40, 104, 197, 255)


def icon(command, height):
    im = Image.new('RGBA', (16*S, height*S))
    d = ImageDraw.Draw(im)
    def line(points, color=BLUE, width=1.15):
        d.line([(round(x*S), round(y*S)) for x,y in points], fill=color, width=max(1,round(width*S)), joint='curve')
    def box(rect):
        d.rectangle(tuple(round(x*S) for x in rect), outline=BLUE, width=S)
    def ellipse(rect):
        d.ellipse(tuple(round(x*S) for x in rect), outline=BLUE, width=S)
    def wave(y=8):
        line([(1,y),(4,y),(5,y-4),(7,y+4),(9,y-3),(11,y+1),(14,y+1)])
    def plus(x=11,y=4):
        line([(x-2,y),(x+2,y)]);line([(x,y-2),(x,y+2)])
    def arrow(x=8,y=8):
        line([(x,y-4),(x,y+3)]);line([(x-3,y),(x,y+3),(x+3,y)])
    def page():
        line([(3,14),(3,2),(10,2),(13,5),(13,14),(3,14)]);line([(10,2),(10,5),(13,5)])
    def badge(letter):
        font=ImageFont.truetype('C:/Windows/Fonts/segoeuib.ttf',8*S)
        d.text((8*S,8*S),letter,font=font,fill=BLUE,anchor='mm')
    c=command.lower()
    if 'fileopendata' in c:
        line([(1,13),(1,4),(6,4),(7,6),(14,6),(12,13),(1,13),(3,7),(14,7)])
    elif 'property' in c or 'pproperty' in c:
        box((2,2,14,14));line([(2,5),(14,5)]);line([(6,5),(6,14)]);line([(3,8),(13,8)]);line([(3,11),(13,11)])
    elif 'about' in c or 'help' in c:
        ellipse((2,2,14,14));badge('?' if 'help' in c else 'i')
    elif 'save' in c or 'report' in c or 'text' in c:
        if 'image' in c or 'view' in c or 'pic' in c:
            box((1,3,14,13));ellipse((3,5,6,8));line([(3,12),(7,8),(9,10),(11,7),(14,11)])
        elif 'excel' in c:
            page();badge('X')
        elif 'word' in c:
            page();badge('W')
        else:
            page();line([(5,7),(11,7)]);line([(5,10),(11,10)])
        if 'bat' in c:
            line([(1,4),(1,15),(12,15)])
    elif 'curror' in c:
        line([(8,1),(8,14)]);line([(1,8),(14,8)])
        if 'nearest' in c:box((6,6,10,10))
        else:ellipse((6,6,10,10))
    elif 'unzoom' in c or 'resume' in c or 'reset' in c:
        d.arc((3*S,3*S,13*S,13*S),40,310,fill=BLUE,width=S)
        line([(3,2),(3,7),(7,7)])
        if 'filter' in c:line([(7,10),(9,12),(14,7)])
    elif 'zoom' in c:
        ellipse((2,2,10,10));line([(9,9),(14,14)],width=1.4)
        if 'xzoom' in c or 'x_zoom' in c:line([(3,6),(9,6)])
        elif 'yzoom' in c or 'y_zoom' in c:line([(6,3),(6,9)])
        else:plus(6,6)
    elif 'moving' in c:
        line([(1,8),(14,8)]);line([(4,5),(1,8),(4,11)]);line([(11,5),(14,8),(11,11)])
        if 'xmoving' not in c:line([(8,1),(8,14)]);line([(5,4),(8,1),(11,4)])
    elif 'annotation' in c or 'title' in c or 'font' in c:
        if 'title' in c or 'font' in c:
            line([(3,3),(13,3)]);line([(8,3),(8,13)]);line([(5,13),(11,13)])
        else:
            line([(2,11),(10,3),(13,6),(5,14),(2,14),(2,11)]);line([(9,4),(12,7)])
    elif 'filter' in c:
        line([(2,3),(14,3),(9,8),(9,13),(6,11),(6,8),(2,3)])
    elif 'addchndata' in c or 'addsignal' in c or 'addsign' in c:
        wave(10);plus(11,3)
    elif 'group' in c:
        box((1,2,6,6));box((10,9,14,13));box((1,9,5,13));line([(4,6),(4,8),(12,8),(12,9)])
    elif 'keyphase' in c:
        line([(1,12),(4,12),(4,3),(6,3),(6,12),(11,12),(11,3),(13,3),(13,12),(15,12)])
    elif 'fullperiod' in c or 'fraqgraph' in c:
        line([(2,13),(2,3)]);line([(5,13),(5,7)]);line([(8,13),(8,2)]);line([(11,13),(11,8)]);line([(14,13),(14,11)])
    elif 'wavefraq' in c:
        box((1,1,14,14));line([(1,8),(14,8)]);line([(2,5),(4,5),(5,2),(7,6),(9,3),(12,5)]);line([(4,13),(4,10)]);line([(7,13),(7,9)])
    elif 'wavegraph' in c:
        wave()
    elif 'trend' in c or 'mobtrend' in c:
        line([(2,2),(2,13),(14,13)]);line([(3,11),(6,8),(9,9),(13,4)])
        if 'mean' in c:line([(3,6),(13,6)])
        elif 'median' in c:badge('M')
        elif 'rms' in c:badge('R')
        elif 'ppvalue' in c:line([(12,3),(12,11)]);line([(10,3),(14,3)])
    elif 't3d' in c:
        line([(2,3),(2,12),(10,12),(14,8),(14,1)]);line([(2,12),(6,8),(14,8)])
        for j in [0,2,4]:line([(3+j,10-j),(5+j,10-j),(6+j,5-j),(8+j,10-j)])
    elif 'axis' in c or 'coherentays' in c:
        line([(8,1),(8,14)]);line([(1,8),(14,8)]);ellipse((4,3,12,12))
    elif 'dynamic' in c or 'balance' in c:
        ellipse((3,3,13,13));line([(8,8),(12,4)]);ellipse((6,6,10,10))
    elif 'load' in c:
        page();arrow(8,8)
    elif 'delete' in c:
        line([(4,4),(12,12)]);line([(12,4),(4,12)])
    elif 'jigger' in c:
        ellipse((2,2,14,14));line([(8,3),(8,13)]);line([(3,8),(13,8)]);ellipse((7,6,11,10))
    elif 'efficiency' in c:
        d.arc((2*S,2*S,14*S,14*S),180,360,fill=BLUE,width=S);line([(8,9),(12,5)]);line([(2,11),(14,11)])
    elif 'cam' in c:
        line([(2,2),(2,13),(14,13)]);line([(3,11),(6,9),(8,5),(11,4),(14,3)])
    elif 'setrange' in c:
        box((2,2,13,13));line([(5,2),(5,13)]);line([(9,2),(9,13)]);line([(2,6),(13,6)]);line([(2,10),(13,10)])
    elif 'stripchart' in c:
        box((1,2,14,14));wave(7);line([(3,11),(12,11)])
    else:
        wave()
    return im.resize((16,height),Image.Resampling.LANCZOS)


for m in re.finditer(r'^(\w+)\s+TOOLBAR\s+(\d+),\s*(\d+)\s*\nBEGIN\s*\n([\s\S]*?)\nEND',text,re.M):
    rid,w,h,body=m.groups();w,h=int(w),int(h)
    if rid not in resources:continue
    commands=re.findall(r'\bBUTTON\s+(\w+)',body)
    p=SRC/resources[rid].replace('\\\\','/').replace('\\','/')
    # New artwork is rendered from geometric primitives, not recolored old pixels.
    strip=Image.new('RGB',(w*len(commands),h),(192,192,192))
    for i,c in enumerate(commands):
        tile=icon(c,h)
        strip.paste(tile,(i*w,0),tile)
    strip.save(p,'BMP')
    print(rid,len(commands),strip.size)
