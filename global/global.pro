TEMPLATE = lib
TARGET = global

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  RootStyle \
  Helpers \
  Enums \
  ParticleProperties \
  KineticVariable \
  ProgressBar

HEADERS += \
  Constants.hh

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
