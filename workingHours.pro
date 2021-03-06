#-------------------------------------------------
#
# Project created by QtCreator 2019-10-08T09:50:18
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = workingHours
TEMPLATE = app

win32-msvc* {
    contains(QT_ARCH, i386) {
        message("msvc 32-bit")
    } else {
        message("msvc 64-bit")
    }
}

win32-g++ {
    message("mingw")
    INCLUDEPATH += C:\dev\3rdParty\PoDoFo\include
    LIBS += -LC:\dev\3rdParty\PoDoFo\lib -lpodofo.dll
}

unix {
    message("*nix")
    LIBS += -lpodofo
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        cbooking.cpp \
        cdailyworking.cpp \
        cmonthlybooking.cpp \
        cmonthlyitemdelegate.cpp \
        cmonthlyview.cpp \
        common.cpp \
        cpublicholiday.cpp \
        cvacation.cpp \
        cyearlyview.cpp \
        czeitnachweis.cpp \
        main.cpp \
        cmainwindow.cpp

HEADERS += \
        cbooking.h \
        cdailyworking.h \
        cmainwindow.h \
        cmonthlybooking.h \
        cmonthlyitemdelegate.h \
        cmonthlyview.h \
        common.h \
        cpublicholiday.h \
        cyearlyview.h \
        czeitnachweis.h \
        cvacation.h

FORMS += \
        cmainwindow.ui \
        cmonthlyview.ui \
        cyearlyview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
