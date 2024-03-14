/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_OnOff.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service ON message
bool p_CmndMsg_OnOff_CreateOnReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId )
{
    pst_hanCmndApiMsg->serviceId    = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId    = CMND_MSG_ONOFF_ON_REQ;
    pst_hanCmndApiMsg->unitId       = u8_UnitId;

    pst_hanCmndApiMsg->dataLength = 0;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service OFF message
bool p_CmndMsg_OnOff_CreateOffReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId )
{
    pst_hanCmndApiMsg->serviceId    = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId    = CMND_MSG_ONOFF_OFF_REQ;
    pst_hanCmndApiMsg->unitId       = u8_UnitId;

    pst_hanCmndApiMsg->dataLength = 0;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service toggle message
bool p_CmndMsg_OnOff_CreateToggleReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId )
{
    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ONOFF_TOGGLE_REQ;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    pst_hanCmndApiMsg->dataLength = 0;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service get attribute message
bool p_CmndMsg_OnOff_CreateGetAttribReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeAttributeId* pst_AttrId )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId    = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId    = CMND_MSG_ONOFF_TOGGLE_REQ;
    pst_hanCmndApiMsg->unitId       = u8_UnitId;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add attribute id IE
    ok = p_hanCmndApi_IeAttributeIdAdd( &st_IeList, pst_AttrId );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service set attribute message
bool p_CmndMsg_OnOff_CreateSetAttribReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeSetAttributeValue* pst_SetAttrValue )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ONOFF_SET_ATTRIB_REQ;
    pst_hanCmndApiMsg->unitId    = u8_UnitId;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add attribute value IE
    ok = p_hanCmndApi_IeSetAttributeValueAdd(&st_IeList, pst_SetAttrValue);

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service set attribute message with response
bool p_CmndMsg_OnOff_CreateSetAttrReqWithRes(   OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                            u8                                      u8_UnitId,
                                            const t_st_hanCmndIeSetAttributeValue*  pst_SetAttrValue )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ONOFF_SET_ATTRIB_REQ_WITH_RES;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add attribute value IE
    ok = p_hanCmndApi_IeSetAttributeValueAdd(&st_IeList, pst_SetAttrValue);

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service On message response
bool p_CmndMsg_OnOff_CreateOnRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeResponse* pst_Response )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ONOFF_ON_RES;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add response IE
    ok = p_hanCmndApi_IeResponseAdd(&st_IeList, pst_Response);

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service Off message response
bool p_CmndMsg_OnOff_CreateOffRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeResponse* pst_Response )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ONOFF_OFF_RES;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add response IE
    ok = p_hanCmndApi_IeResponseAdd(&st_IeList, pst_Response);

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service Toggle message response
bool p_CmndMsg_OnOff_CreateToggleRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeResponse* pst_Response )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ONOFF_TOGGLE_RES;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add response IE
    ok = p_hanCmndApi_IeResponseAdd(&st_IeList, pst_Response);

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of OnOff service get attribute message response
bool p_CmndMsg_OnOff_CreateGetAttribRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeAttributeValue* pst_AttrVal )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ONOFF_GET_ATTRIB_RES;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

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

// Create packet of OnOff service set attribute message response
bool p_CmndMsg_OnOff_CreateSetAttribRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeResponse* pst_Response, const t_st_hanCmndIeAttributeId*  pst_AttributeId )
{
    bool ok;
    t_st_hanIeList st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_ON_OFF;
    pst_hanCmndApiMsg->messageId = CMND_MSG_ONOFF_SET_ATTRIB_RES;
    pst_hanCmndApiMsg->unitId = u8_UnitId;

    // IE list is used to easily add IE object to the list
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add response IE
    ok = p_hanCmndApi_IeResponseAdd( &st_IeList, pst_Response );

    if( ok )
    {
        // Add CMND_IE_ATTRIBUTE_ID
        ok = p_hanCmndApi_IeAttributeIdAdd( &st_IeList, pst_AttributeId );
    }

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
