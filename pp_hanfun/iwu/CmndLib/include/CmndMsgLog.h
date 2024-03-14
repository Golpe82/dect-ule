/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_MSG_LOG_H
#define _CMND_MSG_LOG_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "CmndApiExported.h"
#include "IeList.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// @brief      Print incoming CMND API message
///
/// @param[in]  pst_Msg         A pointer to CMND API Rx message
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_CmndMsgLog_PrintRxMsg( const t_st_hanCmndApiMsg* pst_Msg );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Print outgoing CMND API message
///
/// @param[in]  pst_Msg         A pointer to CMND API Tx message
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_CmndMsgLog_PrintTxMsg( const t_st_hanCmndApiMsg* pst_Msg );

///////////////////////////////////////////////////////////////////////////////
/// @brief  Parse and print an outgoing CMND API message from raw buffer
///
/// @param[in]  u16_BufferLen   CMND API message length
/// @param[in]  u8_Buffer       A pointer to raw buffer of an outgoing CMND API message
///
/// @note   use it only for outgoing messages
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_CmndMsgLog_PrintTxBuffer( u16 u16_BufferLen, const u8* u8_Buffer );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Print CMND API message
///
/// @param[in]  prefix          Prefix string
/// @param[in]  pst_Msg         A pointer to CMND API Rx message
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_CmndMsgLog_Print( const char* prefix, const t_st_hanCmndApiMsg* pst_Msg );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Outputs a CMND API Information element to string
///
/// @param[in]  u8_IeType           Type of IE
/// @param[in]  pst_IeList          A pointer to IeList
/// @param[out] pc_Dst              A pointer to destination buffer
/// @param[out] u16_DstSize         Destination buffer size
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
void p_CmndMsgLog_IeValueToString(  u8                  u8_IeType,
                                    IN  t_st_hanIeList* pst_IeList,
                                    char*               pc_Dst,
                                    u16                 u16_DstSize );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Outputs a buffer to string ("%02x")
///
/// @param[out] pc_Dst              A pointer to destination buffer
/// @param[out] u16_DstSize         Destination buffer size
/// @param[in]  pu8_Src             A pointer to source buffer
/// @param[in]  u16_SrcSize         Source buffer size
///
/// @return     None
///////////////////////////////////////////////////////////////////////////////
bool p_CmndMsgLog_BufferToHexString(    char*       pc_Dst,
                                        u16         u16_DstSize,
                                        const u8*   pu8_Src,
                                        u16         u16_SrcSize );

void p_CmndMsgLog_ParseMsgIEs(  const t_st_hanCmndApiMsg* pst_cmndApiMsg,
                                char*               pc_Dst,
                                u16                 u16_DstSize );


// Print CMND API IE parameter value
void p_CmndMsgLog_GetCmndIeParameter ( char* pc_Dst, u16 u16_DstSize, const t_st_hanCmndIeParameter* pst_Parameter );

// Print CMND API parameter value of EEPROM
bool p_CmndMsgLog_GetParamEeprom     ( char* pc_Dst, u16 u16_DstSize, const t_st_hanCmndIeParameter* pst_Parameter );

// Print CMND API IE parameter value of Production
bool p_CmndMsgLog_GetParamProduction ( char* pc_Dst, u16 u16_DstSize, const t_st_hanCmndIeParameter* pst_Parameter );

extern_c_end

#endif  //_CMND_MSG_LOG_H
