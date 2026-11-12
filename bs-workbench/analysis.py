"""Explicit analysis conventions; desktop equivalence still requires baseline QA."""
import numpy as np
from stable_filter import apply_butterworth, FILTER_ENGINE_VERSION
from sel_reader import SelError

ENGINE_VERSION = 'hsj-web-0.9-wave-workbench-' + FILTER_ENGINE_VERSION


def filter_signal(data, sample_rate, chain):
    """Causal Butterworth with the desktop's leading data prefix, not filtfilt."""
    if not isinstance(chain, list):
        raise SelError('滤波历史格式无效')
    if not np.isfinite(sample_rate) or sample_rate <= 0:
        raise SelError('采样频率无效')
    # No extra copy on the unfiltered path; filtered output never changes the input.
    values = np.asarray(data, dtype=np.float64)
    if chain:
        values = values.copy()
    records = []
    for spec in chain:
        if not isinstance(spec, dict):
            raise SelError('滤波参数无效')
        kind = spec.get('type')
        if kind not in ('lowpass', 'highpass', 'bandpass', 'bandstop'):
            raise SelError('不支持的滤波类型')
        order = spec.get('order', 5)
        if isinstance(order, bool) or not isinstance(order, int) or not 1 <= order <= 100:
            raise SelError('滤波阶数须为 1–100 的整数')
        try:
            cutoff = [float(spec[k]) for k in (('low',) if kind=='lowpass' else ('high',) if kind=='highpass' else ('low','high'))]
        except (ValueError, TypeError, KeyError):
            raise SelError('请填写有效的截止频率')
        if any(not np.isfinite(f) or not 0 < f < sample_rate/2 for f in cutoff):
            raise SelError('截止频率须大于 0 且小于采样频率的一半')
        if len(cutoff)==2 and cutoff[0]>=cutoff[1]:
            raise SelError('带通/带阻的低截止频率须小于高截止频率')
        prefix = 3072 if len(values)>10240 else len(values)//10
        values = apply_butterworth(values, sample_rate, kind, order, cutoff, prefix)
        settings = dict(type=kind, order=order)
        if kind != 'highpass':
            settings['low'] = cutoff[0]
        if kind != 'lowpass':
            settings['high'] = cutoff[-1]
        records.append(dict(settings, cutoff=cutoff, prefix_count=prefix,
                            implementation=FILTER_ENGINE_VERSION, phase='causal', desktop_equivalence='参见滤波数值回归记录，非逐位等价'))
    return values, records


def confidence_pp(values, confidence):
    if isinstance(confidence, bool) or confidence not in (95,97,100):
        raise SelError('峰峰值置信度须为 95、97 或 100')
    if confidence == 100:
        return float(np.ptp(values))
    ordered = np.sort(values)
    tail = (100-confidence)/2
    return float(ordered[int(len(values)*(confidence+tail)/100)]-ordered[int(len(values)*tail/100)])


def envelope(x, y, limit=2400):
    if len(y) <= limit:
        return np.column_stack((x, y)).tolist()
    picked = [0]
    for lo, hi in zip(np.linspace(0, len(y), limit//2+1, dtype=int)[:-1],
                      np.linspace(0, len(y), limit//2+1, dtype=int)[1:]):
        chunk = y[lo:hi]
        picked.extend(sorted({lo + int(chunk.argmin()), lo + int(chunk.argmax())}))
    picked.append(len(y)-1)
    selected = np.unique(picked)
    return np.column_stack((x[selected], y[selected])).tolist()


def analyze(data, sample_rate, start=0, end=None, window='rectangular', remove_mean=False, confidence=97, filters=None, include_original=False, key_phase=None, full_period=False, return_full=False):
    if not np.isfinite(start) or start < 0 or (end is not None and (not np.isfinite(end) or end <= start)):
        raise SelError('请设置有效的分析起止时间')
    duration = len(data)/sample_rate
    if start >= duration or (end is not None and end > duration + 1e-8):
        raise SelError('分析时间超出通道范围')
    requested_start, requested_end = start, end
    delta_time = 1.0/sample_rate
    lo = int(np.ceil(start/delta_time))
    hi = len(data) if end is None else min(len(data), int(np.floor(end/delta_time)))
    if full_period:
        points = np.asarray([] if key_phase is None else key_phase)
        selected = points[(points >= lo) & (points <= hi)]
        if len(selected)<2 or int(selected[-1])<=int(selected[0]):
            raise SelError('当前区间不足两个有效键相点，请扩大区间或关闭键相整周期')
        # FFTProc chooses the first/last key-phase point, truncates float to int,
        # and passes NiData[xMin:xMax] to SetFraqData.
        lo, hi = int(selected[0]), int(selected[-1])
    n = hi-lo
    if n < 4:
        raise SelError('分析区间须至少包含 4 点，请扩大时间范围')
    if window not in ('rectangular', 'hann'):
        raise SelError('不支持的 FFT 窗函数')
    # Filter the full channel first, so selecting a time range does not change filter history.
    processed, filter_records = filter_signal(data, sample_rate, [] if filters is None else filters)
    values = processed[lo:hi]
    mean = float(values.mean())
    ac_rms = float(np.sqrt(np.mean((values-mean)**2)))
    weights = np.ones(n) if window == 'rectangular' else np.hanning(n)
    fft = np.fft.rfft((values-mean if remove_mean else values)*weights)
    # Match desktop SetFraqData: exclude Nyquist, amplitude=2*abs(FFT)/N, DC=0.
    amplitude = (np.abs(fft)*2/weights.sum())[:n//2]
    if not np.all(np.isfinite(amplitude)) or not np.isfinite(ac_rms) or not np.isfinite(mean):
        raise SelError('计算结果超出数值范围，请降低滤波阶数或调整截止频率')
    amplitude[0] = 0
    frequencies = np.arange(len(amplitude))*sample_rate/n
    # Original SetFraqData maps each of the eight largest amplitudes to its
    # first matching bin; tied values can therefore repeat the same frequency.
    candidates=amplitude[1:];count=min(8,len(candidates))
    largest=np.sort(np.partition(candidates,len(candidates)-count)[-count:])[::-1]
    rank=[int(np.flatnonzero(candidates==value)[0])+1 for value in largest]
    peaks = [dict(frequency=float(frequencies[k]), amplitude=float(amplitude[k]),
                  phase_deg=float(np.angle(fft[k], deg=True)) % 360) for k in rank]
    times = np.arange(lo, hi)/sample_rate
    result = dict(engine_version=ENGINE_VERSION,
                parameters=dict(start=lo/sample_rate, end=hi/sample_rate, count=n,
                                sample_rate=sample_rate, window=window, remove_mean=remove_mean,
                                confidence=confidence, filters=filter_records,
                                requested_start=requested_start, requested_end=requested_end,
                                start_index=lo, end_index_exclusive=hi, full_period=full_period,
                                key_phase_count=int(len(selected)) if full_period else 0,
                                delta_frequency=sample_rate/n, interval='[start,end)',
                                fft_mode='键相整周期；单边峰值幅度，直流置零，不含 Nyquist' if full_period else '非整周期；单边峰值幅度，直流置零，不含 Nyquist'),
                statistics=dict(max=float(values.max()), min=float(values.min()), mean=mean,
                                median=float(np.median(values)), peak_to_peak=confidence_pp(values, confidence),
                                peak_to_peak_100=float(np.ptp(values)),
                                rms=float(np.sqrt(np.mean(values**2))), ac_rms=ac_rms,
                                desktop_effective=ac_rms*2*1.41421356),
                peaks=peaks, waveform=envelope(times, values), spectrum=envelope(frequencies, amplitude))
    if include_original and filter_records:
        original, _ = analyze(data, sample_rate, start, end, window, remove_mean, confidence,
                              key_phase=key_phase, full_period=full_period)
        result['original'] = dict(statistics=original['statistics'], parameters=original['parameters'],
                                  waveform=original['waveform'], spectrum=original['spectrum'])
    if return_full:
        full_times=np.arange(len(processed))/sample_rate
        result['waveform_full']=envelope(full_times,processed)
        result['_snapshot_arrays']=(full_times,processed,frequencies,amplitude)
        result['parameters']['wave_start_index']=0
        if 'original' in result:result['original']['waveform_full']=envelope(full_times,data)
        points=np.asarray([] if key_phase is None else key_phase)
        indexes=points.astype(np.int64)
        valid=(indexes>=0)&(indexes<len(processed))
        result['key_phase_points']=np.column_stack((points[valid]/sample_rate,processed[indexes[valid]])).tolist()
    return result, (times, values, frequencies, amplitude)
