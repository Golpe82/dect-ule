/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_KEEP_ALIVE_H
#define _CMND_MSG_KEEP_ALIVE_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiExported.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

//////////////////////////////////////////////////////////////////////////////
/// @brief      Create 'i am alive' message
///
/// @details    Create KeepAlive 'i am alive' packet buffer based on given parameters.
///             The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///             Data is stored in network order.
///
/// @param[out] pst_hanCmndApiMsg       - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_KeepAlive_CreateImAliveReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

//////////////////////////////////////////////////////////////////////////////
/// @brief  Create KeepAlive get attribute packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      pst_AttrId                  - the attribute ID
///
/// @note   If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_KeepAlive_CreateGetAttribReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAttributeId* pst_AttrId );

//////////////////////////////////////////////////////////////////////////////
/// @brief  Create KeepAlive get attribute response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      pst_AttrVal                 - attribute value
///
/// @note   If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_KeepAlive_CreateGetAttribRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeAttributeValue* pst_AttrVal );

//////////////////////////////////////////////////////////////////////////////
/// @brief  Create KeepAlive set attribute packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to packet buffer[CMND_API_MESSAGE_MAX_SIZE]
/// @param[in]      pst_Response                - structure of CMND_IE_RESPONSE
/// @param[in]      pst_AttrId                  - attribute id
///
/// @note   If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_KeepAlive_CreateSetAttribRes(    OUT t_st_hanCmndApiMsg*             pst_hanCmndApiMsg,
                                            const t_st_hanCmndIeResponse*       pst_Response,
                                            const t_st_hanCmndIeAttributeId*    pst_AttrId );

extern_c_end

#endif // _CMND_MSG_KEEP_ALIVE_H
