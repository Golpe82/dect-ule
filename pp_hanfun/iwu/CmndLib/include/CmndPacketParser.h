/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_PACKET_PARSER_H
#define _CMND_PACKET_PARSER_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiExported.h"
#include "CmndApiPacket.h"

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// Parse CMND API packet buffer
///
/// @return     true if ok
///////////////////////////////////////////////////////////////////////////////
bool p_CmndPacketParser_ParseCmndApiPacket( const t_st_Packet* packet, OUT t_st_hanCmndApiMsg* pst_cmndApiMsg );

///////////////////////////////////////////////////////////////////////////////
/// Parse CMND API packet buffer
///
/// @param[in]  u16_BufferLength    - CMND API packet buffer length
/// @param[in]  pu8_Buffer          - pointer to CMND API packet buffer
/// @param[out] pst_cmndApiMsg      - pointer to t_st_hanCmndApiMsg structure
///
/// @return     true if ok
///////////////////////////////////////////////////////////////////////////////
bool p_CmndPacketParser_ParseCmndPacket(    u16                     u16_BufferLength,
                                            const u8*               pu8_Buffer,
                                            OUT t_st_hanCmndApiMsg* pst_cmndApiMsg);

extern_c_end

#endif  //_CMND_PACKET_PARSER_H
