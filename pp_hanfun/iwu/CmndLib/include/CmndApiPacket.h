/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_API_PACKET_H
#define _CMND_API_PACKET_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiExported.h"
#include "IeList.h"

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct{
    u8 buffer[CMNDLIB_API_PACKET_MAX_SIZE];
    u16 length;
} t_st_Packet;

///////////////////////////////////////////////////////////////////////////////
/// @brief          Creates a CMND API packet
///
/// @param[out]     pu8_Buffer          - Pointer to message buffer[CMND_API_PACKET_MAX_SIZE]
/// @param[in]      p_Msg               - CMND API structure
///
/// @return         Length of CMND API packet
///////////////////////////////////////////////////////////////////////////////
// Create a CMND API message
u16 p_CmndApiPacket_CreateFromCmndApiMsg(   OUT     u8*                 pu8_Buffer,
                                            const   t_st_hanCmndApiMsg* p_Msg );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Creates a CMND API format buffer without IE
///
/// @details    Function creates a CMND API format buffer based on given parameters. The <pu8_Buffer>
///             will contain all CMND API message fields. Data is stored in network order.
///
/// @param[out]     pu8_Buffer          - Pointer to message buffer[CMND_API_PACKET_MAX_SIZE]
/// @param[in]      u16_serviceId       - CMND service ID
/// @param[in]      u8_messageId        - CMND message ID of service
/// @param[in]      u8_unitId           - Source unit Id
/// @param[in]      u8_cookie           - Cookie
///
/// @return         Length of CMND API message
///////////////////////////////////////////////////////////////////////////////
u16 p_CmndApiPacket_CreateNoIe( OUT u8* pu8_Buffer,
                                    u16 u16_ServiceId,
                                    u8 u8_MessageId,
                                    u8 u8_UnitId,
                                    u8 u8_Cookie );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Creates a CMND API format buffer
///
/// @details    Function creates a CMND API format buffer based on given parameters. The <pu8_Buffer>
///             will contain all CMND API message fields. Data is stored in network order.
///
/// @param[out]     pu8_Buffer          - Pointer to message buffer[CMND_API_PACKET_MAX_SIZE]
/// @param[in]      u16_serviceId       - CMND service ID
/// @param[in]      u8_messageId        - CMND message ID of service
/// @param[in]      u8_unitId           - Source unit Id
/// @param[in]      u8_cookie           - Cookie
/// @param[in]      dataLength          - Payload data length
///
/// @return         Length of CMND API message
///////////////////////////////////////////////////////////////////////////////
u16 p_CmndApiPacket_Create( OUT u8* pu8_Buffer,
                                u16 u16_ServiceId,
                                u8 u8_MessageId,
                                u8 u8_UnitId,
                                u8 u8_Cookie,
                                const t_st_hanIeList* pst_IeList );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Append IeList to existing CMND API message buffer
///
/// @details    This function is used to append IEs to existing CMND API message buffer
///
/// @param[in,out]      pu8_Buffer  - Pointer to message buffer (exclude 0xDADA) with existing CMND API message
/// @param[in]          u16_len     - Length of existing CMND API message
/// @param[in]          pst_IeList  - Pointer to IeList to append
///
/// @return         Length of CMND API message. 0 if failed
///////////////////////////////////////////////////////////////////////////////
u16 p_CmndApiPacket_AppendIeList(   INOUT u8*               pu8_Buffer,
                                        u16                     u16_len,
                                        const t_st_hanIeList*   pst_IeList );


///////////////////////////////////////////////////////////////////////////////
/// @brief      Calculate Checksum of CMND API message buffer from Length field (exclude 0xDADA).
///
/// @param[in]  pBuffer     - Pointer to message buffer (exclude 0xDADA)
/// @param[in]  u16_len     - Used buffer length
///
/// @return     Calculated checksum
///////////////////////////////////////////////////////////////////////////////
u8 p_CmndApiPacket_CalcCheckSum( const u8* pu8_Buffer, u16 u16_len );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Print packet with direction
///
/// @param[in]  pst_Packet     - Pointer to packet structure
/// @param[in]  direction      - direction ('TX' or 'RX')
///
///////////////////////////////////////////////////////////////////////////////
void p_CmndApiPacket_Print( t_st_Packet* pst_Packet, const char* direction );

extern_c_end

#endif  //_CMND_API_PACKET_H
