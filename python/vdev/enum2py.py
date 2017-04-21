#!/usr/bin/env python3
# -*- cofing: utf-8 -*-

from __future__ import print_function, unicode_literals, absolute_import

import re


def _get_enum_content(fname, enum_type):
    with open(fname, 'r') as f:
        content = ''.join([x.strip() for x in f.readlines()])

    try:
        # 'enum xxx {};' statement
        if re.match(r'enum\s+\w+', enum_type):
            content = re.findall(r'%s\s*{([^{}]*)}\s*;' % enum_type, content)[0]
        # 'typedef [xxx] {} xxx;' statement
        else:
            content = re.findall(r'typedef\s+enum(?:\s*\w+)?\s*{([^{}]*)}\s*%s\s*;' % enum_type, content)[0]
    except IndexError:
        return None

    return content

def _parse_content(content):
    lines = content.strip(',').split(',')
    number = -1
    
    attrs = {}
    for line in lines:
        line = re.sub(r'\s', '', line)
        # xxx = number
        if '=' in line:
            key, val = line.split('=')
            number = int(val)
        else:
            key = line
            number += 1

        attrs[key] = number
    
    return attrs
    
def Enum2Py(fname, enum_type):
    content = _get_enum_content(fname, enum_type)
    if not content:
        raise ValueError('enum "%s" not found' % enum_type)
    attrs = _parse_content(content)
    if not attrs:
        raise ValueError('Can\'t parse "%s"' % attrs)
    return type(enum_type, (), attrs)

