!exists($(PERDAIX10_PATH)) {
    error(Please set PERDAIX10_PATH environment variable!)
}

PERDAIX_LIB_DIR = $(PERDAIX10_PATH)/lib

CLHEPDIR = $(CLHEP_BASE_DIR)

INCLUDEPATH += \
	./include \
	$(PERDAIX10_PATH)/flightsoftware/libraries/1wire \
	$(PERDAIX10_PATH)/flightsoftware/libraries/dataformats \
	$(PERDAIX10_PATH)/flightsoftware/libraries/global \
	$(PERDAIX10_PATH)/flightsoftware/libraries/sensors \
	$(PERDAIX10_PATH)/flightsoftware/onboard/readout \
	$(PERDAIX10_PATH)/flightsoftware/libraries/sipm \
	$(PERDAIX10_PATH)/flightsoftware/libraries/perdaixusb \
	$(PERDAIX10_PATH)/flightsoftware/libraries/networking \
        $(PERDAIX10_PATH)/flightsoftware/libraries/tof \
        $(CLHEP_BASE_DIR)/include

DEPENDPATH += $${INCLUDEPATH}

LIBS += -L$${PERDAIX_LIB_DIR} -lperdaixdataformats -lperdaixusb -lperdaixglobal -lperdaixsensors -lperdaix1wire -lperdaixsipm -lperdaixtof -ldaqlib -lxml++-2.6 -lcolorlog -lutil -lregio -lmlink -lcommipc \
        -lquickusb -lusb

linux-g++-64 {
  LIBS -= -lquickusb -lusb
}
