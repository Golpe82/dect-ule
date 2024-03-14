/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_Fun.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Create packet of Fun service request message
bool p_CmndMsg_Fun_CreateSendReq( OUT t_st_hanCmndApiMsg*   pst_hanCmndApiMsg, const t_st_hanCmndIeFun* pst_Fun )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_FUN;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_FUN_SEND_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;

    // create IE list object with payload of CMND API message "data" field
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    // Add Fun IE
    ok = p_hanCmndApi_IeFunAdd( &st_IeList, pst_Fun );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
