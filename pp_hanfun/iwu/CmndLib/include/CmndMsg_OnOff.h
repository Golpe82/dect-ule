/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_ON_OFF_H
#define _CMND_MSG_ON_OFF_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiExported.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff ON request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - Pointer to message
/// @param[in]      u8_UnitId                   - Source unit ID
///
/// @note   If response is needed then add CMND_IE_RESPONSE_REQUIRED with using
///         #p_CmndMsg_AddResponseRequiredIe.
///         If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateOnReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff OFF request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - Pointer to message
/// @param[in]      u8_UnitId                   - Source unit ID
///
/// @note   If response is needed then add CMND_IE_RESPONSE_REQUIRED with using
///         #p_CmndMsg_AddResponseRequiredIe.
///         If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateOffReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff toggle request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      u8_UnitId                   - Source unit ID
///
/// @note   If response is needed then add CMND_IE_RESPONSE_REQUIRED with using
///         #p_CmndMsg_AddResponseRequiredIe.
///         If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateToggleReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff get attribute request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - Source unit ID
/// @param[in]      pst_AttrId          - attribute id
///
/// @note   If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateGetAttribReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeAttributeId* pst_AttrId );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff set attribute request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - Source unit ID
/// @param[in]      pst_SetAttrValue    - set attribute value
///
/// @note   If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateSetAttribReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeSetAttributeValue* pst_SetAttrValue );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff set attribute request with response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - Source unit ID
/// @param[in]      pst_SetAttrValue    - attribute value
///
/// @note   If destination device is not BS than add CMND_IE_UNIT_ADDR with using
///         #p_CmndMsg_AddUnitAddr.
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateSetAttrReqWithRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeSetAttributeValue* pst_SetAttrValue );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff ON response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - Source unit ID
/// @param[in]      pst_Response        - response
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateOnRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeResponse* pst_Response );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff OFF response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - Source unit ID
/// @param[in]      pst_Response        - response
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateOffRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeResponse* pst_Response );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff toggle response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - Source unit ID
/// @param[in]      pst_Response        - response
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateToggleRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeResponse* pst_Response );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff get attribute response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - Source unit ID
/// @param[in]      pst_AttrVal         - attribute value
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateGetAttribRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeAttributeValue* pst_AttrVal );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create OnOff set attribute response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - Source unit ID
/// @param[in]      pst_Response        - pointer to IeResponse
/// @param[in]      pst_AttributeId     - pointer to AttributeId
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_OnOff_CreateSetAttribRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeResponse* pst_Response, const t_st_hanCmndIeAttributeId*  pst_AttributeId );

extern_c_end

#endif //_CMND_MSG_ON_OFF_H
