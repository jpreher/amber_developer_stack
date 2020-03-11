#!/usr/bin/env python

from distutils.core import setup

d = {'author': u'Eric Cousineau <eacousineau@gmail.com>',
 'description': 'Common scripts.',
 'license': 'BSD',
 'maintainer': u'Eric Cousineau',
 'maintainer_email': 'eacousineau@gmail.com',
 'name': 'common_scripts',
 'package_dir': {'': 'src'},
 'packages': ['common_scripts'],
 'url': 'http://ros.org/wiki/common_scripts',
 'version': '0.1.0'}

setup(**d)
