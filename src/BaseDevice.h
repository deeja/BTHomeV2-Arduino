#include "definitions.h"
#include <Arduino.h>
#include <data_types.h>
static const size_t MAX_ADVERTISEMENT_SIZE = 31;
static const size_t HEADER_SIZE = 9;
static const size_t MAX_MEASUREMENT_SIZE = MAX_ADVERTISEMENT_SIZE - HEADER_SIZE;
static const size_t TYPE_INDICATOR_SIZE = 1;
static const size_t NULL_TERMINATOR_SIZE = 1;

class BaseDevice
{
public:
  BaseDevice(const char *shortName, const char *completeName, bool isTriggerBased);
  size_t getAdvertisementData(uint8_t buffer[MAX_ADVERTISEMENT_SIZE]);
  void resetMeasurement();
  bool addState(BtHomeState, uint8_t state);
  bool addState(BtHomeState sensor, uint8_t state, uint8_t steps);
  bool addUnsignedInteger(BtHomeType sensor, uint64_t value);
  bool addSignedInteger(BtHomeType sensor, int64_t value);
  bool addFloat(BtHomeType sensor, float value);
  bool addRaw(uint8_t sensor, uint8_t *value, uint8_t size);

private:
  bool pushBytes(uint64_t value2, BtHomeType sensor);
  uint8_t _sensorDataIdx = 0;
  byte _sensorData[MAX_MEASUREMENT_SIZE];
  char _shortName[MAX_LENGTH_SHORT_NAME + NULL_TERMINATOR_SIZE];
  char _completeName[MAX_LENGTH_COMPLETE_NAME + NULL_TERMINATOR_SIZE];
  bool hasEnoughSpace(BtHomeState sensor);
  bool hasEnoughSpace(uint8_t size);
  template <typename T>
  bool addInteger(BtHomeType sensor, T value);
  bool _triggerDevice;
};
