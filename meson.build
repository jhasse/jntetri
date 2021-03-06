project('jntetri', ['c', 'cpp'], default_options : ['cpp_std=c++17'])

cxx = meson.get_compiler('cpp')
add_project_arguments(cxx.get_supported_arguments([
	'-Wall', '-Wextra',
	'-Winit-self',
	'-Winconsistent-missing-override',
	# '-Wpedantic', spdlog
	# '-Wfloat-equal', spdlog
	'-Wdelete-non-virtual-dtor',
	'-Wlogical-op',
	'-Wduplicated-cond',
	'-Wduplicated-branches',
	'-Wno-implicit-fallthrough', # cereal
	'-Wno-class-memaccess', # cereal
]), language : 'cpp')

if cxx.get_id() == 'clang'
	add_global_arguments(['-stdlib=libc++'], language : 'cpp')
	add_global_link_arguments(['-lc++', '-lc++abi'], language : 'cpp')
endif

if host_machine.system() == 'windows' and cxx.get_argument_syntax() != 'msvc'
	boost_dep = [
		cxx.find_library('ws2_32'),
	]
else
	boost_dep = dependency('boost', modules : ['system', 'filesystem'])
endif

if host_machine.system() == 'linux'
	add_global_link_arguments(['-lstdc++fs'], language : 'cpp')
endif

if get_option('buildtype') == 'release'
	add_global_arguments('-DNDEBUG', language : 'cpp')
endif

jngl = subproject('jngl')
spdlog_dep = subproject('spdlog', default_options : ['tests=false']).get_variable('spdlog_dep')

src = [
	'src/explosion.cpp',
	'src/Intro.cpp',
	'src/lobby.cpp',
	'src/control.cpp',
	'src/multiplayermenu.cpp',
	'src/highscore.cpp',
	'src/Field.cpp',
	'src/replayrecorder.cpp',
	'src/Login.cpp',
	'src/main.cpp',
	'src/ReplayPlayer.cpp',
	'src/block.cpp',
	'src/Game.cpp',
	'src/optionsmenu.cpp',
	'src/SplitScreen.cpp',
	'src/NetworkControl.cpp',
	'src/NetworkRecorder.cpp',
	'src/gui/chooser.cpp',
	'src/gui/input.cpp',
	'src/gui/Button.cpp',
	'src/gui/ButtonBox.cpp',
	'src/gui/widget.cpp',
	'src/gui/radiobutton.cpp',
	'src/shadow.cpp',
	'src/menu.cpp',
	'src/ReplayControl.cpp',
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
if target_machine.system() == 'windows'
	src += [import('windows').compile_resources('src/icon.rc')]
endif

executable(
	'jntetri', src,
	dependencies : [
		cxx.find_library('dl'),
		dependency('threads'),
		boost_dep,
		subproject('cereal').get_variable('cereal_dep'),
		spdlog_dep,
	],
	include_directories : [
		jngl.get_variable('include'),
	],
	link_with : jngl.get_variable('lib'),
)
