"""Numerical regressions for high-order causal filtering and failure handling."""
import unittest
from unittest.mock import patch
import numpy as np
from scipy.signal import butter, freqz_sos, sosfilt
from analysis import filter_signal
from stable_filter import _design, apply_butterworth
from specialty import orbit_filter
from sel_reader import SelError


class StableFilterTests(unittest.TestCase):
    def test_high_order_bandstop_preserves_response_and_constant(self):
        for order in (5,20,50,100):
            with self.subTest(order=order):
                expected=butter(order,[10,100],btype='bandstop',fs=1024,output='sos')
                actual=np.array(_design('bandstop',order,1024.,(10.,100.)))
                _,a=freqz_sos(expected,worN=32769,fs=1024)
                _,b=freqz_sos(actual,worN=32769,fs=1024)
                np.testing.assert_allclose(a,b,atol=2e-10,rtol=2e-10)
                result=apply_butterworth(np.full(34816,300.),1024,'bandstop',order,[10,100],3072)
                self.assertLess(float(abs(result).max()),1000.)
                self.assertLess(abs(float(result[-1])-300),.5)

    def test_full_block_chain_and_input_ownership(self):
        raw=40+np.random.default_rng(2718).normal(size=140003)
        original=raw.copy();spec=dict(type='bandstop',low=10,high=100,order=50)
        result,_=filter_signal(raw,1024,[spec,spec])
        sos=np.array(_design('bandstop',50,1024.,(10.,100.)))
        once=sosfilt(sos,np.r_[raw[:3072],raw])[3072:]
        expected=sosfilt(sos,np.r_[once[:3072],once])[3072:]
        np.testing.assert_array_equal(raw,original)
        np.testing.assert_allclose(result,expected,atol=1e-10,rtol=1e-10)
        restored,_=filter_signal(raw,1024,[])
        np.testing.assert_array_equal(restored,original)

    def test_prefix_boundaries_and_orbit_legacy_mapping(self):
        for size in (10240,10241):
            raw=np.cos(np.arange(size)*.17)+5
            result,_=filter_signal(raw,1024,[dict(type='lowpass',low=10,order=5)])
            p=3072 if size>10240 else size//10
            sos=np.array(_design('lowpass',5,1024.,(10.,)))
            np.testing.assert_array_equal(result,sosfilt(sos,np.r_[raw[:p],raw])[p:])
        spec=dict(type='bandpass',low=1,high=100,order=7)
        expected=apply_butterworth(raw,1024,'bandpass',5,[1,7],3072)
        np.testing.assert_array_equal(orbit_filter(raw,1024,[spec],True),expected)

    def test_enormous_finite_output_is_rejected(self):
        # Models a finite but grossly incorrect realization, rather than NaN/Inf.
        bad=((1e100,0.,0.,1.,0.,0.),)
        with patch('stable_filter._design',return_value=bad):
            with self.assertRaisesRegex(SelError,'数值不可靠'):
                apply_butterworth(np.ones(100),1024,'lowpass',5,[10],10)

    def test_invalid_or_unrepresentable_design_is_rejected(self):
        raw=np.ones(64)
        for kind,order,cutoff in [('bandstop',100,[100,10]),('lowpass',5,[0]),('highpass',5,[512]),('lowpass',True,[10]),('bandstop',5,[10,float('nan')])]:
            with self.subTest(kind=kind,cutoff=cutoff):
                with self.assertRaises(SelError):apply_butterworth(raw,1024,kind,order,cutoff,6)
        with self.assertRaises(SelError):apply_butterworth(raw,1024,'lowpass',100,[1e-100],6)


if __name__=='__main__':unittest.main()
