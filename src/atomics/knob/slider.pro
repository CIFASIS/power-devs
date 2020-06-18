#-------------------------------------------------
#
# Project created by QtCreator 2012-09-26T15:34:18
#
#-------------------------------------------------

TARGET = ../../../bin/slider
TEMPLATE = app
QMAKE_STRIP = strip -R .comment
QMAKE_POST_LINK=$(STRIP) $(TARGET)

message(Qt version: $$QT_VERSION)
contains(QT_VERSION,5)
{
 message(Adding Widgets headers)
 QT += widgets
}


SOURCES += main.cpp\
        slider.cpp

HEADERS  += slider.h

FORMS    += slider.ui
