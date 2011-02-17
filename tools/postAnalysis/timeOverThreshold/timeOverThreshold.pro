TEMPLATE = app
TARGET = timeOverThreshold

CONFIG += debug

TOPLEVEL = ../../..

FORMS+= \
  MainWindow.ui

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -ltracking \
  -lrootplot \
  -lsetup

CLASSES = \
  MainWindow \
  PostAnalysisPlot \
  timeOverThresholdProjection

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)