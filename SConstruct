#!/usr/bin/python

env = Environment()
debug = ARGUMENTS.get('debug', 0)
profile = ARGUMENTS.get('profile', 0)
autopackage = ARGUMENTS.get('autopackage', 0)
installer = ARGUMENTS.get('installer', 0)
m32 = ARGUMENTS.get('m32', 0)
if int(debug):
	env.Append(CCFLAGS = '-g -Wall')
else:
	env.Append(CCFLAGS = '-O2 -DNDEBUG')
if int(profile):
	env.Append(CCFLAGS = '-pg', _LIBFLAGS = ' -pg')
if int(m32):
	env.Append(CCFLAGS = '-m32', LINKFLAGS = ' -m32')

env.Append(CPPPATH = ['include'])

env.ParseConfig("pkg-config jngl ImageMagick++ --cflags --libs")

env.Append(CCFLAGS = '-DENABLE_BINRELOC', LIBS = 'boost_filesystem')

env.Program(target = "bin/jntetri",
            source = Glob('src/*.cpp') +
                     Glob('src/leveleditor/*.cpp') +
                     Glob('src/engine/*.cpp') +
                     Glob('src/engine/linux/*.c') +
                     Glob('src/gui/*.cpp'))
