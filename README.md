# BTHomeV2-Arduino
[![arduino-library-badge](https://www.ardu-badge.com/badge/BTHomeV2-Arduino.svg?)](https://www.ardu-badge.com/BTHomeV2-Arduino)
## BTHome is an open standard for broadcasting sensor data and button presses over Bluetooth LE 

[<img src="./assets/bthome-logo.png" alt="BTHome Logo" height="100"/>](https://bthome.io/)
[<img src="./assets/ha-logo.png" alt="Home Assistant Logo" height="100" style="background-color:white;"/>](https://www.home-assistant.io/integrations/bthome/)
[<img src="./assets/esphome.svg" alt="ESP Home" height="100" style="background-color:white;"/>](https://esphome.io)

**BTHomeV2-Arduino can work with OR without ESPHome**

In fact it can work without HomeAssistant entirely if you are wanting to roll your own parser.

The goal of this library is to easily develop devices that can send out BTHome events;
More information on BTHome https://bthome.io/

Check the progress table below to see if your requested feature is available. 
Create an issue if you want to see something added.

BTHome makes it easier to add very low power sensing devices to your Home Network using standard Arduino devices such as any ESP32, or more specifically the ultra low power,  battery driven devices such as the FireBeetle 2 ESP32-C6.

![Home Assistant Display](./assets/bthomeha-display.png)

## Why use BTHome? 

Power usage is the main consideration.

A BLE Advertisement, which is used by BTHome:
- Works on any BLE capable board 
- requires no connection.
- is fast to send, only taking a few milliseconds. 
- the wake, sense, send, sleep cycle can be less than a second
- can be debugged using a BLE scanner on your phone

Wifi is
- slow to connect, 
- requires handshakes and IP resolutions
- uses MUCH more power 

Zigbee / ZWave is also an alternative
- Very cool technologies
- Slightly heavier power usage than BLE 
- Requires being connected to a network
- More suitable for "always on" deployments
- Natively works on ESP32 C6 and H2 boards
- [Examples and libraries](https://github.com/espressif/arduino-esp32/tree/master/libraries/Zigbee)
- Can't debug using a phone

## Supported Platforms

### Recommended boards

Seeed and DFRobot both make very good low power boards such as the DFRobot FireBeetle2. 

Any purpose built low power boards should work but AVOID cheap clones. Cheap boards often use linear power regulators which eat batteries.  

### Home Assistant + esphome

#### BT Proxy for ESPHome

Connection through ESPHome requires the use of a permanently powered on ESP32 as a Bluetooth BLE Proxy. I recommend using the *nodemcu32* for this as it *just works!*
Get the `ESP32-WROOM-32U` with the external antenna for the best range.
There is an example config for the BT proxy in the examples folder.

After installing the [BTHome](https://www.home-assistant.io/integrations/bthome/) plugin, the devices should start showing up as discovered devices.

### Home Assistant Only

Similar to above, but instead of ESPHome, install a Bluetooth plugin for Home Assistant.
After installing the [BTHome](https://www.home-assistant.io/integrations/bthome/) plugin, the devices should start showing up as discovered devices.

## Development

### Arduino IDE

Arduino IDE is supported by this library, and has the best board support for Arduino. 

### VSCode + PlatformIO

This code works with PlatformIO too, but it's secondary to the Arduino IDE. 
The reason is that the low power boards like the *Firebeetle 2 ESP32 C6* are not supported by PlatformIO (at time of writing).

## Example code and config

Refer to the directory `./examples`  for specific library implementations.

- ESPHome BLE proxy configuration [ble-proxy.yaml](./examples/esphome/ble-proxy.yaml)
- DFRobot ESP32 Firebeetle 2 C6 + DHT11 Temperature and Humidity using ArduinoBLE  [FireBeetle2C6.ino](./examples/FireBeetle2C6/FireBeetle2C6.ino)
- NimBLE BLE Library  [NimBLE.ino](./examples/NimBLE/NimBLE.ino)
- ESP32 native IDF calls to use BLE 5 for long range and extended payload. [ESP32C6_ESP_BLE5_LongRange_Firebeetle2.ino](./examples/ESP32C6_ESP_BLE5_LongRange_Firebeetle2/ESP32C6_ESP_BLE5_LongRange_Firebeetle2.ino)
- Encryption [NimBLE_Encryption.ino](./examples/NimBLE_Encryption/NimBLE_Encryption.ino)
- XIAO MG24 + BME280 temperature/pressure/humidity sensor using BLE Silabs protocol stack [XIAO_MG24.ino](./examples/XIAO_MG24/XIAO_MG24.ino)

## Usage

This provides a non encrypted client that can send non encrypted data. 
[Encryption is also possible](./examples/NimBLE_Encryption/NimBLE_Encryption.ino)

```cpp
  // Short or long name sent based on packet size
  BtHomeV2Device btHome("short_name", "My longer device name", false);

  // Add humidity
  btHome.addHumidityPercent_Resolution_0_01(50.55f);

  // Add Temperature
  btHome.addTemperature_neg127_to_127_Resolution_1(-22.0f);

  // Set Battery State
  btHome.setBatteryState(BATTERY_STATE_LOW);

  // Set battery percentage 
  btHome.addBatteryPercentage(22);

  uint8_t advertisementData[MAX_ADVERTISEMENT_SIZE];
  size_t size = btHome.getAdvertisementData(advertisementData);


  // Set the advertisement data on BLE
  sendAdvertisement(advertisementData, size); // <- look in the example files for this method

```

## Troubleshooting 

If you are using the [FireBeetle2 ESP-C6](https://wiki.dfrobot.com/SKU_DFR0975_FireBeetle_2_Board_ESP32_C6), or any other `ESP32-C*`, you may need to enable USB CDC to see the Serial output. 

<img src="./assets/CDC-Enabled.png" alt="Arduino IDE CDC Enable" height="100"/>

# Credits 
Thanks to @Chreece and @TheDigital1 + other contributors for providing a good base to work from.

- https://github.com/Chreece/BTHomeV2-ESP32-example
- https://github.com/TheDigital1/ESP32_BTHome

Thanks to @Bluetooth-Devices for the docs

- https://github.com/Bluetooth-Devices


## Implementation Progress

### Encryption is supported!

Take a look at [./examples/NimBLE_Encryption/NimBLE_Encryption.ino](./examples/NimBLE_Encryption/NimBLE_Encryption.ino)

### Data Types 

Quite a few things to implement, but hopefully will get through them all.

| Type   | Object id | Property              | Progress | Notes                                                                              |
| ------ | --------- | --------------------- | -------- | ---------------------------------------------------------------------------------- |
| Sensor | 0x51      | acceleration          | Done     |                                                                                    |
| Sensor | 0x01      | battery               | Done     |                                                                                    |
| Sensor | 0x60      | channel               | Done     |                                                                                    |
| Sensor | 0x12      | co2                   | Done     |                                                                                    |
| Sensor | 0x56      | conductivity          | Done     |                                                                                    |
| Sensor | 0x09      | count                 | Done     |                                                                                    |
| Sensor | 0x3D      | count                 | Done     |                                                                                    |
| Sensor | 0x3E      | count                 | Done     |                                                                                    |
| Sensor | 0x59      | count                 | Done     | Not showing in HA                                                                  |
| Sensor | 0x5A      | count                 | Done     | Not showing in HA                                                                  |
| Sensor | 0x5B      | count                 | Done     | Not showing in HA                                                                  |
| Sensor | 0x43      | current               | Done     |                                                                                    |
| Sensor | 0x5D      | current               | Done     |                                                                                    |
| Sensor | 0x08      | dewpoint              | Done     |                                                                                    |
| Sensor | 0x5E      | direction             | Done     |                                                                                    |
| Sensor | 0x40      | distance (mm)         | Done     | Showing as metres in HA https://github.com/Bluetooth-Devices/bthome-ble/issues/247 |
| Sensor | 0x41      | distance (m)          | Done     |                                                                                    |
| Sensor | 0x42      | duration              | Done     |                                                                                    |
| Sensor | 0x4D      | energy                | Done     |                                                                                    |
| Sensor | 0x0A      | energy                | Done     |                                                                                    |
| Sensor | 0x4B      | gas                   | Done     |                                                                                    |
| Sensor | 0x4C      | gas                   | Done     |                                                                                    |
| Sensor | 0x52      | gyroscope             | Done     |                                                                                    |
| Sensor | 0x03      | humidity              | Done     |                                                                                    |
| Sensor | 0x2E      | humidity              | Done     |                                                                                    |
| Sensor | 0x05      | illuminance           | Done     |                                                                                    |
| Sensor | 0x06      | mass (kg)             | Done     |                                                                                    |
| Sensor | 0x07      | mass (lb)             | Done     |                                                                                    |
| Sensor | 0x14      | moisture              | Done     |                                                                                    |
| Sensor | 0x2F      | moisture              | Done     |                                                                                    |
| Sensor | 0x0D      | pm2.5                 | Done     |                                                                                    |
| Sensor | 0x0E      | pm10                  | Done     |                                                                                    |
| Sensor | 0x0B      | power                 | Done     |                                                                                    |
| Sensor | 0x5C      | power                 | Done     |                                                                                    |
| Sensor | 0x5F      | precipitation         | Done     |                                                                                    |
| Sensor | 0x04      | pressure              | Done     |                                                                                    |
| Sensor | 0x54      | raw                   | Done     |                                                                                    |
| Sensor | 0x3F      | rotation              | Done     |                                                                                    |
| Sensor | 0x44      | speed                 | Done     |                                                                                    |
| Sensor | 0x57      | temperature           | Done     |                                                                                    |
| Sensor | 0x58      | temperature           | Done     |                                                                                    |
| Sensor | 0x45      | temperature           | Done     |                                                                                    |
| Sensor | 0x02      | temperature           | Done     |                                                                                    |
| Sensor | 0x53      | text                  | Done     |                                                                                    |
| Sensor | 0x50      | timestamp             | Done     | Uses the epoch seconds. Use your own library for time management.                  |
| Sensor | 0x13      | tvoc                  | Done     |                                                                                    |
| Sensor | 0x0C      | voltage               | Done     |                                                                                    |
| Sensor | 0x4A      | voltage               | Done     |                                                                                    |
| Sensor | 0x4E      | volume                | Done     |                                                                                    |
| Sensor | 0x47      | volume                | Done     |                                                                                    |
| Sensor | 0x48      | volume                | Done     |                                                                                    |
| Sensor | 0x55      | volume storage        | Done     |                                                                                    |
| Sensor | 0x49      | volume flow rate      | Done     |                                                                                    |
| Sensor | 0x46      | UV index              | Done     |                                                                                    |
| Sensor | 0x4F      | water                 | Done     |                                                                                    |
|        |           |                       |          |                                                                                    |
| binary | 0x15      | battery  (normal/low) | Done     |                                                                                    |
| binary | 0x16      | battery charging      | Done     |                                                                                    |
| binary | 0x17      | carbon monoxide       | Done     |                                                                                    |
| binary | 0x18      | cold                  | Done     |                                                                                    |
| binary | 0x19      | connectivity          | Done     |                                                                                    |
| binary | 0x1A      | door                  | Done     |                                                                                    |
| binary | 0x1B      | garage door           | Done     |                                                                                    |
| binary | 0x1C      | gas                   | Done     |                                                                                    |
| binary | 0x0F      | generic boolean       | Done     |                                                                                    |
| binary | 0x1D      | heat                  | Done     |                                                                                    |
| binary | 0x1E      | light                 | Done     |                                                                                    |
| binary | 0x1F      | lock                  | Done     |                                                                                    |
| binary | 0x20      | moisture              | Done     |                                                                                    |
| binary | 0x21      | motion                | Done     |                                                                                    |
| binary | 0x22      | moving                | Done     |                                                                                    |
| binary | 0x23      | occupancy             | Done     |                                                                                    |
| binary | 0x11      | opening               | Done     |                                                                                    |
| binary | 0x24      | plug                  | Done     |                                                                                    |
| binary | 0x10      | power                 | Done     |                                                                                    |
| binary | 0x25      | presence              | Done     |                                                                                    |
| binary | 0x26      | problem               | Done     |                                                                                    |
| binary | 0x27      | running               | Done     |                                                                                    |
| binary | 0x28      | safety                | Done     |                                                                                    |
| binary | 0x29      | smoke                 | Done     |                                                                                    |
| binary | 0x2A      | sound                 | Done     |                                                                                    |
| binary | 0x2B      | tamper                | Done     |                                                                                    |
| binary | 0x2C      | vibration             | Done     |                                                                                    |
| binary | 0x2D      | window                | Done     |                                                                                    |
|        |           |                       |          |                                                                                    |
| button | 0x00      | None                  | Done     | Button state must transition from None to another State to register                |
| button | 0x01      | press                 | Done     |                                                                                    |
| button | 0x02      | double_press          | Done     |                                                                                    |
| button | 0x03      | triple_press          | Done     |                                                                                    |
| button | 0x04      | long_press            | Done     |                                                                                    |
| button | 0x05      | long_double_press     | Done     |                                                                                    |
| button | 0x06      | long_triple_press     | Done     |                                                                                    |
| button | 0x80      | hold_press            | Done     |                                                                                    |
|        |           |                       |          |                                                                                    |
| dimmer | 0x00      | None                  | Done     | Dimmer state must transition from None to another state to register                |
| dimmer | 0x01      | rotate left           | Done     |                                                                                    |
| dimmer | 0x02      | rotate right          | Done     |                                                                                    |
|        |           |                       |          |                                                                                    |
| misc   | 0x00      | packet id             | Done     | credit [@the78mole](https://github.com/the78mole)                                  |
| device | 0xF0      | device type id        | Done     | credit [@the78mole](https://github.com/the78mole)                                  |
| device | 0xF1      | firmware version      | Done     | credit [@the78mole](https://github.com/the78mole)                                  |
| device | 0xF2      | firmware version      | Done     | credit [@the78mole](https://github.com/the78mole)                                  |

