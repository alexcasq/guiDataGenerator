QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS


include(/home/visioniot/opencvConfig42.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    massivevision.cpp

HEADERS += \
    mainwindow.h \
    massivevision.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
