#!/usr/bin/env python3
# coding=utf-8

# Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Thu Oct 10 12:00:00 2019

"""
Check if the sysif defined in source directory is listed in checksysif.csv, if not, alert to add it in.
"""

from __future__ import absolute_import
import re
import sys
import os
import logging


def get_filelist(dirpath):
    """Get file list from dir path"""
    filelist_w = []
    for home, _, files in os.walk(dirpath):
        for filename in files:
            filelist_w.append(os.path.join(home, filename))
    return filelist_w


class GetSecMethod():
    ''' The class for getting the security method '''

    def __init__(self, codepath, extensions):
        self.outsysifarr = []
        self.outsyscallarr = []
        self.code_path = codepath
        self.ext = extensions
        self.dictsysif = {}
        self.server = []
        self.file_h = []
        self.dictsysifpath = {}

    def match_key_in_line(self, line):
        ''' match key word in line '''
        res = re.search(os.path.join(r'^(\s*)', 'DEFINE', r'(\w*)', '_METHOD', r'(\w*)', r'\((\w+),\s*(\w+),.*'),
                        line.replace('\t', ''))
        if res :
            if res.group(4) != 'test' :
                self.outsysifarr.append(res.group(4) + '_' + res.group(5))
                self.dictsysif[res.group(4) + '_' + res.group(5)] = self.server
                self.dictsysifpath[res.group(4) + '_' + res.group(5)] = self.file_h

    def find_keywords_onefile(self, filepath):
        ''' find key words in the file '''
        filter_h = re.search(r'^.+\.sysif', filepath)
        if filter_h:
            self.server = filepath.rsplit('/', 2)[1]
            self.file_h = filter_h.group()
            with open(filepath, 'r') as f_path:
                lines = f_path.readlines()
            for line in lines:
                self.match_key_in_line(line)

    def find_keywords_dir(self):
        '''Find keywords in code_path dir, the keyfile is *.sysif and keywords is like DEFINE_(XXX)_METHOD...'''
        filelist = get_filelist(self.code_path)
        for file in filelist:
            self.find_keywords_onefile(file)
        self.outsysifarr.sort()


def usage():
    '''print info when the number of elements in sys.argv not equal 2'''
    logging.info("Program need 1 para for code path:")
    logging.info("    arg1: baseline code absolutly path, for example the hm-verify-kernel absolutly path")

if __name__ == '__main__':
    # parameter checking
    if len(sys.argv) != 2:
        usage()
        sys.exit()
    # output path is current directory
    file_path = os.path.abspath(os.path.dirname(__file__))
    sysif_str = []
    errif_str = []

    # output the current the results of collection
    code_path = sys.argv[1]
    collect = GetSecMethod(code_path, 'csv')
    collect.find_keywords_dir()

    #get csv record
    with open(file_path + '/' + 'checksysif.csv', 'r') as f:
        f_line = f.readline()
        for f_line in f.readlines():
            f_new_line = f_line.replace('\t', '')
            if len(f_new_line) > 0 :
                sysif_str.append(f_new_line.split(',', 2)[1])

    #check sysif
    for key in list(collect.dictsysif.keys()) :
        result = key not in sysif_str
        if result :
            errif_str.append(key)

    #output sysif info
    logging.info('unArchived sysif total : %s', str(len(errif_str)))
    logging.info('unArchived sysif : (please add sysif to hm-security : scripts/checksysif.csv)')
    for key in errif_str :
        logging.info('>>>>>>>>>> %s : %s from %s <<<<<<<<<<',
                     collect.dictsysif.get(key), key, collect.dictsysifpath.get(key))

    sys.exit(len(errif_str))
