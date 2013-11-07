#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#
#    Copyright 2008-2011, Lukas Lueg, lukas.lueg@gmail.com
#
#    This file is part of Pyrit.
#
#    Pyrit is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Pyrit is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Pyrit.  If not, see <http://www.gnu.org/licenses/>.

from distutils.core import setup, Extension
from distutils.unixccompiler import UnixCCompiler
import subprocess
import re

VERSION = '0.4.0' 

UnixCCompiler.src_extensions.append('.S')

try:
    svn_info = subprocess.Popen(('svn', 'info'), \
                                stdout=subprocess.PIPE).stdout.read()
    VERSION += ' (svn r%i)' % \
                int(re.compile('Revision: ([0-9]*)').findall(svn_info)[0])
except:
    pass
EXTRA_COMPILE_ARGS = ['-Wall', '-fno-strict-aliasing', \
                      '-DVERSION="%s"' % (VERSION,)]


cpu_extension = Extension(name='cpyrit._cpyrit_cpu',
                    sources = ['cpyrit/_cpyrit_cpu.c',
                               'cpyrit/_cpyrit_cpu_sse2.S'],
                    libraries = ['crypto', 'pcap'],
                    extra_compile_args=EXTRA_COMPILE_ARGS)

setup_args = dict(
        name = 'pyrit',
        version = VERSION,
        description = 'GPU-accelerated attack against WPA-PSK authentication',
        long_description = \
            "Pyrit allows to create massive databases, pre-computing part " \
            "of the WPA/WPA2-PSK authentication phase in a space-time-" \
            "tradeoff. Exploiting the computational power of Many-Core- " \
            "and other platforms through ATI-Stream, Nvidia CUDA, OpenCL " \
            "and VIA Padlock, it is currently by far the most powerful " \
            "attack against one of the world's most used security-protocols.",
        license = 'GNU General Public License v3',
        author = 'Lukas Lueg',
        author_email = 'lukas.lueg@gmail.com',
        url = 'http://pyrit.googlecode.com',
        classifiers = \
              ['Development Status :: 4 - Beta',
               'Environment :: Console',
               'License :: OSI Approved :: GNU General Public License (GPL)',
               'Natural Language :: English',
               'Operating System :: OS Independent',
               'Programming Language :: Python',
               'Topic :: Security'],
        platforms = ['any'],
        packages = ['cpyrit'],
        py_modules = ['pyrit_cli', 'cpyrit.cpyrit',
                      'cpyrit.util', 'cpyrit.pckttools',
                      'cpyrit.config', 'cpyrit.network'],
        scripts = ['pyrit'],
        ext_modules = [cpu_extension],
        options = {'install': {'optimize': 1}})

if __name__ == '__main__':
    setup(**setup_args)
