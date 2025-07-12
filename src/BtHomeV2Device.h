#ifndef BT_HOME_H
#define BT_HOME_H

// https://bthome.io/format/

#include <Arduino.h>
#include "BaseDevice.h"

/**
 * @file BTHome.h
 * @brief BTHome v2 sensor data setter methods with summaries.
 */

/// @brief Battery state options
enum BATTERY_STATE
{
    BATTERY_STATE_NORMAL = 0,
    BATTERY_STATE_LOW = 1
};

class BtHomeV2Device
{
public:
    /// @brief
    /// @param shortName Short name of the device - sent when space is limited. Max 10 characters
    /// @param completeName  Full name of the device - sent when space is available. Max 20 characters
    /// @param isTriggerDevice - If the device sends data when triggered
    BtHomeV2Device(const char *shortName, const char *completeName, bool isTriggerDevice);
    BtHomeV2Device(const char *shortName, const char *completeName, bool isTriggerBased, uint8_t const *const key, const uint8_t macAddress[BLE_MAC_ADDRESS_LENGTH], uint32_t counter = 1);

    size_t getAdvertisementData(uint8_t buffer[MAX_ADVERTISEMENT_SIZE]);

    void clearMeasurementData();

    bool addDeviceTypeId(uint16_t deviceTypeId);
    bool addPacketId(uint8_t packetId);

    // Currently not implemented in Home Assistant, but defined in BThome
    bool addFirmwareVersion3(uint8_t major, uint8_t minor, uint8_t patch);
    bool addFirmwareVersion4(uint8_t major, uint8_t minor, uint8_t patch, uint8_t build);

    /**
     * @brief Set a generic count value in the packet.
     * @param count Arbitrary count (e.g., event count).
     */
    bool addCount_0_4294967295(uint32_t count);
    bool addCount_0_255(uint8_t count);
    bool addCount_0_65535(uint16_t count);
    bool addCount_neg128_127(int8_t count);
    bool addCount_neg32768_32767(int16_t count);
    bool addCount_neg2147483648_2147483647(int32_t count);

    /**
     * @brief Set the distance measurement value in the packet.
     * @param distanceMetres Distance in metres.
     */
    bool addDistanceMetres(float distanceMetres);

    bool addTemperature_neg44_to_44_Resolution_0_35(float degreesCelsius);
    bool addTemperature_neg127_to_127_Resolution_1(int8_t degreesCelsius);
    bool addTemperature_neg3276_to_3276_Resolution_0_1(float degreesCelsius);
    bool addTemperature_neg327_to_327_Resolution_0_01(float degreesCelsius);

    /**
     * @brief Set the distance measurement value in the packet.
     * @param distanceMillimetres Distance in metres.
     */
    bool addDistanceMillimetres(uint16_t millimetres);

    /**
     * @brief Set the battery level value in the packet.
     * @param batteryPercentOrMillivolts Battery level as an unsigned 8-bit value (e.g., percentage or mV depending on implementation).
     */
    bool addBatteryPercentage(uint8_t batteryPercentage);

    bool addText(const char text[]);

    /// @brief Add seconds since the unix epoch
    /// @param secondsSinceUnixEpoch - Seconds since the unix epoch
    /// @return
    bool addTime(uint32_t secondsSinceUnixEpoch);

    bool addRaw(uint8_t *bytes, uint8_t size);

    bool setBatteryState(BATTERY_STATE batteryState);
    bool setBatteryChargingState(Battery_Charging_Sensor_Status batteryChargingState);
    bool setCarbonMonoxideState(Carbon_Monoxide_Sensor_Status carbonMonoxideState);
    bool setColdState(Cold_Sensor_Status coldState);
    bool setConnectivityState(Connectivity_Sensor_Status connectivityState);
    bool setDoorState(Door_Sensor_Status doorState);
    bool setGarageDoorState(Garage_Door_Sensor_Status garageDoorState);
    bool setGasState(Gas_Sensor_Status gasState);
    bool setGenericState(Generic_Sensor_Status genericState);
    bool setHeatState(Heat_Sensor_Status heatState);
    bool setLightState(Light_Sensor_Status lightState);
    bool setLockState(Lock_Sensor_Status lockState);
    bool setMoistureState(Moisture_Sensor_Status moistureState);
    bool setMotionState(Motion_Sensor_Status motionState);
    bool setMovingState(Moving_Sensor_Status movingState);
    bool setOccupancyState(Occupancy_Sensor_Status occupancyState);
    bool setOpeningState(Opening_Sensor_Status openingState);
    bool setPlugState(Plug_Sensor_Status plugState);
    bool setPowerState(Power_Sensor_Status powerState);
    bool setPresenceState(Presence_Sensor_Status presenceState);
    bool setProblemState(Problem_Sensor_Status problemState);
    bool setRunningState(Running_Sensor_Status runningState);
    bool setSafetyState(Safety_Sensor_Status safetyState);
    bool setSmokeState(Smoke_Sensor_Status smokeState);
    bool setSoundState(Sound_Sensor_Status soundState);
    bool setTamperState(Tamper_Sensor_Status tamperState);
    bool setVibrationState(Vibration_Sensor_Status vibrationState);
    bool setWindowState(Window_Sensor_Status windowState);

    bool setButtonEvent(Button_Event_Status buttonEvent);
    bool setDimmerEvent(Dimmer_Event_Status dimmerEvent, uint8_t steps);

    /**
     * @brief Set the humidity value with a resolution of 0.01% in the packet. 2 bytes.
     * @param humidityPercent Relative humidity in percent.
     */
    bool addHumidityPercent_Resolution_0_01(float humidityPercent);

    /**
     * @brief Set the humidity value with a resolution of 0.1% in the packet. 1 byte.
     * @param humidityPercent Relative humidity in percent.
     */
    bool addHumidityPercent_Resolution_1(uint8_t humidityPercent);

    bool addAccelerationMs2(float value);

    bool addChannel(uint8_t value);

    bool addCo2Ppm(uint16_t value);

    bool addConductivityMicrosecondsPerCm(float value);

    bool addCurrentAmps_0_65_Resolution_0_001(float value);

    bool addCurrentAmps_neg32_to_32_Resolution_0_001(float value);

    bool addDewPointDegreesCelsius(float value);

    bool addDirectionDegrees(float value);

    bool addDurationSeconds(float value);

    bool addEnergyKwh_0_to_16777(float value);
    bool addEnergyKwh_0_to_4294967(float value);

    bool addGasM3_0_to_16777(float value);

    bool addGasM3_0_to_4294967(float value);

    bool addGyroscopeDegreeSeconds(float value);

    bool addIlluminanceLux(float value);

    bool addMassKg(float value);

    bool addMassLb(float value);

    bool addMoisturePercent_Resolution_1(uint8_t value);

    bool addMoisturePercent_Resolution_0_01(float value);

    bool addPm2_5UgM3(uint16_t value);

    bool addPm10UgM3(uint16_t value);

    bool addPower_neg21474836_to_21474836_resolution_0_01(float value);

    bool addPower_0_to_167772_resolution_0_01(float value);

    bool addPrecipitationMm(float value);

    bool addPressureHpa(float value);

    bool addRotationDegrees(float value);

    bool addSpeedMs(float value);

    bool addTvocUgm3(uint16_t value);

    bool addVoltage_0_to_6550_resolution_0_1(float value);

    bool addVoltage_0_to_65_resolution_0_001(float value);

    bool addVolumeLitres_0_to_6555_resolution_0_1(float value);

    bool addVolumeLitres_0_to_65550_resolution_1(uint16_t value);

    bool addVolumeLitres_0_to_4294967_resolution_0_001(float value);

    bool addVolumeStorageLitres(float value);

    bool addVolumeFlowRateM3hr(float value);

    bool addUvIndex(float value);

    bool addWaterLitres(float value);

private:
    BaseDevice _baseDevice;
};

#endif // BT_HOME_H
