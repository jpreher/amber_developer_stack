#!/usr/bin/python

import re
from common_scripts.text_processor import TextProcessor

class Custom(TextProcessor):
	def __init__(self):
		TextProcessor.__init__(self, skipArgs = True, description = "Apply python regex to file")
		self.parser.add_argument('pattern', type = str)
		self.parser.add_argument('replace', type = str)
		self.addArguments()
		
	def process(self, oldText):
		return re.sub(self.args.pattern, self.args.replace, oldText, flags = re.M)

Custom().main()
