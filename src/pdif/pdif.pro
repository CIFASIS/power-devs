######################################################################
# Automatically generated by qmake (2.01a) Thu May 28 11:14:23 2009
######################################################################

TEMPLATE = app
win32 {
  TARGET = ../../../tmp_win_installer/powerdevs/bin/pdif
}
unix {
  TARGET = ../../bin/pdif
}
contains(QT_VERSION,5)
{
  message(Adding Widgets headers)
  QT += widgets
}

DEPENDPATH += .
INCLUDEPATH += .
QMAKE_STRIP = strip -R .comment
QMAKE_POST_LINK=$(STRIP) $(TARGET)

# Input
CONFIG += qt 
HEADERS += pdif.h
SOURCES += main.cpp pdif.cpp
RESOURCES += pdif.qrc
RC_FILE = pdif.rc
