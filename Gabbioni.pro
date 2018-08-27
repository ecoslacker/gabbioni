#-------------------------------------------------
#
# Project created by QtCreator 2017-04-05T22:40:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gabbioni
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    simpledesigndialog.cpp \
    gabiondam.cpp \
    utilities.cpp \
    tests/interpol_tests.cpp \
    channelsection.cpp \
    weir.cpp \
    layer.cpp \
    gabbioni.cpp \
    oropezaanalysis.cpp \
    helpdialog.cpp \
    rationalformuladialog.cpp \
    manningeqdialog.cpp \
    myqgraphicsview.cpp \
    dimensiondialog.cpp \
    analysisresultsdialog.cpp \
    dxffiles.cpp \
    reportdialog.cpp \
    roughnesscoefficientdialog.cpp \
    editlayersdialog.cpp \
    datadialog.cpp \
    customtable.cpp \
    customtablenopaste.cpp \
    runoffcoefficientdialog.cpp \
    hydrology.cpp \
    textdialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    simpledesigndialog.h \
    gabiondam.h \
    utilities.h \
    channelsection.h \
    weir.h \
    layer.h \
    oropezaanalysis.h \
    helpdialog.h \
    rationalformuladialog.h \
    manningeqdialog.h \
    myqgraphicsview.h \
    dimensiondialog.h \
    analysisresultsdialog.h \
    dxffiles.h \
    reportdialog.h \
    roughnesscoefficientdialog.h \
    editlayersdialog.h \
    datadialog.h \
    customtable.h \
    customtablenopaste.h \
    runoffcoefficientdialog.h \
    hydrology.h \
    textdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    simpledesigndialog.ui \
    helpdialog.ui \
    rationalformuladialog.ui \
    manningeqdialog.ui \
    dimensiondialog.ui \
    analysisresultsdialog.ui \
    reportdialog.ui \
    roughnesscoefficientdialog.ui \
    editlayersdialog.ui \
    datadialog.ui \
    runoffcoefficientdialog.ui \
    textdialog.ui

RESOURCES += \
    resources.qrc

RC_FILE = Gabbioni.rc
ICON = Gabbioni.icns

unix|win32: LIBS += -L$$PWD/../shared/libcsvdata/lib/ -lcsvdata

INCLUDEPATH += $$PWD/../shared/libcsvdata/include
DEPENDPATH += $$PWD/../shared/libcsvdata/include
