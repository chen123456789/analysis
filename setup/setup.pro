TEMPLATE = lib
TARGET = setup

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += DetectorElement.cc \
           Layer.cc \
           Setup.cc \
           SiPMArray.cc

HEADERS += DetectorElement.hh \
           Layer.hh \
           Setup.hh \
           SiPMArray.hh

INCLUDEPATH += $$TOPLEVEL/event

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -levent

include(../root.pri)