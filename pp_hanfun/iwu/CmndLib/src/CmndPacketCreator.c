/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndPacketCreator.h"
#include "CmndApiPacket.h"
#include "CmndMsg_AttributeReporting.h"
#include "CmndMsg_Alert.h"
#include "CmndMsg_DeviceManagement.h"
#include "CmndMsg_Fun.h"
#include "CmndMsg_General.h"
#include "CmndMsg_KeepAlive.h"
#include "CmndMsg_OnOff.h"
#include "CmndMsg_Production.h"
#include "CmndMsg_Parameters.h"
#include "CmndMsg_System.h"
#include "CmndMsg_Tamper.h"
#include "CmndMsg_VoiceCall.h"

#include <string.h> //memcpy

typedef bool (*CreatorNoParams)(t_st_hanCmndApiMsg* msg);

static bool p_CreatePacket_NoParams(t_st_Packet* packet, CreatorNoParams creator);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// ATTR REP
bool p_AttrRep_GetReportValuesRes(t_st_Packet* packet, t_st_hanCmndIeReportInfoInd* reportInfo)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_AttributeReporting_CreateReportNotificationReq(&msg, reportInfo) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// ALERT

bool p_Alert_NotifyStatusReq(t_st_Packet* packet, u8 unitId, u16 unitType, u32 alertType )
{
    t_st_hanCmndApiMsg msg = {0};
    t_st_hanCmndIeAlert alertIe;

    alertIe.u16_UnitType = unitType;
    alertIe.u32_AlertState = alertType;

    if( p_CmndMsg_Alert_CreateNotifyStatusReq(&msg, unitId, &alertIe) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_Alert_NotifyStatusRes(t_st_Packet* packet, u8 unitId, u8 responseCode)
{
    t_st_hanCmndApiMsg msg = {0};
    t_st_hanCmndIeResponse response;

    response.u8_Result = responseCode;

    if( p_CmndMsg_Alert_CreateNotifyStatusRes(&msg, unitId, &response) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// DEVICE MANAGEMENT

bool p_DeviceManagement_RegisterDeviceReq(t_st_Packet* packet, u8* rfpi)
{
    t_st_hanCmndApiMsg msg = {0};
    t_st_hanCmndIeBaseWanted baseWanted = {0};
    t_st_hanCmndIeBaseWanted* baseWantedToSend = NULL;

    if(rfpi != NULL)
    {
        memcpy(baseWanted.u8_Rfpi,rfpi, CMND_API_RFPI_SIZE);
        baseWantedToSend = &baseWanted;
    }

    if( p_Cmnd_DeviceManagement_CreateRegisterDeviceReq(&msg, baseWantedToSend) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_DeviceManagement_DeregisterDeviceReq(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_Cmnd_DeviceManagement_CreateDeregisterDeviceReq);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// FUN

bool p_Fun_SendReq(t_st_Packet* packet, t_st_hanCmndIeFun* funIe)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_Fun_CreateSendReq(&msg, funIe) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_General_GetStatusReq(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_General_CreateGetStatusReq);
}

bool p_General_GetVersionReq(t_st_Packet* packet, t_en_hanCmndGeneralVersion version)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_General_CreateGetVersionReq(&msg, version) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_General_LinkMaintainStartReq(t_st_Packet* packet, t_st_hanCmndIeLinkMaintain* linkMaintain)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_General_CreateLinkMaintainStartReq(&msg, linkMaintain) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_General_LinkMaintainStopReq(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_General_CreateLinkMaintainStopReq);
}

bool p_General_TransactionStartReq(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_General_CreateTransactionStartReq);
}

bool p_General_TransactionEndReq(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_General_CreateTransactionEndReq);
}

bool p_General_HelloReq(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_General_CreateHelloReq);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// KEEP ALIVE

bool p_KeepAlive_IamAliveReq(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_KeepAlive_CreateImAliveReq);
}

bool p_KeepAlive_GetAttribReq(t_st_Packet* packet, u8 attrId)
{
    t_st_hanCmndApiMsg msg = {0};
    t_st_hanCmndIeAttributeId attrIdStruct;
    attrIdStruct.u8_AttributeId = attrId;

    if( p_CmndMsg_KeepAlive_CreateGetAttribReq(&msg, &attrIdStruct) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_KeepAlive_GetAttribRes(t_st_Packet* packet, t_st_hanCmndIeAttributeValue* attrVal)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_KeepAlive_CreateGetAttribRes(&msg, attrVal) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_KeepAlive_SetAttribRes(t_st_Packet* packet, u8 response_code, u8 attrId)
{
    t_st_hanCmndApiMsg msg = {0};
    t_st_hanCmndIeResponse response;
    t_st_hanCmndIeAttributeId attrIdStruct;

    response.u8_Result = response_code;
    attrIdStruct.u8_AttributeId = attrId;

    if( p_CmndMsg_KeepAlive_CreateSetAttribRes(&msg, &response, &attrIdStruct) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// ON OFF

bool p_OnOff_OnReq(t_st_Packet* packet, u8 unitId)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_OnOff_CreateOnReq(&msg, unitId) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_OffReq(t_st_Packet* packet, u8 unitId)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_OnOff_CreateOffReq(&msg, unitId) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_ToggleReq(t_st_Packet* packet, u8 unitId)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_OnOff_CreateToggleReq(&msg, unitId) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_GetAttribReq(t_st_Packet* packet, u8 unitId, u8 attrId)
{
    t_st_hanCmndApiMsg msg = {0};

    t_st_hanCmndIeAttributeId attrIdStruct;
    attrIdStruct.u8_AttributeId = attrId;

    if( p_CmndMsg_OnOff_CreateGetAttribReq(&msg, unitId, &attrIdStruct) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_SetAttribReq(t_st_Packet* packet, u8 unitId, t_st_hanCmndIeSetAttributeValue* setAttrValue)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_OnOff_CreateSetAttribReq(&msg, unitId, setAttrValue) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_SetAttrReqWithRes(t_st_Packet* packet, u8 unitId, t_st_hanCmndIeSetAttributeValue* setAttrValue)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_OnOff_CreateSetAttrReqWithRes(&msg, unitId, setAttrValue) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_OnRes(t_st_Packet* packet, u8 unitId, u8 response_code)
{
    t_st_hanCmndApiMsg msg = {0};

    t_st_hanCmndIeResponse response;
    response.u8_Result = response_code;

    if( p_CmndMsg_OnOff_CreateOnRes(&msg, unitId, &response) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_OffRes(t_st_Packet* packet, u8 unitId, u8 response_code)
{
    t_st_hanCmndApiMsg msg = {0};

    t_st_hanCmndIeResponse response;
    response.u8_Result = response_code;

    if( p_CmndMsg_OnOff_CreateOffRes(&msg, unitId, &response) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_ToggleRes(t_st_Packet* packet, u8 unitId, u8 response_code)
{
    t_st_hanCmndApiMsg msg = {0};

    t_st_hanCmndIeResponse response;
    response.u8_Result = response_code;

    if( p_CmndMsg_OnOff_CreateToggleRes(&msg, unitId, &response) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_GetAttribRes(t_st_Packet* packet, u8 unitId, t_st_hanCmndIeAttributeValue* attrValue)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_OnOff_CreateGetAttribRes(&msg, unitId, attrValue) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_OnOff_SetAttribRes(t_st_Packet* packet, u8 unitId, u8 response_code, u8 attrId)
{
    t_st_hanCmndApiMsg msg = {0};

    t_st_hanCmndIeAttributeId attrIdStruct;
    t_st_hanCmndIeResponse response;

    response.u8_Result = response_code;
    attrIdStruct.u8_AttributeId = attrId;

    if( p_CmndMsg_OnOff_CreateSetAttribRes(&msg, unitId, &response, &attrIdStruct) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// PRODUCTION

bool p_Production_StartRequest(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_Production_CreateStartReq);
}

bool p_Production_EndRequest(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_Production_CreateEndReq);
}

bool p_CreateProductionInitEepromDefaults( t_st_Packet* packet, t_en_hanCmndMsgProdResetEeprom EeepromType )
{
    t_st_hanCmndApiMsg msg = {0};

    if ( p_CmndMsg_Production_CreateInitEepromDefReq( &msg, EeepromType ) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg( packet->buffer, &msg );
        if ( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// PARAMETERS

bool p_Parameters_ParamGetReq(t_st_Packet* packet, t_st_hanCmndIeParameter* parameter)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_Parameter_CreateParamGetReq(&msg, parameter) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_Parameters_ParamGetDirectReq(t_st_Packet* packet, t_st_hanCmndIeParameterDirect* parameter)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_Parameter_CreateParamGetDirectReq(&msg, parameter) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_Parameters_ParamSetReq(t_st_Packet* packet, t_st_hanCmndIeParameter* parameter)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_Parameter_CreateParamSetReq(&msg, parameter) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_Parameters_ParamSetDirectReq(t_st_Packet* packet, t_st_hanCmndIeParameterDirect* parameter)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_Parameter_CreateParamSetDirectReq(&msg, parameter) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// SLEEP
bool p_Sleep_Cfm(t_st_Packet* packet)
{
    t_st_hanCmndApiMsg msg = {0};

    msg.serviceId = CMND_SERVICE_ID_SLEEP;
    msg.messageId = CMND_MSG_SLEEP_ENTER_SLEEP_CFM;
    msg.unitId = 0;

    packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
    if( packet->length > 0 )
    {
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// SYSTEM

bool p_System_ResetReq(t_st_Packet* packet)
{
    return p_CreatePacket_NoParams(packet, p_CmndMsg_System_CreateResetReq);
}

bool p_System_GetRssi(t_st_Packet* packet)
{
  t_st_hanCmndApiMsg msg = {0};
  if( p_CmndMsg_System_CreateRssiGetReq(&msg) )
  {

    packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
    if( packet->length > 0 )
    {
        return true;
    }
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// TAMPER

bool p_Tamper_NotifyStatusReq(t_st_Packet* packet, u8 unitId, u8 alertStatus)
{
    t_st_hanCmndApiMsg msg = {0};

    t_st_hanCmndIeTamperAlert tamperAlert;

    tamperAlert.u8_AlertStatus = alertStatus;

    if( p_CmndMsg_TamperAlert_CreateSendNotifyStatusReq(&msg, unitId, &tamperAlert) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_Tamper_NotifyStatusRes(t_st_Packet* packet, u8 unitId, u8 response_code)
{
    t_st_hanCmndApiMsg msg = {0};

    t_st_hanCmndIeResponse response;
    response.u8_Result = response_code;

    if( p_CmndMsg_TamperAlert_CreateSendNotifyStatusRes(&msg, unitId, &response) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// VOICE CALL

bool p_VoiceCall_StartCallReq(t_st_Packet* packet, u8 unitId, const t_st_hanCmndIeUleCallSetting* callSettings)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_VoiceCall_CreateStartCallReq(&msg, unitId, callSettings) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_VoiceCall_EndCallReq(t_st_Packet* packet, u8 unitId)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_VoiceCall_CreateEndCallReq(&msg, unitId) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_VoiceCall_StartCallRes(t_st_Packet* packet, u8 unitId, u8 response_code, const t_st_hanCmndIeUleCallSetting* callSettings)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_VoiceCall_CreateStartCallRes(&msg, unitId, response_code, callSettings) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_VoiceCall_EndCallRes(t_st_Packet* packet, u8 unitId, u8 response_code)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_VoiceCall_CreateEndCallRes(&msg, unitId, response_code) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}


bool p_VoiceCall_SetVolumeReq(t_st_Packet* packet, u8 unitId, u8 volume)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_VoiceCall_CreateSetVolumeReq(&msg, unitId, volume) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_VoiceCall_UpVolumeReq(t_st_Packet* packet, u8 unitId)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_VoiceCall_CreateUpVolumeReq(&msg, unitId) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}
bool p_VoiceCall_DownVolumeReq(t_st_Packet* packet, u8 unitId)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_VoiceCall_CreateDownVolumeReq(&msg, unitId) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

bool p_Production_ResetEepromReq(t_st_Packet* packet, t_en_hanCmndMsgProdResetEeprom EeepromType)
{
    t_st_hanCmndApiMsg msg = {0};

    if( p_CmndMsg_Production_CreateResetEeepromReq(&msg, EeepromType) )
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CreatePacket_NoParams(t_st_Packet* packet, CreatorNoParams creator)
{
    t_st_hanCmndApiMsg msg = {0};
    if(creator(&msg))
    {
        packet->length = p_CmndApiPacket_CreateFromCmndApiMsg(packet->buffer, &msg);
        if( packet->length > 0 )
        {
            return true;
        }
    }
    return false;
}
