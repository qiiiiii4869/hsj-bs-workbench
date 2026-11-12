"""Read SEL 20091203, following ScanSel/SortData.cpp (Win32 field widths).

No MFC, NI, window handles or desktop application are required.
"""
from pathlib import Path
import hashlib
import numpy as np


class SelError(ValueError):
    pass


class SelFile:
    def __init__(self, path):
        self.path = Path(path)
        size = self.path.stat().st_size
        # The serialized header is fixed-size; do not read waveform payload here.
        with self.path.open('rb') as stream:
            raw = stream.read(90000)
        pos = 0

        def take(dtype, count=1):
            nonlocal pos
            end = pos + np.dtype(dtype).itemsize * count
            if end > len(raw):
                raise SelError('SEL 文件头不完整')
            values = np.frombuffer(raw, dtype=dtype, count=count, offset=pos).copy()
            pos = end
            return values

        def integer():
            return int(take('<i4')[0])

        def strings(count, width):
            return [bytes(v).split(b'\0', 1)[0].decode('gb18030', errors='replace').strip()
                    for v in take(f'S{width}', count)]

        self.version = integer()
        if self.version != 20091203:
            raise SelError(f'暂不支持 SEL 格式版本 {self.version}，当前支持 20091203')
        self.rates = take('<i4', 64)
        self.divisors = take('<i4', 64)
        self.lengths = take('<i4', 100 * 64).reshape(100, 64)
        self.sample_rate, self.deal_span, self.sort_count, self.sort_length = [integer() for _ in range(4)]
        self.begin = take('<u4', 100)
        self.end = take('<u4', 100)
        self.starts = take('<i4', 100)
        self.ends = take('<i4', 100)
        self.action_starts=take('<i4',100)
        self.action_ends=take('<i4',100)
        self.sort_valid=take('u1',100)  # desktop does not filter these
        self.reference_ids=take('<u2',100)
        type_count = integer()
        para_counts = take('<i4', 16)
        self.ui_states=take('u1',1+16+16*50+64)
        self.set_id = strings(1, 64)[0]
        self.key_index, self.velocity_index, self.channel_count = [integer() for _ in range(3)]
        ids = take('<u2', 64)
        names = strings(64, 64)
        units = strings(64, 16)
        self.a = take('<f4', 64)
        self.b = take('<f4', 64)
        fft_flags = take('u1', 64)
        self.exam = strings(1, 64)[0]
        types = strings(16, 64)
        paras = strings(16 * 50, 64)
        codes = take('<u2', 16 * 50).reshape(16, 50)
        self.header_size = pos
        if not 1 <= self.sort_count <= 100 or not 1 <= self.channel_count <= 64 or not 0 <= type_count <= 16:
            raise SelError('SEL 工况或通道数量超出格式范围')
        active = self.lengths[:self.sort_count, :self.channel_count]
        if np.any(active < 0) or np.any(self.rates[:self.channel_count] <= 0) or self.deal_span <= 0:
            raise SelError('SEL 点数或采样参数无效')
        flat = active.astype(np.int64).ravel()
        self.offsets = (pos + np.r_[0, np.cumsum(flat)[:-1]] * 4).reshape(active.shape)
        if pos + int(flat.sum()) * 4 > size:
            raise SelError('SEL 波形数据被截断，与文件头声明长度不符')
        self.channels = [dict(index=i, id=int(ids[i]), name=names[i] or f'通道 {i+1}',
                              unit=units[i], sample_rate=int(self.rates[i]), fft_enabled=bool(fft_flags[i]),
                              divisor=int(self.divisors[i]),a=float(self.a[i]) if np.isfinite(self.a[i]) else None,b=float(self.b[i]) if np.isfinite(self.b[i]) else None)
                         for i in range(self.channel_count)]
        self.conditions = []
        self._key_cache = {}
        for t in range(type_count):
            if not 0 <= para_counts[t] <= 50:
                raise SelError('SEL 工况参数数量无效')
            for p in range(int(para_counts[t])):
                code = int(codes[t, p])
                if code >= self.sort_count:
                    raise SelError('SEL 工况索引超出范围')
                self.conditions.append(dict(index=len(self.conditions), type=types[t],
                                            name=paras[t*50+p], sort_code=code))
        if not self.conditions:
            raise SelError('SEL 中没有可分析的工况')

    def metadata(self):
        conditions = []
        for item in self.conditions:
            code = item['sort_code']
            channels = []
            for c in self.channels:
                count = self.logical_count(code, c['index'])
                points = self.key_phase_points(item['index'], c['index'])
                available = len(points) >= 2 and int(points[-1]) > int(points[0])
                channels.append(dict(c, stored_count=int(self.lengths[code, c['index']]), count=count,
                                     full_period_available=available, key_phase_count=len(points),
                                     desktop_start=int(points[0])/c['sample_rate'] if available else 0,
                                     desktop_end=int(points[-1])/c['sample_rate'] if available else count/c['sample_rate']))
            ref=int(self.reference_ids[code])
            conditions.append(dict(item, channels=channels,begin_timestamp=int(self.begin[code]),end_timestamp=int(self.end[code]),sort_start=int(self.starts[code]),sort_end=int(self.ends[code]),
                                   action_start=int(self.action_starts[code]),action_end=int(self.action_ends[code]),sort_valid=bool(self.sort_valid[code]),reference_channel_id=ref,
                                   reference_channel_name=next((c['name'] for c in self.channels if c['id']==ref),'')))
        return dict(version=self.version, exam=self.exam, header_size=self.header_size,
                    size=self.path.stat().st_size, conditions=conditions, channels=self.channels,
                    sampling_configuration=dict(sample_rate=self.sample_rate,deal_span=self.deal_span,sort_count=self.sort_count,sort_length=self.sort_length,key_index=self.key_index,velocity_index=self.velocity_index,set_id=self.set_id),
                    ui_states=self.ui_states.tolist())

    def logical_count(self, sort, channel):
        # FileData::GetDataFromFile reads only the defined span from the stored block.
        stored = int(self.lengths[sort, channel])
        wanted = int(self.ends[sort] - self.starts[sort]) * int(self.rates[channel]) * self.deal_span
        if wanted < 0 or wanted > stored:
            raise SelError('工况有效长度与存储点数不一致')
        return wanted

    def read(self, condition, channel):
        if not 0 <= condition < len(self.conditions) or not 0 <= channel < self.channel_count:
            raise SelError('工况或通道不存在')
        code = self.conditions[condition]['sort_code']
        count = self.logical_count(code, channel)
        if count < 4:
            raise SelError('当前工况通道有效数据不足 4 点')
        with self.path.open('rb') as stream:
            stream.seek(int(self.offsets[code, channel]))
            data = np.fromfile(stream, dtype='<f4', count=count).astype(np.float64)
        if data.size != count or not np.all(np.isfinite(data)):
            raise SelError('通道数据不完整或含非有限数值')
        # Stored values are already engineering values; FileData.cpp applies no A/B transform.
        return data

    def key_phase_points(self, condition, channel):
        """FileData::GetKeyPhase -> CNiWaveFraqView::SetKeyPhase, not threshold detection."""
        if not 0 <= condition < len(self.conditions) or not 0 <= channel < self.channel_count:
            raise SelError('工况或通道不存在')
        if self.key_index == -1:
            return np.empty(0, dtype=np.float32)
        if not 0 <= self.key_index < self.channel_count:
            raise SelError('SEL 键相通道索引无效')
        code = self.conditions[condition]['sort_code']
        rate = int(self.rates[channel])
        cache_key = (code, rate)
        if cache_key in self._key_cache:
            return self._key_cache[cache_key]
        key_rate = int(self.rates[self.key_index])
        if self.logical_count(code, self.key_index) < 4:
            return np.empty(0, dtype=np.float32)
        packed_key = (code, 'packed')
        if packed_key not in self._key_cache:
            self._key_cache[packed_key] = self.read(condition, self.key_index).astype(np.float32)
        packed = self._key_cache[packed_key]
        divisor = max(1, (key_rate+rate-1)//rate)
        count = self.logical_count(code, channel)
        parts = []
        for base in range(0, len(packed), key_rate):
            pulses = int(packed[base])
            if pulses <= 0:
                continue
            if pulses >= key_rate or base+1+pulses > len(packed):
                raise SelError('SEL 键相记录的脉冲数量无效')
            # The original x and kp[] are float, including division by d_num.
            points = ((np.float32(base)+packed[base+1:base+1+pulses])/np.float32(divisor)).astype(np.float32)
            parts.append(points[(points >= 0) & (points < count)])
        points = np.concatenate(parts) if parts else np.empty(0, dtype=np.float32)
        if len(points)>1 and np.any(np.diff(points)<0):
            raise SelError('SEL 键相点顺序无效')
        self._key_cache[cache_key] = points
        return points


def sha256(path):
    with open(path, 'rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()
