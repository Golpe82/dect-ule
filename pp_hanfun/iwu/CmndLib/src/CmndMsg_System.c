/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndMsg_System.h"
#include "CmndApiIe.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Get measurements of battery from A2D on CMND
bool p_CmndMsg_System_CreateBatteryMeasureGetReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeBatteryMeasureInfo* pst_MeasureInfo )
{
    bool            ok;
    t_st_hanIeList  st_IeList;

    pst_hanCmndApiMsg->serviceId = CMND_SERVICE_ID_SYSTEM;
    pst_hanCmndApiMsg->messageId = CMND_MSG_SYS_BATTERY_MEASURE_GET_REQ;
    pst_hanCmndApiMsg->unitId = 0;

    // create IE list object
    p_hanIeList_CreateEmpty( pst_hanCmndApiMsg->data, sizeof(pst_hanCmndApiMsg->data), &st_IeList );

    ok = p_hanCmndApi_IeBatteryMeasureInfoAdd( &st_IeList, pst_MeasureInfo );

    // update data length field
    pst_hanCmndApiMsg->dataLength = p_hanIeList_GetListSize( &st_IeList );

    return ok;

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Get RSSI measurement (link quality)
bool p_CmndMsg_System_CreateRssiGetReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_SYSTEM;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_SYS_RSSI_GET_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Enable low battery indication
bool p_CmndMsg_System_CreateBatteryIndEnableReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_SYSTEM;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_SYS_BATTERY_IND_ENABLE_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Disable low battery indication
bool p_CmndMsg_System_CreateBatteryIndDisableReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_SYSTEM;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_SYS_BATTERY_IND_DISABLE_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool p_CmndMsg_System_CreateResetReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg )
{
    pst_hanCmndApiMsg ->serviceId = CMND_SERVICE_ID_SYSTEM;
    pst_hanCmndApiMsg ->messageId = CMND_MSG_SYS_RESET_REQ;
    pst_hanCmndApiMsg ->unitId    = 0;
    pst_hanCmndApiMsg ->dataLength = 0;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
