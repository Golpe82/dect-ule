/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_SYSTEM_H
#define _CMND_MSG_SYSTEM_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiExported.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @brief  Create System get battery measure packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_System_CreateBatteryMeasureGetReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeBatteryMeasureInfo* pst_MeasureInfo );


//////////////////////////////////////////////////////////////////////////
/// @brief  Create System get RSSI measure packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_System_CreateRssiGetReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );


//////////////////////////////////////////////////////////////////////////
/// @brief  Create System enable low battery indication packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_System_CreateBatteryIndEnableReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );


//////////////////////////////////////////////////////////////////////////
/// @brief  Create System disable low battery indication packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_System_CreateBatteryIndDisableReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create System device reset request
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_System_CreateResetReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );


extern_c_end

#endif  //_CMND_MSG_SYSTEM_H
