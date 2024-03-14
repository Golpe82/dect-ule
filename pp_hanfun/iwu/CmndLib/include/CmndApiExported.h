/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_API_EXPORTED_H
#define _CMND_API_EXPORTED_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndLib_Config.h"

typedef u16 t_hanCmndServiceId;         // Service ID is 16bit
typedef u8  t_hanCmndMsgId;             // Message ID is 8bit


#ifdef _MSC_VER
#pragma pack(1) // Enable struct packing till next #pragma pack()
#endif

///////////////////////////////////////////////////////////////////////////////
// General CMND API defines
///////////////////////////////////////////////////////////////////////////////
#define CMND_COOKIE_DONT_CARE               (0)     // Use this value when CMND API cookie is not important
#define CMND_API_VERSION                    "2.90"  // This version is passed in <<CMND_IE_VERSION>> in hello_ind to external MCU

#define CMND_HAN_DIALED_DIGITS_MAX_LEN      ( 30 )
#define CMND_HAN_OTHER_PARTY_NAME_MAX_LEN   ( 30 )
#define CMND_HAN_OTHER_PARTY_ID_MAX_LEN     ( 30 )
#define CMND_API_RFPI_SIZE                  ( 5 )   // size of RFPI field in bytes

#if (HAN_SW_FEATURES & HAN_SW_SUPERMARKET_PRESET)
#define CMND_API_PAYLOAD_MAX_LENGTH         ( 250 )
#else
#define CMND_API_PAYLOAD_MAX_LENGTH         ( 167 )
#endif

typedef u8 t_ReportIdWithType;
#define REPORT_KEY_MAX_COUNT                ( 0xFF )    //!< A total number of report ids in the device
#define CMND_MSG_UNKNOWN                    ( 0xFF )
#define FUN_IFACE_MEMBER_UNKNOWN            ( 0xFF )
#define FUN_IFACE_ID_UNKNOWN                ( 0xFF )

///////////////////////////////////////////////////////////////////////////////
// Define CMND API Service Id
///////////////////////////////////////////////////////////////////////////////

typedef enum
{
    // Management service ids
    CMND_SERVICE_ID_DEVICE_MANAGEMENT               = 0x0001,
    CMND_SERVICE_ID_IDENTIFY                        = 0x0004,
    CMND_SERVICE_ID_ATTRIBUTE_REPORTING             = 0x0006,

    // General service ids
    CMND_SERVICE_ID_GENERAL                         = 0x0000,
    CMND_SERVICE_ID_ALERT                           = 0x0100,
    CMND_SERVICE_ID_TAMPER_ALERT                    = 0x0101,
    CMND_SERVICE_ID_DETECTOR_PROBLEM_ALERT          = 0x0102,
    CMND_SERVICE_ID_BATTERY                         = 0x0103,
    CMND_SERVICE_ID_KEEP_ALIVE                      = 0x0104,
    CMND_SERVICE_ID_ARM_DISARM                      = 0x0105,
    CMND_SERVICE_ID_ON_OFF                          = 0x0106,
    CMND_SERVICE_ID_FUN                             = 0x0108,
    CMND_SERVICE_ID_DEBUG                           = 0x0109,
    CMND_SERVICE_ID_KEY_PRESS                       = 0x010A,

    // System service Ids
    CMND_SERVICE_ID_SYSTEM                          = 0x0201,
    CMND_SERVICE_ID_TECHNICIAN                      = 0x0202,
    CMND_SERVICE_ID_PARAMETERS                      = 0x0203,
    CMND_SERVICE_ID_SLEEP                           = 0x0204,
    CMND_SERVICE_ID_MANUFACTURE_CONFIGURATION       = 0x0206,
    CMND_SERVICE_ID_ULE_VOICE_CALL                  = 0x020A,
    CMND_SERVICE_ID_PRODUCTION                      = 0x020B,
    CMND_SERVICE_ID_SUOTA                           = 0x020C,
    CMND_SERVICE_ID_CERTIFICATION                   = 0x020D,
    CMND_SERVICE_ID_REMOTE_CONTROL                  = 0x020E,

    CMND_SERVICE_ID_SUOTA_PROPRIETARY               = 0x020F,   // for compatibillity with priorietary fun message of suota


    CMND_SERVICE_ID_LAST,
    CMND_SERVICE_ID_UNKNOWN                         = 0xFFFF,
}
t_en_hanCmndServiceId;

/*****************************************************************************
 * Define CMND messages
 *****************************************************************************/


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_CERTIFICATION service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_CERT_PLACE_HOLDER                      = 0x00,

    CMND_MSG_CERT_SET_RESET_SN_REQ                  = 0x01, // Set both the Tx and Rx sequence numbers to parameter value
    CMND_MSG_CERT_SET_RESET_SN_RES                  = 0x02, // Responce for set reset sequence number
    CMND_MSG_CERT_MARK_CCM_KEY_AS_USED_REQ          = 0x03, // Mark the CCM key as used or unused depending on the parameter value
    CMND_MSG_CERT_MARK_CCM_KEY_AS_USED_RES          = 0x04, // Responce for mark the CCM key as used
    CMND_MSG_CERT_SET_TX_MIC_CORRUPTION_NUMBER_REQ  = 0x05, // Control the corruption of the MIC sent with an SDU. Parameter controls the number of MICs to corrupt; there is one MIC on each SDU
    CMND_MSG_CERT_SET_TX_MIC_CORRUPTION_NUMBER_RES  = 0x06, // Responce for set TX MIC corruption number
    CMND_MSG_CERT_SET_TX_SN_REQ                     = 0x07, // Set the Tx sequence number to parameter value
    CMND_MSG_CERT_SET_TX_SN_RES                     = 0x08, // Responce for set Tx sequence number
    CMND_MSG_CERT_GET_TX_SN_REQ                     = 0x09, // Get the Tx sequence number
    CMND_MSG_CERT_GET_TX_SN_RES                     = 0x0A, // Responce for get Tx sequence number
    CMND_MSG_CERT_GET_RX_SN_REQ                     = 0x0B, // Get the Rx sequence number
    CMND_MSG_CERT_GET_RX_SN_RES                     = 0x0C, // Responce for set Rx sequence number
    CMND_MSG_CERT_SET_TX_ACK_DROP_NUMBER_REQ        = 0x0D, // Set the number of acknowledgements to not send
    CMND_MSG_CERT_SET_TX_ACK_DROP_NUMBER_RES        = 0x0E, // Responce for Set Tx Ack Drop number
    CMND_MSG_CERT_SET_PMID_REQ                      = 0x0F, // Change the PMID used in the message generated to set up a ULE service call
    CMND_MSG_CERT_SET_PMID_RES                      = 0x10, // Responce for Set PMID
    CMND_MSG_CERT_GET_PMID_REQ                      = 0x11, // Read the current PMID
    CMND_MSG_CERT_GET_PMID_RES                      = 0x12, // Responce for Get PMID
    CMND_MSG_CERT_SET_PI_REQ                        = 0x13, // Set the Portable Identity to be used in setting up ULE service call messages
    CMND_MSG_CERT_SET_PI_RES                        = 0x14, // Responce for Set Portable Identity
    CMND_MSG_CERT_GET_PI_REQ                        = 0x15, // Retrieve the current Portable Identity
    CMND_MSG_CERT_GET_PI_RES                        = 0x16, // Responce for Get Portable Identity
    CMND_MSG_CERT_SET_PVC_STATE_REQ                 = 0x17, // Force the PT PVC state
    CMND_MSG_CERT_SET_PVC_STATE_RES                 = 0x18, // Responce for Set PVC state
}
t_en_hanCmndMsgCertification;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_PMID
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_PMID1;
    u8  u8_PMID2;
    u8  u8_PMID3;
}
t_st_hanCmndIePMID;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_PORTABLE_IDENTITY
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8 u8_UseAltPIFlag;
    u8 u8_PI1;
    u8 u8_PI2;
    u8 u8_PI3;
    u8 u8_PI4;
    u8 u8_PI5;
}
t_st_hanCmndIePortableIdentity;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_ULE_VOICE_CALL service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_ULE_CALL_START_IND         = 0x01,
    CMND_MSG_ULE_CALL_START_RES         = 0x02,
    CMND_MSG_ULE_CALL_END_IND           = 0x03,
    CMND_MSG_ULE_CALL_END_RES           = 0x04,
    CMND_MSG_ULE_CALL_ACTIVE_REQ        = 0x05,
    CMND_MSG_ULE_CALL_ACTIVE_RES        = 0x06,
    CMND_MSG_ULE_CALL_CODEC_IND         = 0x07,
    CMND_MSG_ULE_CALL_CODEC_RES         = 0x08,
    CMND_MSG_ULE_CALL_START_REQ         = 0x09,
    CMND_MSG_ULE_CALL_START_CFM         = 0x0A,
    CMND_MSG_ULE_CALL_END_REQ           = 0x0B,
    CMND_MSG_ULE_CALL_END_CFM           = 0x0C,
    CMND_MSG_ULE_CALL_RELEASE_IND       = 0x0D,
    CMND_MSG_ULE_CALL_SET_VOLUME_REQ    = 0x0E,
    CMND_MSG_ULE_CALL_SET_VOLUME_CFM    = 0x0F,
    CMND_MSG_ULE_CALL_VOLUME_UP_REQ     = 0x10,
    CMND_MSG_ULE_CALL_VOLUME_UP_CFM     = 0x11,
    CMND_MSG_ULE_CALL_VOLUME_DOWN_REQ   = 0x12,
    CMND_MSG_ULE_CALL_VOLUME_DOWN_CFM   = 0x13,
    CMND_MSG_ULE_CALL_CONNECTED_IND     = 0x14,
    CMND_MSG_ULE_CALL_STATUS_UPDATE_REQ = 0x15,
    CMND_MSG_ULE_CALL_LAST,
}
t_en_hanCmndMsgUleVoiceCallOption;

typedef enum
{
    CMND_MSG_ULE_CALL_CODEC_NB_BASIC    = 0x00,
    CMND_MSG_ULE_CALL_CODEC_WB_G722     = 0x01,
    CMND_MSG_ULE_CALL_CODEC_LAST,
}
t_en_hanCmndMsgUleVoiceCallCodec;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_ULE_CALL_SETTING
///////////////////////////////////////////////////////////////////////////////

// Enum defines the bit mask for fields that are present in the <<CMND_IE_ULE_CALL_SETTING>>
typedef enum
{
    ULE_CALL_IE_PREFFERED_CODEC_MASK        = 0x00001,
    ULE_CALL_IE_DIGITS_MASK                 = 0x00010,
    ULE_CALL_IE_OTHER_PARTY_TYPE_MASK       = 0x00100,
    ULE_CALL_IE_OTHER_PARTY_NAME_MASK       = 0x01000,
    ULE_CALL_IE_OTHER_PARTY_ID_MASK         = 0x10000,
}
t_en_hanCmndUleVoiceCallFieldMask;


typedef PACK_STRUCT
{
    u32 u32_FieldMask;                      // see t_en_hanCmndUleVoiceCallFieldMask
    u8  u8_PreferredCodec;                  // see t_en_hanCmndMsgUleVoiceCallCodec
    u8  u8_OtherPartyType;                  // character that defines remote peer - For example: 'd' for device or 'h' for handset
    u8  u8_DigitsLen;
    u8  pu8_Digits[CMND_HAN_DIALED_DIGITS_MAX_LEN]; // Digits to dial for example for external call
    u8  u8_OtherPartyNameLen;
    u8  pu8_OtherPartyName[CMND_HAN_OTHER_PARTY_NAME_MAX_LEN];  // dial a "name" instead of a number
    u8  u8_OtherPartyIdLen;
    u8  pu8_OtherPartyId[CMND_HAN_OTHER_PARTY_ID_MAX_LEN];      // dial an "id" instead of a number of name
}
t_st_hanCmndIeUleCallSetting;

///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_PRODUCTION service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_PRODUCTION_PLACE_HOLDER        = 0x00,

    CMND_MSG_PROD_START_REQ                 = 0x01,     // Set production mode enable - needs a restart
    CMND_MSG_PROD_END_REQ                   = 0x02,     // Set production mode disable - needs a restart
    CMND_MSG_PROD_CFM                       = 0x03,     // Default Response to a Request
    CMND_MSG_PROD_REF_CLK_TUNE_START_REQ    = 0x04,     // Start 13.824MHz Reference Clock Tuning
    CMND_MSG_PROD_REF_CLK_TUNE_END_REQ      = 0x05,     // End 13.824MHz Reference Clock Tuning
    CMND_MSG_PROD_REF_CLK_TUNE_END_RES      = 0x06,     // Confirm on End Process with New Value returned
    CMND_MSG_PROD_REF_CLK_TUNE_ADJ_REQ      = 0x07,     // Adjust up/down request
    CMND_MSG_PROD_BG_REQ                    = 0x08,     // Band Gap Calibration
    CMND_MSG_PROD_BG_RES                    = 0x09,     // Band Gap Calibration Res with ADC and POR returned
    CMND_MSG_PROD_ATE_INIT_REQ              = 0x0A,     // ATE Test initialize request
    CMND_MSG_PROD_ATE_STOP_REQ              = 0x0B,     // ATE Test STOP request - for all modes ( continuous / rx / tx )
    CMND_MSG_PROD_ATE_CONTINUOUS_START_REQ  = 0x0C,     // CONTINUOUS REQUEST
    CMND_MSG_PROD_ATE_RX_START_REQ          = 0x0D,     // Rx Slot Test request status
    CMND_MSG_PROD_ATE_RX_START_RES          = 0x0E,     // Rx Slot Test req - with value measured, returned multiple times
    CMND_MSG_PROD_ATE_TX_START_REQ          = 0x0F,     // Tx Slot Test request
    CMND_MSG_PROD_ATE_GET_BER_FER_REQ       = 0x10,     // Get BER FER Measurmenet
    CMND_MSG_PROD_INIT_EEPROM_DEF_REQ       = 0x11,     // Initialize eeprom to preset values ( selecting 9 onine menu )
    CMND_MSG_PROD_SPECIFIC_PRESET_REQ       = 0x12,     // Initialize eeprom to preset values ( selecting 9 onine menu )
    CMND_MSG_PROD_SLEEP_REQ                 = 0x13,     // Turn into hibernation mode
    CMND_MSG_PROD_SET_SIMPLE_GPIO_LOW       = 0x14,     // Set simple GPIO to low
    CMND_MSG_PROD_SET_SIMPLE_GPIO_HIGH      = 0x15,     // Set simple GPIO to high
    CMND_MSG_PROD_GET_SIMPLE_GPIO_STATE     = 0x16,     // Get simple GPIO's state
    CMND_MSG_PROD_GET_SIMPLE_GPIO_STATE_RES = 0x17,     // Get simple GPIO's state - response
    CMND_MSG_PROD_SET_ULE_GPIO_LOW          = 0x18,     // Set ULE GPIO to low
    CMND_MSG_PROD_SET_ULE_GPIO_HIGH         = 0x19,     // Set ULE GPIO to high
    CMND_MSG_PROD_GET_ULE_GPIO_STATE        = 0x1a,     // Get ULE GPIO's state
    CMND_MSG_PROD_GET_ULE_GPIO_STATE_RES    = 0x1b,     // Get ULE GPIO's state - response
    CMND_MSG_PROD_SET_ULE_GPIO_DIR_INPUT_REQ= 0x1c,     // Set ULE GPIO to input direction
    CMND_MSG_PROD_RESET_EEPROM              = 0x1d,     // RESET EEPROM HAN/DECT
    CMND_MSG_PROD_FW_UPDATE_REQ             = 0x1e,     // Perform FW update over UART
    CMND_MSG_PROD_GPIO_LOOPBACK_TEST_REQ    = 0x1f,     // GPIOs loopback test
}
t_en_hanCmndMsgProduction;

///////////////////////////////////////////////////////////////////////////////
// Values for CMND_IE_U8 in CMND_MSG_PROD_FW_UPDATE_REQ
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_FWUP_BOOT_LOADER_UPDATE        = 0x02, // Boot loader update
    CMND_FWUP_ERASE_FACTORY_IMAGE       = 0x01,
    CMND_FWUP_DONT_ERASE_FACTORY_IMAGE  = 0x00,
} t_en_hanCmndFwUpdateMode;

/////////////////////////////////////////////////////////////////////////////
/// CMND_MSG_PROD_SPECIFIC_PRESET_REQ, data is CMND_IE_U8, payload is the next enum
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_PRODUCTION_SPECIFIC_CR_LOCAL               = 0x00,
    CMND_MSG_PRODUCTION_SPECIFIC_CR_CMND                = 0x01,
    CMND_MSG_PRODUCTION_SPECIFIC_AC                     = 0x02,
    CMND_MSG_PRODUCTION_SPECIFIC_SMOKE_UART             = 0x03,
    CMND_MSG_PRODUCTION_SPECIFIC_SMOKE                  = 0x04,
    CMND_MSG_PRODUCTION_SPECIFIC_ULE_VOICE_CALL         = 0x05,
    CMND_MSG_PRODUCTION_SPECIFIC_ULE_VOICE_CALL_CMND    = 0x06,
    CMND_MSG_PRODUCTION_SPECIFIC_SPMKT                  = 0x07,
    CMND_MSG_PRODUCTION_SPECIFIC_AC_UART                = 0x08,
    CMND_MSG_PRODUCTION_SPECIFIC_SIMPLE_PWR_MTR_UART    = 0x09,
    CMND_MSG_PRODUCTION_SPECIFIC_SWS_BTN                = 0x0A,
    CMND_MSG_PRODUCTION_SPECIFIC_WAKEUP_UART            = 0x0B,
    CMND_MSG_PRODUCTION_SPECIFIC_SIMPLE_PWR_MTR         = 0x0C,
    CMND_MSG_PRODUCTION_SPECIFIC_EURO_THERMOSTAT        = 0x0D,
    CMND_MSG_PRODUCTION_SPECIFIC_EURO_WALLSWITCH        = 0x0E,
    CMND_MSG_PRODUCTION_SPECIFIC_EURO_WINDOW            = 0x0F,

    CMND_MSG_PRODUCTION_SPECIFIC_LAST,
}
t_en_hanCmndMsgProdSpecificPreset;


/////////////////////////////////////////////////////////////////////////////
/// CMND_MSG_PROD_RESET_EEPROM,  CMND_IE_U8 selects eeprom type
///  to reset
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_PRODUCTION_RESET_HAN_EEPROM                        = 0x01,
    CMND_MSG_PRODUCTION_RESET_DECT_EEPROM                       = 0x02,
}
t_en_hanCmndMsgProdResetEeprom;

///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_ATTRIBUTE_REPORTING service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    // Messages for communication with Base
    CMND_MSG_ATTRREP_CREATE_PERIODIC_REPORT_REQ         = 0x01,     // Recevied CreatePeriodicReport request
    CMND_MSG_ATTRREP_CREATE_PERIODIC_REPORT_RES         = 0x02,     // CreatePeriodicReport response
    CMND_MSG_ATTRREP_CREATE_EVENT_REPORT_REQ            = 0x03,     // Received CreateEventReport
    CMND_MSG_ATTRREP_CREATE_EVENT_REPORT_RES            = 0x04,     // CreateEventReport response
    CMND_MSG_ATTRREP_ADDENTRY_PERIODIC_REQ              = 0x05,     // Received AddEntryPeriodicReport
    CMND_MSG_ATTRREP_ADDENTRY_PERIODIC_RES              = 0x06,     // AddEntryPeriodicReport
    CMND_MSG_ATTRREP_ADDENTRY_EVENT_REQ                 = 0x07,     // Recieved AddEntryEventReport request
    CMND_MSG_ATTRREP_ADDENTRY_EVENT_RES                 = 0x08,     // AddEntryEventReport response
    CMND_MSG_ATTRREP_DELETE_REPORT_REQ                  = 0x09,     // Received DeleteReport
    CMND_MSG_ATTRREP_DELETE_REPORT_RES                  = 0x0A,     // DeleteReport response
    CMND_MSG_ATTRREP_PERIODIC_REPORT_NOTIFICATION_IND   = 0x0B,     // Producing PeriodicReportNotification
    CMND_MSG_ATTRREP_EVENT_REPORT_NOTIFICATION_IND      = 0x0C,     // EventReportNotification
    CMND_MSG_ATTRREP_GET_PERIODIC_REPORT_ENTRIES_REQ    = 0x0D,     // Recevied GetPeriodicReportEntries request
    CMND_MSG_ATTRREP_GET_PERIODIC_REPORT_ENTRIES_RES    = 0x0E,     // GetPeriodicReportEntries response
    CMND_MSG_ATTRREP_GET_EVENT_REPORT_ENTRIES_REQ       = 0x0F,     // Recevied GetEventReportEntries request
    CMND_MSG_ATTRREP_GET_EVENT_REPORT_ENTRIES_RES       = 0x10,     // GetEventReportEntries response

    // Messages for communication with Node Host
    CMND_MSG_ATTRREP_ADD_REPORT_IND                     = 0x11,     // Add new report from CMND
    CMND_MSG_ATTRREP_ADD_REPORT_RES                     = 0x12,     // Response to "ADD" new report with result
    CMND_MSG_ATTRREP_REPORT_NOTIFICATION_REQ            = 0x13,     // Report to CMND about some condition is met
    CMND_MSG_ATTRREP_DELETE_REPORT_IND                  = 0x14,     // Delete report
    CMND_MSG_ATTRREP_GET_REPORT_VALUES_IND              = 0x15,     // Get report values
    CMND_MSG_ATTRREP_GET_REPORT_VALUES_RES              = 0x16,     // Response to "Get" with report values data
    CMND_MSG_ATTRREP_ADD_REPORT_REQ                     = 0x17,     // Add report request by Node Host
    CMND_MSG_ATTRREP_ADD_REPORT_CFM                     = 0x18,     // Response to "ADD" new report from Node Host with result

    CMND_MSG_ATTRREP_GET_ATTRIB_REQ                     = 0x19,     // Get attributes of Attribute Reporting (number of reports)
    CMND_MSG_ATTRREP_GET_ATTRIB_RES                     = 0x1A,     // Get attributes response

    CMND_MSG_ATTRREP_UPDATE_PERIODIC_REPORT_INTERVAL_REQ    = 0x1B, // Update Periodic Interval of the report
    CMND_MSG_ATTRREP_UPDATE_PERIODIC_REPORT_INTERVAL_RES    = 0x1C, // Response to Update Periodic Interval of the report s


}
t_en_hanCmndMsgAttrRep;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_PARAMETERS service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_PARAM_GET_REQ              = 0x01,     // Read parameter from CMND
    CMND_MSG_PARAM_GET_RES              = 0x02,     // Response for 'GET' parameter request
    CMND_MSG_PARAM_SET_REQ              = 0x03,     // Modify CMND parameter
    CMND_MSG_PARAM_SET_RES              = 0x04,     // Response for 'SET' parameter request
    CMND_MSG_PARAM_GET_DIRECT_REQ       = 0x05,     // Direct read access  to CMND eeprom/RAM memory
    CMND_MSG_PARAM_GET_DIRECT_RES       = 0x06,     // Response for direct access request
    CMND_MSG_PARAM_SET_DIRECT_REQ       = 0x07,     // Direct write access to CMND eeprom/RAM memory
    CMND_MSG_PARAM_SET_DIRECT_RES       = 0x08,     // Success/Failure response from write access request

    CMND_MSG_PARAM_LAST
}
t_en_hanCmndMsgParam;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_DEVICE_MANAGEMENT service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_DEV_MGNT_GET_ATTRIB_REQ            = 0x01,     // Read unit attribute
    CMND_MSG_DEV_MGNT_GET_ATTRIB_RES            = 0x02,     // Response for get attribute request
    CMND_MSG_DEV_MGNT_REGISTER_DEVICE_REQ       = 0x03,     // Register device
    CMND_MSG_DEV_MGNT_REGISTER_DEVICE_CFM       = 0x04,     // Registartion started
    CMND_MSG_DEV_MGNT_REGISTER_DEVICE_IND       = 0x05,     // Indication with success status of the registration
    CMND_MSG_DEV_MGNT_DEREGISTER_DEVICE_REQ     = 0x06,     // De-register device request
    CMND_MSG_DEV_MGNT_DEREGISTER_DEVICE_CFM     = 0x07,     // Deregistration started
    CMND_MSG_DEV_MGNT_DEREGISTER_DEVICE_IND     = 0x08,     // Indication on remote de-registration (removed from base)

    CMND_MSG_DEV_MGNT_LAST
}
t_en_hanCmndMsgDevMgnt;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_IDENTIFY service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_IDENTIFY_DEVICE_REQ            = 0x01,     // Indication to device to identify itself
    CMND_MSG_IDENTIFY_DEVICE_RES            = 0x02,     // Response for identification indication
    CMND_MSG_IDENTIFY_SELF_REQ              = 0x03,     // The unit wishes to identify itself

    CMND_MSG_IDENTIFY_LAST
}
t_en_hanCmndMsgIdentify;

enum
{
    CMND_IE_IDENTIFY_STR_MAX_LEN                = 33        // Max length of String in Identify self information interface
};


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_GENERAL service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_GENERAL_SET_ATTRIB_REQ             = 0x1,      // Set service attribute
    CMND_MSG_GENERAL_SET_ATTRIB_RES             = 0x2,      // Confirmation on set request
    CMND_MSG_GENERAL_GET_ATTRIB_REQ             = 0x3,      // Get service attribute
    CMND_MSG_GENERAL_GET_ATTRIB_RES             = 0x4,      // Contains attribute value
    CMND_MSG_GENERAL_HELLO_IND                  = 0x5,      // First welcome message
    CMND_MSG_GENERAL_ERROR_IND                  = 0x6,      // General error notification
    CMND_MSG_GENERAL_LINK_CFM                   = 0x7,      // General confirmation that message has reached the BS
    CMND_MSG_GENERAL_GET_STATUS_REQ             = 0x8,      // General status req
    CMND_MSG_GENERAL_GET_STATUS_RES             = 0x9,      // General status res
    CMND_MSG_GENERAL_HELLO_REQ                  = 0xa,      // Wakeup request
    CMND_MSG_GENERAL_GET_VERSION_REQ            = 0xb,      // Version request
    CMND_MSG_GENERAL_GET_VERSION_RES            = 0xc,      // Version response
    CMND_MSG_GENERAL_TRANSACTION_START_REQ      = 0xd,      // Start transaction during which CMND will not enter hibernation
    CMND_MSG_GENERAL_TRANSACTION_START_CFM      = 0xe,      // Start transaction command received by CMND and confirmed
    CMND_MSG_GENERAL_TRANSACTION_END_REQ        = 0xf,      // End previous transaction, CMND now can enter hibernation when possible
    CMND_MSG_GENERAL_TRANSACTION_END_CFM        = 0x10,     // Confirmation for end transaction request
    CMND_MSG_GENERAL_LINK_MAINTAIN_START_REQ    = 0x11,     // Request to start maintaining link with base
    CMND_MSG_GENERAL_LINK_MAINTAIN_START_CFM    = 0x12,     // Confirmation for request to start maintaining link with base
    CMND_MSG_GENERAL_LINK_MAINTAIN_STOP_REQ     = 0x13,     // Request to stop maintaining link with base
    CMND_MSG_GENERAL_LINK_MAINTAIN_STOP_CFM     = 0x14,     // Confirmation for request to stop maintaining link with base
    CMND_MSG_GENERAL_LINK_MAINTAIN_STOPPED_IND  = 0x15,     // Link maintain stop indication meaning when link maintain timer has expired and not stopped explicitly by previous command

    CMND_MSG_GENERAL_LOG                        = 0xfe,     // Log message
    CMND_MSG_GENERAL_WAKEUP_REQ                 = 0xff,     // Internal, indicaiton of Wakeup


    CMND_MSG_GENERAL_LAST
}
t_en_hanCmndMsgGeneral;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_GENERAL versions of message CMND_MSG_GENERAL_GET_VERSION_REQ
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_GENERAL_VERSION_SW             = 0x1,
    CMND_GENERAL_VERSION_HW             = 0x2,
    CMND_GENERAL_VERSION_EEPROM         = 0x3,
    CMND_GENERAL_VERSION_CMND_API       = 0x4
}
t_en_hanCmndGeneralVersion;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_ALERT service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_ALERT_GET_ATTRIB_REQ                       = 0x1,      // Read unit attribute
    CMND_MSG_ALERT_GET_ATTRIB_RES                       = 0x2,      // Response for get attribute request
    CMND_MSG_ALERT_NOTIFY_STATUS_REQ                    = 0x3,      // Indication on Alert
    CMND_MSG_ALERT_NOTIFY_STATUS_RES                    = 0x4,      // Response of alert notification
    CMND_MSG_ALERT_SET_ATTRIB_REQ                       = 0x5,      // Set attribute request
    CMND_MSG_ALERT_SET_ATTRIB_RES                       = 0x6,      // Response for previous set attribute request
    CMND_MSG_ALERT_SET_ATTRIB_REQ_WITH_RES              = 0x7,      // Set attribute request with Response
    CMND_MSG_ALERT_GET_ATTRIB_PACK_REQ                  = 0x8,      // Get Attribute Pack Request
    CMND_MSG_ALERT_GET_ATTRIB_PACK_RES                  = 0x9,      // Get Attribute Pack Response
    CMND_MSG_ALERT_GET_ATTRIB_DYN_PACK_REQ              = 0xA,      // Get Attribute Dynamic Pack Request
    CMND_MSG_ALERT_GET_ATTRIB_DYN_PACK_RES              = 0xB,      // Get Attribute Dynamic Pack Response
    CMND_MSG_ALERT_SET_ATTRIB_PACK_REQ                  = 0xC,      // Set Attribute Pack Request
    CMND_MSG_ALERT_SET_ATTRIB_PACK_RES                  = 0xD,      // Set Attribute Pack Response
    CMND_MSG_ALERT_SET_ATTRIB_PACK_REQ_WITH_RES         = 0xE,      // Set Attribute Pack Request with Response
    CMND_MSG_ALERT_ATOMIC_SET_ATTRIB_PACK_REQ           = 0xF,      // Atomic Set Attribute Pack Request
    CMND_MSG_ALERT_ATOMIC_SET_ATTRIB_PACK_RES           = 0x10,     // Atomic Set Attribute Pack Response
    CMND_MSG_ALERT_ATOMIC_SET_ATTRIB_PACK_REQ_WITH_RES  = 0x11,     // Atomic Set Attribute Pack Request with Response


    CMND_MSG_ALERT_LAST
}
t_en_hanCmndMsgAlert;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_TAMPER_ALERT service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_TAMPER_ALERT_GET_ATTRIB_REQ        = 0x1,      // Read unit attribute
    CMND_MSG_TAMPER_ALERT_GET_ATTRIB_RES        = 0x2,      // Response for get attribute request
    CMND_MSG_TAMPER_ALERT_NOTIFY_STATUS_REQ     = 0x3,      // Indication on Tamper Alert
    CMND_MSG_TAMPER_ALERT_NOTIFY_STATUS_RES     = 0x4,      // Response of alert notification
    CMND_MSG_TAMPER_ALERT_GET_ATTRIB_PACK_REQ       = 0x5,      // Get Attribute Pack Request
    CMND_MSG_TAMPER_ALERT_GET_ATTRIB_PACK_RES       = 0x6,      // Get Attribute Pack Response
    CMND_MSG_TAMPER_ALERT_GET_ATTRIB_DYN_PACK_REQ   = 0x7,      // Get Attribute Dynamic Pack Request
    CMND_MSG_TAMPER_ALERT_GET_ATTRIB_DYN_PACK_RES   = 0x8,      // Get Attribute Dynamic Pack Response

    CMND_MSG_TAMPER_ALERT_LAST
}
t_en_hanCmndMsgTamperAlert;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_KEEP_ALIVE service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_KEEP_ALIVE_I_AM_ALIVE_REQ              = 0x01,     // Indicates that device is alive
    CMND_MSG_KEEP_ALIVE_I_AM_ALIVE_RES              = 0x02,     // Response for keep alive request
    CMND_MSG_KEEP_ALIVE_GET_ATTRIB_REQ              = 0x03,     // Get unit attribute
    CMND_MSG_KEEP_ALIVE_GET_ATTRIB_RES              = 0x04,     // Response for Get attribute request
    CMND_MSG_KEEP_ALIVE_SET_ATTRIB_REQ              = 0x05,     // Set unit attribute
    CMND_MSG_KEEP_ALIVE_SET_ATTRIB_RES              = 0x06,     // Confirmation for Set Attribute request
    CMND_MSG_KEEP_ALIVE_SET_ATTRIB_REQ_WITH_RES     = 0x07,     // Set unit attribute mandatory res
    CMND_MSG_KEEP_ALIVE_I_AM_ALIVE_WITH_RSSI_REQ    = 0x08,     // Indicates that device is alive with rssi

    CMND_MSG_KEEP_ALIVE_LAST
}
t_en_hanCmndMsgKeepAlive;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_ON_OFF service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_ONOFF_ON_REQ                   = 0x01,     // Switch unit On
    CMND_MSG_ONOFF_ON_RES                   = 0x02,     // Response to Switch On request

    CMND_MSG_ONOFF_OFF_REQ                  = 0x03,     // Set unit Off
    CMND_MSG_ONOFF_OFF_RES                  = 0x04,     // Response to Switch off request

    CMND_MSG_ONOFF_TOGGLE_REQ               = 0x05,     // Toggle unit status
    CMND_MSG_ONOFF_TOGGLE_RES               = 0x06,     // Response to Toggle request

    CMND_MSG_ONOFF_GET_ATTRIB_REQ           = 0x07,     // Get unit attribute
    CMND_MSG_ONOFF_GET_ATTRIB_RES           = 0x08,     // Response for Get attribute request with attribute data

    CMND_MSG_ONOFF_SET_ATTRIB_REQ           = 0x09,     // Set unit attribute
    CMND_MSG_ONOFF_SET_ATTRIB_RES           = 0x0A,     // Response for Set attribute request with attribute data
    CMND_MSG_ONOFF_SET_ATTRIB_REQ_WITH_RES  = 0x0B,     // Set unit attribute with response


    CMND_MSG_ONOFF_LAST
}
t_en_hanCmndMsgOnOff;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_THERMOSTAT service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_THERMOSTAT_GET_ATTRIB_REQ              = 0x01,
    CMND_MSG_THERMOSTAT_GET_ATTRIB_RES              = 0x02,
    CMND_MSG_THERMOSTAT_SET_ATTRIB_REQ              = 0x03,
    CMND_MSG_THERMOSTAT_SET_ATTRIB_REQ_WITH_RES     = 0x04,
    CMND_MSG_THERMOSTAT_SET_ATTRIB_RES              = 0x05,
    CMND_MSG_THERMOSTAT_LAST
}
t_en_hanCmndMsgThermostat;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_TEMPERATURE service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_TEMPERATURE_GET_ATTRIB_REQ             = 0x01,
    CMND_MSG_TEMPERATURE_GET_ATTRIB_RES             = 0x02,
    CMND_MSG_TEMPERATURE_SET_ATTRIB_REQ             = 0x03,
    CMND_MSG_TEMPERATURE_SET_ATTRIB_REQ_WITH_RES    = 0x04,
    CMND_MSG_TEMPERATURE_SET_ATTRIB_RES             = 0x05,
    CMND_MSG_TEMPERATURE_LAST
}
t_en_hanCmndMsgTemperature;




///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_FUN service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_FUN_SEND_REQ               = 0x1,
    CMND_MSG_FUN_RECV_IND               = 0x2,

    CMND_MSG_FUN_LAST
}
t_en_hanCmndMsgFun;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_DEBUG service messgaes
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_DEBUG_ECHO_ON              = 0x01,     // Turn Debug Echo On
    CMND_MSG_DEBUG_ECHO_OFF             = 0x02,     // Turn Debug Echo Off
    CMND_MSG_DEBUG_ECHO_REPLY           = 0x03,     // Debug Reply
    CMND_MSG_DEBUG_EPROM_WR_NOTIFY_REQ  = 0x04,     // Eeprom Write Notification
    CMND_MSG_DEBUG_EPROM_WR_IND         = 0x05,     // Eeprom Write Notification
    CMND_MSG_DEBUG_EPROM_WR_NOTIFY_CFM  = 0x06,     // Eeprom Write Notification Confirmation
    CMND_MSG_DEBUG_LAST
}
t_en_hanCmndMsgDebug;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_SLEEP service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_SLEEP_ENTER_SLEEP_REQ      = 0x1,      // Enter sleep mode
    CMND_MSG_SLEEP_ENTER_SLEEP_CFM      = 0x2,      // Confirmation for starting handling of Sleep request

    CMND_MSG_SLEEP_LAST
}
t_en_hanCmndMsgSleep;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_SYSTEM service messages
///////////////////////////////////////////////////////////////////////////////

typedef enum
{
    CMND_MSG_SYS_BATTERY_MEASURE_GET_REQ    = 0x1,      // Get battery measurement request
    CMND_MSG_SYS_BATTERY_MEASURE_GET_RES    = 0x2,      // Resposne with battery measurement
    CMND_MSG_SYS_RSSI_GET_REQ               = 0x3,      // Get RSSI value
    CMND_MSG_SYS_RSSI_GET_RES               = 0x4,      // Response with RSSI value
    CMND_MSG_SYS_BATTERY_IND_ENABLE_REQ     = 0x5,      // Enable low battery feature
    CMND_MSG_SYS_BATTERY_IND_DISABLE_REQ    = 0x6,      // Disable low battery feature
    CMND_MSG_SYS_BATTERY_IND_LOW_IND        = 0x7,      // Inidcation on Low Battery Detection
    CMND_MSG_SYS_RESET_REQ                  = 0x8,      // Reset Device Request
    CMND_MSG_SYS_BATTERY_END_LIFE_IND       = 0x9,      // Battery End Of Life Indication
}
t_en_hanCmndMsgSystem;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_SUOTA service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_SUOTA_NEW_SW_AVAIALBE_IND                      = 0x1,      // DECT->MCU: new version available on base
    CMND_MSG_SUOTA_NEW_SW_RES                               = 0x2,      // MCU->DECT: response for 'new version available'
    CMND_MSG_SUOTA_DOWNLOAD_START_REQ                       = 0x3,      // MCU->DECT: Start downloading SUOTA image to DECT flash
    CMND_MSG_SUOTA_DOWNLOAD_START_RES                       = 0x4,
    CMND_MSG_SUOTA_IMAGE_READY_IND                          = 0x5,
    CMND_MSG_SUOTA_IMAGE_READY_RES                          = 0x6,
    // not used                                             = 0x6,
    // not used                                             = 0x7,
    CMND_MSG_SUOTA_UPGRADE_COMPLETED_REQ                    = 0x8,
    CMND_MSG_SUOTA_UPGRADE_COMPLETED_RES                    = 0x9,
    CMND_MSG_SUOTA_READ_FILE_REQ                            = 0xA,
    CMND_MSG_SUOTA_READ_FILE_RES                            = 0xB,
    CMND_MSG_SUOTA_DOWNLOAD_ABORT_REQ                       = 0xC,
    CMND_MSG_SUOTA_DOWNLOAD_ABORT_RES                       = 0xD,
    CMND_MSG_SUOTA_CHECK_FOR_NEW_VERSION_REQ                = 0xE,      // MCU->DECT: check the existence of new software
    CMND_MSG_SUOTA_CHECK_FOR_NEW_VERSION_RES                = 0xF,      // DECT->MCU: server indicate if there is any newer version
}
t_en_hanCmndSuota;


///////////////////////////////////////////////////////////////////////////////
// CMND_MSG_SUOTA_IMAGE_READY_IND return codes
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    // General return codes
    CMND_RC_SUOTA_SUCCESS                   = 0x00,     // General success
    CMND_RC_SUOTA_ERR                       = 0x01,     // General failure
    CMND_RC_SUOTA_ERR_SESSION               = 0x02,
    CMND_RC_SUOTA_ERR_GMEP_SEND             = 0x03,
    CMND_RC_SUOTA_ERR_WRITE                 = 0x04,
    CMND_RC_SUOTA_ERR_ERASE                 = 0x05,
    CMND_RC_SUOTA_ERR_CHECKSUM              = 0x06,
    CMND_RC_SUOTA_ERR_FLASH                 = 0x07,
    CMND_RC_SUOTA_ERR_CALL_ENDED            = 0x08,
    CMND_RC_SUOTA_ERR_HTTP                  = 0x09,
    CMND_RC_SUOTA_ERR_VER_UNCHANGED         = 0x0A,
    CMND_RC_SUOTA_LAST
}
t_en_hanCmndSuotaRc;



///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_REMOTE_CONTROL service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_SEND_CONTROL_REQ                       = 0x1,

}
t_en_hanCmndRemoteControl;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_ULE_VOICE_CALL service messages
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_MSG_ULE_VOICE_CALL_START_IND               =               0x01,
    CMND_MSG_ULE_VOICE_CALL_START_RES               =               0x02,
    CMND_MSG_ULE_VOICE_CALL_END_IND                 =               0x03,
    CMND_MSG_ULE_VOICE_CALL_END_RES                 =               0x04,
    CMND_MSG_ULE_VOICE_CALL_ACTIVE_REQ              =               0x05,
    CMND_MSG_ULE_VOICE_CALL_ACTIVE_RES              =               0x06,
    CMND_MSG_ULE_VOICE_CALL_CODEC_REQ               =               0x07,
    CMND_MSG_ULE_VOICE_CALL_CODEC_RES               =               0x08,
    CMND_MSG_ULE_VOICE_CALL_START_REQ               =               0x09,
    CMND_MSG_ULE_VOICE_CALL_START_CFM               =               0x0A,
    CMND_MSG_ULE_VOICE_CALL_END_REQ                 =               0x0B,
    CMND_MSG_ULE_VOICE_CALL_END_CFM                 =               0x0C,
    CMND_MSG_ULE_VOICE_CALL_RELEASE_IND             =               0x0D,
    CMND_MSG_ULE_VOICE_CALL_SET_VOLUME_REQ          =               0x0E,
    CMND_MSG_ULE_VOICE_CALL_SET_VOLUME_CFM          =               0x0F,
    CMND_MSG_ULE_VOICE_CALL_VOLUME_UP_REQ           =               0x10,
    CMND_MSG_ULE_VOICE_CALL_VOLUME_UP_CFM           =               0x11,
    CMND_MSG_ULE_VOICE_CALL_VOLUME_DOWN_REQ         =               0x12,
    CMND_MSG_ULE_VOICE_CALL_VOLUME_DOWN_CFM         =               0x13,
    CMND_MSG_ULE_VOICE_CALL_CONNECTED_IND                   =                               0x14,
    CMND_MSG_ULE_VOICE_CALL_STATUS_UPDATE_REQ               =                               0x15,
}
t_en_hanCmndVoiceCall;

///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_BG_REQ
//
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_MuxInput;
    u8  u8_ResistorFactor;
    u16 u16_SupplyVolt;
}
t_st_hanCmndIeBandGap;


///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_BG_RES
//
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u32 u32_AdcInput;
    u32 u32_PorInput;
}
t_st_hanCmndIeBandGapRes;


///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_ATE_CONT_REQ
//
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_SlotType;        // Sets the packet length of the slot, default should be set to Full slot(320 BIT b-field) Slot - Full=1, Double=2 or Long=3
    u8  u8_TxRx;        // Tx = 't', Rx='r'
    u8  u8_Carrier;     // Sets the frequency of the carrier, the default 0X00 will set the carrier to 1897.344MHz at EU band Carrier, 2 digit
    u8  u8_Ant;         //
}
t_st_hanCmndIeAteContReq;



///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_ATE_RX_REQ
//
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_SlotType;            // Sets the packet length of the slot, default should be set to Full slot(320 BIT b-field) Slot - Full=1, Double=2 or Long=3
    u8  u8_PPSyncPatter;    //
    u8  u8_Slot_Number;    // Sets the frequency of the carrier, the default 0X00 will set the carrier to 1897.344MHz at EU band Carrier, 2 digit
    u8  u8_Carrier;        //
    u8  u8_Ant;
    u8  u8_BerFerEnable;
    u8  u8_NumOfFrames;   //Number of frames for ber measurement
}
t_st_hanCmndIeAteRxReq;



///////////////////////////////////////////////////////////////////////////////
// Information Element:  7.31   CMND_IE_ATE_RX_RES
//
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u16 u16_Ber;
    u8  u8_Fer;
    u8  u8_Rssi;
}
t_st_hanCmndIeAteRxRes;


///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_ATE_TX_REQ
//
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_SlotType;        // Sets the packet length of the slot, default should be set to Full slot(320 BIT b-field) Slot - Full=1, Double=2 or Long=3
    u8  u8_Preamble;        // 'y'=normal - other long
    u8  u8_Slot_Number;    // Sets the frequency of the carrier, the default 0X00 will set the carrier to 1897.344MHz at EU band Carrier, 2 digit
    u8  u8_Carrier;        //
    u8  u8_PowerLevel;         //
    u8  u8_Ant;
    u8  u8_Pattern;

}
t_st_hanCmndIeAteTxReq;


/*****************************************************************************
 * Define general CMND API enums, service attributes and the attributes values
 *****************************************************************************/


///////////////////////////////////////////////////////////////////////////////
// Values for CMND_IE_RESPONSE :: Result parameter
// see t_st_hanCmndIeResponse
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    // General return codes
    CMND_RC_OK                          = 0x00,     // General success
    CMND_RC_FAIL                        = 0x01,     // General failure
    CMND_RC_NOT_REGISTERED              = 0x02,     // Device not registered, message cannot be handled
    CMND_RC_FAIL_NOT_SUPPORTED          = 0x03,     // Function not supported
    CMND_RC_FAIL_READ_ONLY_ATTRIBUTE    = 0x04,     // Attribute is read only
    CMND_RC_FAIL_ERR_CHKSUM             = 0x05,     // Error in checksum
    CMND_RC_FAIL_INVALID_COOKIE         = 0x06,     // Invalid cookie
    CMND_RC_FAIL_UNKNOWN_MSG_ID         = 0x07,     // Unknown service id
    CMND_RC_FAIL_UNKNOWN_SERVICE_ID     = 0x08,     // Unknown service id
    CMND_RC_FAIL_IE_MISSING             = 0x09,     // Missing Information element in CMND API message
    CMND_RC_FAIL_INVALID_SIZE           = 0x0a,     // Received invalid data size
    CMND_RC_FAIL_INVALID_VALUE          = 0x0b,     // Invalid payload
    CMND_RC_FAIL_INVALID_LENGTH         = 0x0c,     // Invalid length
    CMND_RC_FAIL_INVALID_DEVICE_ID      = 0x0d,     // Received wrong device id
    CMND_RC_FAIL_OUT_OF_RANGE           = 0x0e,     // Value, index, array access is out of range
    CMND_RC_FAIL_OPERATION_TIMEOUT      = 0x0f,     // Operation has timedout without completing the full flow
    CMND_RC_FAIL_OPERATION_NO_AVALABLE  = 0x10,     // Requested operation is not supported/available
    CMND_RC_FAIL_INVALID_UNIT_ID        = 0x11,     // Received wrong unit id
    CMND_RC_FAIL_SEND_LOCK              = 0x12,     // Failure in sending ota message. Faliure on locking stage.
    CMND_RC_FAIL_SEND_TRANSMISSION      = 0x13,     // Failure in sending ota message. Faliure on Transmission stage ( meaning lock was successful ).
    CMND_RC_FAIL_NO_RESOURCES           = 0xFE,
    CMND_RC_FAIL_UNKNOWN_REASON         = 0xFF,

    // values for response of CMND_MSG_DEV_MGNT_REGISTER_DEVICE_REQ
    CMND_RC_DEV_MGR_DECT_REG_FAILED                 = 0x01,
    CMND_RC_DEV_MGR_FUN_REG_FAILED_UNKNOWN          = 0x02,
    CMND_RC_DEV_MGR_FUN_REG_FAILED_NO_RESOURCE      = 0x03,
    CMND_RC_DEV_MGR_ULE_CAPS_REG_FAILED             = 0x04,
    CMND_RC_DEV_MGR_DECT_REG_FALLBACK               = 0x05,
    CMND_RC_DEV_MGR_ULE_REG_FAILED_WITH_FALLBACK    = 0x06,
    CMND_RC_DEV_MGR_FUN_REG_FAILED_WITH_FALLBACK    = 0x07,
    //CMND_RC_DEV_MGR_FUN_REG_FAILED_NOT_AUTHORIZED  - Value is used also for reponse of CMND_MSG_DEV_MGNT_DEREGISTER_DEVICE_REQ
    CMND_RC_DEV_MGR_FUN_REG_FAILED_NOT_AUTHORIZED   = 0x08,
    CMND_RC_DEV_MGR_REG_FAILED_ALREADY_ONGOING      = 0x09,

    CMND_RC_LAST
}
t_en_hanCmndRc;

///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_GENERAL service attribute list,
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_ATTRIB_GENERAL_MSG_FORMAT      = 0x01,             // Message Format,  see t_en_hanCmndGeneralMsgFormat
    CMND_ATTRIB_GENERAL_REGISTRATION    = 0x02,             // Registration status

    CMND_ATTRIB_GENERAL_LAST
}
t_en_hanCmndAttribGeneral;


///////////////////////////////////////////////////////////////////////////////
// CMND_ATTRIB_GENERAL_MSG_FORMAT attribute value,
// see t_en_hanCmndAttribGeneral
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_GENERAL_REGULAR_MSG_FORMAT     = 0x1,          // Use subset messaging via Service Ids (everything bu the FUN Service id)
    CMND_GENERAL_FUN_MSG_FORMAT         = 0x2,          // Use superset messaging mode, meaning use FUN OTA content only.

    CMND_GENERAL_MSG_FORMAT_LAST
}
t_en_hanCmndGeneralMsgFormat;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_ON_OFF service attribute list
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_ATTRIB_ONOFF_STATE = 0x1,                      // Device State, see t_en_hanCmndOnOffState

    CMND_ATTRIB_ONOFF_LAST
}
t_en_hanCmndAttribOnOff;

///////////////////////////////////////////////////////////////////////////////
// CMND_ATTRIB_ONOFF_STATE attribute values, see CMND_ATTRIB_ONOFF_STATE
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_ONOFF_STATE_OFF    = 0x0,
    CMND_ONOFF_STATE_ON     = 0x1,
}
t_en_hanCmndOnOffState;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_ALERT service attirbute list
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_ATTRIB_ALERT_STATE     = 0x01,                     // Alert bit mask
    CMND_ATTRIB_ALERT_ENALBE    = 0x02,                     // Alert is enabled or disabled (mask)

    CMND_ATTRIB_ALERT_LAST
}
t_en_hanCmndAttribAlert;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_TAMPER_ALERT service attirbute list
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_ATTRIB_TAMPER_ALERT_STATE = 0x01,                      // Tamper Alert bit mask

    CMND_ATTRIB_TAMPER_ALERT_LAST
}
t_en_hanCmndAttribTamperAlert;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_BATTERY_LOW service attirbute list
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_ATTRIB_BATTERY_STATUS  = 0x0,      // Battery status, see t_en_hanCmndBatteryStatusType

    CMND_ATTRIB_BATTERY_LAST
}
t_en_hanCmndAttribBattery;


///////////////////////////////////////////////////////////////////////////////
// Values for t_en_hanCmndAttribBattery :: CMND_ATTRIB_BATTERY_STATUS attribute
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_BATTERY_STATUS_LOW = 0x0,          // low battery
    CMND_BATTERY_STATUS_OK  = 0x1,          // battery is ok
}
t_en_hanCmndBatteryStatusType;


///////////////////////////////////////////////////////////////////////////////
// CMND_SERVICE_ID_KEEP_ALIVE service attirbute list
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_ATTRIB_KEEP_ALIVE_INTERVAL = 0x1,              // keep alive interval in milliseconds resolution, U32 type

    CMND_ATTRIB_KEEP_ALIVE_LAST
}
t_en_hanCmndAttribKeepAlive;



///////////////////////////////////////////////////////////////////////////////
// Values for CMND_IE_SLEEP_INFO :: u8_PowerUpMode field
// see t_st_hanCmndIeSleepInfo
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_SLEEP_NORMAL_POWER_UP      = 0x0,          // power up in normal mode
    CMND_SLEEP_SAVE_MODE_POWER_UP   = 0x1,          // power up in safe mode
    CMND_SLEEP_PRODUCTION_POWER_UP  = 0x2,          // power up in Production Mode
    CMND_SLEEP_RF_TEST_POWER_UP     = 0x3           // power up in RF Test Mode
}
t_en_hanCmndSleepPowerUpMode;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


// Define CMND Information Elements
/*****************************************************************************
 * IE - Param structures and enumerations
 *****************************************************************************/

// IE structure is Type(8bit), Length(16bit), Value(byte array)
#define CMND_IE_TYPE_SIZE           (1)
#define CMND_IE_LENGTH_SIZE         (2)
#define CMND_IE_HEADER_SIZE         (CMND_IE_TYPE_SIZE + CMND_IE_LENGTH_SIZE)

///////////////////////////////////////////////////////////////////////////////
// Information element types
// ORDER MUST NOT BE CHANGED, ALL VALUES MUST BE SEQUENTIAL
// CODE IN OTHER FILES ASSUME THE ORDER OF THESE DEFINES
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_IE_RESPONSE                    = 0x00,             // Response (ok or error) used in response events
    CMND_IE_ATTRIBUTE_ID                = 0x01,             // Service attribute id
    CMND_IE_ATTRIBUTE_VALUE             = 0x02,             // Service Attribute id + response code + value
    CMND_IE_UNIT_ADDR                   = 0x03,             // HAN unique address: device id + unit id
    CMND_IE_RESPONSE_REQUIRED           = 0x04,             // FUN response requirement
    CMND_IE_FUN                         = 0x05,             // Holds data and length
    CMND_IE_ALERT                       = 0x06,             // Alert status
    CMND_IE_SLEEP_INFO                  = 0x07,             // Holds a request for Safe mode power up
    CMND_IE_REGISTRATION                = 0x08,             // Contains info of Device and its units and the supported interfaces
    CMND_IE_VERSION                     = 0x09,             // Constains CMND API version
    CMND_IE_BATTERY_LEVEL               = 0x0A,             // Contains battery level value
    CMND_IE_PARAMETER                   = 0x0B,             // EEPROM parameter
    CMND_IE_PARAMETER_DIRECT            = 0x0C,             // Direcect access parameter
    CMND_IE_GENERAL_STATUS              = 0x0D,             // Holds sleep readiness state
    CMND_IE_DEREGISTRATION              = 0x0E,             // Devide deregistration details
    CMND_IE_OTA_COOKIE                  = 0x13,             // Conveys OTA message sequence / cookie
    CMND_IE_CREATE_ATTR_REPORT_EVENT    = 0x14,             // Attribute Reporting Create Event Report
    CMND_IE_ATTR_ADD_REPORT_ENTRY       = 0x15,             // Attribute Reporting Add Report Entry (common for Event and Periodic)
    CMND_IE_CREATE_ATTR_REPORT_RESPONSE = 0x16,             // Create Attribute Reporting Response
    CMND_IE_ATTR_DELETE_REPORT          = 0x17,             // Attribute Reporting Delete
    CMND_IE_ATTR_REPORT_NOTIF           = 0x18,             // Attribute Reporting Periodic/Event Notification
    CMND_IE_REGISTRATION_RESPONSE       = 0x1C,             // Response for registration request
    CMND_IE_TAMPER_ALERT                = 0x1D,             // Tamper Alert status
    CMND_IE_U8                          = 0x1E,             // General purpose 8bit value
    CMND_IE_BATTERY_MEASURE_INFO        = 0x1F,             // Battery requests information structure
    CMND_IE_IDENTIFY                    = 0x23,             // Identification for device
    CMND_IE_U32                         = 0x24,             // General purpose 32bit value
    CMND_IE_BG_REQ                      = 0x25,             // Band Gap Request
    CMND_IE_BG_RES                      = 0x26,             // Band Gap Response
    CMND_IE_ATE_CONT_REQ                = 0x27,             // ATE Continuous Test
    CMND_IE_ATE_RX_REQ                  = 0x28,             // ATE RX Test
    CMND_IE_ATE_RX_RES                  = 0x29,             // ATE RX Test Response
    CMND_IE_ATE_TX_REQ                  = 0x2A,             // ATE TX Test
    CMND_IE_BASE_WANTED                 = 0x2C,             // Contains the parameters of a requested base.
    CMND_IE_REPORT_ID                   = 0x2D,             // Attribute report type and ID
    CMND_IE_ADD_REPORT_INFO             = 0x2E,             // Add report information - multiple report entries to add
    CMND_IE_REPORT_INFO                 = 0x2F,             // Report information - multiple attribute values to report
    CMND_IE_CREATE_ATTR_REPORT_PERIODIC = 0x30,             // Attribute Reporting Create Periodic Report
    CMND_IE_REPORT_ENTRIES              = 0x31,             // Attribute Reporting Get Periodic/Event Report Entries Response,
    CMND_IE_NEW_SW_INFO                 = 0x32,             // New Software image info URL / SW / HW
    CMND_IE_CURRENT_SW_INFO             = 0x33,             // Current Software image info  SW/HW
    CMND_IE_IMAGE_TYPE                  = 0x34,             // Image type 0x01=FW (dect) 0x02=Node Host (MCU)
    CMND_IE_SW_VER_INFO                 = 0x35,             // Current Software image info SW only
    CMND_IE_READ_FILE_DATA_RES          = 0x36,             // Read Suota File from Target response
    CMND_IE_READ_FILE_DATA_REQ          = 0x37,             // Read Suota File from Target request
    CMND_IE_U16                         = 0x38,             // General purpose 16bit value
    CMND_IE_PMID                        = 0x39,             // PMID required to set up a ULE service call
    CMND_IE_PORTABLE_IDENTITY           = 0x3A,             // Portable Identity is used in setting up ULE service call messages
    CMND_IE_LOG                         = 0x3B,             //
    CMND_IE_SET_ATTRIBUTE_VALUE         = 0x3C,             // Service Set Attribute id + value
    CMND_IE_DEREGISTRATION_RESPONSE     = 0x3D,             // Response to Deregistration Request
    CMND_IE_GPIO_STATE                  = 0x3E,             // ULE GPIO or Simple gpio identifier and state
    CMND_IE_LINK_MAINTAIN               = 0x3F,             // Link mainatain duration and ping interval
    CMND_IE_ULE_CALL_SETTING            = 0x40,             // ULE Voice call setup parameters
    CMND_IE_UPDATE_REPORT_INTERVAL      = 0x41,             // Attribute Reporting Update Report Interval
    CMND_IE_CHECK_SW_VER_RES            = 0x42,             // Response and SW info in CMND_MSG_SUOTA_CHECK_FOR_NEW_VERSION_RES

    CMND_IE_LAST_TYPE
}
t_en_hanCmndInfoElemType;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_RESPONSE
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_Result;                      // see t_en_hanCmndRc
}
t_st_hanCmndIeResponse;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_UNIT_ADDR
// Unit Address is a pair of Device address and Unit address
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u16     u16_DeviceId;               // Destination/Source device address
    u8      u8_UnitId;                  // Destination/Srouce unit address
}
t_st_hanCmndIeUnitAddr;

///////////////////////////////////////////////////////////////////////////////
// Enum of all FUN message types. This field is used in t_st_hanCmndIeFun
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_FUN_MSG_TYPE_COMMAND                           = (0x01),       // A command id for with reference to selected interface
    CMND_FUN_MSG_TYPE_COMMAND_WITH_RESPONSE             = (0x02),       // A command which requires Response from the destination device
    CMND_FUN_MSG_TYPE_COMMAND_RESPONSE                  = (0x03),       // A response to the command from destination device
    CMND_FUN_MSG_TYPE_ATTRIBUTE_GET                     = (0x04),       // Get attribute for some interface
    CMND_FUN_MSG_TYPE_ATTRIBUTE_GET_RESPONSE            = (0x05),       // A response for Get Attribute message
    CMND_FUN_MSG_TYPE_ATTRIBUTE_SET                     = (0x06),       // Set an attribute for some interface
    CMND_FUN_MSG_TYPE_ATTRIBUTE_SET_WITH_RESPONSE       = (0x07),       // Set an attribute and request Response for this operation
    CMND_FUN_MSG_TYPE_ATTRIBUTE_SET_RESPONSE            = (0x08),       // A response for Set Attribute or Set attribute with response
    CMND_FUN_MSG_TYPE_GET_ATTRIBUTE_PACK                = (0x09),       // Read one or more parameters of interface in one message
    CMND_FUN_MSG_TYPE_GET_ATTRIBUTE_PACK_RESPONSE       = (0x0A),       // A response to get attribute pack request
    CMND_FUN_MSG_TYPE_SET_ATTRIBUTE_PACK                = (0x0B),       // Set one or more attributes in one message
    CMND_FUN_MSG_TYPE_SET_ATTRIBUTE_PACK_WITH_RESPONSE  = (0x0C),       // Set one or more attribute in one message and require response
    CMND_FUN_MSG_TYPE_SET_ATTRIBUTE_PACK_RESPONSE       = (0x0D),       // A response to Set attribute pack command

    CMND_FUN_MSG_TYPE_LAST
}
t_en_hanCmndFunMsgType;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_FUN
// This IE contains a raw FUN messsage as payload
///////////////////////////////////////////////////////////////////////////////

// Max size of FUN IE data payload in bytes
// this value is derived on max size of FUN OTA application Data field
// ( 24 minus the "application header fields" which is 7, equals 17 )
#define CMND_IE_FUN_MAX_DATA_SIZE       (128)

enum
{
    CMND_IE_FUN_HEADER_SIZE = 16,
};

typedef PACK_STRUCT
{
    u8      u8_ExtendedMode;            // 0: Non Extended Mode, 1: Extended Mode
    u16     u16_SrcDeviceId;
    u8      u8_SrcUnitId;

    u8      u8_AddressType;             // 0: Individual Address, 1: Group Address (not for phase 1 product)
    u16     u16_DstDeviceId;
    u8      u8_DstUnitId;

    u8      u8_MessageSequence;         // will be returned in the response
    u8      u8_MessageType;             // see t_en_hanCmndFunMsgType
    u8      u8_InterfaceType;           // 0 - client, 1- sever
    u16     u16_InterfaceId;            // FUN interface id
    u8      u8_InterfaceMember;         // depending on message type, Command or attribute id

    u16     u16_DataLen;
    u8      pu8_Data[CMND_IE_FUN_MAX_DATA_SIZE];
}
t_st_hanCmndIeFun;

///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_ATTRIBUTE_ID
// Unit attribute
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8  u8_AttributeId;
}
t_st_hanCmndIeAttributeId;

///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_ATTRIBUTE_VALUE
///////////////////////////////////////////////////////////////////////////////
enum
{
    CMND_IE_ATTRIBUTE_VALUE_HEADER_SIZE = 3,
};

typedef PACK_STRUCT
{
    u8      u8_AttributeId;                                             // Unique attribute id
    u8      u8_ResponseValue;                                           // Operation result value
    u8      u8_DataLength;                                              // Size of value in bytes
    u8      pu8_Data[1];                                                // Buffer which contains the value
}
t_st_hanCmndIeAttributeValue;


///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_ALERT
// Alert bit mask
// 0x0000= All Alerts Idle
// 0x0001= Alert 0 is Alerting
// 0x0021= Alerts 0 & 5 are Alerting, all other Idle
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u16     u16_UnitType;               // Unit type of the unit which implements this event
    u32     u32_AlertState;             // Alert bit mask
}
t_st_hanCmndIeAlert;

// Alert state usefull values
// Alert state is actually U32 bitmask, and the values below are default predined values
typedef enum
{
    HAN_CMND_ALERT_STATE_IDLE           = 0x0,
    HAN_CMND_ALERT_STATE_ALERTING       = 0x1,
    HAN_CMND_ALERT_STATE_UNKNOWN        = 0x2,
}
t_en_hanCmndIeAlert;


///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_SLEEP_INFO
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8      u8_PowerUpMode;         // Power up mode on next power up, see t_en_hanCmndSleepPowerUpMode
}
t_st_hanCmndIeSleepInfo;

///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_SLEEP_RESPOSE
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8      u8_SleepReady;          // Sleep readiness, see t_en_hanCmndSleepReadyMode
}
t_st_hanCmndIeSleepResponse;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_BASE_WANTED
// Contains the parameters of a requested base : RFPI
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8                      u8_Rfpi[CMND_API_RFPI_SIZE];
}
t_st_hanCmndIeBaseWanted;


///////////////////////////////////////////////////////////////////////////////
// Parameter types for CMND_IE_VERSION
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_IE_VERSION_VERSION_MAX_LEN =       32,                     // CMND module version with build max length
}
t_en__hanCmndIeVersionMaxLen;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_VERSION
// Defines CMND API version
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8          u8_lenVersion;                                                      // CMND module version length with build length
    u8          u8_Version[CMND_IE_VERSION_VERSION_MAX_LEN];                        // CMND module version with build
}
t_st_hanCmndIeVersion;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_BATTERY_LEVEL
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u16         u16_BatteryLevel;                       // battery level in milli volts
}
t_st_hanCmndIeBatteryLevel;


///////////////////////////////////////////////////////////////////////////////
// Parameter types for CMND_IE_PARAMETER
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_PARAM_TYPE_EEPROM      = 0,                // Parameter type EEPROM
    CMND_PARAM_TYPE_PRODUCTION  = 1,                // Parameter type Production

    CMND_PARAM_TYPE_LAST
}
t_en_hanCmndParameterType;

///////////////////////////////////////////////////////////////////////////////
// Definition of all CMND parameters
///////////////////////////////////////////////////////////////////////////////

/* Order important,...  entry for fp_ParamSetHandlers*/
typedef enum
{
    CMND_PARAM_EEPROM_RXTUN =                   0x0,
    CMND_PARAM_EEPROM_IPEI =                    0x1,
    CMND_PARAM_EEPROM_TBR6 =                    0x2,
    CMND_PARAM_EEPROM_DECT_CARRIER =            0x3,
    CMND_PARAM_EEPROM_PROD_ENABLE =             0x4,
    CMND_PARAM_EEPROM_EXT_SLOT_TYPE =           0x5,
    CMND_PARAM_EEPROM_FRIENDLY_NAME =           0x6,
    CMND_PARAM_EEPROM_SW_VERISON =              0x7,
    CMND_PARAM_EEPROM_HW_VERISON =              0x8,
    CMND_PARAM_EEPROM_MANUFACTURE_NAME =        0x9,
    CMND_PARAM_EEPROM_INFO_TABLE =              0xa,
    CMND_PARAM_EEPROM_PLUGIN_MAP =              0xb,
    CMND_PARAM_EEPROM_AUX_BG_PROG =             0xc,
    CMND_PARAM_EEPROM_POR_BG_CFG =              0xd,
    CMND_PARAM_EEPROM_DECT_FULL_POWER =         0xe,
    CMND_PARAM_EEPROM_DECT_PA2_COMP =           0xf,
    CMND_PARAM_EEPROM_DECT_SUPPORT_FCC =        0x10,
    CMND_PARAM_EEPROM_DECT_DEVIATION =          0x11,
    CMND_PARAM_EEPROM_HAN_REG_RETRY_TIMEOUT =   0x12,
    CMND_PARAM_EEPROM_HAN_LOCK_MAX_RETRY =      0x13,
    CMND_PARAM_EEPROM_HAN_REG_PIN_CODE =        0x14,
    CMND_PARAM_EEPROM_HAN_ENABLE_AUTO_REG =     0x15,
    CMND_PARAM_EEPROM_HAN_SYS_OFF_USED =        0x16,
    CMND_PARAM_EEPROM_HAN_INFO_LOCATION =       0x17,
    CMND_PARAM_EEPROM_HAN_HBR_OSC =             0x18,
    CMND_PARAM_EEPROM_HAN_RETRANSMIT_URGENT =   0x19,
    CMND_PARAM_EEPROM_HAN_RETRANSMIT_NORMAL =   0x1a,
    CMND_PARAM_EEPROM_HAN_PAGING_CAPS =         0x1b,
    CMND_PARAM_EEPROM_HAN_MIN_SLEEP_TIME =      0x1c,
    CMND_PARAM_EEPROM_HAN_PLUGIN_SUPPORTED =    0x1d,
    CMND_PARAM_EEPROM_DECT_EMC              =   0x1e,
    CMND_PARAM_EEPROM_RSSI_SETTINGS         =   0x1f,
    CMND_PARAM_EEPROM_HAN_GENERAL_FLAGS     =   0x20,
    CMND_PARAM_EEPROM_HAN_HANDLED_EXTERNALLY =  0x21,
    CMND_PARAM_EEPROM_HAN_ACTUAL_RESPONSE_TIME =0x22,
    CMND_PARAM_EEPROM_HAN_DEVICE_ENABLE =       0x23,
    CMND_PARAM_EEPROM_HAN_DEVICE_UID    =       0x24,
    CMND_PARAM_EEPROM_HAN_SERIAL_NUM    =       0x25,
    CMND_PARAM_DEFINED_HF_CORE_RELEASE_VER  =   0x26,
    CMND_PARAM_DEFINED_PROFILE_RELEASE_VER  =   0x27,
    CMND_PARAM_DEFINED_INTERFACE_RELEASE_VER =  0x28,
    CMND_PARAM_EEPROM_HAN_KEEPALIVE_TIMEOUT =   0x29,
    CMND_PARAM_EEPROM_REGISTRATION_STATUS   =   0x2a,
    CMND_PARAM_EEPROM_HAN_HIBERNATION_WATCHDOG =0x2b,
    CMND_PARAM_EEPROM_ULE_GPIO_MAPPING_EVENT =  0x2c,
    CMND_PARAM_EEPROM_ATTR_REPORTING_SUPPORTED =0x2d,

    CMND_PARAM_EEPROM_LAST
}
t_en_hanCmndParameter;

#define CMND_IE_PARAMETER_HEADER_SIZE 4
#define CMND_IE_PARAMETER_DATA_MAX_LENGTH (CMND_API_PAYLOAD_MAX_LENGTH - CMND_IE_HEADER_SIZE - CMND_IE_PARAMETER_HEADER_SIZE)

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_PARAMETER
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8                      e_ParamType;            // Parameter type, see t_en_hanCmndParameterType
    u8                      e_Param;                // Parameter identifier, t_en_hanCmndParameter in case of eeprom / case of produciton
    u16                     u16_DataLen;            // Length of data
    u8                      pu8_Data[CMND_IE_PARAMETER_DATA_MAX_LENGTH];
}
t_st_hanCmndIeParameter;


///////////////////////////////////////////////////////////////////////////////
// Parameter address types for CMND_IE_PARAMETER_DIRECT
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMMD_PARAM_ADDRESS_TYPE_HAN_EEPROM,         // Direct parameter address type EEPROM
    CMND_PARAM_ADDRESS_TYPE_RAM,                // Direct parameter address type RAM
    CMMD_PARAM_ADDRESS_TYPE_DECT_EEPROM,        // Direct parameter address type EEPROM DECT
    CMND_PARAM_ADDRESS_TYPE_DAIF,               // Direct parameter address type DAIF
    CMND_PARAM_ADDRESS_TYPE_LAST
}
t_en_hanCmndParamerterAddrType;

#define CMND_IE_PARAMETER_DIRECT_HEADER_SIZE 7
#define CMND_IE_PARAMETER_DIRECT_DATA_MAX_LENGTH (CMND_API_PAYLOAD_MAX_LENGTH - CMND_IE_HEADER_SIZE - CMND_IE_PARAMETER_DIRECT_HEADER_SIZE)

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_PARAMETER_DIRECT
// This IE is conveys address and data for direct EEPROM or RAM read/write access
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8                      e_AddressType;      // EEPROM or RAM destination address (see t_en_hanCmndParamerterAddrType)
    u32                     u32_Offset;         // EERPOM offset or RAM address
    u16                     u16_DataLen;        // Length of data
    u8                      pu8_Data[CMND_IE_PARAMETER_DIRECT_DATA_MAX_LENGTH];
}
t_st_hanCmndIeParameterDirect;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_GENERAL_STATUS
// Define EEPROM statuses
//////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_GEN_STATUS_EEPROM_READY        = 0x0,
    CMND_GEN_STATUS_EEPROM_NON_VALID    = 0x1,
}
t_en_hanCmnd_GeneralStatus_EepromStatus;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_GENERAL_STATUS
// Define Registration statuses
//////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_GEN_STATUS_REGISTERED          = 0x00,     // Device registered
    CMND_GEN_STATUS_NOT_REGISTERED      = 0x01,     // Not registered
}
t_en_hanCmnd_GeneralStatus_RegistrationStatus;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_GENERAL_STATUS
//////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_PowerupMode;             // see t_en_hanCmndSleepPowerUpMode
    u8  u8_RegStatus;               // see t_en_hanCmnd_GeneralStatus_RegistrationStatus
    u8  u8_EepromStatus;            // see t_en_hanCmnd_GeneralStatus_EepromStatus
    u16 u16_DeviceID;               // Device ID
}
t_st_hanCmndIeGeneralStatus;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_OTA_COOKIE
//
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8  OtaCookie;
}
t_st_hanCmndIeOtaCookie;


///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_ATTR_REPORTING_INTERFACE

///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u16     u16_ReceiverDevAddr;
    u8      u8_ReceiverUnitId;
    u32     u32_PeriodicInterval;
}
t_st_hanCmndIeAttrRepInt;

typedef enum
{
    HAN_CMND_ATTR_REPORTING_MANDATORY = 0x00,
    HAN_CMND_ATTR_REPORTING_DYNAMIC = 0xFF,
}
e_hanCmndIeAttrReInt;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_REPORT_ID
///////////////////////////////////////////////////////////////////////////////

/// A CMND_IE_REPORT_ID struct
typedef PACK_STRUCT
{
    u8  u8_ReportId;    //!< A report id
}
t_st_hanCmndIeReportId;


typedef enum
{
    CHANCMNDAPI_ADDEVENT_REPORT_NUM_ENTRIES     = 3,
    CHANCMNDAPI_ADDEVENT_REPORT_NUM_ATTR        = 3,
    CHANCMNDAPI_ADDEVENT_REPORT_MAX_TOTAL_SIZE  = 100,
    CHANCMNDAPI_ADDEVENT_REPORT_HEADER_SIZE     = 2,
    CHANCMNDAPI_ATTR_REPORT_MAX_VALUE_SIZE      = 4,
    CHANCMNDAPI_ATTR_REPORT_NTF_NUM_ENTRIES     = 3,
    CHANCMNDAPI_ATTR_REPORT_NTF_NUM_ATTR        = 3,
    CHANCMNDAPI_REPORT_ENTRIES_HEADER_SIZE      = 1,
}
e_hanCmndAttrRepNumDefs;

typedef PACK_STRUCT
{
    u8      u8_AttributeId;
    u8      u8_TypeOfReporting;
    u8      u8_AttributeSize;
    u32     u32_AttributeValue;
}
t_st_hanCmndIeAttrCond;

typedef PACK_STRUCT
{
    u16 u16_ReceiverDevAddr;
    u8  u8_ReceiverUnitId;
    u32 u32_PeriodicInterval;
    u8  u8_ReportId; // with Report Type
    u8  u8_NumEntries;
    // Holds the report entries as they are transmitted via fun.
    u8  u8_ReportEntries[CHANCMNDAPI_ADDEVENT_REPORT_MAX_TOTAL_SIZE];
}
t_st_hanCmndIeAddReport;

typedef PACK_STRUCT
{
    u8  u8_ReportId; // with Report Type
    u8  u8_NumEntries;
    // Holds the report entries as they are transmitted via fun.
    u8  u8_ReportEntries[CHANCMNDAPI_ADDEVENT_REPORT_MAX_TOTAL_SIZE];
}
t_st_hanCmndIeAddReportEntries;

typedef enum
{
    CMND_ATTR_REP_RC_OK                             = 0x00,     // OK
    CMND_ATTR_REP_RC_FAIL_NOT_AUTHORIZED            = 0x01,     // Fail: Not authorized
    CMND_ATTR_REP_RC_FAIL_INVALID_ARGUMENT          = 0x02,     // Fail: Invalid argument
    CMND_ATTR_REP_RC_FAIL_NO_RESOURCES              = 0xFE,
    CMND_ATTR_REP_RC_FAIL_UNKNOWN_REASON            = 0xFF,
}
t_en_hanCmndAttrRepRc;

typedef PACK_STRUCT
{
    u8  u8_ResponseCode;                        // see t_en_hanCmndRc
    u8  u8_ReportRules[CHANCMNDAPI_ADDEVENT_REPORT_MAX_TOTAL_SIZE];
}
t_st_hanCmndIeReportEntries;

typedef PACK_STRUCT
{
    u8  u8_ReportId; // with Report Type
}
t_st_hanCmndIeDeleteReport;

typedef PACK_STRUCT
{
    u8  u8_ReportId; // with Report Type
    u32 u32_PeriodicInterval;
}
t_st_hanCmndIeUpdatePeriodicReport;



///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_ATTR_REPORT_RESPONSE
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_Result;                      // see t_en_hanCmndRc
    u8  u8_ReportId; // with Report Type
}
t_st_hanCmndIeAttrRepResponse;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_REPORT_INFO
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_UnitId;
    u16 u16_InterfaceId;
    u8  u8_NumOfAttrib;
    t_st_hanCmndIeAttrCond  st_ReportDataFields[CHANCMNDAPI_ATTR_REPORT_NTF_NUM_ATTR];
}
t_st_hanCmndIeNtfReportEntry;

typedef PACK_STRUCT
{
    u8  u8_ReportId; // with Report Type
    u8  u8_NumOfReportEntries;
    t_st_hanCmndIeNtfReportEntry st_NtfReportEntries[CHANCMNDAPI_ATTR_REPORT_NTF_NUM_ENTRIES];
}
t_st_hanCmndIeReportInfoInd;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_REGISTRATION_RESPONSE
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_ResponseCode;
    u8  u8_DiscriminatorType;       // Proprietary / NonProrprietary
    u16 u16_DeviceAddress;          // Allocated device id
    u16 u16_DiscriminatorValue;     // This field must be present only if DiscriminatorType is 0x01
}
t_st_hanCmndIeRegistrationResponse;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_DEREGISTRATION_RESPONSE
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_ResponseCode;            // 0x00 - Ok    0x08 - Fail: Not authorized    0xFF - Fail: Unknown reason
}
t_st_hanCmndIeDeRegistrationResponse;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_TAMPER_ALERT
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_AlertStatus;             // Alert status On or Off, see t_en_hanCmndTamperAlertStatus
}
t_st_hanCmndIeTamperAlert;

// tamper status enumeration - On or Off
typedef enum
{
    CMND_TAMPER_ALERT_STATUS_OFF        = 0x0,
    CMND_TAMPER_ALERT_STATUS_ON         = 0x1,
    CMND_TAMPER_ALERT_STATUS_UNKNOWN    = 0x2,
}
t_en_hanCmndTamperAlertStatus;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_BATTERY_MEASURE_INFO
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CMND_BAT_MEASURE_MODE_LOAD      = 0x1,      // Measure battery voltage during latest load (tx or rx)
    CMND_BAT_MEASURE_MODE_NOW       = 0x2,      // Measure battery voltage now
}
t_en_hanCmndBatteryMeasureMode;

typedef PACK_STRUCT
{
    u8  u8_MeasurementMode;         // see t_en_hanCmndBatteryMeasureMode
}
t_st_hanCmndIeBatteryMeasureInfo;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_U8
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8          u8_Data;                    // 8bit general data
}
t_st_hanCmndIeU8;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_U16
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u16         u16_Data;                   // 16bit general data
}
t_st_hanCmndIeU16;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_U32
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u32         u32_Data;                   // 32bit general data
}
t_st_hanCmndIeU32;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_IDENTIFY
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8*         pu8_Str;                    // 8bit buffer general data
}
t_st_hanCmndIeIdentify;

#define CMND_MSG_SUOTA_URL_MAX_LENGTH (255) // Support ULE ALLIANCE HF-CR #0012 (MAX URL of 255Bytes).
#define CMND_MSG_SUOTA_VER_MAX_LENGTH (15)

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_NEW_SW_INFO
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_swVerLen;
    u8  swStr[CMND_MSG_SUOTA_VER_MAX_LENGTH];
    u8  u8_hwVerLen;
    u8  hwStr[CMND_MSG_SUOTA_VER_MAX_LENGTH];
    u8  u8_urlStrLen;
    u8  urlStr[CMND_MSG_SUOTA_URL_MAX_LENGTH];
}
t_st_hanCmndIeSuotaNewSwReady;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_CURRENT_SW_INFO
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_swVerLen;
    u8  swStr[CMND_MSG_SUOTA_VER_MAX_LENGTH];
    u8  u8_hwVerLen;
    u8  hwStr[CMND_MSG_SUOTA_VER_MAX_LENGTH];
}
t_st_hanCmndIeSuotaCurrentSw;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_SW_VER_INFO
///////////////////////////////////////////////////////////////////////////////
enum
{
    CMND_IE_SUOTA_SW_VER_HEADER_SIZE    = 2,            // size of t_st_hanCmndIeSuotaSwVer without swStr buffer
    CMND_IE_SUOTA_SW_VER_INFO_VER_LEN   = 32,       // length of SW string
};

typedef PACK_STRUCT
{
    u8  u8_Result;                                  // Result of the upgrade
    u8  u8_swVerLen;
    u8  swStr[CMND_IE_SUOTA_SW_VER_INFO_VER_LEN];       // The new SW version after the successful upgrade
}
t_st_hanCmndIeSuotaSwVer;

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_CHECK_SW_VER_RES
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8  u8_Result;                                  // Result of the check version request
    u8  u8_swVerLen;
    u8  swStr[CMND_MSG_SUOTA_VER_MAX_LENGTH];
    u8  u8_hwVerLen;
    u8  hwStr[CMND_MSG_SUOTA_VER_MAX_LENGTH];
    u8  u8_urlStrLen;
    u8  urlStr[CMND_MSG_SUOTA_URL_MAX_LENGTH];
}
t_st_hanCmndIeSuotaCheckSwVerRes;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_READ_FILE_DATA_RES
///////////////////////////////////////////////////////////////////////////////
#define HHANAPP_SUOTA_PAYLOAD_MAX_LENGTH HHANAPP_CMND_DATA_PAYLOAD_MAX_LENGTH
typedef PACK_STRUCT
{
    u32             u32_Offset;
    u16             u16_Length;
    u8              u8_Data[CMND_API_PAYLOAD_MAX_LENGTH];
}t_st_hanCmndIeFileDataRes;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_READ_FILE_DATA_REQ
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u32             u32_Offset;
    u16             u16_Length;
}t_st_hanCmndIeFileDataReq;

///////////////////////////////////////////////////////////////////////////////
// Information Element:  CMND_IE_SET_ATTRIBUTE_VALUE
///////////////////////////////////////////////////////////////////////////////
enum
{
    CMND_IE_SET_ATTRIBUTE_VALUE_HEADER_SIZE = 2,
};

typedef PACK_STRUCT
{
    u8      u8_AttributeId;                                             // Unique attribute id
    u8      u8_DataLength;                                              // Size of value in bytes
    u8      pu8_Data[1];                                                // Buffer which contains the value
}
t_st_hanCmndIeSetAttributeValue;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_DEREGISTRATION
//
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u16 u16_DeviceAddress;      // Device to be de registered
}
t_st_hanCmndIeDeRegistration;




///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_GPIO_STATE
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u8  u8_GpioId;                      // ULE GPO or Simple GPIO id, see CMND API SPEC.
    u8  u8_GpioState;                   // 0x0 - low, 0x1 - high, 0xFF - unknown
}
t_st_hanCmndIeGpioState;


///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_LINK_MAINTAIN
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    u16         u16_LinkMaintainTime;   // Time to keep Link Maintain, sec
    u16         u16_PingInterval;       // Interval between base pinging, ms
}
t_st_hanCmndIeLinkMaintain;

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

//  The size of the struct is HHANAPP_CMND_DATA_PAYLOAD_MAX_LENGTH + 10 (177). The largest pool is 180.
//  If this struct gets bigger, the largest memory pool size should be changed as well.
typedef struct st_cmndApiMsg
{
    u8      cookie;                         //!< Unique message identifier between CMND and Node Host.
    u8      unitId;                         //!< Node Host -> CMND: identifies source unit. CMND -> Node Host: destination unit ID.
    u16     serviceId;                      //!< Defines an ID for a logical group of messages.
    u8      messageId;                      //!< Message ID under the logical group of messages in the specified Service ID.
    u8      checkSum;                       //!< Checksum field is byte summation from Length to Data (including Length, not including Checksum field).
    u8      data[CMNDLIB_DATA_PAYLOAD_MAX_LENGTH];      //!< Message payload.
    u16     dataLength;                     //!< Length of payload.
    u16     nodeDeviceId;
}
t_st_hanCmndApiMsg;
typedef t_st_hanCmndApiMsg t_st_Msg;

#ifdef _MSC_VER
#pragma pack() //Disable struct packing for MS Visual Studio compiler
#endif

#endif  // _CMND_API_EXPORTED_H
