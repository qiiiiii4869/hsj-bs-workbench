"""Dispatch native SEL and explicitly configured external CHD waveforms."""
import json
from pathlib import Path
import numpy as np
from sel_reader import SelFile,SelError


class ChdFile:
    def __init__(self,path):
        self.path=Path(path);self.config=json.loads(self.path.with_suffix('.json').read_text('utf-8'))
        text=self.path.read_bytes().decode('utf-8-sig',errors='strict')
        try:self.values=np.asarray([float(v) for v in text.split()],dtype=np.float64)
        except ValueError:raise SelError('CHD 须为仅含数值的波形文本；频谱结果 CHD 不能当波形导入')
        if len(self.values)<4 or not np.all(np.isfinite(self.values)):raise SelError('CHD 至少需要 4 个有限数值采样点')
        rate=float(self.config['sample_rate'])
        if not np.isfinite(rate) or not rate.is_integer() or rate<=0:raise SelError('采样频率须为正整数 Hz')
        self.rates=np.asarray([int(rate)]);self.channel_count=1;self.conditions=[dict(index=0,type='外部数据',name='CHD 波形',sort_code=0)]
        self.channels=[dict(index=0,id=0,name=self.config['title'],unit=self.config.get('unit',''),sample_rate=int(rate),fft_enabled=True)]
        self.version='CHD';self.exam=self.config['title']
    def read(self,condition,channel):
        if condition!=0 or channel!=0:raise SelError('外部 CHD 仅有一个通道')
        return self.values.copy()
    def key_phase_points(self,condition,channel):return np.empty(0)
    def logical_count(self,condition,channel):return len(self.values)
    def metadata(self):
        c=dict(self.channels[0],count=len(self.values),stored_count=len(self.values),full_period_available=False,key_phase_count=0,desktop_start=0,desktop_end=len(self.values)/int(self.rates[0]))
        return dict(version='CHD',exam=self.exam,size=self.path.stat().st_size,color=self.config.get('color','#1762b5'),channels=self.channels,conditions=[dict(self.conditions[0],channels=[c])])


def open_reader(path):return ChdFile(path) if Path(path).suffix.lower()=='.chd' else SelFile(path)
