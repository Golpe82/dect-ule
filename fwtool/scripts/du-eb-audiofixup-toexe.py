# SPDX-License-Identifier: MIT
from distutils.core import setup
import py2exe

setup(
    console=['du-eb-audiofixup.py'],
    zipfile=None,
    options={
        "py2exe": {
            "bundle_files": 1,
            'compressed': 1,
            'optimize': 2,
            'dist_dir': '.',
            'dll_excludes': ['w9xpopen.exe', 'crypt32.dll'],
        },
    }
)
