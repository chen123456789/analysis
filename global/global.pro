TEMPLATE = lib
TARGET = global

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  RootStyle \
  Helpers \
  Enums

HEADERS += \
  Constants.hh

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
