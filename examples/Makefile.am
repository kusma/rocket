if HAVE_BASS
noinst_PROGRAMS = example_bass
example_bass_SOURCES = example_bass.cpp
example_bass_CXXFLAGS = $(SDL_CFLAGS) $(GL_CFLAGS) $(GLU_CFLAGS)
example_bass_CPPFLAGS = $(BASS_CPPFLAGS)
example_bass_LDADD = $(top_builddir)/lib/librocket.la $(SDL_LIBS) -lbass $(GL_LIBS) $(GLU_LIBS)
example_bass_LDFLAGS = $(BASS_LDFLAGS)
endif
