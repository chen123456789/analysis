#ifndef SensorTypes_hh
#define SensorTypes_hh

namespace SensorTypes
{

  enum Type {

    // Our own sensors
    START = 0,
    TRD_PRESSURE = START,
    TOF_VOLTAGE,
    HPE_0x3000_TEMP,
    HPE_0x3001_TEMP,
    HPE_0x3100_TEMP,
    HPE_0x3101_TEMP,
    HPE_0x3300_TEMP,
    HPE_0x3301_TEMP,
    HPE_0x3700_TEMP,
    HPE_0x3701_TEMP,
    HPE_0x6000_TEMP,
    HPE_0x6001_TEMP,
    HPE_0x6100_TEMP,
    HPE_0x6101_TEMP,
    HPE_0x6200_TEMP,
    HPE_0x6201_TEMP,
    HPE_0x6300_TEMP,
    HPE_0x6301_TEMP,
    HPE_0x6400_TEMP,
    HPE_0x6401_TEMP,
    HPE_0x6500_TEMP,
    HPE_0x6501_TEMP,
    HPE_0x6600_TEMP,
    HPE_0x6601_TEMP,
    HPE_0x6700_TEMP,
    HPE_0x6701_TEMP,
    HPE_0x7800_TEMP,
    HPE_0x7801_TEMP,
    HPE_0x7900_TEMP,
    HPE_0x7901_TEMP,
    HPE_0x7a00_TEMP,
    HPE_0x7a01_TEMP,
    HPE_0x7b00_TEMP,
    HPE_0x7b01_TEMP,
    HPE_0x7c00_TEMP,
    HPE_0x7c01_TEMP,
    HPE_0x7d00_TEMP,
    HPE_0x7d01_TEMP,
    HPE_0x7e00_TEMP,
    HPE_0x7e01_TEMP,
    HPE_0x7f00_TEMP,
    HPE_0x7f01_TEMP,
    TOF_1_TEMP,
    TOF_2_TEMP,
    TOF_3_TEMP,
    TOF_4_TEMP,
    TOF_5_TEMP,
    TOF_6_TEMP,
    TOF_7_TEMP,
    TOF_8_TEMP,
    TRIGGER_BOARD_TEMP,
    TRACKER_1_VOLTAGE,
    TRACKER_2_VOLTAGE,
    TRACKER_3_VOLTAGE,
    TRD_VOLTAGE,
    CPU_TEMP,
    PC_TEMP,
    TDC_TEMP,
    TRIGGER_RATE,
    TRD_UFE_BOTTOM_COLD_TEMP,
    TRD_UFE_TOP_COLD_TEMP,
    TRACKER_UFE_BOTTOM_COLD_TEMP,
    TRACKER_UFE_TOP_COLD_TEMP,
    TRACKER_UFE_TOP_HOT_TEMP,
    TRACKER_UFE_BOTTOM_HOT_TEMP,
    TRD_UFE_TOP_HOT_TEMP,
    TRD_UFE_BOTTOM_HOT_TEMP,
    POWER_UFE_TEMP,
    USB_BOARD_TEMP,
    POWER_MID_TEMP,
    TRD_TUBE_TOP_HOT_TEMP,
    TRD_TUBE_TOP_COLD_TEMP,
    TRD_GAS_COLD_TEMP,
    TRD_GAS_HOT_TEMP,
    TRD_TUBE_BOTTOM_HOT_TEMP,
    TRD_TUBE_BOTTOM_COLD_TEMP,
    POWER_GAS_TEMP,
    OUTLET_TEMP,
    INLET_TEMP,
    BAT_BOTTOM_TEMP,
    BAT_TOP_TEMP,

    // ATC sensors
    ATC_LATITUDE,
    ATC_LONGITUDE,
    ATC_HEIGHT,
    ATC_HORIZONTAL_SPEED,
    ATC_HEADING,

    // EBASS sensors
    EBASS_LATITUDE,
    EBASS_LONGITUDE,
    EBASS_HEIGHT,
    EBASS_HORIZONTAL_SPEED,
    EBASS_HEADING,
    EBASS_TEMP_OUT,
    EBASS_TEMP_GAS,
    EBASS_PRESSURE,

    N_SENSOR_TYPES = EBASS_PRESSURE + 1,
    END = N_SENSOR_TYPES
  };

  Type convertFromString(char* string);

}

#endif /* SensorTypes_hh */
