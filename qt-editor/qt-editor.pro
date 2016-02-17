TEMPLATE = app
TARGET = qt-editor
DEPENDPATH += . GeneratedFiles
INCLUDEPATH += .

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
}

HEADERS += \
    mainwindow.h \
    rownumberview.h \
    synctrack.h \
    trackarea.h \
    syncpage.h \
    syncpageview.h \
    trackgroupnameview.h \
    trackview.h

FORMS += mainwindow.ui

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    rownumberview.cpp \
    synctrack.cpp \
    trackarea.cpp \
    syncpage.cpp \
    syncpageview.cpp \
    trackgroupnameview.cpp \
    trackview.cpp
