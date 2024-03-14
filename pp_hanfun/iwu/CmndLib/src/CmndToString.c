/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndToString.h"
#include "CmndApiExported.h"
#include "CmndLib_UserImpl_StringUtil.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Management services
static const char* p_CmndToString_DeviceManagementMessageId     ( u8 u8_MessageId );
static const char* p_CmndToString_AttributeReportingMessageId   ( u8 u8_MessageId );

// General services
static const char* p_CmndToString_GeneralMessageId      ( u8 u8_MessageId );
static const char* p_CmndToString_AlertMessageId        ( u8 u8_MessageId );
static const char* p_CmndToString_TamperAlertMessageId  ( u8 u8_MessageId );
static const char* p_CmndToString_KeepAliveMessageId    ( u8 u8_MessageId );
static const char* p_CmndToString_OnOffMessageId        ( u8 u8_MessageId );
static const char* p_CmndToString_FUNMessageId          ( u8 u8_MessageId );

// System services
static const char* p_CmndToString_SystemMessageId       ( u8 u8_MessageId );
static const char* p_CmndToString_ParametersMessageId   ( u8 u8_MessageId );
static const char* p_CmndToString_SleepMessageId        ( u8 u8_MessageId );
static const char* p_CmndToString_UleVoiceCallMessageId ( u8 u8_MessageId );
static const char* p_CmndToString_ProductionMessageId   ( u8 u8_MessageId );
static const char* p_CmndToString_SuotaMessageId        ( u8 u8_MessageId );

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_CMND_SERVICE_ID_TO_STRING(x) case(CMND_SERVICE_ID_##x):return FLASHSTR(#x)


const char* p_CmndToString_ServiceId( u16 u16_ServiceId )
{
    switch ( u16_ServiceId )
    {
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( GENERAL );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( SYSTEM );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( DEVICE_MANAGEMENT );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( ALERT );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( ATTRIBUTE_REPORTING );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( FUN );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( ON_OFF );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( SUOTA );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( PARAMETERS );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( PRODUCTION );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( SLEEP );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( ULE_VOICE_CALL );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( KEEP_ALIVE );
        ENUM_CASE_CMND_SERVICE_ID_TO_STRING( TAMPER_ALERT );
    }

    return "UNKNOWN CMND_SERVICE_ID";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

const char* p_CmndToString_MessageId( u16 u16_ServiceId, u8 u8_MessageId )
{
    switch ( u16_ServiceId )
    {
        case CMND_SERVICE_ID_GENERAL:
        {
            return p_CmndToString_GeneralMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_SYSTEM:
        {
            return p_CmndToString_SystemMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_DEVICE_MANAGEMENT:
        {
            return p_CmndToString_DeviceManagementMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_ALERT:
        {
            return p_CmndToString_AlertMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_ATTRIBUTE_REPORTING:
        {
            return p_CmndToString_AttributeReportingMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_FUN:
        {
            return p_CmndToString_FUNMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_ON_OFF:
        {
            return p_CmndToString_OnOffMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_SUOTA:
        {
            return p_CmndToString_SuotaMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_PARAMETERS:
        {
            return p_CmndToString_ParametersMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_SLEEP:
        {
            return p_CmndToString_SleepMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_ULE_VOICE_CALL:
        {
            return p_CmndToString_UleVoiceCallMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_PRODUCTION:
        {
            return p_CmndToString_ProductionMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_KEEP_ALIVE:
        {
            return p_CmndToString_KeepAliveMessageId( u8_MessageId );
        }
        case CMND_SERVICE_ID_TAMPER_ALERT:
        {
            return p_CmndToString_TamperAlertMessageId( u8_MessageId );
        }
    }
    return "UNKNOWN CMND_MESSAGE_ID";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_IE_TO_STRING(x)  case(CMND_##x):return FLASHSTR(#x)

const char* p_CmndToString_IeType( u8 u8_IeType )
{
    switch ( u8_IeType )
    {
        ENUM_CASE_IE_TO_STRING( IE_RESPONSE );
        ENUM_CASE_IE_TO_STRING( IE_ATTRIBUTE_ID );
        ENUM_CASE_IE_TO_STRING( IE_ATTRIBUTE_VALUE );
        ENUM_CASE_IE_TO_STRING( IE_UNIT_ADDR );
        ENUM_CASE_IE_TO_STRING( IE_RESPONSE_REQUIRED );
        ENUM_CASE_IE_TO_STRING( IE_FUN );
        ENUM_CASE_IE_TO_STRING( IE_ALERT );
        ENUM_CASE_IE_TO_STRING( IE_SLEEP_INFO );
        ENUM_CASE_IE_TO_STRING( IE_REGISTRATION );
        ENUM_CASE_IE_TO_STRING( IE_VERSION );
        ENUM_CASE_IE_TO_STRING( IE_BATTERY_LEVEL );
        ENUM_CASE_IE_TO_STRING( IE_PARAMETER );
        ENUM_CASE_IE_TO_STRING( IE_PARAMETER_DIRECT );
        ENUM_CASE_IE_TO_STRING( IE_GENERAL_STATUS );
        ENUM_CASE_IE_TO_STRING( IE_DEREGISTRATION );
        ENUM_CASE_IE_TO_STRING( IE_OTA_COOKIE );
        ENUM_CASE_IE_TO_STRING( IE_CREATE_ATTR_REPORT_EVENT );
        ENUM_CASE_IE_TO_STRING( IE_ATTR_ADD_REPORT_ENTRY );
        ENUM_CASE_IE_TO_STRING( IE_CREATE_ATTR_REPORT_RESPONSE );
        ENUM_CASE_IE_TO_STRING( IE_ATTR_DELETE_REPORT );
        ENUM_CASE_IE_TO_STRING( IE_ATTR_REPORT_NOTIF );
        ENUM_CASE_IE_TO_STRING( IE_REGISTRATION_RESPONSE );
        ENUM_CASE_IE_TO_STRING( IE_TAMPER_ALERT );
        ENUM_CASE_IE_TO_STRING( IE_U8 );
        ENUM_CASE_IE_TO_STRING( IE_BATTERY_MEASURE_INFO );
        ENUM_CASE_IE_TO_STRING( IE_IDENTIFY );
        ENUM_CASE_IE_TO_STRING( IE_U32 );
        ENUM_CASE_IE_TO_STRING( IE_BG_REQ );
        ENUM_CASE_IE_TO_STRING( IE_BG_RES );
        ENUM_CASE_IE_TO_STRING( IE_ATE_CONT_REQ );
        ENUM_CASE_IE_TO_STRING( IE_ATE_RX_REQ );
        ENUM_CASE_IE_TO_STRING( IE_ATE_RX_RES );
        ENUM_CASE_IE_TO_STRING( IE_ATE_TX_REQ );
        ENUM_CASE_IE_TO_STRING( IE_BASE_WANTED );
        ENUM_CASE_IE_TO_STRING( IE_REPORT_ID );
        ENUM_CASE_IE_TO_STRING( IE_ADD_REPORT_INFO );
        ENUM_CASE_IE_TO_STRING( IE_REPORT_INFO );
        ENUM_CASE_IE_TO_STRING( IE_CREATE_ATTR_REPORT_PERIODIC );
        ENUM_CASE_IE_TO_STRING( IE_REPORT_ENTRIES );
        ENUM_CASE_IE_TO_STRING( IE_NEW_SW_INFO );
        ENUM_CASE_IE_TO_STRING( IE_CURRENT_SW_INFO );
        ENUM_CASE_IE_TO_STRING( IE_IMAGE_TYPE );
        ENUM_CASE_IE_TO_STRING( IE_SW_VER_INFO );
        ENUM_CASE_IE_TO_STRING( IE_READ_FILE_DATA_RES );
        ENUM_CASE_IE_TO_STRING( IE_READ_FILE_DATA_REQ );
        ENUM_CASE_IE_TO_STRING( IE_U16  );
        ENUM_CASE_IE_TO_STRING( IE_PMID );
        ENUM_CASE_IE_TO_STRING( IE_PORTABLE_IDENTITY );
        ENUM_CASE_IE_TO_STRING( IE_LOG );
        ENUM_CASE_IE_TO_STRING( IE_SET_ATTRIBUTE_VALUE );
        ENUM_CASE_IE_TO_STRING( IE_DEREGISTRATION_RESPONSE );
        ENUM_CASE_IE_TO_STRING( IE_GPIO_STATE );
        ENUM_CASE_IE_TO_STRING( IE_LINK_MAINTAIN );
        ENUM_CASE_IE_TO_STRING( IE_ULE_CALL_SETTING );
        ENUM_CASE_IE_TO_STRING( IE_LAST_TYPE );
    }
    return "UNKNOWN CMND_IE";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_POWERUP_MODE_TO_STRING(x)    case(CMND_SLEEP_##x):return FLASHSTR(#x)

const char* p_CmndToString_PowerupMode( u8 u8_RetCode )
{
    switch(u8_RetCode)
    {
        ENUM_CASE_POWERUP_MODE_TO_STRING( NORMAL_POWER_UP );
        ENUM_CASE_POWERUP_MODE_TO_STRING( PRODUCTION_POWER_UP );
        ENUM_CASE_POWERUP_MODE_TO_STRING( RF_TEST_POWER_UP );
        ENUM_CASE_POWERUP_MODE_TO_STRING( SAVE_MODE_POWER_UP );
    }
    return "UNKNOWN POWERUP MODE";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_RETCODE_TO_STRING(x) case(CMND_RC_##x):return FLASHSTR(#x)

const char* p_CmndToString_RetCode( u8 u8_RetCode )
{
    switch ( u8_RetCode )
    {
        ENUM_CASE_RETCODE_TO_STRING( OK );
        ENUM_CASE_RETCODE_TO_STRING( FAIL );
        ENUM_CASE_RETCODE_TO_STRING( NOT_REGISTERED );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_NOT_SUPPORTED );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_READ_ONLY_ATTRIBUTE );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_ERR_CHKSUM );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_INVALID_COOKIE );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_UNKNOWN_MSG_ID );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_UNKNOWN_SERVICE_ID );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_IE_MISSING );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_INVALID_SIZE );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_INVALID_VALUE );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_INVALID_LENGTH );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_INVALID_DEVICE_ID );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_OUT_OF_RANGE );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_OPERATION_TIMEOUT );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_OPERATION_NO_AVALABLE );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_INVALID_UNIT_ID );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_SEND_LOCK );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_SEND_TRANSMISSION );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_NO_RESOURCES );
        ENUM_CASE_RETCODE_TO_STRING( FAIL_UNKNOWN_REASON );
    }
    return "UNKNOWN CMND_RC";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_PARAM_EEPROM_TO_STRING(x)    case(CMND_PARAM_EEPROM_##x):return FLASHSTR(#x)

char const *p_CmndToString_EepromParameter( u8 u8_Param )
{
    switch ( u8_Param )
    {
        ENUM_CASE_PARAM_EEPROM_TO_STRING( RXTUN );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( IPEI );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( TBR6 );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( DECT_CARRIER );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( PROD_ENABLE );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( EXT_SLOT_TYPE );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( FRIENDLY_NAME );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( SW_VERISON );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HW_VERISON );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( MANUFACTURE_NAME );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( INFO_TABLE );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( PLUGIN_MAP );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( AUX_BG_PROG );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( POR_BG_CFG );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( DECT_FULL_POWER );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( DECT_PA2_COMP );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( DECT_SUPPORT_FCC );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( DECT_DEVIATION );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_REG_RETRY_TIMEOUT );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_LOCK_MAX_RETRY );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_REG_PIN_CODE );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_ENABLE_AUTO_REG );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_SYS_OFF_USED );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_INFO_LOCATION );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_HBR_OSC );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_RETRANSMIT_URGENT );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_RETRANSMIT_NORMAL );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_PAGING_CAPS );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_MIN_SLEEP_TIME );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_PLUGIN_SUPPORTED );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( DECT_EMC );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( RSSI_SETTINGS );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_GENERAL_FLAGS );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_HANDLED_EXTERNALLY );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_ACTUAL_RESPONSE_TIME );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_DEVICE_ENABLE );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_DEVICE_UID );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( HAN_SERIAL_NUM );
        ENUM_CASE_PARAM_EEPROM_TO_STRING( LAST );
    }
    return "UNKNOWN CMND_PARAM_EEPROM";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_DEV_MGNT_TO_STRING(x)    case(CMND_MSG_DEV_MGNT_##x):return FLASHSTR(#x)

static const char* p_CmndToString_DeviceManagementMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_DEV_MGNT_TO_STRING( REGISTER_DEVICE_REQ );
        ENUM_CASE_MSG_DEV_MGNT_TO_STRING( REGISTER_DEVICE_CFM );
        ENUM_CASE_MSG_DEV_MGNT_TO_STRING( REGISTER_DEVICE_IND );
        ENUM_CASE_MSG_DEV_MGNT_TO_STRING( DEREGISTER_DEVICE_REQ );
        ENUM_CASE_MSG_DEV_MGNT_TO_STRING( DEREGISTER_DEVICE_CFM );
        ENUM_CASE_MSG_DEV_MGNT_TO_STRING( DEREGISTER_DEVICE_IND );
        ENUM_CASE_MSG_DEV_MGNT_TO_STRING( GET_ATTRIB_REQ );
        ENUM_CASE_MSG_DEV_MGNT_TO_STRING( GET_ATTRIB_RES );
    }
    return "UNKNOWN CMND_MSG_DEV_MGNT";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_ATTRREP_TO_STRING(x) case(CMND_MSG_ATTRREP_##x):return FLASHSTR(#x)

const char* p_CmndToString_AttributeReportingMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_ATTRREP_TO_STRING( CREATE_PERIODIC_REPORT_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( CREATE_PERIODIC_REPORT_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( CREATE_EVENT_REPORT_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( CREATE_EVENT_REPORT_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( ADDENTRY_PERIODIC_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( ADDENTRY_PERIODIC_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( ADDENTRY_EVENT_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( ADDENTRY_EVENT_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( DELETE_REPORT_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( DELETE_REPORT_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( PERIODIC_REPORT_NOTIFICATION_IND );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( EVENT_REPORT_NOTIFICATION_IND );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( GET_PERIODIC_REPORT_ENTRIES_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( GET_PERIODIC_REPORT_ENTRIES_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( GET_EVENT_REPORT_ENTRIES_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( GET_EVENT_REPORT_ENTRIES_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( ADD_REPORT_IND );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( ADD_REPORT_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( REPORT_NOTIFICATION_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( DELETE_REPORT_IND );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( GET_REPORT_VALUES_IND );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( GET_REPORT_VALUES_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( ADD_REPORT_REQ );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( ADD_REPORT_CFM );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( GET_ATTRIB_RES );
        ENUM_CASE_MSG_ATTRREP_TO_STRING( GET_ATTRIB_REQ );
    }
    return "UNKNOWN CMND_MSG_ATTRREP";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_GENERAL_TO_STRING(x) case(CMND_MSG_GENERAL_##x):return FLASHSTR(#x)

static const char* p_CmndToString_GeneralMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_GENERAL_TO_STRING( GET_ATTRIB_RES );
        ENUM_CASE_MSG_GENERAL_TO_STRING( GET_ATTRIB_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( ERROR_IND );
        ENUM_CASE_MSG_GENERAL_TO_STRING( GET_STATUS_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( GET_STATUS_RES );
        ENUM_CASE_MSG_GENERAL_TO_STRING( GET_VERSION_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( GET_VERSION_RES );
        ENUM_CASE_MSG_GENERAL_TO_STRING( HELLO_IND );
        ENUM_CASE_MSG_GENERAL_TO_STRING( LINK_CFM );
        ENUM_CASE_MSG_GENERAL_TO_STRING( SET_ATTRIB_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( SET_ATTRIB_RES );
        ENUM_CASE_MSG_GENERAL_TO_STRING( TRANSACTION_END_CFM );
        ENUM_CASE_MSG_GENERAL_TO_STRING( TRANSACTION_START_CFM );
        ENUM_CASE_MSG_GENERAL_TO_STRING( HELLO_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( LINK_MAINTAIN_START_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( LINK_MAINTAIN_STOP_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( LINK_MAINTAIN_START_CFM );
        ENUM_CASE_MSG_GENERAL_TO_STRING( LINK_MAINTAIN_STOP_CFM );
        ENUM_CASE_MSG_GENERAL_TO_STRING( LINK_MAINTAIN_STOPPED_IND );
        ENUM_CASE_MSG_GENERAL_TO_STRING( LOG );
        ENUM_CASE_MSG_GENERAL_TO_STRING( TRANSACTION_END_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( TRANSACTION_START_REQ );
        ENUM_CASE_MSG_GENERAL_TO_STRING( WAKEUP_REQ );
    }
    return "UNKNOWN CMND_MSG_GENERAL";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_ALERT_TO_STRING(x)   case(CMND_MSG_ALERT_##x):return FLASHSTR(#x)

static const char* p_CmndToString_AlertMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_ALERT_TO_STRING( NOTIFY_STATUS_RES );
        ENUM_CASE_MSG_ALERT_TO_STRING( NOTIFY_STATUS_REQ );
        ENUM_CASE_MSG_ALERT_TO_STRING( ATOMIC_SET_ATTRIB_PACK_REQ );
        ENUM_CASE_MSG_ALERT_TO_STRING( ATOMIC_SET_ATTRIB_PACK_RES );
        ENUM_CASE_MSG_ALERT_TO_STRING( ATOMIC_SET_ATTRIB_PACK_REQ_WITH_RES );
        ENUM_CASE_MSG_ALERT_TO_STRING( GET_ATTRIB_DYN_PACK_REQ );
        ENUM_CASE_MSG_ALERT_TO_STRING( GET_ATTRIB_DYN_PACK_RES );
        ENUM_CASE_MSG_ALERT_TO_STRING( GET_ATTRIB_PACK_REQ );
        ENUM_CASE_MSG_ALERT_TO_STRING( GET_ATTRIB_PACK_RES );
        ENUM_CASE_MSG_ALERT_TO_STRING( GET_ATTRIB_REQ );
        ENUM_CASE_MSG_ALERT_TO_STRING( GET_ATTRIB_RES );
        ENUM_CASE_MSG_ALERT_TO_STRING( SET_ATTRIB_PACK_REQ );
        ENUM_CASE_MSG_ALERT_TO_STRING( SET_ATTRIB_PACK_RES );
        ENUM_CASE_MSG_ALERT_TO_STRING( SET_ATTRIB_REQ_WITH_RES );
        ENUM_CASE_MSG_ALERT_TO_STRING( SET_ATTRIB_REQ );
        ENUM_CASE_MSG_ALERT_TO_STRING( SET_ATTRIB_RES );
    }
    return "UNKNOWN CMND_MSG_ALERT";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_TAMPER_ALERT_TO_STRING(x)    case(CMND_MSG_TAMPER_ALERT_##x):return FLASHSTR(#x)

static const char* p_CmndToString_TamperAlertMessageId( u8 u8_MessageId )
{
    switch  ( u8_MessageId )
    {
        ENUM_CASE_MSG_TAMPER_ALERT_TO_STRING( NOTIFY_STATUS_RES );
        ENUM_CASE_MSG_TAMPER_ALERT_TO_STRING( NOTIFY_STATUS_REQ );
        ENUM_CASE_MSG_TAMPER_ALERT_TO_STRING( GET_ATTRIB_REQ );
        ENUM_CASE_MSG_TAMPER_ALERT_TO_STRING( GET_ATTRIB_RES );
    }
    return "UNKNOWN CMND_MSG_TAMPER_ALERT";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING(x)  case(CMND_MSG_KEEP_ALIVE_##x):return FLASHSTR(#x)

static const char* p_CmndToString_KeepAliveMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING( GET_ATTRIB_REQ );
        ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING( GET_ATTRIB_RES );
        ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING( SET_ATTRIB_REQ );
        ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING( SET_ATTRIB_RES );
        ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING( I_AM_ALIVE_REQ );
        ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING( I_AM_ALIVE_RES );
        ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING( I_AM_ALIVE_WITH_RSSI_REQ );
        ENUM_CASE_MSG_KEEP_ALIVE_TO_STRING( SET_ATTRIB_REQ_WITH_RES );
    }
    return "UNKNOWN CMND_MSG_KEEP_ALIVE";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_ONOFF_TO_STRING(x)   case(CMND_MSG_ONOFF_##x):return FLASHSTR(#x)

static const char* p_CmndToString_OnOffMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_ONOFF_TO_STRING( ON_REQ );
        ENUM_CASE_MSG_ONOFF_TO_STRING( ON_RES );
        ENUM_CASE_MSG_ONOFF_TO_STRING( OFF_REQ );
        ENUM_CASE_MSG_ONOFF_TO_STRING( OFF_RES );
        ENUM_CASE_MSG_ONOFF_TO_STRING( TOGGLE_REQ );
        ENUM_CASE_MSG_ONOFF_TO_STRING( TOGGLE_RES );
        ENUM_CASE_MSG_ONOFF_TO_STRING( GET_ATTRIB_REQ );
        ENUM_CASE_MSG_ONOFF_TO_STRING( GET_ATTRIB_RES );
        ENUM_CASE_MSG_ONOFF_TO_STRING( SET_ATTRIB_REQ );
        ENUM_CASE_MSG_ONOFF_TO_STRING( SET_ATTRIB_RES );
        ENUM_CASE_MSG_ONOFF_TO_STRING( SET_ATTRIB_REQ_WITH_RES );
    }
    return "UNKNOWN CMND_MSG_ONOFF";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_FUN_TO_STRING(x)   case(CMND_MSG_FUN_##x):return FLASHSTR(#x)

static const char* p_CmndToString_FUNMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_FUN_TO_STRING( SEND_REQ );
        ENUM_CASE_MSG_FUN_TO_STRING( RECV_IND );
    }
    return "UNKNOWN CMND_MSG_FUN";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_SYS_TO_STRING(x) case(CMND_MSG_SYS_##x):return FLASHSTR(#x)

static const char* p_CmndToString_SystemMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_SYS_TO_STRING( BATTERY_IND_LOW_IND );
        ENUM_CASE_MSG_SYS_TO_STRING( BATTERY_MEASURE_GET_REQ );
        ENUM_CASE_MSG_SYS_TO_STRING( BATTERY_MEASURE_GET_RES );
        ENUM_CASE_MSG_SYS_TO_STRING( RSSI_GET_REQ );
        ENUM_CASE_MSG_SYS_TO_STRING( RSSI_GET_RES );
        ENUM_CASE_MSG_SYS_TO_STRING( BATTERY_IND_DISABLE_REQ );
        ENUM_CASE_MSG_SYS_TO_STRING( BATTERY_IND_ENABLE_REQ );
        ENUM_CASE_MSG_SYS_TO_STRING( RESET_REQ );
    }
    return "UNKNOWN CMND_MSG_SYS";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_PARAM_TO_STRING(x)   case(CMND_MSG_PARAM_##x):return FLASHSTR(#x)

static const char* p_CmndToString_ParametersMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_PARAM_TO_STRING( GET_REQ );
        ENUM_CASE_MSG_PARAM_TO_STRING( GET_RES );
        ENUM_CASE_MSG_PARAM_TO_STRING( SET_REQ );
        ENUM_CASE_MSG_PARAM_TO_STRING( SET_RES );
        ENUM_CASE_MSG_PARAM_TO_STRING( GET_DIRECT_REQ );
        ENUM_CASE_MSG_PARAM_TO_STRING( GET_DIRECT_RES );
        ENUM_CASE_MSG_PARAM_TO_STRING( SET_DIRECT_REQ );
        ENUM_CASE_MSG_PARAM_TO_STRING( SET_DIRECT_RES );
    }
    return "UNKNOWN CMND_MSG_PARAM";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING(x)  case(CMND_MSG_ULE_VOICE_CALL_##x):return FLASHSTR(#x)

static const char* p_CmndToString_UleVoiceCallMessageId(u8 u8_MessageId)
{
    switch (u8_MessageId)
    {

        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( START_IND );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( START_RES );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( END_IND );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( END_RES );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( ACTIVE_REQ );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( ACTIVE_RES );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( CODEC_REQ );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( CODEC_RES );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( START_REQ );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( START_CFM );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( END_REQ );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( END_CFM );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( RELEASE_IND );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( SET_VOLUME_REQ );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( SET_VOLUME_CFM );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( VOLUME_UP_REQ );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( VOLUME_UP_CFM );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( VOLUME_DOWN_REQ );
        ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( VOLUME_DOWN_CFM );
                ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( CONNECTED_IND );
                ENUM_CASE_MSG_ULE_VOICE_CALL_TO_STRING( STATUS_UPDATE_REQ );
    }
    return "UNKNOWN CMND_MSG_ULE_VOICE_CALL";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_SLEEP_TO_STRING(x)   case(CMND_MSG_SLEEP_##x):return FLASHSTR(#x)

static const char* p_CmndToString_SleepMessageId(u8 u8_MessageId)
{
    switch (u8_MessageId)
    {

        ENUM_CASE_MSG_SLEEP_TO_STRING( ENTER_SLEEP_REQ );
        ENUM_CASE_MSG_SLEEP_TO_STRING( ENTER_SLEEP_CFM );
    }
    return "UNKNOWN CMND_MSG_SLEEP";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_PROD_TO_STRING(x)    case(CMND_MSG_PROD_##x):return FLASHSTR(#x)

static const char* p_CmndToString_ProductionMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_PROD_TO_STRING( START_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( END_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( CFM );
        ENUM_CASE_MSG_PROD_TO_STRING( REF_CLK_TUNE_START_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( REF_CLK_TUNE_END_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( REF_CLK_TUNE_END_RES );
        ENUM_CASE_MSG_PROD_TO_STRING( REF_CLK_TUNE_ADJ_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( BG_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( BG_RES );
        ENUM_CASE_MSG_PROD_TO_STRING( ATE_INIT_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( ATE_STOP_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( ATE_CONTINUOUS_START_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( ATE_RX_START_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( ATE_RX_START_RES );
        ENUM_CASE_MSG_PROD_TO_STRING( ATE_TX_START_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( ATE_GET_BER_FER_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( INIT_EEPROM_DEF_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( SPECIFIC_PRESET_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( SLEEP_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( SET_SIMPLE_GPIO_LOW );
        ENUM_CASE_MSG_PROD_TO_STRING( SET_SIMPLE_GPIO_HIGH );
        ENUM_CASE_MSG_PROD_TO_STRING( GET_SIMPLE_GPIO_STATE );
        ENUM_CASE_MSG_PROD_TO_STRING( GET_SIMPLE_GPIO_STATE_RES );
        ENUM_CASE_MSG_PROD_TO_STRING( SET_ULE_GPIO_LOW );
        ENUM_CASE_MSG_PROD_TO_STRING( SET_ULE_GPIO_HIGH );
        ENUM_CASE_MSG_PROD_TO_STRING( GET_ULE_GPIO_STATE );
        ENUM_CASE_MSG_PROD_TO_STRING( GET_ULE_GPIO_STATE_RES );
        ENUM_CASE_MSG_PROD_TO_STRING( SET_ULE_GPIO_DIR_INPUT_REQ );
        ENUM_CASE_MSG_PROD_TO_STRING( FW_UPDATE_REQ );
    }
    return "UNKNOWN CMND_MSG_PROD";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define  ENUM_CASE_MSG_SUOTA_TO_STRING(x)   case(CMND_MSG_SUOTA_##x):return FLASHSTR(#x)

static const char* p_CmndToString_SuotaMessageId( u8 u8_MessageId )
{
    switch ( u8_MessageId )
    {
        ENUM_CASE_MSG_SUOTA_TO_STRING( NEW_SW_AVAIALBE_IND );
        ENUM_CASE_MSG_SUOTA_TO_STRING( DOWNLOAD_START_RES );
        ENUM_CASE_MSG_SUOTA_TO_STRING( IMAGE_READY_IND );
        ENUM_CASE_MSG_SUOTA_TO_STRING( READ_FILE_RES );
        ENUM_CASE_MSG_SUOTA_TO_STRING( DOWNLOAD_ABORT_REQ );
        ENUM_CASE_MSG_SUOTA_TO_STRING( DOWNLOAD_START_REQ );
        ENUM_CASE_MSG_SUOTA_TO_STRING( IMAGE_READY_RES );
        ENUM_CASE_MSG_SUOTA_TO_STRING( NEW_SW_RES );
        ENUM_CASE_MSG_SUOTA_TO_STRING( READ_FILE_REQ );
        ENUM_CASE_MSG_SUOTA_TO_STRING( UPGRADE_COMPLETED_REQ );
        ENUM_CASE_MSG_SUOTA_TO_STRING( UPGRADE_COMPLETED_RES );
    }
    return "UNKNOWN CMND_MSG_SUOTA";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
