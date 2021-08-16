QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport charts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS_RELEASE += /O2

SOURCES += \
    base64.c \
    main.cpp \
    liveviewer.cpp \
    portselectdialog.cpp \
    serial_io.cpp \
    viewerbutton.cpp \
    vieweroption.cpp \
    viewerparam.cpp

HEADERS += \
    base64.h \
    liveviewer.h \
    portselectdialog.h \
    serial_io.h \
    viewerbutton.h \
    vieweroption.h \
    viewerparam.h

FORMS += \
    liveviewer.ui \
    portselectdialog.ui \
    viewerbutton.ui \
    vieweroption.ui \
    viewerparam.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
