#!/usr/bin/python
"""
@brief Utility to resolve package URI to absolute file path if prefixed with 'package://' protocol
@author Eric Cousineau <eacousineau@gmail.com>
"""
import os
import rospkg

def resolve_package_uri(path):
    prefix = 'package://'
    if path.startswith(prefix):
        sub = path[len(prefix):]
        pieces = sub.split(os.path.sep)
        pkg = pieces[0]
        rest = os.path.join(*pieces[1:])
        return os.path.join(rospkg.RosPack().get_path(pkg), rest)
    else:
        return path
