/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _FUN_PROPRIETARY_H
#define _FUN_PROPRIETARY_H

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"

////////////////////////////////////////////////////////////////////////////////
// Proprietary Unit Types
// These ids must be within the following range :
// FUN_UNIT_TYPE_PROPRIETARY_START_OFFSET <= id <= FUN_UNIT_TYPE_PROPRIETARY_END_OFFSET
////////////////////////////////////////////////////////////////////////////////
enum
{
    FUN_UNIT_TYPE_PROPRIETARY_THERMOMETER           = 0xFF01,   // Thermometer Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_VOICE_PROMPTER        = 0xFF02,   // Voice Prompter Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_REMOTE_CONTROL        = 0xFF03,   // Remote Control Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_PRICE_TAG             = 0xFF04,   // Super Market Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_CAMERA                = 0xFF05,   // Camera Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_BUTTON                = 0xFF06,   // Button Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_LED                   = 0xFF07,   // LED Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_ULE_VOICE_CALL        = 0xFF0A,   // ULE Voice Call Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_APPLIANCE_MULTIMETER  = 0xFF0B,   // Appliance Multimeter Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_ASAFE_RACKEYE         = 0xFF0B,   // ASAFE RACKEYE Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_ULEASY                = 0xFF0C,   // ULEasy (raw data) Unit Type
    FUN_UNIT_TYPE_PROPRIETARY_SENSI_COMBO_SENSOR    = 0xFF0D,   // Sensi Combo Sensor Unit Type
};

////////////////////////////////////////////////////////////////////////////////
// Proprietary FUN interface id
////////////////////////////////////////////////////////////////////////////////
enum
{
    FUN_INTERFACE_SPI_METERING              = 0x7F00,   // Power Metering using SPI
    FUN_INTERFACE_RF_TEST_MODE              = 0x7F01,   // proprietary interfaces - RF TEST MODE
    FUN_INTERFACE_CNTCT_BASE_IN_MULTIFRAME  = 0x7F02,   // proprietary interfaces - Contact Base in exact Multi Frame
    FUN_INTERFACE_CNTCT_BASE_IN_MF_MANAGER  = 0x7F03,   // proprietary interfaces - SuperMarket Manager
    FUN_INTERFACE_EEPROM_ACCESS             = 0x7F04,   // Eeprom access from BS on device
    FUN_INTERFACE_SUOTA_COMPATABILITY       = 0x7F05,   // SW upgrade interface
    FUN_INTERFACE_REMOTE_CONTROL            = 0x7F07,   // DSPG Remote Control
    FUN_INTERFACE_VOICE_PROMPTER            = 0x7F06,   // Playing voice prompt
    //FUN_INTERFACE_TEMPERATURE             = 0x7F08,   // EVS Thermometer which sends the temperature periodically
    FUN_INTERFACE_DEV_STAT                  = 0x7F09,   // Device statistics
    FUN_INTERFACE_NOTIFICATION              = 0x7F0A,   // DSPG Notification Interface
    FUN_INTERFACE_LOG_DATA                  = 0x7F0B,   // DSPG Log Data Interface
    FUN_INTERFACE_BUTTON                    = 0x7F0C,   // DSPG Button Interface
    FUN_INTERFACE_LED                       = 0x7F0D,   // DSPG LED Interface
    FUN_INTERFACE_EURO_THERMO_CONFIG        = 0x7F0E,   // Eurot Thermostat Configuration
    FUN_INTERFACE_EURO_WINDOW_CONFIG        = 0x7F0F,   // Eurot Window Configuration
    FUN_INTERFACE_ULE_STREAMING             = 0x7F10,   // ULE streaming interface
    FUN_INTERFACE_ULE_VOICE_CALL            = 0x7F11,   // DSPG ULE Voice Call Interface
    FUN_INTERFACE_APPLIANCE_MULTIMETER      = 0x7F13,   // DSPG Appliance Multimeter Interface
    FUN_INTERFACE_SUOTA_IMAGE_TRANSFER      = 0x7F14,   // DSPG SUOTA Image Transfer Interface
    FUN_INTERFACE_EMZA_MOTION_ANALYSIS      = 0x7F15,   // EMZA Motion Analysis Interface
    FUN_INTERFACE_ULEASY                    = 0x7F16,   // DSPG ULEasy (raw data) Interface
    FUN_INTERFACE_SENSI_COMBO_SENSOR        = 0x7F17,   // Sensi Combo Sensor Interface
};

// Eeprom Access From BS interface Command ID
typedef enum
{
    FUN_IF_EEPROM_ACCESS_READ       = 0x1,
    FUN_IF_EEPROM_ACCESS_WRITE      = 0x2,
}
t_en_hanIf_EepromAccess_CmndId;

// Eeprom Access From BS interface Address Types
typedef enum
{
    FUN_IF_EEPROM_ACCESS_ADDRESS_TYPE_EEPROM_DECT       = 0x0,
    FUN_IF_EEPROM_ACCESS_ADDRESS_TYPE_EEPROM_HAN        = 0x1,
    FUN_IF_EEPROM_ACCESS_ADDRESS_TYPE_RAM               = 0x2,
    FUN_IF_EEPROM_ACCESS_ADDRESS_TYPE_SRAM              = 0x3,

    FUN_IF_EEPROM_ACCESS_ADDRESS_TYPE_LAST,
}
t_en_hanIf_EepromAccessFromBs_AddressType;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// RF Test Mode Interface commands
typedef enum
{
    FUN_IF_RF_TEST_MODE_CMD_ENABLE = 0x1,
    FUN_IF_RF_TEST_MODE_CMD_TX_TEST = 0x2,
}
t_en_hanIf_RfTestMode_Server_Cmd;


typedef enum
{
    FUN_IF_RF_TEST_MODE_CMD_SEND = 0x1,
}
t_en_hanIf_RfTestMode_Client_Cmd;


// RF Test Mode interface attributes
typedef enum
{
    FUN_IF_RF_TEST_MODE_ATTRIB_BULKSIZE     = 0x1,
    FUN_IF_RF_TEST_MODE_ATTRIB_INTER_BULKS  = 0x2,
    FUN_IF_RF_TEST_MODE_ATTRIB_INTER_PACKT  = 0x3,
    FUN_IF_RF_TEST_MODE_ATTRIB_PCKT_COUNTER = 0x4,
    FUN_IF_RF_TEST_MODE_ATTRIB_PACK         = 0x5,
    FUN_IF_RF_TEST_MODE_ATTRIB_LAST,
}
t_en_hanIf_RfTestMode_Attrib;

typedef u8 t_RfTestModeAttribVal;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Device Statistics Interface commands
typedef enum
{
    FUN_IF_DEVICE_STAT_CMD_RESET = 0x1,
    FUN_IF_DEVICE_STAT_NOTIFY_CP_STAT  = 0x2,
}
t_en_hanIf_DevStat_Server_Cmd;

// Device Statistics interface attributes
typedef enum
{
    FUN_IF_DEVICE_STAT_ATTRIB_TX_SUCC           = 0x1,
    FUN_IF_DEVICE_STAT_ATTRIB_TX_FAIL           = 0x2,
    FUN_IF_DEVICE_STAT_ATTRIB_RX_PACKETS        = 0x3,
    FUN_IF_DEVICE_STAT_ATTRIB_LOCK_SUCC         = 0x4,
    FUN_IF_DEVICE_STAT_ATTRIB_LOCK_FAIL         = 0x5,
    FUN_IF_DEVICE_STAT_ATTRIB_PINGS             = 0x6,
    FUN_IF_DEVICE_STAT_ATTRIB_ADD_QUEUE_FAIL    = 0x7,
    FUN_IF_DEVICE_STAT_ATTRIB_SETUP_AGAIN       = 0x8,

    FUN_IF_DEVICE_STAT_ATTRIB_LAST
}
t_en_hanIf_DevStat_Attrib;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// REMOTE CONTROL

typedef enum
{
    FUN_IF_REMOTE_CONTROL_ID_CMD                    = 0x1,  // Client notifies on new remote control id detected

}
t_en_hanIf_RemoteControl_Server_Cmd;

// Voice Prompter Interface commands
typedef enum
{
    FUN_IF_VOICE_PROMPT_CMD_PLAY = 0x1,
}
t_en_hanIf_VoicePrompter_Server_Cmd;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Thermometer Interface commands
typedef enum
{
    FUN_IF_THERMOMETER_CMD_REPORT = 0x1,
}
t_en_hanIf_Thermometer_Server_Cmd;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// SuperMarket  Manager Interface commands
typedef enum
{
    FUN_IF_SM_MGR_CMD_SCHEDULE = 0x1,
    FUN_IF_SM_MGR_CMD_PRICE = 0x2,
    FUN_IF_SM_MGR_CMD_DELETE = 0x3,
    FUN_IF_SM_MGR_CMD_PRICE_ALL = 0x4,
}
t_en_hanIf_SuperMarketManager_Server_Cmd;

// SuperMarket  Manager Interface commands
typedef enum
{
    FUN_IF_SM_OTA_CMD_GET_PRICE = 0x1,
}
t_en_hanIf_SuperMarket_OTA_Client_Cmd;

typedef enum
{
    FUN_IF_SM_MGR_CMD_STATUS = 0x1,
}
t_en_hanIf_SuperMarketManager_Client_Cmd;


// SuperMarket  Manager interface attributes
typedef enum
{
    FUN_IF_SM_MANGER_ATTRIB_SCHEDULE_WIN                = 0x1,
    FUN_IF_SM_MANGER_ATTRIB_BASE_FMF                    = 0x2,
    FUN_IF_SM_MANGER_ATTRIB_PRICE_SENDING_RETRY_NUM     = 0x3,
}
t_en_hanIf_SuperMarketManager_Attrib;


// SuperMarket  Manager interface attributes
typedef enum
{
    FUN_IF_SM_MANGER_ATTRIB_RES_OK  = 0x0,
    FUN_IF_SM_MANGER_ATTRIB_RES_FAL = 0x1,
}
t_en_hanIf_SuperMarketManager_AttribRes;


// SuperMarket  OTA Interface commands
typedef enum
{
    FUN_IF_SM_OTA_CMD_SCHEDULE = 0x1,
    FUN_IF_SM_OTA_CMD_PRICE = 0x2,
}
t_en_hanIf_SuperMarketOTA_Server_Cmd;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// DSPG Notification Interface commands
typedef enum
{
    FUN_IF_NOTIFICATION_CMD_NOTIFY = 0x1,

}
t_en_hanIf_Notification_Server_Cmd;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Log Data Interface commands
typedef enum
{
    FUN_IF_LOG_DATA_CMD_REPORT = 0x1,
}
t_en_hanIf_Log_Data_Server_Cmd;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Button Interface commands
typedef enum
{
    FUN_IF_BUTTON_CMD_SHORT_PRESS   = 0x1,
    FUN_IF_BUTTON_CMD_LONG_PRESS    = 0x2,
    FUN_IF_BUTTON_CMD_DOUBLE_CLICK  = 0x3,
}
t_en_hanIf_Button_Server_Cmd;

// Button interface attributes
typedef enum
{
    FUN_IF_BUTTON_ATTRIB_SHORT_PRESS_DURATION   = 0x1,
    FUN_IF_BUTTON_ATTRIB_LONG_PRESS_DURATION    = 0x2,
    FUN_IF_BUTTON_ATTRIB_DOUBLE_CLICK_DELAY     = 0x3,

    FUN_IF_BUTTON_ATTRIB_LAST
}
t_en_hanIf_Button_Attrib;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Led Interface commands
typedef enum
{
    FUN_IF_LED_CMD_ON       = 0x1,
    FUN_IF_LED_CMD_OFF      = 0x2,
    FUN_IF_LED_CMD_BLINK    = 0x3,
    FUN_IF_LED_CMD_BREATHE  = 0x4,
}
t_en_hanIf_Led_Server_Cmd;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///
// FUN_INTERFACE_EURO_THERMO_CONFIG Attributes
typedef enum
{
    FUN_INTERFACE_EURO_THERMO_CONFIG_ATTRIB_WINDOW_CONFIG               = 0x01,
    FUN_INTERFACE_EURO_THERMO_CONFIG_ATTRIB_CONTACT_STATE               = 0x02,
    FUN_INTERFACE_EURO_THERMO_CONFIG_ATTRIB_INTERNAL_EXTERNAL           = 0x03,
    FUN_INTERFACE_EURO_THERMO_CONFIG_ATTRIB_INSTALL_POS                 = 0x04,

    FUN_INTERFACE_EURO_THERMO_CONFIG_ATTRIB_LAST,
}
t_en_hanIf_EuroThermostatConfig_Attrib;

// Button interface attributes
typedef enum
{
    FUN_IF_THERMOSTAT_CONFIG_ATTRIB_EXTERNAL_WINDOW_CONTACT_NOT_USED    = 0x0,
    FUN_IF_THERMOSTAT_CONFIG_ATTRIB_EXTERNAL_WINDOW_CONTACT_USED        = 0x2,
    FUN_IF_THERMOSTAT_CONFIG_ATTRIB_LAST
}
t_en_hanIf_EuroThermostatConfigAttribValue;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///
// FUN_INTERFACE_EURO_WINDOW_CONFIG Attributes
typedef enum
{
    FUN_INTERFACE_EURO_WINDOW_CONFIG_ATTRIB_PROX_LOW_THRESHOLD              = 0x01,
    FUN_INTERFACE_EURO_WINDOW_CONFIG_ATTRIB_PROX_HIGH_THRESHOLD             = 0x02,
    FUN_INTERFACE_EURO_WINDOW_CONFIG_ATTRIB_PROX_LED_CURRENT                = 0x03,
    FUN_INTERFACE_EURO_WINDOW_CONFIG_ATTRIB_PROX_MEASURMENT                 = 0x04,

    FUN_INTERFACE_EURO_WINDOW_CONFIG_ATTRIB_LAST,
}
t_en_hanIf_EuroWindowConfig_Attrib;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///
// FUN_INTERFACE_ULE_STREAMING commands

typedef enum
{
    FUN_IF_ULE_STREAMING_NOTIFY = 0x01,
}
t_en_hanIf_UleStreaming_Cmd;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// FUN_INTERFACE_ULE_VOICE_CALL

// ULE Voice Call Interface commands
typedef enum
{
    FUN_IF_ULE_VOICE_CALL_START             = 0x1,
    FUN_IF_ULE_VOICE_CALL_END               = 0x2,
    FUN_IF_ULE_VOICE_CALL_CODEC_LIST        = 0x3,
}
t_en_hanIf_Ule_Voice_Call_Server_Cmd;


// ULE voice call field identifiers
typedef enum
{
    FUN_ULE_VOICE_CALL_PREFERRED_CODEC_FIELD_ID     = 0x1,
    FUN_ULE_VOICE_CALL_DIGITS_FIELD_ID              = 0x2,
    FUN_ULE_VOICE_CALL_OTHER_PARTY_TYPE_FIELD_ID    = 0x3,
    FUN_ULE_VOICE_CALL_OTHER_PARTY_NAME_FIELD_ID    = 0x4,
    FUN_ULE_VOICE_CALL_OTHER_PARTY_ID_FIELD_ID      = 0x5,
}
t_en_hanIf_UleVoiceCall_FieldId;

// ULE voice call codec types
typedef enum
{
    ULE_CALL_CODEC_NB_BASIC                         = 0x00,
    ULE_CALL_CODEC_WB_G722                          = 0x01,

    ULE_CALL_CODEC_LAST,
}
t_en_hanIf_UleVoiceCall_Codec;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#endif // _FUN_PROPRIETARY_H
