/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_Parameters.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of parameter get request message
bool p_CmndMsg_Parameter_CreateParamGetReq( OUT t_st_hanCmndApiMsg*         pst_hanCmndApiMsg,
                                            const t_st_hanCmndIeParameter*  pst_Param )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PARAMETERS;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PARAM_GET_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;


    // create IE list object with payload of CMND API message "data" field
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );
    // Add parameter IE
    ok = p_hanCmndApi_IeParameterAdd( &st_IeList, pst_Param );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of Parameter get direct request message
bool p_CmndMsg_Parameter_CreateParamGetDirectReq(   OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                                const t_st_hanCmndIeParameterDirect*        pst_DirectParam )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PARAMETERS;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PARAM_GET_DIRECT_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;

    // create IE list object with payload of CMND API message "data" field
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add direct parameter IE
    ok = p_hanCmndApi_IeParameterDirectAdd( &st_IeList, pst_DirectParam );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of parameter set request message
bool p_CmndMsg_Parameter_CreateParamSetReq( OUT t_st_hanCmndApiMsg*         pst_hanCmndApiMsg,
                                        const t_st_hanCmndIeParameter*      pst_Parameter )
{
    bool            ok;
    t_st_hanIeList  st_IeList;
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_PARAMETERS;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_PARAM_SET_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;

    // create IE list object with payload of CMND API message "data" field
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add parameter IE
    ok = p_hanCmndApi_IeParameterAdd( &st_IeList, pst_Parameter );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of parameter set direct request message
bool p_CmndMsg_Parameter_CreateParamSetDirectReq(   OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                                const t_st_hanCmndIeParameterDirect*    pst_DirectParam )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_PARAMETERS;
    pst_hanCmndApiMsg->messageId = CMND_MSG_PARAM_SET_DIRECT_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object with payload of CMND API message "data" field
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof( pst_hanCmndApiMsg->data ), &st_IeList );

    // Add direct parameter IE
    ok = p_hanCmndApi_IeParameterDirectAdd( &st_IeList, pst_DirectParam );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
