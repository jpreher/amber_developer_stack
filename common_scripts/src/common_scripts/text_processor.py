import sys, os, re, argparse, yaml

def indentify(stuff, rep = 1, indent = '\t'):
	""" From http://code.activestate.com/recipes/66055-changing-the-indentation-of-a-multi-line-string/#c4 """
	lines = [(rep * indent) + line for line in stuff.splitlines()]
	return "\n".join(lines)

# Add options to avoid writing to file if no change occurs?

class TextProcessor(object):
	""" Simple class to write specifics to replace sed type stuff """
	def __init__(self, processFunc = None, description = '', skipArgs = False):
		self.processFunc = processFunc
		if processFunc and len(description) == 0:
			description = processFunc.__doc__
		self.parser = argparse.ArgumentParser(description = description)
		if not skipArgs:
			self.addArguments()
	
	def addArguments(self):
		self.parser.add_argument('inputs', type = argparse.FileType('r'), nargs='+', help = 'File to process')
		self.parser.add_argument('-i', '--in-place', action = 'store_true', help = 'Output to the input file')
		self.parser.add_argument('-o', '--output', type = argparse.FileType('w'), default = None, help = 'Output file')
		self.parser.add_argument('-e', '--extra-line', action = 'store_true', help = 'Print extra new line at the end')
		self.parser.add_argument('-v', '--verbose', action='store_true', help='Print file name to stderr (useful for loops)')
	
	def process(self, oldText):
		""" Set processFunc or overload this and return the formatted text """
		assert self.processFunc
		return self.processFunc(oldText)

	def processFile(self, inFile, outFile):
		try:
			oldText = inFile.read()
			if outFile is None:
				outName = inFile.name
			else:
				outName = outFile.name
			newText = self.process(oldText)
			print >>sys.stderr, "[ {} -> {} ]".format(inFile.name, outName)
			if not self.args.in_place or newText != oldText:
				if outFile is None:
					inFile.close()
					outFile = open(inFile.name, 'w')
				outFile.write(newText)
				if self.args.extra_line:
					outFile.write('\n')
			else:
				print >>sys.stderr, "\t<no changes>"
		except IOError, e:
			print >>sys.stderr, "Error: {}".format(e.msg)
	
	def main(self, *args):
		""" Do stuff """
		args = self.parser.parse_args(*args)

		if args.output is not None:
			assert not args.in_place, "Cannot specify both --output and --in-place"
			assert len(args.inputs) == 1, "Can only specify one input file with a file name"
		elif not args.in_place:
			args.output = sys.stdout

		self.args = args
		for inFile in args.inputs:
			self.processFile(inFile, args.output)
		
		
		

