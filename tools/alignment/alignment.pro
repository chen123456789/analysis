TEMPLATE = app
TARGET = alignment

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/alignment

LIBS += \
  -L$$TOPLEVEL/lib \
  -lalignment \
  -lprocessing \
  -lSimpleEvent \
  -lsetup \
  -ltracking \
  -lmillepede

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
