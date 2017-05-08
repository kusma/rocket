project('rocket', 'c', 'cpp', meson_version: '>=0.44')

sdl = dependency('sdl')
gl = dependency('gl')
glu = dependency('glu', required : false)

cxx = meson.get_compiler('cpp')

# find qt5
qt5 = import('qt5')
qt5_modules = ['Core', 'Gui', 'Widgets', 'Network', 'Xml']
qt5_dep = dependency('qt5', modules: qt5_modules)

# find bass
bass_lib = cxx.find_library(
	'bass',
	dirs : join_paths(meson.current_source_dir(), 'lib'),
	required : false)

if bass_lib.found()
	bass_inc = include_directories('include')
	bass = declare_dependency(
		dependencies : bass_lib,
		include_directories : bass_inc)
else
	bass = disabler()
endif

subdir('editor')
subdir('lib')
subdir('examples')
