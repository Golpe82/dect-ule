/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_GENERAL_H
#define _CMND_MSG_GENERAL_H

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
/// @brief  Create General get status packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg       - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_General_CreateGetStatusReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );


//////////////////////////////////////////////////////////////////////////////
/// @brief  Create General get version packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg       - pointer to message
/// @param[in]      pst_Fun                 - The Fun structure
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_General_CreateGetVersionReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, t_en_hanCmndGeneralVersion en_VersionType );

//////////////////////////////////////////////////////////////////////////////
/// @brief  Create General start link maintaining request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      pst_LinkMaintain            - The link maintaining parameters
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_General_CreateLinkMaintainStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, t_st_hanCmndIeLinkMaintain* pst_LinkMaintain );

//////////////////////////////////////////////////////////////////////////////
/// @brief  Create General stop link maintaining request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_General_CreateLinkMaintainStopReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

//////////////////////////////////////////////////////////////////////////////
/// @brief  Create General hello packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg       - pointer to message
/// @param[in]      pst_Fun                 - The Fun structure
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_General_CreateHelloReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

//////////////////////////////////////////////////////////////////////////////
/// @brief  Create General transaction start packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg       - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_General_CreateTransactionStartReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

//////////////////////////////////////////////////////////////////////////////
/// @brief  Create General transaction end packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg       - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_General_CreateTransactionEndReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

extern_c_end

#endif  //_CMND_MSG_GENERAL_H
