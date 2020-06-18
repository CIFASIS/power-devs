TEMPLATE = lib
CONFIG+=staticlib
QT += core
TARGET=pdppt
contains(QT_VERSION,5)
{
message(Adding Widgets headers)
QT += widgets  
}


PDPPT=../../pdppt
# Input
HEADERS += $$PDPPT/codegenerator.h \
           $$PDPPT/modelatomic.h \
           $$PDPPT/modelchild.h \
           $$PDPPT/modelcoupled.h \
           $$PDPPT/modelparameter.h \
           $$PDPPT/parser.h
SOURCES += $$PDPPT/codegenerator.cpp $$PDPPT/parser.cpp

unix {
headers.path    = /usr/include/pdppt
headers.files   += $$HEADERS
lib.path = /usr/lib/
lib.files = ./libpdppt.a
}
INSTALLS  += headers lib
