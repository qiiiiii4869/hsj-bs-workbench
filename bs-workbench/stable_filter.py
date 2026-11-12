"""Balanced causal Butterworth sections shared by waveform and orbit analysis.

Each section is normalized in the passband before ordering. This avoids the
large hidden attenuation/amplification of the default high-order bandstop SOS.
No NI runtime is required. Prefix selection remains the caller's legacy policy.
"""
from functools import lru_cache
import numpy as np
from scipy.signal import butter, sosfilt, zpk2sos
from sel_reader import SelError

FILTER_ENGINE_VERSION = 'balanced-butterworth-v1'
_BLOCK_SIZE = 65536


@lru_cache(maxsize=128)
def _design(kind, order, sample_rate, cutoff):
    try:
        # The unused global gain can overflow/underflow in SciPy's product,
        # even when the roots and section-local passband gains are reliable.
        with np.errstate(over='ignore', invalid='ignore', divide='ignore'):
            zeros, poles, _ = butter(order, cutoff[0] if len(cutoff)==1 else cutoff,
                                     btype=kind, fs=sample_rate, output='zpk')
        if not np.all(np.isfinite(zeros)) or not np.all(np.isfinite(poles)):
            raise FloatingPointError()
        with np.errstate(over='raise', invalid='raise', divide='raise'):
            sos = zpk2sos(zeros, poles, 1.0)
            if not np.all(np.isfinite(sos)):
                raise FloatingPointError()
            # Schur conditions check the rounded, actually executed quadratic.
            if np.any(np.abs(sos[:, 5]) >= 1) or np.any(sos[:, 3:].sum(axis=1) <= 0) or np.any(sos[:, 3]-sos[:, 4]+sos[:, 5] <= 0):
                raise FloatingPointError()
            if kind == 'lowpass':
                anchor = 0.; frequencies = np.linspace(0, cutoff[0], 49)
            elif kind == 'highpass':
                anchor = sample_rate/2; frequencies = np.linspace(cutoff[0], anchor, 49)
            elif kind == 'bandpass':
                low, high = cutoff
                anchor = sample_rate/np.pi*np.arctan(np.sqrt(np.tan(np.pi*low/sample_rate)*np.tan(np.pi*high/sample_rate)))
                frequencies = np.r_[np.linspace(low, high, 65), anchor]
            else:
                low, high = cutoff; anchor = 0.
                frequencies = np.r_[np.linspace(0, low, 33), np.linspace(high, sample_rate/2, 65)]
            point = 1. if anchor==0 else -1. if anchor==sample_rate/2 else np.exp(-2j*np.pi*anchor/sample_rate)
            powers = np.array([1, point, point*point])
            gain = np.abs((sos[:, :3] @ powers)/(sos[:, 3:] @ powers))
            if not np.all(np.isfinite(gain)) or np.any(gain <= 0):
                raise FloatingPointError()
            sos[:, :3] /= gain[:, None]
            points = np.exp(-2j*np.pi*frequencies/sample_rate)
            powers = np.array([np.ones(len(points)), points, points*points])
            response = (sos[:, :3] @ powers)/(sos[:, 3:] @ powers)
            if not np.all(np.isfinite(response)):
                raise FloatingPointError()
            logs = np.log(np.maximum(np.abs(response), np.finfo(float).tiny))
            # Keep intermediate gain balanced throughout the passband, including
            # both passbands of a bandstop, rather than considering DC alone.
            remaining = list(range(len(sos))); ordered = []; partial = np.zeros(len(points))
            while remaining:
                index = min(remaining, key=lambda i: np.max(np.abs(partial+logs[i])))
                partial += logs[index]; remaining.remove(index); ordered.append(index)
            if np.any(partial > np.log1p(1e-6)):
                raise FloatingPointError()
            # Immutable cache contents; each caller gets its own writable array.
            return tuple(tuple(float(v) for v in row) for row in sos[ordered])
    except (ValueError, FloatingPointError, np.linalg.LinAlgError):
        raise SelError('当前参数无法可靠生成稳定滤波器，请调整阶数或截止频率') from None


def apply_butterworth(values, sample_rate, kind, order, cutoff, prefix):
    """Zero-state causal filtering, with the caller's leading prefix exactly once.

    The unit-gain Butterworth energy bound also catches enormous *finite*
    outputs. It is a numerical safety gate, not an NI equivalence assertion.
    """
    data = np.asarray(values, dtype=np.float64)
    if data.ndim!=1 or not len(data) or not np.all(np.isfinite(data)):
        raise SelError('滤波输入须为非空的有限数值序列')
    if kind not in ('lowpass','highpass','bandpass','bandstop') or isinstance(order, bool) or not isinstance(order, int) or not 1<=order<=100:
        raise SelError('滤波类型或阶数无效')
    cutoff = tuple(float(v) for v in cutoff)
    if not np.isfinite(sample_rate) or sample_rate<=0 or len(cutoff)!=(2 if kind in ('bandpass','bandstop') else 1) or any(not np.isfinite(v) or not 0<v<sample_rate/2 for v in cutoff) or len(cutoff)==2 and cutoff[0]>=cutoff[1]:
        raise SelError('滤波截止频率须位于(0,Fs/2)，且频带下限小于上限')
    if not isinstance(prefix, int) or not 0<=prefix<=len(data):
        raise SelError('滤波前补长度无效')
    sos = np.array(_design(kind, order, float(sample_rate), cutoff), dtype=np.float64)
    delays = np.zeros((len(sos), 2), dtype=np.float64)
    scale = float(np.max(np.abs(data))) or 1.
    input_energy = 0.
    for lo in range(0, len(data), _BLOCK_SIZE):
        normalized = data[lo:lo+_BLOCK_SIZE]/scale
        input_energy += float(np.dot(normalized, normalized))
    normalized = data[:prefix]/scale
    input_energy += float(np.dot(normalized, normalized))
    allowed_energy = input_energy*(1+1e-6)+1e-12
    output_energy = 0.

    def validate(block, state):
        nonlocal output_energy
        if not np.all(np.isfinite(block)) or not np.all(np.isfinite(state)):
            raise FloatingPointError()
        normalized = block/scale
        output_energy += float(np.dot(normalized, normalized))
        if not np.isfinite(output_energy) or output_energy > allowed_energy:
            raise FloatingPointError()

    output = np.empty_like(data)
    try:
        with np.errstate(over='raise', invalid='raise', divide='raise'):
            if prefix:
                leading, delays = sosfilt(sos, data[:prefix], zi=delays)
                validate(leading, delays)
            for lo in range(0, len(data), _BLOCK_SIZE):
                block, delays = sosfilt(sos, data[lo:lo+_BLOCK_SIZE], zi=delays)
                validate(block, delays)
                output[lo:lo+len(block)] = block
    except FloatingPointError:
        raise SelError('当前滤波结果数值不可靠，已停止返回结果；请调整阶数或截止频率') from None
    return output
