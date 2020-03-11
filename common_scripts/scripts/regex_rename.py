#!/usr/bin/python

import os
import sys
import re
import argparse
import subprocess, shlex

#NOTE: Does not work if pattern begins with '-'. Figure that out?

parser = argparse.ArgumentParser(description = 'Rename files using Python regex')
parser.add_argument('-n', '--dry-run', dest = 'dryRun', action = 'store_true', help = 'Only print result')
parser.add_argument('-i', '--include-dir', action = 'store_true', help = 'Include directory in renaming')
parser.add_argument('-c', '--command', dest = 'command', default = 'mv', help = 'Command to execute for renaming, in the form of "COMMAND OLD NEW"')
parser.add_argument('pattern', type = str, help = 'Regex pattern')
parser.add_argument('replace', type = str, help = 'Regex replacement')
parser.add_argument('targets', type = str, nargs = '*', help = 'Target files to rename. If none, uses all files.')

args = parser.parse_args()

targets = args.targets
if len(targets) == 0:
	targets = os.listdir('.')

def rename(src, dest):
	print "{} '{}' '{}'".format(args.command, target, targetNew)
	if not args.dryRun:
		if os.path.exists(targetNew):
			raise Exception("Rename already exists.")
		#os.rename(target, targetNew)
		pieces = shlex.split(args.command)
		pieces += [target, targetNew]
		subprocess.call(pieces)

try:
	# Make sure all targets exist
	invalid = []
	for target in targets:
		if not os.path.exists(target):
			invalid.append(target)
	if invalid:
		raise Exception("Targets do not exist:\n\t{}".format('\n\t'.join(invalid)))

	for target in targets:
		# Check for conflicts?
		if not args.include_dir:
			(base, target) = os.path.split(target)
		targetNew = re.sub(args.pattern, args.replace, target)
		if not args.include_dir:
			target = os.path.join(base, target)
		if targetNew != target:
			rename(target, targetNew)
			
except Exception as e:
	print 'Error: {}'.format(e.message)
