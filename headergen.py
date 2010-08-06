#!/usr/bin/env python

def curly_brace(iter8r): #finds opposing curly brace
	bracecount = 0
	while iter8r < len(i):
		line = i[iter8r]
		bracecount = bracecount + line.count('{') - line.count('}')
		iter8r = iter8r + 1
		if bracecount == 0:
			break
	return iter8r

#open files specified on the command line
import sys
for arg in sys.argv[1:]:
	i = open(arg, 'r')
	i = i.read()
	i = i.split('\n')
	stem = arg[:-2]
	
	#add the multiple link stuff
	o = '#ifndef '+stem.upper()+'HEADER\n#define '+stem.upper()+'HEADER\n\n'
	
	#work through file
	iter8r = 0
	while iter8r < len(i):
		line = i[iter8r]
		#strip functions to prototypes
		if line.__contains__('('):
			if not line.split('(')[0].__contains__('static'):
				o = o + line.replace('{', ';') + '\n'
			iter8r = curly_brace(iter8r)
		#leave typedefs in
		elif line.__contains__('typedef'):
			for iter8r in range(iter8r, curly_brace(iter8r)):
				o = o+i[iter8r]+'\n'
			iter8r = iter8r+1
		#define consts as extern
		elif line.__contains__('const') or line.__contains__('float'):
			#WARNING CRUDE HACK ALERT
			if line.__contains__('dimensions'):
				line = 'const ' + line
			#CRUDE HACK OVER, RELAX
			if line.__contains__('='):
				line = line.split('=')[0]+';'
			o = o + 'extern '+line+'\n'
			iter8r = iter8r+1
		elif line.__contains__('static'):
			iter8r = iter8r+1
		#leave remaining stuff in
		else:
			o = o + line + '\n'
			iter8r = iter8r+1
	#finalize
	o = o.strip()
	o = o+'\n\n#endif\n'
	#write
	f = open(stem+'.h', 'w')
	f.write(o)
	f.close()
