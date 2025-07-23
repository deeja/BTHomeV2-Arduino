#ifndef BT_HOME_DATA_TYPES_H
#define BT_HOME_DATA_TYPES_H

#pragma once
#include <Arduino.h>
#include <vector>

// You can use struct inheritance in C++ to inherit properties.
// For example, BtHomeType can inherit from BtHomeState if you want to reuse the 'id' field:

struct BtHomeState
{
    uint8_t id;
    uint8_t byteCount;
};

struct BtHomeType : public BtHomeState
{
    float scale;       // Multiplier to apply before serializing
    bool signed_value; // true if value is signed, false if unsigned

    BtHomeType(uint8_t id, float scale, uint8_t byteCount, bool signed_value)
    {
        this->id = id;
        this->scale = scale;
        this->byteCount = byteCount;
        this->signed_value = signed_value;
    }
};

// Now BtHomeType has 'id' from BtHomeState, plus its own fields.
const BtHomeType device_type_ID = {0xF0, 1.0f, 2, false};
const BtHomeType firmware_3_bytes = {0xF2, 1.0f, 3, false};
const BtHomeType firmware_4_bytes = {0xF1, 1.0f, 4, false};
const BtHomeType packet_id = {0x00, 1.0f, 1, false};

const BtHomeType temperature_int8 = {0x57, 1.0f, 1, true};
const BtHomeType temperature_int8_scale_0_35 = {0x58, 0.35f, 1, true};
const BtHomeType temperature_int16_scale_0_1 = {0x45, 0.1f, 2, true};
const BtHomeType temperature_int16_scale_0_01 = {0x02, 0.01f, 2, true};

const BtHomeType count_uint8 = {0x09, 1.0f, 1, false};
const BtHomeType count_uint16 = {0x3D, 1.0f, 2, false};
const BtHomeType count_uint32 = {0x3E, 1.0f, 4, true};
const BtHomeType count_int8 = {0x59, 1.0f, 1, true};
const BtHomeType count_int16 = {0x5A, 1.0f, 2, true};
const BtHomeType count_int32 = {0x5B, 1.0f, 4, true};

const BtHomeType voltage_0_001 = {0x0C, 0.001f, 2, false};
const BtHomeType voltage_0_1 = {0x4A, 0.1f, 2, false};

const BtHomeType battery_percentage = {0x01, 1.0f, 1, false};

const BtHomeType distance_millimetre = {0x40, 1.0f, 2, false};
const BtHomeType distance_metre = {0x41, 0.1f, 2, false};

const BtHomeType acceleration = {0x51, 0.001f, 2, false};
const BtHomeType channel = {0x60, 1.0f, 1, false};
const BtHomeType co2 = {0x12, 1.0f, 2, false};
const BtHomeType conductivity = {0x56, 1.0f, 2, false};

const BtHomeType current_uint16 = {0x43, 0.001f, 2, false};
const BtHomeType current_int16 = {0x5D, 0.001f, 2, true};
const BtHomeType dewpoint = {0x08, 0.01f, 2, true};
const BtHomeType direction = {0x5E, 0.01f, 2, false};
const BtHomeType duration_uint24 = {0x42, 0.001f, 3, false};
const BtHomeType energy_uint32 = {0x4D, 0.001f, 4, true};
const BtHomeType energy_uint24 = {0x0A, 0.001f, 3, false};
const BtHomeType gas_uint24 = {0x4B, 0.001f, 3, false};
const BtHomeType gas_uint32 = {0x4C, 0.001f, 4, true};
const BtHomeType gyroscope = {0x52, 0.001f, 2, false};
const BtHomeType humidity_uint16 = {0x03, 0.01f, 2, false};
const BtHomeType humidity_uint8 = {0x2E, 1.0f, 1, false};
const BtHomeType illuminance = {0x05, 0.01f, 3, false};
const BtHomeType mass_kg = {0x06, 0.01f, 2, false};
const BtHomeType mass_lb = {0x07, 0.01f, 2, false};
const BtHomeType moisture_uint16 = {0x14, 0.01f, 2, false};
const BtHomeType moisture_uint8 = {0x2F, 1.0f, 1, false};
const BtHomeType pm2_5 = {0x0D, 1.0f, 2, false};
const BtHomeType pm10 = {0x0E, 1.0f, 2, false};
const BtHomeType power_uint24 = {0x0B, 0.01f, 3, false};
const BtHomeType power_int32 = {0x5C, 0.01f, 4, true};
;
const BtHomeType precipitation = {0x5F, 0.1f, 2, false};
const BtHomeType pressure = {0x04, 0.01f, 3, false};
const BtHomeType rotation = {0x3F, 0.1f, 2, true};
const BtHomeType speed = {0x44, 0.01f, 2, false};
const BtHomeType timestamp = {0x50, 1.0f, 4, false};
const BtHomeType tvoc = {0x13, 1.0f, 2, false};

const BtHomeType volume_uint32 = {0x4E, 0.001f, 4, true};
const BtHomeType volume_uint16_scale_0_1 = {0x47, 0.1f, 2, false};
const BtHomeType volume_uint16_scale_1 = {0x48, 1.0f, 2, false};
const BtHomeType volume_storage = {0x55, 0.001f, 4, true};
const BtHomeType volume_flow_rate = {0x49, 0.001f, 2, false};
const BtHomeType UV_index = {0x46, 0.1f, 1, false};
const BtHomeType water_litre = {0x4F, 0.001f, 4, true};
const BtHomeType time_type = {0x50, 1.0f, 4, false};

// raw (0x54)  require custom serialization

const BtHomeState battery_state = {0x15, 1}; // Battery state, 1 byte, 0 = normal, 1 = low
const BtHomeState battery_charging = {0x16, 1};
const BtHomeState carbon_monoxide = {0x17, 1};
const BtHomeState cold = {0x18, 1};
const BtHomeState connectivity = {0x19, 1};
const BtHomeState door = {0x1A, 1};
const BtHomeState garage_door = {0x1B, 1};
const BtHomeState gas = {0x1C, 1};
const BtHomeState generic_boolean = {0x0F, 1};
const BtHomeState heat = {0x1D, 1};
const BtHomeState light = {0x1E, 1};
const BtHomeState lock = {0x1F, 1};
const BtHomeState moisture = {0x20, 1};
const BtHomeState motion = {0x21, 1};
const BtHomeState moving = {0x22, 1};
const BtHomeState occupancy = {0x23, 1};
const BtHomeState opening = {0x11, 1};
const BtHomeState plug = {0x24, 1};
const BtHomeState power = {0x10, 1};
const BtHomeState presence = {0x25, 1};
const BtHomeState problem = {0x26, 1};
const BtHomeState running = {0x27, 1};
const BtHomeState safety = {0x28, 1};
const BtHomeState smoke = {0x29, 1};
const BtHomeState sound = {0x2A, 1};
const BtHomeState tamper = {0x2B, 1};
const BtHomeState vibration = {0x2C, 1};
const BtHomeState window = {0x2D, 1};

const BtHomeState button = {0x3A, 1};
const BtHomeState dimmer = {0x3C, 2}; // Dimmer = state + steps

enum Button_Event_Status
{
    Button_Event_Status_None = 0x00,
    Button_Event_Status_Press = 0x01,
    Button_Event_Status_Double_Press = 0x02,
    Button_Event_Status_Triple_Press = 0x03,
    Button_Event_Status_Long_Press = 0x04,
    Button_Event_Status_Long_Double_Press = 0x05,
    Button_Event_Status_Long_Triple_Press = 0x06,
    Button_Event_Status_Hold_Press = 0x8
};

enum Dimmer_Event_Status
{
    Dimmer_Event_Status_None = 0x00,
    Dimmer_Event_Status_RotateLeft = 0x01,
    Dimmer_Event_Status_RotateRight = 0x02
};

enum Battery_Sensor_Status
{
    Battery_Sensor_Status_Normal = 0,
    Battery_Sensor_Status_Low = 1
};

enum Battery_Charging_Sensor_Status
{
    Battery_Charging_Sensor_Status_Not_Charging = 0,
    Battery_Charging_Sensor_Status_Charging = 1
};

enum Carbon_Monoxide_Sensor_Status
{
    Carbon_Monoxide_Sensor_Status_Not_Detected = 0,
    Carbon_Monoxide_Sensor_Status_Detected = 1
};

enum Cold_Sensor_Status
{
    Cold_Sensor_Status_Normal = 0,
    Cold_Sensor_Status_Cold = 1
};

enum Connectivity_Sensor_Status
{
    Connectivity_Sensor_Status_Disconnected = 0,
    Connectivity_Sensor_Status_Connected = 1
};

enum Door_Sensor_Status
{
    Door_Sensor_Status_Closed = 0,
    Door_Sensor_Status_Open = 1
};

enum Garage_Door_Sensor_Status
{
    Garage_Door_Sensor_Status_Closed = 0,
    Garage_Door_Sensor_Status_Open = 1
};

enum Gas_Sensor_Status
{
    Gas_Sensor_Status_Clear = 0,
    Gas_Sensor_Status_Detected = 1
};

enum Generic_Sensor_Status
{
    Generic_Sensor_Status_Off = 0,
    Generic_Sensor_Status_On = 1
};

enum Heat_Sensor_Status
{
    Heat_Sensor_Status_Normal = 0,
    Heat_Sensor_Status_Hot = 1
};

enum Light_Sensor_Status
{
    Light_Sensor_Status_No_Light = 0,
    Light_Sensor_Status_Detected = 1
};

enum Lock_Sensor_Status
{
    Lock_Sensor_Status_Locked = 0,
    Lock_Sensor_Status_Unlocked = 1
};

enum Moisture_Sensor_Status
{
    Moisture_Sensor_Status_Dry = 0,
    Moisture_Sensor_Status_Wet = 1
};

enum Motion_Sensor_Status
{
    Motion_Sensor_Status_Clear = 0,
    Motion_Sensor_Status_Detected = 1
};

enum Moving_Sensor_Status
{
    Moving_Sensor_Status_Not_Moving = 0,
    Moving_Sensor_Status_Moving = 1
};

enum Occupancy_Sensor_Status
{
    Occupancy_Sensor_Status_Clear = 0,
    Occupancy_Sensor_Status_Detected = 1
};

enum Opening_Sensor_Status
{
    Opening_Sensor_Status_Closed = 0,
    Opening_Sensor_Status_Open = 1
};

enum Plug_Sensor_Status
{
    Plug_Sensor_Status_Unplugged = 0,
    Plug_Sensor_Status_Plugged_In = 1
};

enum Power_Sensor_Status
{
    Power_Sensor_Status_Off = 0,
    Power_Sensor_Status_On = 1
};

enum Presence_Sensor_Status
{
    Presence_Sensor_Status_Away = 0,
    Presence_Sensor_Status_Home = 1
};

enum Problem_Sensor_Status
{
    Problem_Sensor_Status_OK = 0,
    Problem_Sensor_Status_Problem = 1
};

enum Running_Sensor_Status
{
    Running_Sensor_Status_Not_Running = 0,
    Running_Sensor_Status_Running = 1
};

enum Safety_Sensor_Status
{
    Safety_Sensor_Status_Unsafe = 0,
    Safety_Sensor_Status_Safe = 1
};

enum Smoke_Sensor_Status
{
    Smoke_Sensor_Status_Clear = 0,
    Smoke_Sensor_Status_Detected = 1
};

enum Sound_Sensor_Status
{
    Sound_Sensor_Status_Clear = 0,
    Sound_Sensor_Status_Detected = 1
};

enum Tamper_Sensor_Status
{
    Tamper_Sensor_Status_Off = 0,
    Tamper_Sensor_Status_On = 1
};

enum Vibration_Sensor_Status
{
    Vibration_Sensor_Status_Clear = 0,
    Vibration_Sensor_Status_Detected = 1
};

enum Window_Sensor_Status
{
    Window_Sensor_Status_Closed = 0,
    Window_Sensor_Status_Open = 1
};

#endif // BT_HOME_DATA_TYPES_H
