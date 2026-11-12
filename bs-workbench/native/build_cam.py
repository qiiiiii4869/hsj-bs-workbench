"""Compile isolated original CAM math; does not execute it or run tests."""
from pathlib import Path
import os
import subprocess
import shutil

root=Path(__file__).resolve().parents[2]
vc=root/'dev-deps/vc80-trial/VC'
env=os.environ.copy()
folder=Path(__file__).resolve().parent
compiler=shutil.which('cl.exe')
if compiler is None and (vc/'bin/cl.exe').is_file():
    compiler=str(vc/'bin/cl.exe')
    env['PATH']=str(vc.parent/'Common7/IDE')+';'+str(vc/'bin')+';'+env['PATH']
    env['INCLUDE']=';'.join(str(vc/p) for p in ['include','PlatformSDK/include'])
    env['LIB']=';'.join(str(vc/p) for p in ['lib','PlatformSDK/lib'])
if compiler is None:
    raise SystemExit('Install Visual Studio C++ Build Tools and run this script in its Developer Command Prompt. The packaged cam_math.exe can be used without rebuilding.')
# /Fename works with both VC80 and modern MSVC. VC80 does not interpret the
# colon form consistently. Static CRT avoids a machine-specific VC runtime.
result=subprocess.run([compiler,'/nologo','/O2','/MT','/EHsc','/D_CRT_SECURE_NO_DEPRECATE','cam_math.cpp','/Fecam_math.exe','/link','/manifest:no'],cwd=folder,env=env,capture_output=True)
(folder/'build.log').write_bytes(result.stdout+result.stderr)
if result.returncode:raise SystemExit(result.returncode)
if not (folder/'cam_math.exe').is_file():raise SystemExit('Compiler did not create cam_math.exe; read native/build.log.')
print('Built isolated CAM numerical executable; no execution or tests performed.')
