import hashlib
import json
import math
from pathlib import Path
import tempfile
import unittest
import urllib.request
import urllib.error
import numpy as np
from sel_reader import SelFile, SelError
from analysis import analyze, envelope, confidence_pp, filter_signal

ROOT = Path(__file__).resolve().parent


class CoreTests(unittest.TestCase):
    def test_confidence_matches_desktop_integer_rank_rule(self):
        values=np.arange(100,dtype=float)
        self.assertEqual(confidence_pp(values,95),95)
        self.assertEqual(confidence_pp(values,97),97)
        self.assertEqual(confidence_pp(values,100),99)
        # Desktop truncates rank indexes: do not substitute interpolated quantiles.
        self.assertEqual(confidence_pp(np.arange(17,dtype=float),95),16)
        for confidence in (0,96,101,True,'95'):
            with self.assertRaises(SelError):confidence_pp(values,confidence)

    def test_butterworth_low_high_match_analytic_response(self):
        fs,n,cutoff,order=1024,16384,40,4
        t=np.arange(n)/fs
        original=np.sin(2*np.pi*10*t)+np.sin(2*np.pi*200*t)
        saved=original.copy()
        for kind in ('lowpass','highpass'):
            filtered,records=filter_signal(original,fs,[dict(type=kind,order=order,low=cutoff,high=cutoff)])
            self.assertEqual(records[0]['prefix_count'],3072)
            for frequency in (10,200):
                measured=2*abs(np.mean(filtered*np.exp(-2j*np.pi*frequency*t)))
                ratio=np.tan(np.pi*frequency/fs)/np.tan(np.pi*cutoff/fs)
                expected=1/math.sqrt(1+(ratio if kind=='lowpass' else 1/ratio)**(2*order))
                self.assertAlmostEqual(measured,expected,places=7)
        np.testing.assert_array_equal(original,saved)

    def test_band_filters_chain_and_restore(self):
        fs=1024;n=16384;t=np.arange(n)/fs
        signal=sum(np.sin(2*np.pi*f*t) for f in (5,50,200))
        spec=dict(type='bandpass',order=5,low=30,high=80)
        passed,records=filter_signal(signal,fs,[spec])
        stopped,_=filter_signal(signal,fs,[dict(spec,type='bandstop')])
        amplitude=lambda a,f:2*abs(np.mean(a*np.exp(-2j*np.pi*f*t)))
        self.assertGreater(amplitude(passed,50),.99)
        self.assertLess(amplitude(passed,5),.001)
        self.assertLess(amplitude(stopped,50),.001)
        chained,history=filter_signal(signal,fs,[spec,spec])
        self.assertEqual(len(history),2)
        self.assertLessEqual(amplitude(chained,50),amplitude(passed,50)+1e-9)
        restored,_=filter_signal(signal,fs,[])
        np.testing.assert_array_equal(restored,signal)
        _,short_records=filter_signal(signal[:1000],fs,[spec])
        self.assertEqual(short_records[0]['prefix_count'],100)

    def test_filter_validation(self):
        base=dict(type='bandpass',order=5,low=10,high=100)
        for change in ({'order':0},{'order':101},{'order':1.5},{'order':True},{'low':0},{'high':512},{'high':float('nan')},{'low':200},{'type':'unknown'}):
            with self.assertRaises(SelError):filter_signal(np.arange(100),1024,[dict(base,**change)])
        # All legacy order values remain supported.
        high_order,_=filter_signal(np.sin(np.arange(2048)),1024,[dict(type='lowpass',order=100,low=100)])
        self.assertTrue(np.all(np.isfinite(high_order)))

    def test_migration_baseline_preserves_every_entry(self):
        baseline=json.loads((ROOT.parent/'frontend-review'/'frontend-inventory.json').read_text('utf-8'))
        ledger=json.loads((ROOT/'migration-ledger.json').read_text('utf-8'))
        self.assertEqual([x['review_id'] for x in ledger['commands']],[x['review_id'] for x in baseline['commands']])
        self.assertEqual(ledger['resources'],baseline['resources'])
        self.assertEqual(ledger['direct_dispatches'],baseline['direct_dispatches'])
        self.assertEqual(len(ledger['features']),70)
        self.assertEqual(len(ledger['dialogs']),45)
        self.assertEqual(sum(len(x['entries']) for x in ledger['dialogs']),650)
        self.assertEqual(len(ledger['events']),276)
        self.assertFalse(ledger['complete_replacement_ready'])
        self.assertEqual(ledger['accepted_count'],0)

    def test_known_signal_and_desktop_effective(self):
        fs, n = 1024, 4096
        t = np.arange(n)/fs
        signal = 8 + 3*np.sin(2*np.pi*17*t)
        result, arrays = analyze(signal, fs)
        self.assertAlmostEqual(result['statistics']['mean'], 8)
        self.assertAlmostEqual(result['statistics']['ac_rms'], 3/math.sqrt(2))
        self.assertAlmostEqual(result['statistics']['desktop_effective'], 6, places=6)
        self.assertAlmostEqual(result['peaks'][0]['frequency'], 17)
        self.assertAlmostEqual(result['peaks'][0]['amplitude'], 3)
        self.assertEqual(arrays[3][0], 0)
        # Direct DFT independently verifies the FFT scale, avoiding a duplicate FFT assertion.
        direct = abs(sum(signal[k]*complex(math.cos(-2*math.pi*68*k/n), math.sin(-2*math.pi*68*k/n)) for k in range(n)))*2/n
        self.assertAlmostEqual(result['peaks'][0]['amplitude'], direct)

    def test_hann_amplitude_compensation(self):
        x = 5*np.sin(2*np.pi*32*np.arange(8192)/1024)
        r, _ = analyze(x, 1024, window='hann')
        self.assertAlmostEqual(r['peaks'][0]['amplitude'], 5, places=5)

    def test_interval_and_invalid_parameters(self):
        r, a = analyze(np.arange(1000, dtype=float), 100, start=1.25, end=2.5)
        self.assertEqual(r['parameters']['count'], 125)
        self.assertEqual(a[1][0], 125)
        self.assertEqual(a[1][-1], 249)
        for args in ({'start':-1}, {'start':3, 'end':2}, {'end':float('nan')}, {'end':11}, {'window':'unknown'}):
            with self.assertRaises(SelError):
                analyze(np.arange(1000), 100, **args)

    def test_display_reduction_preserves_global_extrema(self):
        values = np.zeros(100000);values[12345]=37;values[54321]=-29
        reduced = np.asarray(envelope(np.arange(len(values)), values))
        self.assertEqual(reduced[:,1].max(),37)
        self.assertEqual(reduced[:,1].min(),-29)
        self.assertTrue(np.all(np.diff(reduced[:,0])>0))

    def test_all_real_samples_and_channels(self):
        files=list((ROOT.parent/'测试系统实例').rglob('*.SEL'))
        self.assertEqual(len(files),3)
        counts=[]
        for path in files:
            reader=SelFile(path)
            self.assertEqual(reader.header_size,89569)
            metadata=reader.metadata()
            checked=0
            for condition in metadata['conditions']:
                for channel in condition['channels']:
                    if channel['count']<4:continue
                    data=reader.read(condition['index'],channel['index'])
                    self.assertEqual(len(data),channel['count'])
                    checked+=1
            counts.append((path.name,len(metadata['conditions']),reader.channel_count,checked))
        (ROOT/'verification-samples.json').write_text(json.dumps(counts,ensure_ascii=False,indent=2),'utf-8')

    def test_rejects_corrupt_files(self):
        path=ROOT.parent/'测试系统实例'/'电厂变负荷.SEL'
        with tempfile.TemporaryDirectory() as folder:
            corrupt=Path(folder)/'bad.sel'
            corrupt.write_bytes(path.read_bytes()[:89569])
            with self.assertRaises(SelError):SelFile(corrupt)
            corrupt.write_bytes(b'invalid')
            with self.assertRaises(SelError):SelFile(corrupt)


class ApiTests(unittest.TestCase):
    base='http://127.0.0.1:8765'

    def setUp(self):
        self.boot=json.load(urllib.request.urlopen(self.base+'/api/bootstrap'))

    def post(self, route, body=None, headers=None, raw=None):
        request=urllib.request.Request(self.base+route,data=raw if raw is not None else json.dumps(body).encode(),headers={'Content-Type':'application/json','X-HSJ-Token':self.boot['token'],**(headers or {})})
        return urllib.request.urlopen(request)

    def test_real_analysis_and_exports(self):
        identifier=self.boot['samples'][0]['id']
        info=json.load(self.post('/api/open',{'file_id':identifier}))
        self.assertEqual(info['filename'],'电厂变负荷.SEL')
        params={'file_id':identifier,'condition':0,'channel':0,'start':0,'end':10}
        result=json.load(self.post('/api/analyze',params))
        self.assertEqual(result['parameters']['count'],info['channels'][0]['sample_rate']*10)
        self.assertEqual(result['source']['sha256'],info['sha256'])
        exported=self.post('/api/export',dict(params,kind='waveform')).read().decode('utf-8-sig').splitlines()
        self.assertEqual(len(exported),result['parameters']['count']+6)
        record=json.load(self.post('/api/export',dict(params,kind='json')))
        self.assertEqual(result['statistics'],record['statistics'])

    def test_filtered_export_confidence_title_and_full_range(self):
        params=dict(file_id=self.boot['samples'][0]['id'],condition=0,channel=0,start=2,end=5,
                    confidence=97,title='上导摆度 · 滤波对照',filters=[dict(type='lowpass',order=5,low=10)])
        filtered=json.load(self.post('/api/analyze',params))
        exported=json.load(self.post('/api/export',dict(params,kind='json')))
        self.assertEqual(filtered['statistics'],exported['statistics'])
        self.assertEqual(exported['parameters']['confidence'],97)
        self.assertEqual(exported['source']['title'],params['title'])
        self.assertEqual(exported['parameters']['filters'][0]['type'],'lowpass')
        restored=json.load(self.post('/api/analyze',dict(params,filters=[])))
        raw=json.load(self.post('/api/analyze',dict(params,filters=[])))
        self.assertEqual(restored['statistics'],raw['statistics'])
        self.assertNotEqual(filtered['statistics']['peak_to_peak'],raw['statistics']['peak_to_peak'])
        full=json.load(self.post('/api/analyze',dict(params,start=0,end=None)))
        self.assertEqual(full['parameters']['count'],full['source']['total_count'])
        self.assertGreater(full['parameters']['count'],filtered['parameters']['count'])
        for change in ({'confidence':96},{'filters':[dict(type='lowpass',order=0,low=10)]},{'title':'  '}):
            with self.assertRaises(urllib.error.HTTPError) as caught:self.post('/api/analyze',dict(params,**change))
            self.assertEqual(caught.exception.code,400)

    def test_bad_upload_does_not_persist(self):
        before=set((ROOT/'data').glob('*'))
        with self.assertRaises(urllib.error.HTTPError) as caught:
            self.post('/api/upload',raw=b'bad data',headers={'X-Filename':'bad.sel'})
        self.assertEqual(caught.exception.code,400)
        self.assertEqual(before,set((ROOT/'data').glob('*')))

    def test_upload_roundtrip(self):
        data=(ROOT.parent/'测试系统实例'/'数据'/'TEST1.SEL').read_bytes()
        result=json.load(self.post('/api/upload',raw=data,headers={'X-Filename':'TEST1.SEL'}))
        try:
            self.assertEqual(result['sha256'],hashlib.sha256(data).hexdigest())
            reopened=json.load(self.post('/api/open',{'file_id':result['file_id']}))
            self.assertEqual(reopened['conditions'],result['conditions'])
        finally:
            for suffix in ('.sel','.json'):(ROOT/'data'/f"{result['file_id']}{suffix}").unlink(missing_ok=True)

    def test_csrf_path_and_invalid_arguments(self):
        for body in ({'file_id':'../../server.py'},{'file_id':'no-file'}):
            with self.assertRaises(urllib.error.HTTPError) as caught:self.post('/api/open',body)
            self.assertEqual(caught.exception.code,400)
        with self.assertRaises(urllib.error.HTTPError) as caught:self.post('/api/open',{},headers={'X-HSJ-Token':'wrong'})
        self.assertEqual(caught.exception.code,403)
        with self.assertRaises(urllib.error.HTTPError) as caught:self.post('/api/open',{},headers={'Origin':'https://untrusted.invalid'})
        self.assertEqual(caught.exception.code,403)


if __name__=='__main__':
    unittest.main(verbosity=2)
