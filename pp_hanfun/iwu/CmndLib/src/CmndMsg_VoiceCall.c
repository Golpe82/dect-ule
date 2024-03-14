/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_VoiceCall.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create VoiceCall send call start request packet buffer based on given parameters
bool p_CmndMsg_VoiceCall_CreateStartCallReq ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeUleCallSetting* pst_IeCallSettings )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ULE_VOICE_CALL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ULE_VOICE_CALL_START_REQ;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // create IE list object with payload of CMND API message "data" field
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add CMND_IE_ULE_CALL_SETTING
    ok = p_hanCmndApi_IeCallSettingsAdd ( &st_IeList, pst_IeCallSettings );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create VoiceCall send call end request packet buffer based on given parameters
bool p_CmndMsg_VoiceCall_CreateEndCallReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_ULE_VOICE_CALL;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_ULE_VOICE_CALL_END_REQ;
    pst_hanCmndApiMsg ->unitId    = u8_UnitId;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create VoiceCall send call start response packet buffer based on given parameters
bool p_CmndMsg_VoiceCall_CreateStartCallRes ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, u8 u8_ResponseCode, const t_st_hanCmndIeUleCallSetting* pst_IeCallSettings )
{
    bool                    ok;
    t_st_hanIeList      st_IeList;
    t_st_hanCmndIeResponse  st_Response;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ULE_VOICE_CALL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ULE_VOICE_CALL_START_RES;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // create IE list object with payload of CMND API message "data" field
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    st_Response.u8_Result = u8_ResponseCode;

    // Add CMND_IE_RESPONSE
    ok = p_hanCmndApi_IeResponseAdd ( &st_IeList, &st_Response );

    if( ok )
    {
        // Add CMND_IE_ULE_CALL_SETTING
        ok = p_hanCmndApi_IeCallSettingsAdd( &st_IeList, pst_IeCallSettings );
    }

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create VoiceCall send call end response packet buffer based on given parameters
bool p_CmndMsg_VoiceCall_CreateEndCallRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, u8 u8_ResponseCode )
{
    bool                    ok;
    t_st_hanIeList      st_IeList;
    t_st_hanCmndIeResponse  st_Response;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ULE_VOICE_CALL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ULE_VOICE_CALL_END_RES;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // create IE list object with payload of CMND API message "data" field
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );


    st_Response.u8_Result = u8_ResponseCode;

    // Add CMND_IE_RESPONSE
    ok = p_hanCmndApi_IeResponseAdd( &st_IeList, &st_Response );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_VoiceCall_CreateSetVolumeReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, u8 u8_Volume )
{
    t_st_hanCmndIeU8    st_Volume;

    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ULE_VOICE_CALL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ULE_VOICE_CALL_SET_VOLUME_REQ;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    st_Volume.u8_Data = u8_Volume;

    // Add IE U8
    ok = p_hanCmndApi_IeU8Add( &st_IeList, &st_Volume );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_VoiceCall_CreateUpVolumeReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId)
{
    pst_hanCmndApiMsg ->serviceId       = CMND_SERVICE_ID_ULE_VOICE_CALL;
    pst_hanCmndApiMsg ->messageId       = CMND_MSG_ULE_VOICE_CALL_VOLUME_UP_REQ;
    pst_hanCmndApiMsg ->unitId          = u8_UnitId;
    pst_hanCmndApiMsg ->dataLength      = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_VoiceCall_CreateDownVolumeReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId)
{
    pst_hanCmndApiMsg ->serviceId       = CMND_SERVICE_ID_ULE_VOICE_CALL;
    pst_hanCmndApiMsg ->messageId       = CMND_MSG_ULE_VOICE_CALL_VOLUME_DOWN_REQ;
    pst_hanCmndApiMsg ->unitId          = u8_UnitId;
    pst_hanCmndApiMsg ->dataLength      = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
