#ifdef __ZEPHYR__
#include <cstring>
#endif

#include "BtHomeV2Device.h"

void BtHomeV2Device::clearMeasurementData()
{
    return BaseDevice::resetMeasurement();
}

/// @brief Builds an outgoing wrapper for the current measurement data.
/// @param payload
/// @return
size_t BtHomeV2Device::getAdvertisementData(uint8_t buffer[MAX_ADVERTISEMENT_SIZE])
{
    return BaseDevice::getAdvertisementData(buffer);
}

BtHomeV2Device::BtHomeV2Device(const char *shortName, const char *completeName, bool isTriggerDevice) : BaseDevice(shortName, completeName, isTriggerDevice) // Initialize with default device name and trigger-based device flag    
{
}

BtHomeV2Device::BtHomeV2Device(const char *shortName, const char *completeName, bool isTriggerDevice, uint8_t const* const key, const uint8_t macAddress[BLE_MAC_ADDRESS_LENGTH], uint32_t counter) : BaseDevice(shortName, completeName, isTriggerDevice, key, macAddress, counter){
}


bool BtHomeV2Device::addDeviceTypeId(uint16_t deviceTypeId)
{
    return BaseDevice::addUnsignedInteger(device_type_ID, deviceTypeId);
}
bool BtHomeV2Device::addPacketId(uint8_t packetId)
{
    return BaseDevice::addUnsignedInteger(packet_id, packetId);
}

bool BtHomeV2Device::addFirmwareVersion3(uint8_t major, uint8_t minor, uint8_t patch)
{
    uint32_t fwVersion = major << 16 | minor << 8 | patch;
    return BaseDevice::addUnsignedInteger(firmware_3_bytes, fwVersion);
}
bool BtHomeV2Device::addFirmwareVersion4(uint8_t major, uint8_t minor, uint8_t patch, uint8_t build)
{
    uint32_t fwVersion = major << 24 | minor << 16 | patch << 8 | build;
    return BaseDevice::addUnsignedInteger(firmware_4_bytes, fwVersion);
}

bool BtHomeV2Device::addTemperature_neg44_to_44_Resolution_0_35(float degreesCelsius)
{
    return BaseDevice::addFloat(temperature_int8_scale_0_35, degreesCelsius);
}
bool BtHomeV2Device::addTemperature_neg127_to_127_Resolution_1(int8_t degreesCelsius)
{
    return BaseDevice::addFloat(temperature_int8, degreesCelsius);
}
bool BtHomeV2Device::addTemperature_neg3276_to_3276_Resolution_0_1(float degreesCelsius)
{
    return BaseDevice::addFloat(temperature_int16_scale_0_1, degreesCelsius);
}
bool BtHomeV2Device::addTemperature_neg327_to_327_Resolution_0_01(float degreesCelsius)
{
    return BaseDevice::addFloat(temperature_int16_scale_0_01, degreesCelsius);
}

bool BtHomeV2Device::addDistanceMetres(float metres)
{
    return BaseDevice::addFloat(distance_metre, metres);
}

bool BtHomeV2Device::addDistanceMillimetres(uint16_t millimetres)
{
    return BaseDevice::addUnsignedInteger(distance_millimetre, millimetres);
}

bool BtHomeV2Device::addCount_0_4294967295(uint32_t count)
{
    return BaseDevice::addUnsignedInteger(count_uint32, count);
}

bool BtHomeV2Device::addCount_0_255(uint8_t count)
{
    return BaseDevice::addUnsignedInteger(count_uint8, count);
}
bool BtHomeV2Device::addCount_0_65535(uint16_t count)
{
    return BaseDevice::addUnsignedInteger(count_uint16, count);
}
bool BtHomeV2Device::addCount_neg128_127(int8_t count)
{
    return BaseDevice::addSignedInteger(count_int8, static_cast<uint64_t>(count));
}
bool BtHomeV2Device::addCount_neg32768_32767(int16_t count)
{
    return BaseDevice::addSignedInteger(count_int16, static_cast<uint64_t>(count));
}
bool BtHomeV2Device::addCount_neg2147483648_2147483647(int32_t count)
{
    return BaseDevice::addSignedInteger(count_int32, static_cast<uint64_t>(count));
}

bool BtHomeV2Device::addHumidityPercent_Resolution_0_01(float humidityPercent)
{
    return BaseDevice::addFloat(humidity_uint16, humidityPercent);
}

bool BtHomeV2Device::addHumidityPercent_Resolution_1(uint8_t humidityPercent)
{
    return BaseDevice::addFloat(humidity_uint8, humidityPercent);
}

bool BtHomeV2Device::addText(const char text[])
{
    return BaseDevice::addRaw(0x53, (uint8_t *)text, strlen(text));
}

bool BtHomeV2Device::addTime(uint32_t secondsSinceEpoch)
{
    return BaseDevice::addUnsignedInteger(timestamp, secondsSinceEpoch);
}

bool BtHomeV2Device::addRaw(uint8_t *bytes, uint8_t size)
{
    return BaseDevice::addRaw(0x54, bytes, size);
}

bool BtHomeV2Device::addBatteryPercentage(uint8_t batteryPercentage)
{
    return BaseDevice::addUnsignedInteger(battery_percentage, batteryPercentage);
}

bool BtHomeV2Device::setBatteryState(BATTERY_STATE batteryState)
{
    return BaseDevice::addState(battery_state, batteryState);
}

bool BtHomeV2Device::setBatteryChargingState(Battery_Charging_Sensor_Status batteryChargingState)
{
    return BaseDevice::addState(battery_charging, batteryChargingState);
}

bool BtHomeV2Device::setCarbonMonoxideState(Carbon_Monoxide_Sensor_Status carbonMonoxideState)
{
    return BaseDevice::addState(carbon_monoxide, carbonMonoxideState);
}

bool BtHomeV2Device::setColdState(Cold_Sensor_Status coldState)
{
    return BaseDevice::addState(cold, coldState);
}

bool BtHomeV2Device::setConnectivityState(Connectivity_Sensor_Status connectivityState)
{
    return BaseDevice::addState(connectivity, connectivityState);
}

bool BtHomeV2Device::setDoorState(Door_Sensor_Status doorState)
{
    return BaseDevice::addState(door, doorState);
}

bool BtHomeV2Device::setGarageDoorState(Garage_Door_Sensor_Status garageDoorState)
{
    return BaseDevice::addState(garage_door, garageDoorState);
}

bool BtHomeV2Device::setGasState(Gas_Sensor_Status gasState)
{
    return BaseDevice::addState(gas, gasState);
}

bool BtHomeV2Device::setGenericState(Generic_Sensor_Status genericState)
{
    return BaseDevice::addState(generic_boolean, genericState);
}

bool BtHomeV2Device::setHeatState(Heat_Sensor_Status heatState)
{
    return BaseDevice::addState(heat, heatState);
}

bool BtHomeV2Device::setLightState(Light_Sensor_Status lightState)
{
    return BaseDevice::addState(light, lightState);
}

bool BtHomeV2Device::setLockState(Lock_Sensor_Status lockState)
{
    return BaseDevice::addState(lock, lockState);
}

bool BtHomeV2Device::setMoistureState(Moisture_Sensor_Status moistureState)
{
    return BaseDevice::addState(moisture, moistureState);
}

bool BtHomeV2Device::setMotionState(Motion_Sensor_Status motionState)
{
    return BaseDevice::addState(motion, motionState);
}

bool BtHomeV2Device::setMovingState(Moving_Sensor_Status movingState)
{
    return BaseDevice::addState(moving, movingState);
}

bool BtHomeV2Device::setOccupancyState(Occupancy_Sensor_Status occupancyState)
{
    return BaseDevice::addState(occupancy, occupancyState);
}

bool BtHomeV2Device::setOpeningState(Opening_Sensor_Status openingState)
{
    return BaseDevice::addState(opening, openingState);
}

bool BtHomeV2Device::setPlugState(Plug_Sensor_Status plugState)
{
    return BaseDevice::addState(plug, plugState);
}

bool BtHomeV2Device::setPowerState(Power_Sensor_Status powerState)
{
    return BaseDevice::addState(power, powerState);
}

bool BtHomeV2Device::setPresenceState(Presence_Sensor_Status presenceState)
{
    return BaseDevice::addState(presence, presenceState);
}

bool BtHomeV2Device::setProblemState(Problem_Sensor_Status problemState)
{
    return BaseDevice::addState(problem, problemState);
}

bool BtHomeV2Device::setRunningState(Running_Sensor_Status runningState)
{
    return BaseDevice::addState(running, runningState);
}

bool BtHomeV2Device::setSafetyState(Safety_Sensor_Status safetyState)
{
    return BaseDevice::addState(safety, safetyState);
}

bool BtHomeV2Device::setSmokeState(Smoke_Sensor_Status smokeState)
{
    return BaseDevice::addState(smoke, smokeState);
}

bool BtHomeV2Device::setSoundState(Sound_Sensor_Status soundState)
{
    return BaseDevice::addState(sound, soundState);
}

bool BtHomeV2Device::setTamperState(Tamper_Sensor_Status tamperState)
{
    return BaseDevice::addState(tamper, tamperState);
}

bool BtHomeV2Device::setVibrationState(Vibration_Sensor_Status vibrationState)
{
    return BaseDevice::addState(vibration, vibrationState);
}

bool BtHomeV2Device::setWindowState(Window_Sensor_Status windowState)
{
    return BaseDevice::addState(window, windowState);
}

bool BtHomeV2Device::setButtonEvent(Button_Event_Status buttonEvent)
{
    return BaseDevice::addState(button, buttonEvent);
}

bool BtHomeV2Device::setDimmerEvent(Dimmer_Event_Status dimmerEvent, uint8_t steps)
{
    return BaseDevice::addState(dimmer, dimmerEvent, steps);
}

bool BtHomeV2Device::addAccelerationMs2(float value)
{
    return BaseDevice::addFloat(acceleration, value);
}

bool BtHomeV2Device::addChannel(uint8_t value)
{
    return BaseDevice::addUnsignedInteger(channel, value);
}

bool BtHomeV2Device::addCo2Ppm(uint16_t value)
{
    return BaseDevice::addUnsignedInteger(co2, value);
}

bool BtHomeV2Device::addConductivityMicrosecondsPerCm(float value)
{
    return BaseDevice::addFloat(conductivity, value);
}

bool BtHomeV2Device::addCurrentAmps_neg32_to_32_Resolution_0_001(float value)
{
    return BaseDevice::addFloat(current_int16, value);
}

bool BtHomeV2Device::addCurrentAmps_0_65_Resolution_0_001(float value)
{
    return BaseDevice::addFloat(current_uint16, value);
}

bool BtHomeV2Device::addDewPointDegreesCelsius(float value)
{
    return BaseDevice::addFloat(dewpoint, value);
}

bool BtHomeV2Device::addDirectionDegrees(float value)
{
    return BaseDevice::addFloat(direction, value);
}

bool BtHomeV2Device::addDurationSeconds(float value)
{
    return BaseDevice::addFloat(duration_uint24, value);
}

bool BtHomeV2Device::addEnergyKwh_0_to_16777(float value)
{
    return BaseDevice::addFloat(energy_uint24, value);
}

bool BtHomeV2Device::addEnergyKwh_0_to_4294967(float value)
{
    return BaseDevice::addFloat(energy_uint32, value);
}

bool BtHomeV2Device::addGasM3_0_to_16777(float value)
{
    return BaseDevice::addFloat(gas_uint24, value);
}

bool BtHomeV2Device::addGasM3_0_to_4294967(float value)
{
    return BaseDevice::addFloat(gas_uint32, value);
}

bool BtHomeV2Device::addGyroscopeDegreeSeconds(float value)
{
    return BaseDevice::addFloat(gyroscope, value);
}

bool BtHomeV2Device::addIlluminanceLux(float value)
{
    return BaseDevice::addFloat(illuminance, value);
}

bool BtHomeV2Device::addMassKg(float value)
{
    return BaseDevice::addFloat(mass_kg, value);
}

bool BtHomeV2Device::addMassLb(float value)
{
    return BaseDevice::addFloat(mass_lb, value);
}

bool BtHomeV2Device::addMoisturePercent_Resolution_1(uint8_t value)
{
    return BaseDevice::addUnsignedInteger(moisture_uint8, value);
}

bool BtHomeV2Device::addMoisturePercent_Resolution_0_01(float value)
{
    return BaseDevice::addFloat(moisture_uint16, value);
}

bool BtHomeV2Device::addPm2_5UgM3(uint16_t value)
{
    return BaseDevice::addUnsignedInteger(pm2_5, value);
}

bool BtHomeV2Device::addPm10UgM3(uint16_t value)
{
    return BaseDevice::addUnsignedInteger(pm10, value);
}

bool BtHomeV2Device::addPower_neg21474836_to_21474836_resolution_0_01(float value)
{
    return BaseDevice::addFloat(power_int32, value);
}

bool BtHomeV2Device::addPower_0_to_167772_resolution_0_01(float value)
{
    return BaseDevice::addFloat(power_uint24, value);
}

bool BtHomeV2Device::addPrecipitationMm(float value)
{
    return BaseDevice::addFloat(precipitation, value);
}

bool BtHomeV2Device::addPressureHpa(float value)
{
    return BaseDevice::addFloat(pressure, value);
}

bool BtHomeV2Device::addRotationDegrees(float value)
{
    return BaseDevice::addFloat(rotation, value);
}

bool BtHomeV2Device::addSpeedMs(float value)
{
    return BaseDevice::addFloat(speed, value);
}

bool BtHomeV2Device::addTvocUgm3(uint16_t value)
{
    return BaseDevice::addUnsignedInteger(tvoc, value);
}

bool BtHomeV2Device::addVoltage_0_to_6550_resolution_0_1(float value)
{
    return BaseDevice::addFloat(voltage_0_1, value);
}

bool BtHomeV2Device::addVoltage_0_to_65_resolution_0_001(float value)
{
    return BaseDevice::addFloat(voltage_0_001, value);
}

bool BtHomeV2Device::addVolumeLitres_0_to_6555_resolution_0_1(float value)
{
    return BaseDevice::addFloat(volume_uint16_scale_0_1, value);
}

bool BtHomeV2Device::addVolumeLitres_0_to_65550_resolution_1(uint16_t value)
{
    return BaseDevice::addUnsignedInteger(volume_uint16_scale_1, value);
}

bool BtHomeV2Device::addVolumeLitres_0_to_4294967_resolution_0_001(float value)
{
    return BaseDevice::addFloat(volume_uint32, value);
}

bool BtHomeV2Device::addVolumeStorageLitres(float value)
{
    return BaseDevice::addFloat(volume_storage, value);
}

bool BtHomeV2Device::addVolumeFlowRateM3hr(float value)
{
    return BaseDevice::addFloat(volume_flow_rate, value);
}

bool BtHomeV2Device::addUvIndex(float value)
{
    return BaseDevice::addFloat(UV_index, value);
}

bool BtHomeV2Device::addWaterLitres(float value)
{
    return BaseDevice::addFloat(water_litre, value);
}

