/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_VOICE_CALL_H
#define _CMND_MSG_VOICE_CALL_H

#include "TypeDefs.h"
#include "CmndApiIe.h"
#include "CmndApiExported.h"

extern_c_begin

//////////////////////////////////////////////////////////////////////////
/// @brief  Create VoiceCall send call start request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - unit ID
/// @param[in]      pst_IeCallSettings  - CMND_IE_ULE_CALL_SETTING
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_VoiceCall_CreateStartCallReq ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, const t_st_hanCmndIeUleCallSetting* pst_IeCallSettings );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create VoiceCall send call end request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - unit ID
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_VoiceCall_CreateEndCallReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create VoiceCall send call start response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - unit ID
/// @param[in]      u8_ResponseCode     - response code
/// @param[in]      pst_IeCallSettings  - CMND_IE_ULE_CALL_SETTING
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_VoiceCall_CreateStartCallRes ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, u8 u8_ResponseCode, IN const t_st_hanCmndIeUleCallSetting* pst_IeCallSettings );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create VoiceCall send call end response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - unit ID
/// @param[in]      u8_ResponseCode     - response code
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_VoiceCall_CreateEndCallRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, u8 u8_ResponseCode );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create VoiceCall send 'set volume' request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - unit ID
/// @param[in]      u8_Volume           - volume level
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_VoiceCall_CreateSetVolumeReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId, u8 u8_Volume );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create VoiceCall send 'up volume' request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - unit ID
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_VoiceCall_CreateUpVolumeReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId);

//////////////////////////////////////////////////////////////////////////
/// @brief  Create VoiceCall send 'down volume' request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      u8_UnitId           - unit ID
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_VoiceCall_CreateDownVolumeReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, u8 u8_UnitId);

extern_c_end

#endif // _CMND_MSG_VOICE_CALL_H
