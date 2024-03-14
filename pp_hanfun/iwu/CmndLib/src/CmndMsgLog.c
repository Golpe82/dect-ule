/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsgLog.h"
#include "CmndApiExported.h"
#include "Endian.h"
#include "CmndApiIe.h"
#include "CmndPacketParser.h"
#include "Logger.h"
#include "CmndToString.h"
#include "CmndApiStringUtil.h"
#include "CmndLib_UserImpl_StringUtil.h"
#include "CmndMsg.h"
#include <string.h>

#define CALL_SETTINGS_STRING_LEN 20

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define PARSE_MSG_IES

// constants
enum
{
    MAX_IE_LENGTH               = 167,  //!< Maximum IE length: type + len + value
    MAX_IE_STR_LENGTH           = 300,  //!< Maximum IE length represented as string
    CMND_PARAM_MAX_STRING_LEN   = 100,  //!< Maximum number of characters in CMND API parameter string value
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/// Internal IE types
typedef enum
{
    CMND_IE_INTERNAL_FIRST_TYPE         = 0x80, //!< First IE type
    CMND_IE_BIND_ENTRY                  = 0x80, //!< All details to add/remove entry from bind table
    CMND_IE_GROUP_ID                    = 0x81, //!< Group id and name
    CMND_IE_GROUP_ENTRY                 = 0x82, //!< Single entry in group table
    CMND_IE_GROUP_INFO                  = 0x83, //!< Group Information - all device-units in the table
    CMND_IE_ATTRIBUTE_PACK_VALUE        = 0x85, //!< Values for pack of attributes
    CMND_IE_ATTRIBUTE_ID_LIST           = 0x86, //!< List of Attribute Ids
    CMND_IE_ATTRIBUTE_SET_PACK_RESPONSE = 0x87, //!< List of attribute Ids and the success/fail status for set pack request
    CMND_IE_RFTESTMODE_ENABLE           = 0x88, //!< RF Test Mode Enable / Disable
    CMND_IE_RFTESTMODE_STAT             = 0x89, //!< RF Test Mode Statistics
    CMND_IE_OTA_CONTROL                 = 0x8A, //!< Send Control - Retransmission & Lock
    CMND_IE_SIMPLE_PWR_METERING_REPORT  = 0x8B, //!< Simple Power Metering Report
    CMND_IE_EEPROM_OFFSET               = 0x8C, //!< Service EEPROM type + offset + data length
    CMND_IE_EEPROM_OFFSET_VALUE         = 0x8D, //!< Service EEPROM type + offset + data length + data
    CMND_IE_INTERNAL_LAST_TYPE,                 //!< Last IE type

}
en_hanCmndInfoElemTypeInternal;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_CmndMsgLog_PrintRxMsg( const t_st_hanCmndApiMsg* pst_Msg )
{
    p_CmndMsgLog_Print( "CMND->MCU: ", pst_Msg );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_CmndMsgLog_PrintTxMsg( const t_st_hanCmndApiMsg* pst_Msg )
{
    p_CmndMsgLog_Print( "MCU->CMND: ", pst_Msg );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_CmndMsgLog_PrintTxBuffer( u16 u16_BufferLen, const u8* u8_Buffer )
{
    t_st_hanCmndApiMsg st_Msg = {0};
    bool ok;

    ok = p_CmndPacketParser_ParseCmndPacket( u16_BufferLen-4, &u8_Buffer[4], &st_Msg );

    if ( ok )
    {
        p_CmndMsgLog_PrintTxMsg( &st_Msg );
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_CmndMsgLog_Print( const char* prefix, const t_st_hanCmndApiMsg* pst_Msg )
{

    const char* serviceIdStr = p_CmndToString_ServiceId( pst_Msg->serviceId );
    const char* messageIdStr = p_CmndToString_MessageId( pst_Msg->serviceId, pst_Msg->messageId );
    char ac_IesStr[MAX_IE_STR_LENGTH] = {0};
#ifdef PARSE_MSG_IES
    p_CmndMsgLog_ParseMsgIEs( pst_Msg, ac_IesStr, sizeof(ac_IesStr) );
#endif
    if ( prefix == NULL )
    {
        prefix = "";
    }

    LOG_INFO (  "%s"
                "%s<%04x> %s<%02x> %s",
                prefix,
                serviceIdStr,
                pst_Msg->serviceId,
                messageIdStr,
                pst_Msg->messageId,
                ac_IesStr );

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_CmndMsgLog_IeValueToString(  u8                  u8_IeType,
                                    IN  t_st_hanIeList* pst_IeList,
                                    char*               pc_Dst,
                                    u16                 u16_DstSize )
{
    switch ( u8_IeType )
    {
        case CMND_IE_RESPONSE:
        {
            t_st(CMND_IE_RESPONSE) st_Response = {0};
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_RESPONSE), OUT &st_Response, sizeof(st_Response));
            p_CmndLib_UserImpl_snprintf( pc_Dst, u16_DstSize, "%s<%02d>", p_CmndToString_RetCode( st_Response.u8_Result ), st_Response.u8_Result );
            break;
        }
        case CMND_IE_VERSION:
        {
            t_st(CMND_IE_VERSION) st_Version = {0};
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_VERSION), OUT &st_Version, sizeof(st_Version));
            p_CmndLib_UserImpl_snprintf( pc_Dst, u16_DstSize, "Version: %s", st_Version.u8_Version );
            break;
        }
        case CMND_IE_BATTERY_LEVEL:
        {
            t_st(CMND_IE_BATTERY_LEVEL) st_BatteryLevel = {0};
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_BATTERY_LEVEL), OUT &st_BatteryLevel, sizeof(st_BatteryLevel));
            p_CmndLib_UserImpl_snprintf( pc_Dst, u16_DstSize, "BatteryLevel: %u", st_BatteryLevel.u16_BatteryLevel );
            break;
        }
        case CMND_IE_GENERAL_STATUS:
        {
            t_st(CMND_IE_GENERAL_STATUS) st_GenStatus = {0};
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_GENERAL_STATUS), OUT &st_GenStatus, sizeof(st_GenStatus));
            p_CmndLib_UserImpl_snprintf(    pc_Dst,
                            u16_DstSize,
                            "DeviceId: %u, "
                            "EepromStatus: %u, "
                            "PowerupMode: %u, "
                            "RegStatus: %u",
                            st_GenStatus.u16_DeviceID,
                            st_GenStatus.u8_EepromStatus,
                            st_GenStatus.u8_PowerupMode,
                            st_GenStatus.u8_RegStatus );
            break;
        }
        case CMND_IE_REGISTRATION_RESPONSE:
        {
            t_st(CMND_IE_REGISTRATION_RESPONSE) st_RegistrationResponse = {0};
            p_CmndMsg_IeGetFromList(    IN pst_IeList,
                                        p_CMND_IE_GETTER(CMND_IE_REGISTRATION_RESPONSE),
                                        OUT &st_RegistrationResponse,
                                        sizeof(st_RegistrationResponse));

            p_CmndLib_UserImpl_snprintf( pc_Dst,
                         u16_DstSize,
                         "ResponseCode: 0x%02X, "
                         "DiscriminatorType: 0x%02X, "
                         "DeviceAddress: 0x%04X, "
                         "DiscriminatorValue: 0x%04X",
                         st_RegistrationResponse.u8_ResponseCode,
                         st_RegistrationResponse.u8_DiscriminatorType,
                         st_RegistrationResponse.u16_DeviceAddress,
                         st_RegistrationResponse.u16_DiscriminatorValue );
            break;
        }
        case CMND_IE_PARAMETER:
        {
            t_st(CMND_IE_PARAMETER) st_Parameter = {0};
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_PARAMETER), OUT &st_Parameter, sizeof(st_Parameter));
            break;
        }
        case CMND_IE_BG_REQ:
        {
            t_st(CMND_IE_BG_REQ) st_BandGag = {0};
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_BG_REQ), OUT &st_BandGag, sizeof(st_BandGag));
            p_CmndLib_UserImpl_snprintf( pc_Dst, u16_DstSize, "MuxInput: 0x%02X, ResistorFactor: 0x%02X, SupplyVolt: 0x%04X", st_BandGag.u8_MuxInput, st_BandGag.u8_ResistorFactor, st_BandGag.u16_SupplyVolt );
            break;
        }
        case CMND_IE_BG_RES:
        {
            t_st(CMND_IE_BG_RES) st_BandGagRes = {0};
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_BG_RES), OUT &st_BandGagRes, sizeof(st_BandGagRes));
            p_CmndLib_UserImpl_snprintf( pc_Dst,u16_DstSize, "AdcInput: %d mV, PorInput: %d mV", (unsigned int) st_BandGagRes.u32_AdcInput, (unsigned int) st_BandGagRes.u32_PorInput );
            break;
        }
        case CMND_IE_IMAGE_TYPE:
        {
            t_st(CMND_IE_U8) st_ImageType = {0};
            p_hanCmndApi_IeSuotaImageTypeGet( pst_IeList, &st_ImageType );
            p_CmndLib_UserImpl_snprintf( pc_Dst, u16_DstSize, "ImageType: 0x%08x", st_ImageType.u8_Data );
            break;
        }
        case CMND_IE_ALERT:
        {
            t_st(CMND_IE_ALERT) st_Alert = {0};
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_ALERT), OUT &st_Alert, sizeof(st_Alert));
            p_CmndLib_UserImpl_snprintf( pc_Dst, u16_DstSize, "UnitType: 0x%04x, AlertState: 0x%08x", (unsigned int) st_Alert.u16_UnitType, (unsigned int) st_Alert.u32_AlertState );
            break;
        }
        case CMND_IE_NEW_SW_INFO:
        {
            t_st_hanCmndIeSuotaNewSwReady st_Sw = {0};
            p_hanCmndApi_IeSuotaNewSwReadyGet( pst_IeList, &st_Sw );
            p_CmndLib_UserImpl_snprintf(    pc_Dst,
                            u16_DstSize,
                            "SwStr: '%s', HwStr: '%s', Url: '%s'",
                            st_Sw.swStr,
                            st_Sw.hwStr,
                            st_Sw.urlStr );
            break;
        }
        case CMND_IE_FUN:
        {
            t_st(CMND_IE_FUN) st_FunMsg = {0};
            char ac_RawBufferStr[MAX_IE_STR_LENGTH] = {0};

            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_FUN), OUT &st_FunMsg, sizeof(st_FunMsg));

            p_CmndMsgLog_BufferToHexString( ac_RawBufferStr, sizeof(ac_RawBufferStr), st_FunMsg.pu8_Data, st_FunMsg.u16_DataLen );

            p_CmndLib_UserImpl_snprintf( pc_Dst, u16_DstSize,
                        "DstDeviceId: 0x%04x, "
                        "SrcDeviceId: 0x%04x, "
                        "AddressType: %u, "
                        "DstUnitId: 0x%04x, "
                        "MessageSequence: %u, "
                        "MessageType: %u, "
                        "InterfaceType: %u, "
                        "InterfaceId: 0x%04x, "
                        "InterfaceMember: 0x%02x, "
                        "RawData[%u]: %s",
                        st_FunMsg.u16_DstDeviceId,
                        st_FunMsg.u16_SrcDeviceId,
                        st_FunMsg.u8_AddressType,
                        st_FunMsg.u8_DstUnitId,
                        st_FunMsg.u8_MessageSequence,
                        st_FunMsg.u8_MessageType,
                        st_FunMsg.u8_InterfaceType,
                        st_FunMsg.u16_InterfaceId,
                        st_FunMsg.u8_InterfaceMember,
                        st_FunMsg.u16_DataLen,
                        ac_RawBufferStr
                      );
            break;
        }
        case CMND_IE_ULE_CALL_SETTING:
        {
            t_st(CMND_IE_ULE_CALL_SETTING) st_CallSettings = { 0 };
            char ac_DigitsBufferStr[CALL_SETTINGS_STRING_LEN] = { 0 };
            char ac_PartyNameBufferStr[CALL_SETTINGS_STRING_LEN] = { 0 };
            char ac_PartyIdBufferStr[CALL_SETTINGS_STRING_LEN] = { 0 };
            p_CmndMsg_IeGetFromList(IN pst_IeList, p_CMND_IE_GETTER(CMND_IE_ULE_CALL_SETTING), OUT &st_CallSettings, sizeof(st_CallSettings));
            memcpy( ac_DigitsBufferStr, st_CallSettings.pu8_Digits, st_CallSettings.u8_DigitsLen );
            memcpy( ac_PartyNameBufferStr, st_CallSettings.pu8_OtherPartyName, st_CallSettings.u8_OtherPartyNameLen );
            memcpy( ac_PartyIdBufferStr, st_CallSettings.pu8_OtherPartyId, st_CallSettings.u8_OtherPartyIdLen );

            p_CmndLib_UserImpl_snprintf(    pc_Dst, u16_DstSize,
                            "FieldMask: 0x%08x, "
                            "PrefferedCodec: 0x%02x, "
                            "OtherPartyType: 0x%02x, "
                            "DigitsLen: 0x%02x "
                            "Digits: %s, "
                            "OtherPartyNameLen: 0x%02x, "
                            "OtherPartyName: %s, "
                            "OtherPartyIdLen: 0x%02x, "
                            "OtherPartyId: %s",
                            st_CallSettings.u32_FieldMask,
                            st_CallSettings.u8_PreferredCodec,
                            st_CallSettings.u8_OtherPartyType,
                            st_CallSettings.u8_DigitsLen,
                            ac_DigitsBufferStr,
                            st_CallSettings.u8_OtherPartyNameLen,
                            ac_PartyNameBufferStr,
                            st_CallSettings.u8_OtherPartyIdLen,
                            ac_PartyIdBufferStr);
            break;
        }
        default:
        {
            t_st_hanIeStruct st_Ie = {0};
            char ac_RawBufferStr[MAX_IE_STR_LENGTH] = {0};

            p_hanIeList_FindIeByType( pst_IeList, u8_IeType, &st_Ie );

            p_CmndMsgLog_BufferToHexString( ac_RawBufferStr, sizeof(ac_RawBufferStr), st_Ie.pu8_Data, st_Ie.u16_Len );

            p_CmndLib_UserImpl_snprintf( pc_Dst, u16_DstSize, "RawData[%u]: %s", st_Ie.u16_Len, ac_RawBufferStr );
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void p_CmndMsgLog_ParseMsgIEs(  const t_st_hanCmndApiMsg* pst_cmndApiMsg,
                                char*               pc_Dst,
                                u16                 u16_DstSize )
{
    u16                 ieLen=0;
    u16                 i = 0;
    u8                  u8_IeType = 0;
    char                ac_IeContent[MAX_IE_STR_LENGTH] = {0};
    const char*         pc_IeTypeStr;
    t_st_hanIeList      st_IeList = {0};

    p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, "[",1);

    p_hanIeList_CreateWithPayload( pst_cmndApiMsg->data, pst_cmndApiMsg->dataLength, &st_IeList );

    while ( i < pst_cmndApiMsg->dataLength )
    {
        u8_IeType = pst_cmndApiMsg->data[i];
        pc_IeTypeStr = p_CmndToString_IeType( u8_IeType );

#ifdef ARDUINO
        strcat_P( pc_Dst, pc_IeTypeStr );
#else
        p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, pc_IeTypeStr, strlen(pc_IeTypeStr));
#endif
        p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, " ", 1 );
        p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, "[", 1 );

        if (    u8_IeType < CMND_IE_LAST_TYPE ||
                ( u8_IeType >= CMND_IE_INTERNAL_FIRST_TYPE && u8_IeType < CMND_IE_INTERNAL_LAST_TYPE ) )
        {
            i += 1;

            memcpy( &ieLen,& ( pst_cmndApiMsg->data[i] ),sizeof ( ieLen ) );

            ieLen = p_Endian_net2hos16( ieLen );

            if ( ieLen > MAX_IE_LENGTH )
                break;


            p_CmndMsgLog_IeValueToString( u8_IeType, &st_IeList, ac_IeContent, sizeof( ac_IeContent ) );

            i += sizeof( ieLen ) + ieLen;
        }
        else
        {
            i = pst_cmndApiMsg->dataLength;
            p_CmndLib_UserImpl_snprintf( ac_IeContent, sizeof(ac_IeContent), "Not valid IE type: %u", u8_IeType );
        }
        p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, ac_IeContent, strlen( ac_IeContent ) );
        p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, "]", 1 );
        if ( i < pst_cmndApiMsg->dataLength )
        {
            p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, ", ", 2 );
        }
    }

    p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, "]", 1 );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsgLog_BufferToHexString(    char*       pc_Dst,
                                        u16         u16_DstSize,
                                        const u8*   pu8_Src,
                                        u16         u16_SrcSize )
{
    u16 n = 0;
    u16 i;

    if ( !pc_Dst || !pu8_Src )
    {
        return false;
    }
    if ( u16_DstSize == 0 || u16_SrcSize == 0 )
    {
        pc_Dst[0] = 0;
        return false;
    }

    if ( u16_DstSize < u16_SrcSize*3 )
    {
        LOG_ERROR( "p_CmndMsgLog_BufferToHexString: Buffer too small!" );
        return false;
    }

    for ( i = 0; i<u16_SrcSize; i++ )
    {
        char tmp[4] = {0};
        n += p_CmndLib_UserImpl_snprintf( tmp, sizeof(tmp), "%02x", pu8_Src[i] );

        if ( i != u16_SrcSize-1 )
        {
            const char* pc_Space = " ";
            //strnlen( pc_Space, sizeof( pc_Space ) - 1 ) is not compatible with Keil
            p_CmndLib_UserImpl_strncat( tmp, sizeof(tmp), pc_Space, strlen( pc_Space ) );
            n++;
        }

        //strnlen( tmp, sizeof( tmp ) - 1 ) is not compatible with Keil
        p_CmndLib_UserImpl_strncat( pc_Dst, u16_DstSize, tmp, strlen( tmp ) );
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Print CMND API parameter value
void p_CmndMsgLog_GetCmndIeParameter ( char* pc_Dst, u16 u16_DstSize, const t_st_hanCmndIeParameter* pst_Parameter)
{
    bool bPrinted = false;

    switch ( pst_Parameter->e_ParamType )
    {
        case CMND_PARAM_TYPE_EEPROM:
        {
            bPrinted = p_CmndMsgLog_GetParamEeprom( pc_Dst, u16_DstSize, pst_Parameter );
            break;
        }

        case CMND_PARAM_TYPE_PRODUCTION:
        {
            bPrinted = p_CmndMsgLog_GetParamProduction( pc_Dst, u16_DstSize, pst_Parameter );
            break;
        }
    }

    if ( !bPrinted )
    {
        if (pst_Parameter->u16_DataLen>0)
        {
//          LOG_CUSTOM( LOG_LEVEL_INFO,
//              p_Logger_AppendBuffer( LOG_LEVEL_INFO, "Parameter Data", pu8_ParamData, pst_Parameter->u16_DataLen ) );
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Print CMND API EEPROM parameter value
bool p_CmndMsgLog_GetParamEeprom( char* pc_Dst, u16 u16_DstSize, const t_st_hanCmndIeParameter* pst_Parameter )
{
    bool ok = true;
    u8 e_Param = pst_Parameter->e_Param;
    char str[CMND_PARAM_MAX_STRING_LEN] = {0};

    switch ( e_Param )
    {
        case CMND_PARAM_EEPROM_RXTUN:
        case CMND_PARAM_EEPROM_TBR6:
        case CMND_PARAM_EEPROM_DECT_CARRIER:
        case CMND_PARAM_EEPROM_HAN_SYS_OFF_USED:
        case CMND_PARAM_EEPROM_HW_VERISON:
        case CMND_PARAM_EEPROM_HAN_HBR_OSC:
        case CMND_PARAM_EEPROM_HAN_PAGING_CAPS:
        case CMND_PARAM_EEPROM_HAN_GENERAL_FLAGS:
        case CMND_PARAM_EEPROM_RSSI_SETTINGS:
        {
            p_CmndLib_UserImpl_snprintf( str, sizeof(str), "%02x", pst_Parameter->pu8_Data[0]);
            break;
        }

        case CMND_PARAM_EEPROM_IPEI:
        {
            i8 i;
            for ( i=0; i<5; i++ )
            {
                char tmp[3] = {0};
                p_CmndLib_UserImpl_snprintf( tmp, sizeof(tmp), "%02x", pst_Parameter->pu8_Data[i] );
                p_CmndLib_UserImpl_strncat( str, sizeof(str), tmp, strlen(tmp) );
                if ( i!=4)
                {
                    p_CmndLib_UserImpl_strncat( str, sizeof(str), " ", 1 );
                }
            }
            break;
        }
        case CMND_PARAM_EEPROM_FRIENDLY_NAME:
        {
            u8 numOfUnits = pst_Parameter->pu8_Data[0];
            u8 i = 0;
            u8 pos = 0;
            char tmp[CMND_PARAM_MAX_STRING_LEN] = {0};
            char tmp2[CMND_PARAM_MAX_STRING_LEN] = {0};

            pos++;

            p_CmndLib_UserImpl_snprintf( str, sizeof(str), "numOfUnits<%u>: ", numOfUnits );

            for ( i=0; i<numOfUnits; i++ )
            {
                u8 u8_UnitId = pst_Parameter->pu8_Data[pos];
                pos++;

                p_StringUtils_GetString( tmp, sizeof(tmp), &pst_Parameter->pu8_Data[pos], pst_Parameter->u16_DataLen - pos );
                pos += pst_Parameter->pu8_Data[pos]+1;

                p_CmndLib_UserImpl_snprintf( tmp2, sizeof(tmp2), "<unitId<%u>, unitName<%s>>", u8_UnitId, tmp );
                p_CmndLib_UserImpl_strncat( str, sizeof(str), tmp2, strlen(tmp2) );

                if ( i!=numOfUnits-1 )
                {
                    p_CmndLib_UserImpl_strncat( str, sizeof(str), ", ", 2 );
                }
            }
            break;
        }
        case CMND_PARAM_EEPROM_SW_VERISON:
        {
            p_StringUtils_GetString( str, sizeof(str), pst_Parameter->pu8_Data, pst_Parameter->u16_DataLen );

            //LOG_INFO( "%s <%s>", p_CmndToString_EepromParameter( e_Param ), str );
            break;
        }

        //-------------------------------------------------------------------------
        // Add your code here: to print other params
        //-------------------------------------------------------------------------

        default:
        {
            ok = false;
            break;
        }
    }

    p_CmndLib_UserImpl_snprintf(    pc_Dst, u16_DstSize, "%s<%02x> len<%u> val<%s>",    p_CmndToString_EepromParameter( e_Param ),
                                                                        e_Param,
                                                                        pst_Parameter->u16_DataLen,
                                                                        str);

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Print CMND API EEPROM parameter value
bool p_CmndMsgLog_GetParamProduction( char* pc_Dst, u16 u16_DstSize, const t_st_hanCmndIeParameter* pst_Parameter)
{
    bool bPrinted = true;

    u8 e_Param = pst_Parameter->e_Param;

    switch ( e_Param )
    {
        //-------------------------------------------------------------------------
        // Add your code here: to print CMND parameter of Production type
        //-------------------------------------------------------------------------
        default:
        {
            bPrinted = false;
            break;
        }
    }
    return bPrinted;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
