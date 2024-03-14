/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_SUOTA_H
#define _CMND_MSG_SUOTA_H

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
/// @brief  Create SUOTA download start request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Suota_CreateDownloadStartReq(    OUT t_st_hanCmndApiMsg*                 pst_hanCmndApiMsg,
                                            const t_st_hanCmndIeSuotaNewSwReady*    pst_SwInfo,
                                            const t_st_hanCmndIeU8*                 pst_ImageType );

//////////////////////////////////////////////////////////////////////////
/// @brief  Create SUOTA download abort request packet buffer based on given parameters.
///         The <pu8_Buffer> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pu8_Buffer          - pointer to packet buffer[CMND_API_PACKET_MAX_SIZE]
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Suota_CreateDownloadAbortReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );


//////////////////////////////////////////////////////////////////////////
/// @brief  Create SUOTA read file request packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg   - pointer to message
/// @param[in]      pst_ReadFileData    - specifies offset and length to read
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Suota_CreateReadFileReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeFileDataReq* pst_ReadFileData );


//////////////////////////////////////////////////////////////////////////
/// @brief  Create SUOTA upgrade completed packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg       - pointer to message
/// @param[in]      pst_UpgradeCompleted    - holds SUOTA result and current SW Version
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Suota_CreateUpgradeCompletedReq( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeSuotaSwVer* pst_UpgradeCompleted );


//////////////////////////////////////////////////////////////////////////
/// @brief  Create SUOTA NewSw response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
/// @param[in]      pst_Response        - success / fail status
/// @return         true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Suota_CreateNewSwRes( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg, const t_st_hanCmndIeResponse* pst_Response );


//////////////////////////////////////////////////////////////////////////
/// @brief  Create SUOTA image ready response packet buffer based on given parameters.
///         The <pst_hanCmndApiMsg> will contain all CMND API message fields.
///         Data is stored in network order.
///
/// @param[out]     pst_hanCmndApiMsg           - pointer to message
///
/// @return     true when success
//////////////////////////////////////////////////////////////////////////
bool p_CmndMsg_Suota_CreateImageReadyRes ( OUT t_st_hanCmndApiMsg* pst_hanCmndApiMsg );

extern_c_end

#endif  //_CMND_MSG_SUOTA_H
