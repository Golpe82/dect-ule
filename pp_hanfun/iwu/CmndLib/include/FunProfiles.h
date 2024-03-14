/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _FUN_PROFILES_H
#define _FUN_PROFILES_H

#include "TypeDefs.h"

#ifdef _MSC_VER
#pragma pack(1) // Enable struct packing till next #pragma pack()
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// List of Unit types
///////////////////////////////////////////////////////////


#define FUN_UNIT_TYPE_MANAGEMENT                                 0x0000
#define FUN_UNIT_TYPE_MANAGEMENT_ID                              0x0000

#define HAN_PAGING_CAPABILITIES_PAGING                           0x01

typedef enum {

    // Home Control Units
    FUN_UNIT_TYPE_SIMPLE_ONOFF_SWITCHABLE                   = 0x0100,   // Simple On/Off Switchable unit or appliance
    FUN_UNIT_TYPE_SIMPLE_ONOFF_SWITCH                       = 0x0101,   // On Off Switch for connecting appliances
    FUN_UNIT_TYPE_SIMPLE_LEVEL_CONTROLLABLE                 = 0x0102,   // Simple Level Controllable without implementing on/off
    FUN_UNIT_TYPE_SIMPLE_LEVEL_CONTROL                      = 0x0103,   // Simple Level Control without implementing on/off
    FUN_UNIT_TYPE_SIMPLE_LEVEL_CONTROLABLE_SWITCHABLE       = 0x0104,   // Simple Level Controllable Switchable with on/off implementation
    FUN_UNIT_TYPE_SIMPLE_LEVEL_CONTROL_SWITCH               = 0x0105,   // Simple Level Control switch with on/off implementation
    FUN_UNIT_TYPE_AC_OUTLET                                 = 0x0106,   // On/Off AC Outlet for connecting appliances
    FUN_UNIT_TYPE_AC_OUTLET_WITH_POWER_METERING             = 0x0107,   // AC Outlet with Power Metering
    FUN_UNIT_TYPE_SIMPLE_LIGHT                              = 0x0108,   // On/Off light for connecting appliances
    FUN_UNIT_TYPE_DIMMABLE_LIGHT                            = 0x0109,   // Allows a unit to receive and interpret both on/off and level control requests
    FUN_UNIT_TYPE_DIMMER_SWITCH                             = 0x010A,   // Allows a unit to send both on/off and level control requests
    FUN_UNIT_TYPE_SIMPLE_DOOR_LOCK                          = 0x010B,   // Allows a unit to receive and interpret on (lock)/off (unlock) requests
    FUN_UNIT_TYPE_DOOR_BELL                                 = 0x010C,
    FUN_UNIT_TYPE_SIMPLE_POWER_METER                        = 0x010D,   // Allows a unit to do and provide measurements over electric quantities
    FUN_UNIT_TYPE_SIMPLE_BUTTON                             = 0x0111,   // Allows a unit to identify several types of button presses and send notifications when they occur
    FUN_UNIT_TYPE_CONTROLLABLE_THERMOSTAT                   = 0x0112,   // Allows a unit to control the temperature of an indoor area, either by cooling or heating it
    FUN_UNIT_TYPE_SIMPLE_VISUAL_CONTROL                     = 0x0113,   // Allows a unit to be able to make some visual effects with a led

    FUN_UNIT_TYPE_COLOR_BULB                                = 0x0115,   // Unit Type for Color Bulb
    FUN_UNIT_TYPE_DIMMABLE_COLOR_BULB                       = 0x0116,   // Unit Type for Dimmable Color Bulb

    // Security Units
    FUN_UNIT_TYPE_SIMPLE_DETECTOR                           = 0x0200,   // Generic Detection Sensor
    FUN_UNIT_TYPE_DOOR_OPEN_CLOSE_DETECTOR                  = 0x0201,   // Open / close door detector
    FUN_UNIT_TYPE_WINDOW_OPEN_CLOSE_DETECTOR                = 0x0202,   // Open/close window detector
    FUN_UNIT_TYPE_MOTION_DETECTOR                           = 0x0203,   // Motion Detection Sensor
    FUN_UNIT_TYPE_SMOKE_DETECTOR                            = 0x0204,   // Smoke Sensor
    FUN_UNIT_TYPE_GAS_DETECTOR                              = 0x0205,   // carbon monoxide detector
    FUN_UNIT_TYPE_FLOOD_DETECTOR                            = 0x0206,   // Flood detector
    FUN_UNIT_TYPE_GLASS_BREAK_DETECTOR                      = 0x0207,   // Glass Break detector
    FUN_UNIT_TYPE_VIBRATION_DETECTOR                        = 0x0208,   // Vibration Sensor

    FUN_UNIT_TYPE_SIREN                                     = 0x0280,   // Siren
    FUN_UNIT_TYPE_ALERTABLE                                 = 0x0281,   // Alertable

    // Homecare Units
    FUN_UNIT_TYPE_SIMPLE_PENDANT                            = 0x0300,   // Simple Panic button

    // Miscellaneous Units
    FUN_UNIT_TYPE_USER_INTERFACE                            = 0x0410,   // User control unit (e.g. keypad)
    FUN_UNIT_TYPE_GENERIC_APPLICATION_LOGIC                 = 0x0411,   // Application logic unit, (e.g. can be at the concentrator, Portal, user controller)


    // Proprietary Unit Types
    FUN_UNIT_TYPE_PROPRIETARY_START_OFFSET                  = 0xFF00,   // starting offset of proprietary unit type range
    FUN_UNIT_TYPE_PROPRIETARY_END_OFFSET                    = 0xFFFF,   // end offset of a proprietary unit type range

    FUN_UNIT_TYPE_UNKNOWN                                   = 0xFFFF,
}
t_en_hanFunUnitType;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// List of Interface types
///////////////////////////////////////////////////////////

typedef enum
{
    // Service interfaces
    FUN_INTERFACE_DEVICE_MANAGEMENT         = 0x0001,   // Device Management (Add, View)
    FUN_INTERFACE_BIND_MANAGEMENT           = 0x0002,   // Bind Management (Add, Remove, etc.)
    FUN_INTERFACE_GROUP_MANAGEMENT          = 0x0003,   // Group Management (Add, Remove, View, etc.)
    FUN_INTERFACE_IDENTIFY                  = 0x0004,   // Identify Unit/Group in system
    FUN_INTERFACE_DEVICE_INFORMATION        = 0x0005,   // Device attributes (hw version, sw version, emc, etc.)
    FUN_INTERFACE_ATTRIBUTE_REPORTING       = 0x0006,   // Attributes value reporting
    FUN_INTERFACE_TAMPER_ALERT              = 0x0101,   // When device wants to indicate a Tamper alert
    FUN_INTERFACE_TIME                      = 0x0102,   // Service that allows a device to maintain time referenced to UTC
    FUN_INTERFACE_POWER                     = 0x0110,   // Service that allows a device to provide information about the power supplies it has and their characteristics
    FUN_INTERFACE_RSSI                      = 0x0111,   // Use when device wants to indicate the RSSI
    FUN_INTERFACE_KEEP_ALIVE                = 0x0115,   // When device wants to indicate its alive

    // Functional Interfaces
    FUN_INTERFACE_ALERT                     = 0x0100,   // When device wants to indicate an alert
    FUN_INTERFACE_ON_OFF                    = 0x0200,   // Turning device On, Off
    FUN_INTERFACE_LEVEL_CONTROL             = 0x0201,   // Use to set some device feature to a defined level
    FUN_INTERFACE_COLOR_CONTROL             = 0x0202,   // Use to set a color
    FUN_INTERFACE_SIMPLE_POWER_METERING     = 0x0300,   // Use when device requires doing or providing measurements over electric quantities
    FUN_INTERFACE_TEMPERATURE               = 0x0301,   // Simple Temperature
    FUN_INTERFACE_HUMIDITY                  = 0x0302,   // Simple Humidity
    FUN_INTERFACE_THERMOSTAT                = 0x0303,   // Simple Thermostat
    FUN_INTERFACE_SIMPLE_BUTTON             = 0x0304,   // Use this interface to receive several button press related notifications
    FUN_INTERFACE_SIMPLE_VISUAL_CONTROL     = 0x0305,   // Use this interface to control some visual effects on some visual indicator (display, LED, lamp, etc)
    FUN_INTERFACE_SUOTA                     = 0x0400,   // This service enables a device to perform Software Update over the

    FUN_INTERFACE_RESERVED_OFFSET           = 0x7F00,   // Starting index reserve   d for proprietary interfaces
}
t_en_hanFunInterfaceId;


/**
 * Interface Type Mask
 * bit 0..14 interface id
 * bit 15 interface type
 * 1 = server
 * 0 = client
 */
typedef enum
{
    FUN_INTERFACE_SERVER_MASK = 0x8000,
    FUN_INTERFACE_CLIENT_MASK = 0x0000,
}
t_en_hanFunInterfaceTypeMask;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Device Management Interface Server commands
typedef enum
{
    FUN_IF_DEV_MNG_CMD_REGISTER_DEVICE          = 0x1,
    FUN_IF_DEV_MNG_CMD_DEREGISTER_DEVICE        = 0x2,
    FUN_IF_DEV_MNG_CMD_START_SESS_READ_REG_INFO = 0x3,
    FUN_IF_DEV_MNG_CMD_END_SESS_READ_REG_INFO   = 0x4,
    FUN_IF_DEV_MNG_CMD_GET_ENTRIES              = 0x5,
}
t_en_hanIf_DevMng_Server_Cmd;

// DM interface attributes
typedef enum
{
    FUN_IF_DEV_MNG_ATTRIB_NUMBER_OF_ENTRIES     = 0x1,
}
t_en_hanIf_DevManager_AttribId;

// DM interface attributes
typedef enum
{
    FUN_IF_DEV_MNG_ATTRIB_RES_OK    = 0x0,
    FUN_IF_DEV_MNG_ATTRIB_RES_FAL   = 0x1,
}
t_en_hanIf_DevManager_AttribRes;

typedef enum
{
    FUN_IF_DEV_MNG_REG_RESP_DISCR_TYPE_NON_PROPRIETARY  = 0x0,
    FUN_IF_DEV_MNG_REG_RESP_DISCR_TYPE_PROPRIETARY      = 0x1,
}
t_en_hanIf_DevMnd_RegResp_DiscrType;


typedef enum
{
    FUN_IF_DEV_MNG_DEREGISTER_RESPONSE_OK   = 0x0,
    FUN_IF_DEV_MNG_DEREGISTER_RESPONSE_FAIL_NOT_AUTHORIZED  = 0x1,
    FUN_IF_DEV_MNG_DEREGISTER_RESPONSE_FAIL_UNKNOWN_REASON  = 0xFF,

}
t_en_hanIf_DevMng_Server_DeRegisterDeviceCmd_Response;


typedef enum
{
    FUN_IF_DEV_MNG_START_SESSION_RESPONSE_OK    = 0x0,
    FUN_IF_DEV_MNG_START_SESSION_FAIL_NOT_AUTHORIZED    = 0x1,
    FUN_IF_DEV_MNG_START_SESSION_FAIL_NO_RESOURCES      = 0xFE,
    FUN_IF_DEV_MNG_START_SESSION_FAIL_UNKNOWN_REASON    = 0xFF,

}
t_en_hanIf_DevMng_Server_StartSessionCmd_Response;


typedef enum
{
    FUN_IF_DEV_MNG_END_SESSION_RESPONSE_OK  = 0x0,
    FUN_IF_DEV_MNG_END_SESSION_FAIL_UNKNOWN_REASON  = 0xFF,

}
t_en_hanIf_DevMng_Server_EndSessionCmd_Response;
typedef enum
{
    FUN_IF_DEV_MNG_REG_RESP_UID_TYPE_NONE           = 0x0,
    FUN_IF_DEV_MNG_REG_RESP_UID_TYPE_IPUI           = 0x1,
    FUN_IF_DEV_MNG_REG_RESP_UID_TYPE_IEEE_MAC_48    = 0x2,
    FUN_IF_DEV_MNG_REG_RESP_UID_TYPE_URI            = 0x3,

}
t_en_hanIf_DevMnd_Reg_Dev_UID_Type;

#define FUN_IF_DEV_MNG_UID_LENGTH_IPUI          5
#define FUN_IF_DEV_MNG_UID_LENGTH_IEEE_MAC_48   12

////////////////////////////////////////////////////////////////////////////////
// Define bitfields for Device Managment interface
//               FieldName                                  Type    BitOffet    BitWidth
////////////////////////////////////////////////////////////////////////////////
DEFINE_BITFIELD( FUN_IF_DEV_MNG_REG_DISCRIMINATOR_TYPE,     u8,     7,          1 );
DEFINE_BITFIELD( FUN_IF_DEV_MNG_REG_DEV_UID_TYPE,           u8,     0,          7 );
DEFINE_BITFIELD( FUN_IF_DEV_MNG_REG_RES_DISCRIMINATOR_TYPE, u16,    15,         1 );
DEFINE_BITFIELD( FUN_IF_DEV_MNG_REG_RES_DEVICE_ADDRESS,     u16,    0,          15);


// Device Management Interface Server attributes
typedef enum
{
    FUN_IF_DEV_MNG_ATTRIB_NUM_OF_UNITS      = 0x1,

    FUN_IF_DEV_MNG_ATTRIB_LAST
}
t_en_hanIf_DevMng_Attrib;

// Device Management response values for FUN_IF_DEV_MNG_CMD_REGISTER_DEVICE command
typedef enum
{
    FUN_IF_DEV_MNG_REGISTER_RESPONSE_OK                     = 0x0,
    FUN_IF_DEV_MNG_REGISTER_RESPONSE_FAIL_NOT_AUTHORIZED    = 0x1,
    FUN_IF_DEV_MNG_REGISTER_RESPONSE_FAIL_NO_RESOURCES      = 0x2,
    FUN_IF_DEV_MNG_REGISTER_RESPONSE_FAIL_UNKNOWN_REASON    = 0xFF
}
t_en_hanIf_DevMng_Server_RegisterDeviceCmd_Response;

typedef enum
{
    FUN_IF_DEV_MNG_GET_DEVICE_ENTRIES_RESPONSE_OK                                                   = 0x0,
    FUN_IF_DEV_MNG_GET_DEVICE_ENTRIES_RESPONSE_FAIL_INVALID_ARGUMENT                                = 0x2,
    FUN_IF_DEV_MNG_GET_DEVICE_ENTRIES_RESPONSE_FAIL_READ_SESSION_NOT_ESTABLISHED                    = 0x20,
    FUN_IF_DEV_MNG_GET_DEVICE_ENTRIES_RESPONSE_FAIL_ENTRIES_TABLE_MODIFIED                          = 0x21,
    FUN_IF_DEV_MNG_GET_DEVICE_ENTRIES_RESPONSE_FAIL_UNKNOWN_REASON                              = 0xFF
}
t_en_hanIf_DevMng_Server_GetDeviceEntries_Response;

// TODO: define the rest of command ,attributes and response values

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Device Information Interface attributes
typedef enum
{
    FUN_IF_DEVICE_INFO_ATTRIB_HF_CORE_RELEASE_VER               = 0x1,      // version of the HF Protocol Specification and of the HF Core Services and Interfaces
    FUN_IF_DEVICE_INFO_ATTRIB_PROFILE_RELEASE_VER               = 0x2,      // version of the HF Profile Release implemented on the device
    FUN_IF_DEVICE_INFO_ATTRIB_INTERFACE_RELEASE_VER         = 0x3,      // version of the HF Interface Release implemented on the device
    FUN_IF_DEVICE_INFO_ATTRIB_PAGING_CAPS                       = 0x4,      // informs upon the paging and broadcast capabilities of the device
    FUN_IF_DEVICE_INFO_ATTRIB_MIN_SLEEP_TIME                    = 0x5,      // the minimum sleeping period a device supports
    FUN_IF_DEVICE_INFO_ATTRIB_ACTUAL_PAGING_INTERVAL            = 0x6,      // defines the actual paging interval to be used by a page able device
    FUN_IF_DEVICE_INFO_ATTRIB_APP_VER                           = 0x7,      // the software version of the application running in the device
    FUN_IF_DEVICE_INFO_ATTRIB_HW_VER                            = 0x8,      // Hardware version of the device
    FUN_IF_DEVICE_INFO_ATTRIB_EMC                               = 0x9,      // Equipment Manufacturer Code, U8 * 2
    FUN_IF_DEVICE_INFO_ATTRIB_IPUI_OR_RFPI                      = 0xA,      // U8 * 5
    FUN_IF_DEVICE_INFO_ATTRIB_MANUFACTURE_NAME              = 0xB,      // U8 + U8* 32
    FUN_IF_DEVICE_INFO_ATTRIB_LOCATION                          = 0xC,      // Text which defines the physical location of the device
    FUN_IF_DEVICE_INFO_ATTRIB_DEVICE_ENABLE                 = 0xD,      // Specifies if the device is enabled or disabled
    FUN_IF_DEVICE_INFO_ATTRIB_FRIENDLY_NAME                 = 0xE,      // a user friendly and meaningful device name
    FUN_IF_DEVICE_INFO_ATTRIB_DEVICE_UID                        = 0xF,      // Device unitque identifier
    FUN_IF_DEVICE_INFO_ATTRIB_DEVICE_SERIAL_NUMBER          = 0x10,     // Device serial number

    FUN_IF_DEVICE_INFO_ATTRIB_LAST,
}
t_en_hanIf_DeviceInfo_Attrib;

typedef enum
{
    FUN_IF_DEVICE_INFO_GET_ATTRIB_RES_OK                                = 0x0,
    FUN_IF_DEVICE_INFO_GET_ATTRIB_RES_FAIL_NOT_SUPPORTED                = 0x3,
    FUN_IF_DEVICE_INFO_GET_ATTRIB_RES_FAIL_UNKNOWN_REASON       = 0xFF,
}
t_en_hanIf_DeviceInfo_GetAttribRes;

typedef enum
{
    FUN_IF_DEVICE_INFO_GET_ATTRIB_PACK_RES_OK                               = 0x0,
    FUN_IF_DEVICE_INFO_GET_ATTRIB_PACK_RES_FAIL_NOT_SUPPORTED               = 0x3,
    FUN_IF_DEVICE_INFO_GET_ATTRIB_PACK_RES_FAIL_UNKNOWN_REASON      = 0xFF,
}
t_en_hanIf_DeviceInfo_GetAttribPackRes;

typedef enum
{
    FUN_IF_DEVICE_INFO_SET_ATTRIB_RES_OK                                = 0x0,
    FUN_IF_DEVICE_INFO_SET_ATTRIB_RES_FAIL_NOT_SUPPORTED                = 0x3,
    FUN_IF_DEVICE_INFO_SET_ATTRIB_RES_FAIL_READ_ONLY_ATTRIBUTE      = 0x4,
    FUN_IF_DEVICE_INFO_SET_ATTRIB_RES_FAIL_UNKNOWN_REASON       = 0xFF,
}
t_en_hanIf_DeviceInfo_SetAttribRes;

typedef enum
{
    FUN_DEVICE_INFO_ATTRIBUTE_PACK_TYPE_MANDATORY                   = 0x00,
    FUN_DEVICE_INFO_ATTRIBUTE_PACK_TYPE_ALL                         = 0xFE,
    FUN_DEVICE_INFO_ATTRIBUTE_PACK_TYPE_DYNAMIC                     = 0xFF,
}t_en_hanIf_DeviceInfo_Attribute_Pack;

enum
{
    // Lengths of specific attributes in Device Information interface
    FUN_IF_DEVICE_INFO_EMC_LEN                  = 2,
    FUN_IF_DEVICE_INFO_IPUI_OR_RFPI_LEN                 = 5,                // Length of IPUI field in Device Information profile
    FUN_IF_DEVICE_INFO_MAX_STRING_LEN           = 32,               // Max number of characters in string based fields in Device Information profile

    // FUN_IF_DEVICE_INFO_ATTRIB_DEVICE_UID: values
    FUN_IF_DEVICE_INFO_DEVICE_UID_NONE          = 0x0,
    FUN_IF_DEVICE_INFO_DEVICE_UID_DECT          = 0x1,
    FUN_IF_DEVICE_INFO_DEVICE_UID_MAC           = 0x2,
    FUN_IF_DEVICE_INFO_DEVICE_UID_URI           = 0x3,
};

typedef enum
{
    FUN_IF_DEVICE_INFO_MNG_ATTRIB_RES_OK    = 0x0,
    FUN_IF_DEVICE_INFO_MNG_ATTRIB_RES_FAIL  = 0x1,
}
t_en_hanIf_DeviceInfo_AttribRes;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Bind Interface commands
typedef enum
{
    FUN_IF_BIND_CMD_ADD_BIND                                                = 0x01,
    FUN_IF_BIND_CMD_REMOVE_BIND                                             = 0x02,
    FUN_IF_BIND_CMD_REMOVE_BIND_BY_SRC_DEV_ADDR                         = 0x03,
    FUN_IF_BIND_CMD_REMOVE_BIND_BY_SRC_DEV_ADDR_AND_UNIT_ID         = 0x04,
    FUN_IF_BIND_CMD_REMOVE_BIND_BY_DST_DEV_ADDR                         = 0x05,
    FUN_IF_BIND_CMD_REMOVE_BIND_BY_DST_DEV_ADDR_AND_UNIT_ID         = 0x06,
    FUN_IF_BIND_CMD_START_SESSION_READ_BIND_INFO                            = 0x07,
    FUN_IF_BIND_CMD_END_SESSION_READ_BIND_INFO                          = 0x08,
    FUN_IF_BIND_CMD_GET_BIND_ENTRIES_BY_INDEX                               = 0x09,
    FUN_IF_BIND_CMD_GET_BIND_ENTRIES_BY_SRC_DEV_ADDR                        = 0x0A,
    FUN_IF_BIND_CMD_GET_BIND_ENTRIES_BY_SRC_DEV_ADDR_AND_UNIT_ID        = 0x0B,
    FUN_IF_BIND_CMD_GET_BIND_ENTRIES_BY_DST_DEV_ADDR                    = 0x0C,
    FUN_IF_BIND_CMD_GET_BIND_ENTRIES_BY_DST_DEV_ADDR_AND_UNIT_ID        = 0x0D,
}
t_en_hanIf_Bind_Server_Cmd;

typedef enum
{
    FUN_IF_BIND_MNG_ADD_BIND_RESPONSE_OK                                    = 0x0,
    FUN_IF_BIND_MNG_ADD_BIND_RESPONSE_FAIL_NOT_AUTHORIZED               = 0x1,
    FUN_IF_BIND_MNG_ADD_BIND_RESPONSE_FAIL_INVALID_ARGUMENT         = 0x2,
    FUN_IF_BIND_MNG_ADD_BIND_RESPONSE_FAIL_NO_RESOURCES                 = 0xFE,
    FUN_IF_BIND_MNG_ADD_BIND_RESPONSE_FAIL_UNKNOWN_REASON               = 0xFF
}
t_en_hanIf_BindMng_Server_AddBindCmd_Response;

typedef enum
{
    FUN_IF_BIND_MNG_REMOVE_BIND_RESPONSE_OK                                 = 0x0,
    FUN_IF_BIND_MNG_REMOVE_BIND_RESPONSE_FAIL_NOT_AUTHORIZED                = 0x1,
    FUN_IF_BIND_MNG_REMOVE_BIND_RESPONSE_FAIL_INVALID_ARGUMENT              = 0x2,
    FUN_IF_BIND_MNG_REMOVE_BIND_RESPONSE_FAIL_UNKNOWN_REASON                = 0xFF
}
t_en_hanIf_BindMng_Server_RemoveBindCmd_Response;

typedef enum
{
    FUN_IF_BIND_MNG_START_SESSION_RESPONSE_OK                                   = 0x0,
    FUN_IF_BIND_MNG_START_SESSION_RESPONSE_FAIL_NOT_AUTHORIZED                  = 0x1,
    FUN_IF_BIND_MNG_START_SESSION_RESPONSE_FAIL_NO_RESOURCES                    = 0xFE,
    FUN_IF_BIND_MNG_START_SESSION_RESPONSE_FAIL_UNKNOWN_REASON              = 0xFF

}
t_en_hanIf_BindMng_Server_StartSessionCmd_Response;

typedef enum
{
    FUN_IF_BIND_MNG_END_SESSION_RESPONSE_OK                                 = 0x0,
    FUN_IF_BIND_MNG_END_SESSION_RESPONSE_FAIL_UNKNOWN_REASON                = 0xFF

}
t_en_hanIf_BindMng_Server_EndSessionCmd_Response;

typedef enum
{
    FUN_IF_BIND_MNG_GET_BIND_ENTRIES_RESPONSE_OK                                                        = 0x0,
    FUN_IF_BIND_MNG_GET_BIND_ENTRIES_RESPONSE_FAIL_INVALID_ARGUMENT                             = 0x2,
    FUN_IF_BIND_MNG_GET_BIND_ENTRIES_RESPONSE_FAIL_READ_SESSION_NOT_ESTABLISHED                 = 0x20,
    FUN_IF_BIND_MNG_GET_BIND_ENTRIES_RESPONSE_FAIL_ENTRIES_TABLE_MODIFIED                           = 0x21,
    FUN_IF_BIND_MNG_GET_BIND_ENTRIES_RESPONSE_FAIL_UNKNOWN_REASON                               = 0xFF

}
t_en_hanIf_BindMng_Server_GetBindEntriesCmd_Response;

typedef enum
{
    FUN_IF_BIND_MNG_ATTRIB_RES_OK   = 0x0,
    FUN_IF_BIND_MNG_ATTRIB_RES_FAIL = 0x1,
}
t_en_hanIf_BindManager_AttribRes;

// TODO: to complete attributes and structures

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Group Interface commands
typedef enum
{
    FUN_IF_GROUP_CMD_CREATE_GROUP       = 0x1,
    FUN_IF_GROUP_CMD_DELETE_GROUP       = 0x2,
    FUN_IF_GROUP_CMD_ADD_TO_GROUP       = 0x3,
    FUN_IF_GROUP_CMD_REMOVE_FROM_GROUP  = 0x4,
    FUN_IF_GROUP_CMD_GET_GROUP_INFO     = 0x5,
}
t_en_hanIf_Group_Server_Cmd;


typedef enum
{
    FUN_IF_GROUP_MNG_CREATE_GROUP_RESPONSE_OK                                   = 0x0,
    FUN_IF_GROUP_MNG_CREATE_GROUP_RESPONSE_FAIL_NOT_AUTHORIZED              = 0x1,
    FUN_IF_GROUP_MNG_CREATE_GROUP_RESPONSE_FAIL_NO_RESOURCES                    = 0xFE,
    FUN_IF_GROUP_MNG_CREATE_GROUP_RESPONSE_FAIL_UNKNOWN_REASON              = 0xFF
}
t_en_hanIf_GroupMng_Server_CreateGroupCmd_Response;


typedef enum
{
    FUN_IF_GROUP_MNG_DELETE_GROUP_RESPONSE_OK                                   = 0x0,
    FUN_IF_GROUP_MNG_DELETE_GROUP_RESPONSE_FAIL_NOT_AUTHORIZED              = 0x1,
    FUN_IF_GROUP_MNG_DELETE_GROUP_RESPONSE_FAIL_INEXISTENT_ID_OR_RULE           = 0x23,
    FUN_IF_GROUP_MNG_DELETE_GROUP_RESPONSE_FAIL_UNKNOWN_REASON              = 0xFF
}
t_en_hanIf_GroupMng_Server_DeleteGroupCmd_Response;


typedef enum
{
    FUN_IF_GROUP_MNG_ADD_TO_GROUP_RESPONSE_OK                                   = 0x0,
    FUN_IF_GROUP_MNG_ADD_TO_GROUP_RESPONSE_FAIL_NOT_AUTHORIZED              = 0x1,
    FUN_IF_GROUP_MNG_ADD_TO_GROUP_RESPONSE_FAIL_INVALID_ARGUMENT                = 0x2,
    FUN_IF_GROUP_MNG_ADD_TO_GROUP_RESPONSE_FAIL_INEXISTENT_ID_OR_RULE       = 0x23,
    FUN_IF_GROUP_MNG_ADD_TO_GROUP_RESPONSE_FAIL_NO_RESOURCES                    = 0xFE,
    FUN_IF_GROUP_MNG_ADD_TO_GROUP_RESPONSE_FAIL_UNKNOWN_REASON              = 0xFF
}
t_en_hanIf_GroupMng_Server_AddToGroupCmd_Response;


typedef enum
{
    FUN_IF_GROUP_MNG_REMOVE_FROM_GROUP_RESPONSE_OK                                  = 0x0,
    FUN_IF_GROUP_MNG_REMOVE_FROM_GROUP_RESPONSE_FAIL_NOT_AUTHORIZED             = 0x1,
    FUN_IF_GROUP_MNG_REMOVE_FROM_GROUP_RESPONSE_FAIL_INVALID_ARGUMENT           = 0x2,
    FUN_IF_GROUP_MNG_REMOVE_FROM_GROUP_RESPONSE_FAIL_INEXISTENT_ID_OR_RULE      = 0x23,
    FUN_IF_GROUP_MNG_REMOVE_FROM_GROUP_RESPONSE_FAIL_UNKNOWN_REASON         = 0xFF
}
t_en_hanIf_GroupMng_Server_RemoveFromGroupCmd_Response;


typedef enum
{
    FUN_IF_GROUP_MNG_GET_GROUP_INFO_RESPONSE_OK                                 = 0x0,
    FUN_IF_GROUP_MNG_GET_GROUP_INFO_RESPONSE_FAIL_NOT_AUTHORIZED                = 0x1,
    FUN_IF_GROUP_MNG_GET_GROUP_INFO_RESPONSE_FAIL_INEXISTENT_ID_OR_RULE     = 0x23,
    FUN_IF_GROUP_MNG_GET_GROUP_INFO_RESPONSE_FAIL_NO_RESOURCES              = 0xFE,
    FUN_IF_GROUP_MNG_GET_GROUP_INFO_RESPONSE_FAIL_UNKNOWN_REASON                = 0xFF
}
t_en_hanIf_GroupMng_Server_GetGroupInfoCmd_Response;

typedef enum
{
    FUN_IF_GROUP_MNG_ATTRIB_RES_OK  = 0x0,
    FUN_IF_GROUP_MNG_ATTRIB_RES_FAIL    = 0x1,
}
t_en_hanIf_GroupManager_AttribRes;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Identify Interface commands
typedef enum
{
    FUN_IF_IDENTIFY_CMD_C2S_IDENTIFICATION  = 0x1
}
t_en_hanIf_Identify_Server_Cmd;

typedef enum
{
    FUN_IF_IDENTIFY_CMD_S2C_IDENTIFICATION  = 0x1,
}
t_en_hanIf_Identify_Client_Cmd;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Alert Interface commands
typedef enum
{
    FUN_IF_ALERT_CMD_STATUS = 0x1,
}
t_en_hanIf_Alert_Server_Cmd;

// Alert Interface attributes
typedef enum
{
    FUN_IF_ALERT_ATTRIB_STATE   = 0x1,                  // Alert U32 bit mask
    FUN_IF_ALERT_ATTRIB_ENABLE  = 0x2,                  // Indicates if an alert is enabled or disabled
    FUN_IF_ALERT_ATTRIB_LAST

}
t_en_hanIf_Alert_Attrib;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Tamper Alert Interface commands
typedef enum
{
    FUN_IF_TAMPER_ALERT_CMD_ALERT_ON    = 0x1,          // Command used to notify that tampering is being detected
    FUN_IF_TAMPER_ALERT_CMD_ALERT_OFF   = 0x2,          // Command used to notify that tampering stopped being detected
}
t_en_hanIf_TamperAlert_Server_Cmd;

// Tamper Alert Interface attributes
typedef enum
{
    FUN_IF_TAMPER_ALERT_ATTRIB_STATE    = 0x1,          // Indicates the current state of the tampering detection
}
t_en_hanIf_TamperAlert_Attrib;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Level Control Interface commands
typedef enum
{
    FUN_IF_LEVEL_CONTROL_CMD_SET_LEVEL = 0x1,           // Set the curent level of some device feature
}
t_en_hanIf_LevelControl_Server_Cmd;

// Level Control Interface attributes
typedef enum
{
    FUN_IF_LEVEL_CONTROL_ATTRIB_CURRENT_LEVEL   = 0x1,  // Current value as a percentage of maximum value allowed

    FUN_IF_LEVEL_CONTROL_ATTRIB_LAST
}
t_en_hanIf_LevelControl_Attrib;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// Detector Problem Alert Interface commands
typedef enum
{
    FUN_IF_DETECTOR_PROB_ALERT_CMD_GEN_ALERT_OFF    = 0x1,
    FUN_IF_DETECTOR_PROB_ALERT_CMD_GEN_ALERT_ON     = 0x2,
    FUN_IF_DETECTOR_PROB_ALERT_CMD_MASK_ALERT_OFF   = 0x3,
    FUN_IF_DETECTOR_PROB_ALERT_CMD_MASK_ALERT_ON    = 0x4,
}
t_en_hanIf_DetectorProblemAlert_Server_Cmd;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Keep Alive Interface commands
typedef enum
{
    FUN_IF_KEEP_ALIVE_CMD_I_AM_ALIVE            = 0x1,
    FUN_IF_KEEP_ALIVE_CMD_I_AM_ALIVE_WITH_RSSI  = 0x2,
}
t_en_hanIf_KeepAlive_Server_Cmd;

// Keep Alive interface attributes
typedef enum
{
    FUN_IF_KEEP_ALIVE_ATTRIB_INTERVAL = 0x1,
}
t_en_hanIf_KeepAlive_Attrib;

typedef u32 t_KeepAliveIntervalAttribVal;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Contact Base in Exact MultiFrame Interface commands FUN_INTERFACE_CNTCT_BASE_IN_MULTIFRAME


// Multiframe
typedef enum
{
    FUN_IF_CNTCT_BASE_IN_MULTIFRAME_ATTRIB_MULTIFRAME_NUM = 0x1,
    FUN_IF_CNTCT_BASE_IN_MULTIFRAME_ATTRIB_UPDATE_TEXT = 0x2,
}
t_en_hanIf_CntctBaseMultiFrame_Attrib;

typedef u32 t_CntctBaseMultiFrameAttribVal;




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Eeprom Access Cmds Interface commands
typedef enum
{
    FUN_IF_EEPROM_ACCESS_CMD_GET = 0x1,
    FUN_IF_EEPROM_ACCESS_CMD_SET = 0x2,
}
t_en_hanIf_EepromAccess_Server_Cmd;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Arm Disarm Interface commands
typedef enum
{
    FUN_IF_ARM_DISARM_ATTRIB_ARM    = 0x1,
    FUN_IF_ARM_DISARM_ATTRIB_DISARM = 0x1,
}
t_en_hanIf_ArmDisarm_Server_Cmd;

// Arm Disarm Interface attributes
typedef enum
{
    FUN_IF_ARM_DISARM_ATTRIB_STATUS = 0x1,              // Disarm / Arm
}
t_en_hanIf_ArmDisarmAttrib;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// On-Off Interface commands
typedef enum
{
    FUN_IF_ON_OFF_CMD_ON        = 0x1,
    FUN_IF_ON_OFF_CMD_OFF       = 0x2,
    FUN_IF_ON_OFF_CMD_TOGGLE    = 0x3,
}
t_en_hanIf_OnOff_Server_Cmd;


typedef enum
{
    FUN_IF_ONOFF_ATTRIB_STATE   = 0x1,                  // OnOffState attribute
}
t_en_hanIf_OnOffAttrib;

typedef u8 t_onOffAttribVal;

// Default command response structure
typedef enum
{
    FUN_IF_ONOFF_ATTRIB_VAL_OFF = 0,
    FUN_IF_ONOFF_ATTRIB_VAL_ON  = 1,
}
t_en_onOffAttribValues;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Default command response structure
typedef enum
{
    FUN_DEFAULT_CMD_RESPONSE_OK     = 0,
    FUN_DEFAULT_CMD_RESPONSE_FAIL   = 1,
}
t_en_hanDefaultCmdResponse;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Attribute Reporting Interface commands

#define HAN_ATTR_REP_REPORT_TYPE_PERIODIC               0x00
#define HAN_ATTR_REP_REPORT_TYPE_EVENT                  0x80

/* Server Commmands */
typedef enum
{
    FUN_IF_ATTR_REPORTING_CMD_CREATE_PERIODIC_REPORT        = 0x1,
    FUN_IF_ATTR_REPORTING_CMD_CREATE_EVENT_REPORT           = 0x2,
    FUN_IF_ATTR_REPORTING_CMD_ADDENTRY_PERIODIC             = 0x3,
    FUN_IF_ATTR_REPORTING_CMD_ADDENTRY_EVENT                = 0x4,
    FUN_IF_ATTR_REPORTING_CMD_DELETE_REPORT                 = 0x5,
    FUN_IF_ATTR_REPORTING_CMD_GET_PERIODIC_REPORT_ENTRIES   = 0x6,
    FUN_IF_ATTR_REPORTING_CMD_GET_EVENT_REPORT_ENTRIES      = 0x7,
}
t_en_hanIf_Rep_Server_Cmd;

/* Client Commmands */
typedef enum
{
    FUN_IF_ATTR_REPORTING_CMD_PERIODIC_REPORT_NOTIFICATION  = 0x1,
    FUN_IF_ATTR_REPORTING_CMD_EVENT_REPORT_NOTIFICATION     = 0x2,
}
t_en_hanIf_Rep_Server_Cmd_Send;


typedef enum
{
    FUN_ATTRIBUTE_PACK_TYPE_MANDATORY                   = 0x00,
    FUN_ATTRIBUTE_PACK_TYPE_ALL                         = 0xFE,
    FUN_ATTRIBUTE_PACK_TYPE_DYNAMIC                     = 0xFF,
}t_en_hanIf_Rep_Attribute_Pack;

/* Response on create/add entry/delete request*/
typedef enum
{
    FUN_IF_ATTR_REPORTING_RES_OK                        = 0X00,
    FUN_IF_ATTR_REPORTING_RES_NOT_AUTH                  = 0X01,
    FUN_IF_ATTR_REPORTING_RES_INVALID                   = 0X02,
    FUN_IF_ATTR_REPORTING_RES_NOT_SUPPORTED             = 0x03,     // Fail: Not supported
    FUN_IF_ATTR_REPORTING_NOT_ENOUGH                    = 0XFE,
    FUN_IF_ATTR_REPORTING_RES_UNKNOWN                   = 0XFF,
}
t_en_hanIf_Rep_Response_Value;

/* Type Of Reporting */
typedef enum
{
    FUN_IF_ATTR_REPORTING_TYPE_CHANGE_OF_VAL            = 0X00,
    FUN_IF_ATTR_REPORTING_TYPE_HIGH_THRESH              = 0X01,
    FUN_IF_ATTR_REPORTING_TYPE_LOW_THRESH               = 0X02,
    FUN_IF_ATTR_REPORTING_TYPE_EQUAL                    = 0X03,
}
t_en_hanIf_Rep_ReportingType_Value;

#define FUN_IF_ATTR_REPORTING_TYPE_COV_VALUE_CHNAGED    0x00

// Attribute Reporting Interface attributes
typedef enum
{
    FUN_IF_ATTR_REPORTING_NUMBER_OF_REPORTS             = 0x1,          // the total number of reports registered on any given time
    FUN_IF_ATTR_REPORTING_NUMBER_OF_PERIODIC_REPORTS    = 0x2,          // the total number of periodic reports registered on any given time
    FUN_IF_ATTR_REPORTING_NUMBER_OF_EVENT_REPORTS       = 0x3,          // the total number of event reports registered on any given time

    FUN_IF_ATTR_REPORTING_LAST,
}
t_en_hanIf_Rep_Attrib;

typedef enum
{
    FUN_IF_ATTR_REPORTING_ATTRIB_RES_OK     = 0x0,
    FUN_IF_ATTR_REPORTING_ATTRIB_RES_FAIL   = 0x1,
}
t_en_hanIf_Rep_AttribRes;


typedef u16 t_Rep_NumOfReportsAttribVal;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Simple Power Metering Interface commands
typedef enum
{
    FUN_IF_SIMPLE_POWER_METERING_CMD_REPORT                     = 0x1,
}
t_en_hanIf_SimplePowerMetering_Server_Cmd;

typedef enum
{
    FUN_IF_SIMPLE_POWER_METERING_CMD_MEASUREMENT_RESET          = 0x1,
}
t_en_hanIf_SimplePowerMetering_Client_Cmd;


// Simple Power Metering interface attributes
typedef enum
{
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_ENERGY                  = 0x1,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_ENERGY_AT_LAST_RESET    = 0x2,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_TIME_AT_LAST_RESET      = 0x3,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_INSTANTANEOUS_POWER     = 0x4,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_AVERAGE_POWER           = 0x5,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_AVERAGE_POWER_INTERVAL  = 0x6,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_VOLTAGE                 = 0x7,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_CURRENT                 = 0x8,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_FREQUENCY               = 0x9,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_POWER_FACTOR            = 0xA,
    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_REPORT_INTERVAL         = 0xB,

    FUN_IF_SIMPLE_POWER_METERING_ATTRIB_LAST,
}
t_en_hanIf_SimplePowerMetering_Attrib;

typedef enum
{
    FUN_IF_POWER_CMD_LOW_MAINS_VOLTAGE_ALERT            = 0x01,
    FUN_IF_POWER_CMD_HIGH_MAINS_VOLTAGE_ALERT           = 0x02,
    FUN_IF_POWER_CMD_LOW_BATTERY_VOLTAGE_ALERT          = 0x03,
    FUN_IF_POWER_CMD_HIGH_BATTERY_VOLTAGE_ALERT         = 0x04,
    FUN_IF_POWER_CMD_LOW_BATTERY_CHARGE_VOLTAGE_ALERT   = 0x05,
    FUN_IF_POWER_CMD_HIGH_BATTERY_CHARGE_VOLTAGE_ALERT  = 0x06,
}
t_en_hanIf_Power_Cmd;

typedef enum
{
    FUN_IF_POWER_ATTRIB_MAINS_VOLTAGE                       = 0x01,
    FUN_IF_POWER_ATTRIB_MAINS_FREQUENCY                     = 0x02,
    FUN_IF_POWER_ATTRIB_MAINS_ALARMS                        = 0x03,
    FUN_IF_POWER_ATTRIB_MAINS_MINIMUM_VOLTAGE_THRESHOLD     = 0x04,
    FUN_IF_POWER_ATTRIB_MAINS_MAXIMUM_VOLTAGE_THRESHOLD     = 0x05,
    FUN_IF_POWER_ATTRIB_MAINS_VOLTAGE_DWELL_TRIP_POINT      = 0x06,
    FUN_IF_POWER_ATTRIB_BATTERY_VOLTAGE                     = 0x07,
    FUN_IF_POWER_ATTRIB_BATTERY_MANUFACTURER                = 0x08,
    FUN_IF_POWER_ATTRIB_BATTERY_TYPE                        = 0x09,
    FUN_IF_POWER_ATTRIB_BATTERY_AHR_RATING                  = 0x0A,
    FUN_IF_POWER_ATTRIB_BATTERY_CELL_QUANTITY               = 0x0B,
    FUN_IF_POWER_ATTRIB_BATTERY_RATED_VOLTAGE               = 0x0C,
    FUN_IF_POWER_ATTRIB_BATTERY_STATUS                      = 0x0D,
    FUN_IF_POWER_ATTRIB_BATTERY_MINIMUM_VOLTAGE_THRESHOLD   = 0x0E,
    FUN_IF_POWER_ATTRIB_BATTERY_MAXIMUM_VOLTAGE_THRESHOLD   = 0x0F,
    FUN_IF_POWER_ATTRIB_BATTERY_CHARGE_LEVEL                = 0x10,
    FUN_IF_POWER_ATTRIB_BATTERY_MINIMUM_CHARGE_LEVEL        = 0x11,
    FUN_IF_POWER_ATTRIB_BATTERY_MAXIMUM_CHARGE_LEVEL        = 0x12,
    FUN_IF_POWER_ATTRIB_POWER_SOURCE                        = 0x13,

    FUN_IF_POWER_ATTRIB_LAST
}
t_en_hanIf_Power_Attrib;

typedef enum
{
    FUN_IF_BIND_MNG_ATTRIB_NUMBER_OF_ENTRIES        = 0x01,
}
t_en_hanIf_BindManagement_Attrib;

typedef enum
{
    FUN_IF_GROUP_MNG_ATTRIB_NUMBER_OF_GROUPS        = 0x01,
}
t_en_hanIf_GroupManagement_Attrib;

typedef enum
{
    FUN_IF_TIME_ATTRIB_TIME                         = 0x01,
}
t_en_hanIf_Time_Attrib;



// FUN_INTERFACE_RSSI

// Server Attribute
typedef enum
{
    FUN_IF_RSSI_ATTRIB_VALUE    = 0x1,                  //
}
t_en_hanIf_Rssi_Attrib;




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// SUOTA Software Upgrade over the air --  Interface commands
typedef enum
{
    FUN_IF_SUOTA_CHECK_FOR_VERSION                      = 0x1,  // can check if a newer version is available for it in the suota serve
    FUN_IF_SUOTA_CMD_UPGRADE_COMPLETED                  = 0x2,  // The device shall inform the server upon completion of the upgrade indicating success or failure
}
t_en_hanIf_Suota_Server_Cmd;

typedef enum
{
    FUN_IF_SUOTA_CMD_NEW_SW_AVAILABLE                   = 0x1,  // server notifies the device of the exiting of new version
    //FUN_IF_SUOTA_CMD_UPDATE_CFM                           = 0x1,
}
t_en_hanIf_Suota_Client_Cmd;


typedef enum
{
    FUN_IF_SUOTA_NEWVER_RES_UPGRADE_INITIATED               = 0x0,
    FUN_IF_SUOTA_NEWVER_RES_INVLAID_SW                      = 0x11,
    FUN_IF_SUOTA_NEWVER_RES_UNSUPPORTED_HW                  = 0x12,
    FUN_IF_SUOTA_NEWVER_RES_BATTERY_LOW                     = 0x13,
    FUN_IF_SUOTA_NEWVER_RES_UNKNOW_FAILURE                  = 0xFF,
}
t_en_hanIf_Suota_ClientResCode_NewVersion;


typedef enum
{
    FUN_IF_SUOTA_UPGRADE_COMPLETE_RES_UPGRADE_SUCCESSFUL                = 0x0,
    FUN_IF_SUOTA_UPGRADE_COMPLETE_RES_UPGRADE_ABORTED                   = 0x11,
    FUN_IF_SUOTA_UPGRADE_COMPLETE_RES_GMEP_SESSION_ERR                  = 0x12,
    FUN_IF_SUOTA_UPGRADE_COMPLETE_RES_INVALID_IMAGE                     = 0x13,
    FUN_IF_SUOTA_UPGRADE_COMPLETE_RES_MEM_ERR                           = 0x14,
    FUN_IF_SUOTA_UPGRADE_COMPLETE_RES_BATTERY_LOW                       = 0x15,
    FUN_IF_SUOTA_UPGRADE_COMPLETE_RES_UNKNOW_FAILURE                    = 0xFF,
}
t_en_hanIf_Suota_ServerResCode_UpgradeCompleted;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Simple Button Interface commands
typedef enum
{
    FUN_IF_SIMPLE_BUTTON_CMD_SHORT_PRESS        = 0x1,
    FUN_IF_SIMPLE_BUTTON_CMD_LONG_PRESS         = 0x2,
    FUN_IF_SIMPLE_BUTTON_CMD_EXTRA_LONG_PRESS   = 0x3,
    FUN_IF_SIMPLE_BUTTON_CMD_DOUBLE_PRESS       = 0x4,
}
t_en_hanIf_Simple_Button_Server_Cmd;

// Simple Button interface attributes
typedef enum
{
    FUN_IF_SIMPLE_BUTTON_ATTRIB_SHORT_PRESS_MAX_DURATION        = 0x1,
    FUN_IF_SIMPLE_BUTTON_ATTRIB_EXTRA_LONG_PRESS_MIN_DURATION   = 0x2,
    FUN_IF_SIMPLE_BUTTON_ATTRIB_DOUBLE_PRESS_GAP_DURATION       = 0x3,

    FUN_IF_SIMPLE_BUTTON_ATTRIB_LAST
}
t_en_hanIf_Simple_Button_Attrib;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Simple Visual Control Interface commands
typedef enum
{
    FUN_IF_SIMPLE_VISUAL_CONTROL_CMD_ON         = 0x1,
    FUN_IF_SIMPLE_VISUAL_CONTROL_CMD_OFF        = 0x2,
    FUN_IF_SIMPLE_VISUAL_CONTROL_CMD_BLINK      = 0x3,
    FUN_IF_SIMPLE_VISUAL_CONTROL_CMD_FADE       = 0x4,
    FUN_IF_SIMPLE_VISUAL_CONTROL_CMD_BREATHE    = 0x5,
}
t_en_hanIf_Simple_Visual_Control_Server_Cmd;



// FUN_INTERFACE_THERMOSTAT Attributes
typedef enum
{
    FUN_IF_THERMOSTAT_ATTRIB_SUPPORTED_MODES                = 0x01,     // Read Only
    FUN_IF_THERMOSTAT_ATTRIB_OPERATING_MODE                 = 0x02,     // see t_en_hanIf_Thermostat_Mode
    FUN_IF_THERMOSTAT_ATTRIB_FAN_MODE                       = 0x03,     // see t_en_hanIf_Thermostat_FanMode
    FUN_IF_THERMOSTAT_ATTRIB_HEAT_MODE_TEMPERATURE          = 0x04,     // -32768 to +32767
    FUN_IF_THERMOSTAT_ATTRIB_COOL_MODE_TEMPERATURE          = 0x05,     // -32768 to +32767
    FUN_IF_THERMOSTAT_ATTRIB_HEAT_MODE_TEMPERATURE_OFFSET   = 0x08,     // -32768 to +32767
    FUN_IF_THERMOSTAT_ATTRIB_BOOST_DURATION                 = 0x0A,     //  0X00-0XFF

    FUN_IF_THERMOSTAT_ATTRIB_LAST,
}
t_en_hanIf_Thermostat_Attrib;

// FUN_IF_THERMOSTAT_ATTRIB_MODE Modes
typedef enum
{
    //FUN_IF_THERMOSTAT_MODE_OFF                                = 0x01,
    FUN_IF_THERMOSTAT_MODE_HEAT                             = 0x01,
    FUN_IF_THERMOSTAT_MODE_COOLING                          = 0x02,
    FUN_IF_THERMOSTAT_MODE_AUTOMATIC                        = 0x04,

}
t_en_hanIf_Thermostat_Mode;


//FUN_IF_THERMOSTAT_ATTRIB_FAN_MODE
typedef enum
{
    FUN_IF_THERMOSTAT_FAN_MODE_OFF                          = 0x10,
    FUN_IF_THERMOSTAT_FAN_MODE_ON                           = 0x20,
    FUN_IF_THERMOSTAT_FAN_MODE_AUTO                         = 0x40,

}
t_en_hanIf_Thermostat_FanMode;



//FUN_IF_THERMOSTAT_ATTRIB_SUPPORTED_MODES
typedef enum
{
    FUN_IF_THERMOSTAT_HEATING_ONLY                          = 0x01,
    FUN_IF_THERMOSTAT_COOLING_ONLY                          = 0x02,
    FUN_IF_THERMOSTAT_HEAT_COOL                             = 0x04,
    FUN_IF_THERMOSTAT_FAN_NOT_SUPPORTED                     = 0x10,
    FUN_IF_THERMOSTAT_FAN_SUPPORTED                         = 0x20,
    FUN_IF_THERMOSTAT_FAN_SUPPORT_AUTO                      = 0x40,
}
t_en_hanIf_Thermostat_SupportedMode;



// FUN_INTERFACE_SCHEDULING Attributes


/* Helper structure for Return Address and msg sequence for replies */
typedef struct st_ReturnAddress
{
    u16     u16_DstDeviceId;
    u8      u8_DstUnitId;
    u8      u8_MsgSeq;
}t_st_ReturnAddress;



// FUN_INTERFACE_WEEKLY_SCHEDULING Commands
typedef enum
{
    FUN_IF_WEEKLY_SCHEDULING_CMD_ACTIVATE_SCHEDULER             = 0x1,
    FUN_IF_WEEKLY_SCHEDULING_CMD_DEFINE_WEEKLY_EVENT            = 0x2,
    FUN_IF_WEEKLY_SCHEDULING_CMD_UPDATE_WEEKLY_EVENT_STATUS     = 0x3,
    FUN_IF_WEEKLY_SCHEDULING_CMD_GET_WEEKLY_EVENT_ENTRY         = 0x4,
    FUN_IF_WEEKLY_SCHEDULING_CMD_DELETE_WEEKLY_EVENT            = 0x5,
    FUN_IF_WEEKLY_SCHEDULING_CMD_DELETE_ALL_WEEKLY_EVENT        = 0x6,
}
t_en_hanIf_WeeklyScheduling_Server_Cmd;

typedef PACK_STRUCT {
    u8 u8_EntryID;
    u8 u8_EventStatus; // 0x00 disabled, 0x80 enabled note only MSB Bit used - other or reserved
    u8 u8_DayOfWeek;
    u8 u8_Hour;
    u8 u8_Minute;
    u8 u8_ProgramId;
}t_st_hanFunSchedDayOfWeek;


#define HAN_FUN_SCHEDULING_MAX_ACTION_LEN 40
typedef PACK_STRUCT {
    u8                        u8_responseCode;
    t_st_hanFunSchedDayOfWeek st_hanFunSchedDayOfWeek;
}t_st_hanFunSchedDayOfWeekRes;


// FUN_INTERFACE_PROGRAM Commands
typedef enum
{
    FUN_IF_PROGRAM_CMD_DEFINE               = 0x1,
    FUN_IF_PROGRAM_CMD_INVOKE               = 0x2,
    FUN_IF_PROGRAM_CMD_DELETE               = 0x3,
    FUN_IF_PROGRAM_CMD_DELETE_ALL           = 0x4,
    FUN_IF_PROGRAM_CMD_GET_PROGRAM_ACTIONS  = 0x5,
}
t_en_hanIf_Program_Server_Cmd;


typedef PACK_STRUCT {
    u8  u8_DstUnitId;           // Destination unit which this action is to be sent
    u8  u8_Type;                // Message Type  0x01=Command , 0x06=Set Attribute Request
    u16 u16_InterfaceType_ID;   // interface type client/server, Interface ID u15
    u8  u8_InterfaceMember;     // command id / attribute id
    u16 u16_PayloadLen;         // Size of the attribute or payload ....
    u8  Payload[1];
}t_st_hanFunActionEntry;

// FUN_INTERFACE_TEMPERATURE
#define HAN_TEMPERATURE_ATTRIB_MAX_MEASURED                 0x7FFF

typedef enum
{
    FUN_IF_TEMPERATURE_ATTRIB_MEASURED_TEMPERATURE          = 0x01,//This attribute holds the current measured temperature, in one hundredth (1/100) of Celsius degrees (�C).
}
t_en_hanIf_Temperature_Server_Attrib;

#ifdef _MSC_VER
#pragma pack() //Disable struct packing for MS Visual Studio compiler
#endif


#endif // _FUN_PROFILES_H
