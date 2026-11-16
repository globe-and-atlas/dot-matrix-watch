"""Capture fixture Hi-Fi from the already-owned Emery emulator."""
from pathlib import Path
import argparse
import json
import socket
import subprocess
import time
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
PALETTES=[(0,0x55FF55,0x55FF55),(0,0xFFAA00,0xFFAA00),(0x000055,0xAAFFFF,0x55AAFF),(0x550055,0xFFAAFF,0xFF55AA),(0,0xFFFFFF,0xFFFFFF),(0xFFFFFF,0,0xAA0000),(0x000055,0xFFFFFF,0xFF55AA)]

def rgb(value):
    return ((value>>16)&255,(value>>8)&255,value&255)

def screenshot(port, peak=255):
    path=ROOT/'.tmp/native.ppm'
    path.unlink(missing_ok=True)
    with socket.create_connection(('localhost',port),timeout=5) as connection:
        connection.recv(65536)
        connection.sendall(f'screendump "{path.resolve()}"\n'.encode())
        deadline=time.monotonic()+5
        while time.monotonic()<deadline:
            connection.recv(65536)
            try:
                image=Image.open(path).convert('RGB');break
            except (FileNotFoundError,OSError):continue
        else:raise TimeoutError('Incomplete QEMU screendump')
    actual=max(max(pixel) for pixel in image.get_flattened_data())
    image=image.point(lambda channel:round(channel/max(actual,1)*peak/85)*85)
    path.unlink(missing_ok=True)
    assert image.size==(200,228)
    return image

def numbers():
    import re
    source=(ROOT/'watchface/src/c/glyphs.h').read_text().split('NUMBERS[10][7]={',1)[1].split('};',1)[0]
    rows=re.findall(r'\{([0-9,]+)\}',source)
    result=[[int(n) for n in row.split(',')] for row in rows]
    assert len(result)==10
    return result


def check(image,hour,minute,fmt,date,palette,battery):
    bg,fg,accent=map(rgb,PALETTES[palette]);digits=numbers()
    display=hour if fmt==2 else (hour%12 or 12)
    values=[display//10,display%10,minute//10,minute%10];positions=[9,50,113,154]
    points=[(positions[i]+col*8,52+row*10) for i,n in enumerate(values) for row in range(7) for col in range(5) if digits[n][row]&(1<<(4-col))]
    left=min([97]+[x-2 for x,y in points]);right=max([101]+[x+2 for x,y in points]);offset=int((199-left-right)/2)
    assert image.getpixel((0,0))==bg,'Background differs'
    for i,n in enumerate(values):
        for row in range(7):
            for col in range(5):
                x=positions[i]+offset+col*8;y=52+row*10
                assert image.getpixel((x,y))==(fg if digits[n][row]&(1<<(4-col)) else bg),'Digit dot pattern differs'
                if col<4:assert image.getpixel((x+4,y))==bg,'Horizontal dots touch'
                if row<6:assert image.getpixel((x,y+5))==bg,'Vertical dots touch'
    assert image.getpixel((99+offset,72))==fg and image.getpixel((99+offset,92))==fg,'Colon differs'
    ink=[x for y in range(50,115) for x in range(200) if image.getpixel((x,y))==fg]
    assert abs((min(ink)+max(ink))/2-99.5)<=.5,'Clock ink off center'
    ampm=any(pixel==fg for pixel in image.crop((0,128,200,141)).get_flattened_data())
    assert ampm==(fmt==1),'AMPM differs'
    date_ink=any(pixel==fg for pixel in image.crop((0,151,200,164)).get_flattened_data())
    assert date_ink==(date!=0),'Date differs'
    battery_ink=[x for y in range(196,223) for x in range(200) if image.getpixel((x,y))==accent]
    midpoint=(min(battery_ink)+max(battery_ink))/2
    assert abs(midpoint-99.5)<=.5,'Battery group off center'
    return {'clock_midpoint':(min(ink)+max(ink))/2,'battery_midpoint':midpoint}


def send(keys,uuid,values):
    result=subprocess.run(['pebble','send-app-message','--emulator','emery','--app-uuid',uuid,'--int']+[f'{keys.get(k,k)}={v}' for k,v in values.items()],cwd=ROOT/'watchface',capture_output=True,text=True,timeout=45)
    if result.returncode:raise RuntimeError(result.stderr[-1500:])


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--monitor',type=int,required=True);options=parser.parse_args()
    keys=json.loads((ROOT/'watchface/build/js/message_keys.json').read_text());uuid=json.loads((ROOT/'watchface/package.json').read_text())['pebble']['uuid']
    cases=[('default',10,8,2,1,0,82)]
    cases += [(f'palette-{p}',10,8,2,1,p,82) for p in range(6)]
    cases += [('custom',10,8,2,1,6,82)]
    cases += [(f'date-{d}',10,8,2,d,0,82) for d in range(5)]
    cases += [(f'{name}-{fmt}h',h,m,fmt,1,0,82) for name,h,m in [('midnight',0,0),('noon',12,0),('late',23,59),('eleven',11,11),('twenty',20,20)] for fmt in [1,2]]
    cases += [('battery-empty',10,8,2,1,0,0),('battery-full',10,8,2,1,0,100)]
    evidence=[]
    for name,h,m,fmt,date,palette,battery in cases:
        send(keys,uuid,{'Palette':palette,'TimeFormat':fmt,'DateFormat':date,'Background':0x000055,'Foreground':0xFFFFFF,'Accent':0xFF55AA,11000:h,11001:m,11004:battery})
        for attempt in range(30):
            image=screenshot(options.monitor)
            try:result=check(image,h,m,fmt,date,palette,battery);break
            except AssertionError as failure:error=failure;time.sleep(.1)
        else:raise AssertionError(f'{name}: {error}')
        image.save(ROOT/f'media/dot-{name}.png');evidence.append({'case':name,**result});print('PASS:',name,flush=True)
    (ROOT/'.tmp/native-checks.json').write_text(json.dumps(evidence,indent=2))
    labels=['Mint','Amber','Ice','Rose','White','Paper']
    board=Image.new('RGB',(660,536),(32,32,32));draw=ImageDraw.Draw(board)
    for i,label in enumerate(labels):
        x=i%3*220+10;y=i//3*268;board.paste(Image.open(ROOT/f'media/dot-palette-{i}.png'),(x,y));draw.text((x,y+236),label,fill='white')
    board.save(ROOT/'media/dot-palettes.png')
    Image.open(ROOT/'media/dot-default.png').resize((400,456),Image.Resampling.NEAREST).save(ROOT/'media/dot-default-2x.png')
    send(keys,uuid,{'Palette':0,'TimeFormat':0,'DateFormat':1,11000:10,11001:8,11004:82})


if __name__=='__main__':main()
