# vim:ts=2:sw=2:expandtab:autoindent:filetype=python:
#
# Copyright (c) 2008 Aristid Breitkreuz, Ruediger Sonderfeld
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

import sys, os
import Utils, Options

VERSION = 'dev'
APPNAME = 'flusspferd'

srcdir = '.'
blddir = 'build'

def init(): pass

def set_options(opt):
    opt.tool_options('compiler_cxx')
    opt.tool_options('boost')
    opt.add_option('--with-cxxflags', action='store', nargs=1, dest='cxxflags',
                   help='Set non-standard CXXFLAGS')
    opt.add_option('--enable-tests', action='store_true',
                   help='Enable tests')
    opt.add_option('--enable-sandbox', action='store_true',
                   help='Enable sandbox tests')
    opt.add_option('--enable-io', action='store_true',
                   help='Enable IO support')
    opt.add_option('--enable-xml', action='store_true',
                   help='Enable XML support')

def configure(conf):
    u = conf.env.append_unique
    darwin = sys.platform.startswith('darwin')
    conf.check_message('platform', '', 1, sys.platform)

    print '%s :' % 'Creating implementation link'.ljust(conf.line_just),
    try: os.unlink('include/flusspferd/implementation')
    except OSError: pass
    os.symlink('spidermonkey', 'include/flusspferd/implementation')
    Utils.pprint('GREEN', 'ok')

    if darwin:
        u('CXXDEFINES', 'APPLE')

    if Options.options.cxxflags:
        conf.env['CXXFLAGS'] = str(Options.options.cxxflags)
    else:
        u('CXXFLAGS', '-pipe -Wno-long-long -Wall -W -pedantic -std=c++98')
        #u('CXXFLAGS', '-O3 -DNDEBUG')
        u('CXXFLAGS', '-O0 -g -DDEBUG')

    conf.check_tool('compiler_cxx')
    conf.check_tool('misc')
    conf.check_tool('boost')

    conf.env['CXXFLAGS_GCOV'] = '-fprofile-arcs -ftest-coverage'
    conf.env['LINKFLAGS_GCOV'] = '-fprofile-arcs -ftest-coverage'

    boostlib = ['thread']
    if Options.options.enable_tests:
      boostlib += ['unit_test_framework']

    # boost
    conf.check_boost(lib = boostlib, min_version='1.36.0', mandatory=1)

    # spidermonkey
    ret = conf.check_cxx(lib = 'js', uselib_store='JS')
    if ret == None:
        conf.check_cxx(lib = 'mozjs', uselib_store='JS', mandatory=1)
    conf.check_cxx(header_name = 'js/jsapi.h', mandatory = 1,
                   uselib_store='JS_H',
                   defines=['XP_UNIX', 'JS_C_STRINGS_ARE_UTF8'])

    # xml
    if Options.options.enable_xml:
        conf.check_cfg(package = 'libxml-2.0', uselib_store='LIBXML2',
                       atleast_version='2.6.0', args = '--cflags --libs')

    conf.env['ENABLE_TESTS'] = Options.options.enable_tests
    conf.env['ENABLE_SANDBOX'] = Options.options.enable_sandbox
    conf.env['ENABLE_XML'] = Options.options.enable_xml
    conf.env['ENABLE_IO'] = Options.options.enable_io

def build_pkgconfig(bld):
    obj = bld.new_task_gen('subst')
    obj.source = 'flusspferd.pc.in'
    obj.target = 'flusspferd.pc'
    obj.dict = {
        'PREFIX': bld.env['PREFIX'],
        'LIBDIR': os.path.normpath(bld.env['PREFIX'] + '/lib'),
        'INCLUDEDIR': os.path.normpath(bld.env['PREFIX'] + '/include'),
        'VERSION': VERSION
        }
    obj.install_path = os.path.normpath(bld.env['PREFIX'] + '/lib/pkgconfig/')
    obj.apply()

def build(bld):
    bld.add_subdirs('src')
    if bld.env['ENABLE_TESTS']:
      bld.add_subdirs('test')
    if bld.env['ENABLE_SANDBOX']:
      bld.add_subdirs('sandbox')
    build_pkgconfig(bld)
    bld.install_files('${PREFIX}/include/flusspferd/', 'include/flusspferd/*.hpp')
    bld.install_files('${PREFIX}/include/flusspferd/implementation/', 'include/flusspferd/implementation/*.hpp')
    bld.install_files('${PREFIX}/lib/pkgconfig/', 'flusspferd.pc')

def shutdown(): pass
