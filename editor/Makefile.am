EXTRA_DIST = \
	appicon.icns \
	appicon.ico \
	clientsocket.cpp \
	clientsocket.h \
	editor.cpp \
	editor.pro \
	editor.qrc \
	editor.rc \
	mainwindow.cpp \
	mainwindow.h \
	syncdocument.cpp \
	syncdocument.h \
	synctrack.h \
	trackview.cpp \
	trackview.h

Makefile.qmake: editor.pro
	$(QMAKE) -makefile -o Makefile.qmake $(srcdir)/editor.pro

all-local: Makefile.qmake
	$(MAKE) -f Makefile.qmake $(AM_MAKEFLAGS) all

check-local: Makefile.qmake
	$(MAKE) -f Makefile.qmake $(AM_MAKEFLAGS) check

clean-local: Makefile.qmake
	$(MAKE) -f Makefile.qmake $(AM_MAKEFLAGS) clean
	rm -f Makefile.qmake
