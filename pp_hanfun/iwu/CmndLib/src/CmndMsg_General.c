/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_General.h"
#include "Endian.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of Get status message
bool p_CmndMsg_General_CreateGetStatusReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_GENERAL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_GENERAL_GET_STATUS_REQ;
    pst_hanCmndApiMsg->unitId = 0;
    pst_hanCmndApiMsg->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of get version message
bool p_CmndMsg_General_CreateGetVersionReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, t_en_hanCmndGeneralVersion en_VersionType )
{
    t_st_hanCmndIeU8    st_VersionType;

    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_GENERAL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_GENERAL_GET_VERSION_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    st_VersionType.u8_Data = (u8) en_VersionType;

    // Add IE U8
    ok = p_hanCmndApi_IeU8Add( &st_IeList, &st_VersionType );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of start link maintain
bool p_CmndMsg_General_CreateLinkMaintainStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, t_st_hanCmndIeLinkMaintain* pst_LinkMaintain )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_GENERAL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_GENERAL_LINK_MAINTAIN_START_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add IE LINK MAINTAIN
    ok = p_hanCmndApi_IeLinkMaintainAdd( &st_IeList, pst_LinkMaintain );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of stop link maintain
bool p_CmndMsg_General_CreateLinkMaintainStopReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_GENERAL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_GENERAL_LINK_MAINTAIN_STOP_REQ;
    pst_hanCmndApiMsg->unitId = 0;
    pst_hanCmndApiMsg->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of hello request message
bool p_CmndMsg_General_CreateHelloReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_GENERAL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_GENERAL_HELLO_REQ;
    pst_hanCmndApiMsg->unitId = 0;
    pst_hanCmndApiMsg->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_General_CreateTransactionStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_GENERAL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_GENERAL_TRANSACTION_START_REQ;
    pst_hanCmndApiMsg->unitId = 0;
    pst_hanCmndApiMsg->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_General_CreateTransactionEndReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_GENERAL;
    pst_hanCmndApiMsg->messageId = CMND_MSG_GENERAL_TRANSACTION_END_REQ;
    pst_hanCmndApiMsg->unitId = 0;
    pst_hanCmndApiMsg->dataLength = 0;

    return true;
}
