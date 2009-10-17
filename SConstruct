#!/usr/bin/python

debug = int(ARGUMENTS.get('debug', 0))
variant_dir='bin/release'
if debug == 1:
	variant_dir='bin/debug'
SConscript('src/SConscript', variant_dir=variant_dir, duplicate=0)
