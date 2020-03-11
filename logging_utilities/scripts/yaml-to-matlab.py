#!/usr/bin/python

import os
import argparse
import yaml
import scipy.io

parser = argparse.ArgumentParser()
parser.add_argument('input', type=str)
parser.add_argument('-o', '--output', type=str, default=None)
parser.add_argument('-v', '--verbose', action='store_true')
args = parser.parse_args()

if args.output is None:
    args.output = os.path.splitext(args.input)[0] + '.mat'

if args.verbose:
    print "Converting '{}' to '{}'".format(args.input, args.output)

with open(args.input, 'r') as f:
    data = yaml.load(f)

scipy.io.savemat(args.output, data, oned_as='column')
