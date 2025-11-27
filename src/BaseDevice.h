#ifndef BT_HOME_BASE_DEVICE_H
#define BT_HOME_BASE_DEVICE_H

#include "definitions.h"
#ifndef __ZEPHYR__
#include <Arduino.h>
#endif
#include "data_types.h"
#ifndef __ZEPHYR__
#include "mbedtls/ccm.h"
#endif

static const size_t MAX_ADVERTISEMENT_SIZE = 31;
static const size_t HEADER_SIZE = 9;
static const size_t MAX_MEASUREMENT_SIZE = MAX_ADVERTISEMENT_SIZE - HEADER_SIZE;
static const size_t TYPE_INDICATOR_SIZE = 1;
static const size_t NULL_TERMINATOR_SIZE = 1;
static const size_t ENCRYPTION_KEY_LENGTH = 16;
static const size_t BLE_MAC_ADDRESS_LENGTH = 6;
static const size_t NONCE_LEN = 13;
static const size_t MIC_LEN = 4;

#define BIND_KEY_LEN 16
#define ENCRYPTION_ADDITIONAL_BYTES 12

class BaseDevice
{
public:
  BaseDevice(const char *shortName, const char *completeName, bool isTriggerBased, uint8_t const *const key, const uint8_t macAddress[BLE_MAC_ADDRESS_LENGTH], uint32_t counter);
  BaseDevice(const char *shortName, const char *completeName, bool isTriggerBased);
  size_t getAdvertisementData(uint8_t buffer[MAX_ADVERTISEMENT_SIZE]);
  void resetMeasurement();
  bool addState(BtHomeState, uint8_t state);
  bool addState(BtHomeState sensor, uint8_t state, uint8_t steps);
  bool addUnsignedInteger(BtHomeType sensor, uint64_t value);
  bool addSignedInteger(BtHomeType sensor, int64_t value);
  bool addFloat(BtHomeType sensor, float value);
  bool addRaw(uint8_t sensor, uint8_t *value, uint8_t size);
  uint8_t remainingBytes();

private:
  bool pushBytes(uint64_t value2, BtHomeState sensor);
  uint8_t _sensorDataIdx = 0;
  std::vector<std::vector<uint8_t>> _sensorData;
  char _shortName[MAX_LENGTH_SHORT_NAME + NULL_TERMINATOR_SIZE];
  char _completeName[MAX_LENGTH_COMPLETE_NAME + NULL_TERMINATOR_SIZE];
  bool hasEnoughSpace(BtHomeState sensor);
  bool hasEnoughSpace(uint8_t size);
  template <typename T>
  bool addInteger(BtHomeType sensor, T value);
  bool _triggerDevice = false;
  bool _useEncryption = false;
  uint32_t _counter = 1;
#ifndef __ZEPHYR__
  mbedtls_ccm_context _encryptCTX;
#endif  
  uint8_t _macAddress[BLE_MAC_ADDRESS_LENGTH];
  uint8_t bindKey[BIND_KEY_LEN];
  size_t getMeasurementByteArray(uint8_t sortedBytes[MAX_ADVERTISEMENT_SIZE]);
};

#endif // BT_HOME_BASE_DEVICE_H