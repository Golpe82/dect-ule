/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_API_STRING_UTILS_H
#define _CMND_API_STRING_UTILS_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// @brief      Make string (byte 1 - length, bytes 2-n - characters) from null-terminated string
///
/// @param[out] pu8_Dst         - Pointer to destination buffer
/// @param[in]  u16_DstSize     - Destination buffer size
/// @param[in]  pc_Src          - Pointer to source NULL-terminated string
///
/// @return     Number of bytes, used in destination buffer
///////////////////////////////////////////////////////////////////////////////
u8 p_StringUtils_MakeString(    u8*         pu8_Dst,
                                u16         u16_DstSize,
                                const char* pc_Src );

#ifndef VAR_ON_CMND

///////////////////////////////////////////////////////////////////////////////
/// @brief      Get string from buffer
///
/// @param[out] pc_Dst          - Pointer to destination string buffer
/// @param[in]  u16_DstSize     - Destination buffer size
/// @param[in]  pu8_Src         - Pointer to source buffer
/// @param[in]  u16_SrcLen      - Source buffer length
///
/// @return     true if ok
///////////////////////////////////////////////////////////////////////////////
bool p_StringUtils_GetString(   char*       pc_Dst,
                                u16         u16_DstSize,
                                const u8*   pu8_Src,
                                u16         u16_SrcLen );

///////////////////////////////////////////////////////////////////////////////
/// @brief      Convert Hex string to binary array
///
/// @param[in]  srcString       Hex string
/// @param[in]  dstBuffer       buffer for output
///
/// @return     message buffer length
///////////////////////////////////////////////////////////////////////////////
int p_StringUtils_HexStringToBin( const char* hexString, u32 hexStringLen, u8* pu8_Dst, u32 u32_DstBufSize );

// Convert terminated Hex string text buffer into binary array
int p_StringUtils_HexStringTermToBin( const char* hexString, u8* pu8_Dst, u32 u32_DstBufSize );

#endif // VAR_ON_CMND

///////////////////////////////////////////////////////////////////////////////
/// @brief      Add unit friendly name to buffer of friendly names
///
/// @param[in,out]  pu8_Dst         - Pointer to friendly name buffer
/// @param[in]      u8_DstSize      - Size of friendly name buffer
/// @param[in]      u8_UnitId       - Unit ID
/// @param[in]      pc_UnitName     - Unit name
/// @param[out]     pu8_DstUsedLen  - Used length of pu8_Dst buffer
///
/// @return     Number of bytes, used in destination buffer
///////////////////////////////////////////////////////////////////////////////
bool p_StringUtils_AddDeviceFriendlyName(   INOUT   u8*         pu8_Dst,
                                                    u8          u8_DstSize,
                                                    u8          u8_UnitId,
                                                    const char* pc_UnitName,
                                            OUT     u8*         pu8_DstUsedLen );

extern_c_end

#endif  // _CMND_API_STRING_UTILS_H
