/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_KeepAlive.h"
#include "Endian.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of Keep Alive message
bool p_CmndMsg_KeepAlive_CreateImAliveReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg->serviceId    = CMND_SERVICE_ID_KEEP_ALIVE;
    pst_hanCmndApiMsg->messageId    = CMND_MSG_KEEP_ALIVE_I_AM_ALIVE_REQ;
    pst_hanCmndApiMsg->unitId       = 0;
    pst_hanCmndApiMsg->dataLength   = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of KeepAlive get attribute message
bool p_CmndMsg_KeepAlive_CreateGetAttribRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAttributeValue* pst_AttrVal )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_KEEP_ALIVE;
    pst_hanCmndApiMsg->messageId = CMND_MSG_KEEP_ALIVE_GET_ATTRIB_RES;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add attribute value IE
    ok = p_hanCmndApi_IeAttributeValueAdd(&st_IeList, pst_AttrVal);

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of KeepAlive get attribute message
bool p_CmndMsg_KeepAlive_CreateGetAttribReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAttributeId* pst_AttrId )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_KEEP_ALIVE;
    pst_hanCmndApiMsg->messageId = CMND_MSG_KEEP_ALIVE_GET_ATTRIB_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add attribute id IE
    ok = p_hanCmndApi_IeAttributeIdAdd(&st_IeList, pst_AttrId);

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of KeepAlive set attribute message
bool     p_CmndMsg_KeepAlive_CreateSetAttribRes(    OUT t_st_hanCmndApiMsg*             pst_hanCmndApiMsg,
                                            const t_st_hanCmndIeResponse*       pst_Response,
                                            const t_st_hanCmndIeAttributeId*    pst_AttrId )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_KEEP_ALIVE;
    pst_hanCmndApiMsg->messageId = CMND_MSG_KEEP_ALIVE_SET_ATTRIB_RES;
    pst_hanCmndApiMsg->unitId = 0;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add response IE
    ok = p_hanCmndApi_IeResponseAdd(&st_IeList, pst_Response);

    if( ok )
    {
        // Add attribute id IE
        ok = p_hanCmndApi_IeAttributeIdAdd( &st_IeList, pst_AttrId );
    }

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
