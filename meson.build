project('jntetri', ['c', 'cpp'], default_options : ['cpp_std=c++1z'])

cxx = meson.get_compiler('cpp')
if cxx.get_id() == 'clang'
	add_global_arguments(['-stdlib=libc++'], language : 'cpp')
	add_global_link_arguments(['-lc++', '-lc++abi'], language : 'cpp')
endif

if host_machine.system() == 'linux'
	boost_dep = dependency('boost', modules : ['serialization', 'system'])
	add_global_link_arguments(['-lstdc++fs'], language : 'cpp')
else
	boost_dep = [
		cxx.find_library('boost_serialization-mt'),
		cxx.find_library('boost_system-mt'),
		cxx.find_library('boost_filesystem-mt'),
		cxx.find_library('ws2_32'),
	]
endif

if get_option('buildtype') == 'release'
	add_global_arguments('-DNDEBUG', language : 'cpp')
endif

jngl = subproject('jngl')

src = [
	'src/explosion.cpp',
	'src/intro.cpp',
	'src/lobby.cpp',
	'src/control.cpp',
	'src/multiplayermenu.cpp',
	'src/highscore.cpp',
	'src/field.cpp',
	'src/replayrecorder.cpp',
	'src/login.cpp',
	'src/main.cpp',
	'src/replayplayer.cpp',
	'src/block.cpp',
	'src/game.cpp',
	'src/optionsmenu.cpp',
	'src/splitscreen.cpp',
	'src/gui/chooser.cpp',
	'src/gui/input.cpp',
	'src/gui/button.cpp',
	'src/gui/buttonbox.cpp',
	'src/gui/widget.cpp',
	'src/gui/radiobutton.cpp',
	'src/shadow.cpp',
	'src/menu.cpp',
	'src/replaycontrol.cpp',
	'src/tetromino.cpp',
	'src/engine/screen.cpp',
	'src/engine/work.cpp',
	'src/engine/procedure.cpp',
	'src/engine/debugoverlay.cpp',
	'src/engine/resizegraphics.cpp',
	'src/engine/vector2d.cpp',
	'src/engine/socket.cpp',
	'src/engine/options.cpp',
	'src/engine/fade.cpp',
	'src/engine/paths.cpp',
	'src/pausemenu.cpp',
	'src/gameoverscreen.cpp',
]
if target_machine.system() == 'linux'
	src += [
		'src/engine/linux/binreloc.c',
	]
endif
if target_machine.system() == 'windows'
	src += [import('windows').compile_resources('src/icon.rc')]
endif

executable(
	'jntetri', src,
	dependencies : [
		cxx.find_library('dl'),
		dependency('threads'),
		boost_dep
	],
	include_directories : [
		jngl.get_variable('include'),
	],
	link_with : jngl.get_variable('lib'),
)