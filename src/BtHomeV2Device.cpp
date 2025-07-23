#include "BtHomeV2Device.h"

void BtHomeV2Device::clearMeasurementData()
{
    return _baseDevice.resetMeasurement();
}

/// @brief Builds an outgoing wrapper for the current measurement data.
/// @param payload
/// @return
size_t BtHomeV2Device::getAdvertisementData(uint8_t buffer[MAX_ADVERTISEMENT_SIZE])
{
    return _baseDevice.getAdvertisementData(buffer);
}

BtHomeV2Device::BtHomeV2Device(const char *shortName, const char *completeName, bool isTriggerDevice) : _baseDevice(shortName, completeName, isTriggerDevice) // Initialize with default device name and trigger-based device flag
{
}

BtHomeV2Device::BtHomeV2Device(const char *shortName, const char *completeName, bool isTriggerBased, uint8_t const* const key, const uint8_t macAddress[BLE_MAC_ADDRESS_LENGTH], uint32_t counter) : _baseDevice(shortName, completeName, isTriggerBased, key, macAddress, counter){
}

bool BtHomeV2Device::addDeviceTypeId(uint16_t deviceTypeId)
{
    return _baseDevice.addUnsignedInteger(device_type_ID, deviceTypeId);
}
bool BtHomeV2Device::addPacketId(uint8_t packetId)
{
    return _baseDevice.addUnsignedInteger(packet_id, packetId);
}

bool BtHomeV2Device::addFirmwareVersion3(uint8_t major, uint8_t minor, uint8_t patch)
{
    uint32_t fwVersion = major << 16 | minor << 8 | patch;
    return _baseDevice.addUnsignedInteger(device_type_ID, fwVersion);
}
bool BtHomeV2Device::addFirmwareVersion4(uint8_t major, uint8_t minor, uint8_t patch, uint8_t build)
{
    uint32_t fwVersion = major << 24 | minor << 16 | patch << 8 | build;
    return _baseDevice.addUnsignedInteger(device_type_ID, fwVersion);
}

bool BtHomeV2Device::addTemperature_neg44_to_44_Resolution_0_35(float degreesCelsius)
{
    return _baseDevice.addFloat(temperature_int8_scale_0_35, degreesCelsius);
}
bool BtHomeV2Device::addTemperature_neg127_to_127_Resolution_1(int8_t degreesCelsius)
{
    return _baseDevice.addFloat(temperature_int8, degreesCelsius);
}
bool BtHomeV2Device::addTemperature_neg3276_to_3276_Resolution_0_1(float degreesCelsius)
{
    return _baseDevice.addFloat(temperature_int16_scale_0_1, degreesCelsius);
}
bool BtHomeV2Device::addTemperature_neg327_to_327_Resolution_0_01(float degreesCelsius)
{
    return _baseDevice.addFloat(temperature_int16_scale_0_01, degreesCelsius);
}

bool BtHomeV2Device::addDistanceMetres(float metres)
{
    return _baseDevice.addFloat(distance_metre, metres);
}

bool BtHomeV2Device::addDistanceMillimetres(uint16_t millimetres)
{
    return _baseDevice.addUnsignedInteger(distance_millimetre, millimetres);
}

bool BtHomeV2Device::addCount_0_4294967295(uint32_t count)
{
    return _baseDevice.addUnsignedInteger(count_uint32, count);
}

bool BtHomeV2Device::addCount_0_255(uint8_t count)
{
    Serial.print("Adding count 0-255: ");
    Serial.println(count);
    return _baseDevice.addUnsignedInteger(count_uint8, count);
}
bool BtHomeV2Device::addCount_0_65535(uint16_t count)
{
    return _baseDevice.addUnsignedInteger(count_uint16, count);
}
bool BtHomeV2Device::addCount_neg128_127(int8_t count)
{
    return _baseDevice.addSignedInteger(count_int8, static_cast<uint64_t>(count));
}
bool BtHomeV2Device::addCount_neg32768_32767(int16_t count)
{
    return _baseDevice.addSignedInteger(count_int16, static_cast<uint64_t>(count));
}
bool BtHomeV2Device::addCount_neg2147483648_2147483647(int32_t count)
{
    return _baseDevice.addSignedInteger(count_int32, static_cast<uint64_t>(count));
}

bool BtHomeV2Device::addHumidityPercent_Resolution_0_01(float humidityPercent)
{
    return _baseDevice.addFloat(humidity_uint16, humidityPercent);
}

bool BtHomeV2Device::addHumidityPercent_Resolution_1(uint8_t humidityPercent)
{
    return _baseDevice.addFloat(humidity_uint8, humidityPercent);
}

bool BtHomeV2Device::addText(const char text[])
{
    return _baseDevice.addRaw(0x53, (uint8_t *)text, strlen(text));
}

bool BtHomeV2Device::addTime(uint32_t secondsSinceEpoch)
{
    return _baseDevice.addUnsignedInteger(timestamp, secondsSinceEpoch);
}

bool BtHomeV2Device::addRaw(uint8_t *bytes, uint8_t size)
{
    return _baseDevice.addRaw(0x54, bytes, size);
}

bool BtHomeV2Device::addBatteryPercentage(uint8_t batteryPercentage)
{
    return _baseDevice.addUnsignedInteger(battery_percentage, batteryPercentage);
}

bool BtHomeV2Device::setBatteryState(BATTERY_STATE batteryState)
{
    return _baseDevice.addState(battery_state, batteryState);
}

bool BtHomeV2Device::setBatteryChargingState(Battery_Charging_Sensor_Status batteryChargingState)
{
    return _baseDevice.addState(battery_charging, batteryChargingState);
}

bool BtHomeV2Device::setCarbonMonoxideState(Carbon_Monoxide_Sensor_Status carbonMonoxideState)
{
    return _baseDevice.addState(carbon_monoxide, carbonMonoxideState);
}

bool BtHomeV2Device::setColdState(Cold_Sensor_Status coldState)
{
    return _baseDevice.addState(cold, coldState);
}

bool BtHomeV2Device::setConnectivityState(Connectivity_Sensor_Status connectivityState)
{
    return _baseDevice.addState(connectivity, connectivityState);
}

bool BtHomeV2Device::setDoorState(Door_Sensor_Status doorState)
{
    return _baseDevice.addState(door, doorState);
}

bool BtHomeV2Device::setGarageDoorState(Garage_Door_Sensor_Status garageDoorState)
{
    return _baseDevice.addState(garage_door, garageDoorState);
}

bool BtHomeV2Device::setGasState(Gas_Sensor_Status gasState)
{
    return _baseDevice.addState(gas, gasState);
}

bool BtHomeV2Device::setGenericState(Generic_Sensor_Status genericState)
{
    return _baseDevice.addState(generic_boolean, genericState);
}

bool BtHomeV2Device::setHeatState(Heat_Sensor_Status heatState)
{
    return _baseDevice.addState(heat, heatState);
}

bool BtHomeV2Device::setLightState(Light_Sensor_Status lightState)
{
    return _baseDevice.addState(light, lightState);
}

bool BtHomeV2Device::setLockState(Lock_Sensor_Status lockState)
{
    return _baseDevice.addState(lock, lockState);
}

bool BtHomeV2Device::setMoistureState(Moisture_Sensor_Status moistureState)
{
    return _baseDevice.addState(moisture, moistureState);
}

bool BtHomeV2Device::setMotionState(Motion_Sensor_Status motionState)
{
    return _baseDevice.addState(motion, motionState);
}

bool BtHomeV2Device::setMovingState(Moving_Sensor_Status movingState)
{
    return _baseDevice.addState(moving, movingState);
}

bool BtHomeV2Device::setOccupancyState(Occupancy_Sensor_Status occupancyState)
{
    return _baseDevice.addState(occupancy, occupancyState);
}

bool BtHomeV2Device::setOpeningState(Opening_Sensor_Status openingState)
{
    return _baseDevice.addState(opening, openingState);
}

bool BtHomeV2Device::setPlugState(Plug_Sensor_Status plugState)
{
    return _baseDevice.addState(plug, plugState);
}

bool BtHomeV2Device::setPowerState(Power_Sensor_Status powerState)
{
    return _baseDevice.addState(power, powerState);
}

bool BtHomeV2Device::setPresenceState(Presence_Sensor_Status presenceState)
{
    return _baseDevice.addState(presence, presenceState);
}

bool BtHomeV2Device::setProblemState(Problem_Sensor_Status problemState)
{
    return _baseDevice.addState(problem, problemState);
}

bool BtHomeV2Device::setRunningState(Running_Sensor_Status runningState)
{
    return _baseDevice.addState(running, runningState);
}

bool BtHomeV2Device::setSafetyState(Safety_Sensor_Status safetyState)
{
    return _baseDevice.addState(safety, safetyState);
}

bool BtHomeV2Device::setSmokeState(Smoke_Sensor_Status smokeState)
{
    return _baseDevice.addState(smoke, smokeState);
}

bool BtHomeV2Device::setSoundState(Sound_Sensor_Status soundState)
{
    return _baseDevice.addState(sound, soundState);
}

bool BtHomeV2Device::setTamperState(Tamper_Sensor_Status tamperState)
{
    return _baseDevice.addState(tamper, tamperState);
}

bool BtHomeV2Device::setVibrationState(Vibration_Sensor_Status vibrationState)
{
    return _baseDevice.addState(vibration, vibrationState);
}

bool BtHomeV2Device::setWindowState(Window_Sensor_Status windowState)
{
    return _baseDevice.addState(window, windowState);
}

bool BtHomeV2Device::setButtonEvent(Button_Event_Status buttonEvent)
{
    return _baseDevice.addState(button, buttonEvent);
}

bool BtHomeV2Device::setDimmerEvent(Dimmer_Event_Status dimmerEvent, uint8_t steps)
{
    return _baseDevice.addState(dimmer, dimmerEvent, steps);
}

bool BtHomeV2Device::addAccelerationMs2(float value)
{
    return _baseDevice.addFloat(acceleration, value);
}

bool BtHomeV2Device::addChannel(uint8_t value)
{
    return _baseDevice.addUnsignedInteger(channel, value);
}

bool BtHomeV2Device::addCo2Ppm(uint16_t value)
{
    return _baseDevice.addUnsignedInteger(co2, value);
}

bool BtHomeV2Device::addConductivityMicrosecondsPerCm(float value)
{
    return _baseDevice.addFloat(conductivity, value);
}

bool BtHomeV2Device::addCurrentAmps_neg32_to_32_Resolution_0_001(float value)
{
    return _baseDevice.addFloat(current_int16, value);
}

bool BtHomeV2Device::addCurrentAmps_0_65_Resolution_0_001(float value)
{
    return _baseDevice.addFloat(current_uint16, value);
}

bool BtHomeV2Device::addDewPointDegreesCelsius(float value)
{
    return _baseDevice.addFloat(dewpoint, value);
}

bool BtHomeV2Device::addDirectionDegrees(float value)
{
    return _baseDevice.addFloat(direction, value);
}

bool BtHomeV2Device::addDurationSeconds(float value)
{
    return _baseDevice.addFloat(duration_uint24, value);
}

bool BtHomeV2Device::addEnergyKwh_0_to_16777(float value)
{
    return _baseDevice.addFloat(energy_uint24, value);
}

bool BtHomeV2Device::addEnergyKwh_0_to_4294967(float value)
{
    return _baseDevice.addFloat(energy_uint32, value);
}

bool BtHomeV2Device::addGasM3_0_to_16777(float value)
{
    return _baseDevice.addFloat(gas_uint24, value);
}

bool BtHomeV2Device::addGasM3_0_to_4294967(float value)
{
    return _baseDevice.addFloat(gas_uint32, value);
}

bool BtHomeV2Device::addGyroscopeDegreeSeconds(float value)
{
    return _baseDevice.addFloat(gyroscope, value);
}

bool BtHomeV2Device::addIlluminanceLux(float value)
{
    return _baseDevice.addFloat(illuminance, value);
}

bool BtHomeV2Device::addMassKg(float value)
{
    return _baseDevice.addFloat(mass_kg, value);
}

bool BtHomeV2Device::addMassLb(float value)
{
    return _baseDevice.addFloat(mass_lb, value);
}

bool BtHomeV2Device::addMoisturePercent_Resolution_1(uint8_t value)
{
    return _baseDevice.addUnsignedInteger(moisture_uint8, value);
}

bool BtHomeV2Device::addMoisturePercent_Resolution_0_01(float value)
{
    return _baseDevice.addFloat(moisture_uint16, value);
}

bool BtHomeV2Device::addPm2_5UgM3(uint16_t value)
{
    return _baseDevice.addUnsignedInteger(pm2_5, value);
}

bool BtHomeV2Device::addPm10UgM3(uint16_t value)
{
    return _baseDevice.addUnsignedInteger(pm10, value);
}

bool BtHomeV2Device::addPower_neg21474836_to_21474836_resolution_0_01(float value)
{
    return _baseDevice.addFloat(power_int32, value);
}

bool BtHomeV2Device::addPower_0_to_167772_resolution_0_01(float value)
{
    return _baseDevice.addFloat(power_uint24, value);
}

bool BtHomeV2Device::addPrecipitationMm(float value)
{
    return _baseDevice.addFloat(precipitation, value);
}

bool BtHomeV2Device::addPressureHpa(float value)
{
    return _baseDevice.addFloat(pressure, value);
}

bool BtHomeV2Device::addRotationDegrees(float value)
{
    return _baseDevice.addFloat(rotation, value);
}

bool BtHomeV2Device::addSpeedMs(float value)
{
    return _baseDevice.addFloat(speed, value);
}

bool BtHomeV2Device::addTvocUgm3(uint16_t value)
{
    return _baseDevice.addUnsignedInteger(tvoc, value);
}

bool BtHomeV2Device::addVoltage_0_to_6550_resolution_0_1(float value)
{
    return _baseDevice.addFloat(voltage_0_1, value);
}

bool BtHomeV2Device::addVoltage_0_to_65_resolution_0_001(float value)
{
    return _baseDevice.addFloat(voltage_0_001, value);
}

bool BtHomeV2Device::addVolumeLitres_0_to_6555_resolution_0_1(float value)
{
    return _baseDevice.addFloat(volume_uint16_scale_0_1, value);
}

bool BtHomeV2Device::addVolumeLitres_0_to_65550_resolution_1(uint16_t value)
{
    return _baseDevice.addUnsignedInteger(volume_uint16_scale_1, value);
}

bool BtHomeV2Device::addVolumeLitres_0_to_4294967_resolution_0_001(float value)
{
    return _baseDevice.addFloat(volume_uint32, value);
}

bool BtHomeV2Device::addVolumeStorageLitres(float value)
{
    return _baseDevice.addFloat(volume_storage, value);
}

bool BtHomeV2Device::addVolumeFlowRateM3hr(float value)
{
    return _baseDevice.addFloat(volume_flow_rate, value);
}

bool BtHomeV2Device::addUvIndex(float value)
{
    return _baseDevice.addFloat(UV_index, value);
}

bool BtHomeV2Device::addWaterLitres(float value)
{
    return _baseDevice.addFloat(water_litre, value);
}
