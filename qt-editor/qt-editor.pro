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
    syncpage.h \
    syncpageview.h \
    synctrack.h \
    trackarea.h \
    trackgroupnameview.h \
    trackview.h

FORMS += mainwindow.ui

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    rownumberview.cpp \
    syncpage.cpp \
    syncpageview.cpp \
    trackarea.cpp \
    trackgroupnameview.cpp \
    trackview.cpp
