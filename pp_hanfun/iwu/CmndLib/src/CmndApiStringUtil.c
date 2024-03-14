/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#include "CmndApiStringUtil.h"
#include "CmndLib_UserImpl_StringUtil.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Make string (byte 1 - length, bytes 2 to n - characters) from null-terminated string
u8 p_StringUtils_MakeString(    u8*         pu8_Dst,
                                u16         u16_DstSize,
                                const char* pc_Src )
{
    u8 u8_StrLen = STATIC_CAST( u8, strlen(pc_Src) );

    if ( u16_DstSize > u8_StrLen )
    {
        // String length
        pu8_Dst[0] = u8_StrLen;

        // String data
        memcpy( &pu8_Dst[1], pc_Src, u8_StrLen );

        // Add size of String length
        u8_StrLen += 1;
    }
    else
    {
        u8_StrLen = 0;
    }

    return u8_StrLen;
}

#ifndef VAR_ON_CMND

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Get string from buffer
bool p_StringUtils_GetString(   char*       pc_Dst,
                                u16         u16_DstSize,
                                const u8*   pu8_Src,
                                u16         u16_SrcLen )
{
    bool ok = false;
    u8 u8_len = pu8_Src[0];


    if ( u16_DstSize > u8_len && u8_len < u16_SrcLen )
    {
        memset( &pc_Dst[u8_len], 0, u16_DstSize - u8_len );
        memcpy( pc_Dst, &pu8_Src[1], u8_len );
        ok = true;
    }
    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Convert Hex string text buffer into binary array
int p_StringUtils_HexStringToBin( const char* hexString, u32 hexStringLen, u8* pu8_Dst, u32 u32_DstBufSize )
{
    const int CHARS_PER_BYTE = 3;
    u32 i = 0;
    u32 u32_StrLen;

    u32_StrLen = p_CmndLib_UserImpl_strnlen( hexString, hexStringLen );

    for( i = 0; i * CHARS_PER_BYTE < u32_StrLen; i ++ )
    {
        int Value = 0;
        // TODO enable it for CLI applications
        //fflush(stdin);
        sscanf( &hexString[i * CHARS_PER_BYTE], "%2x", &Value );
        if( i  < u32_DstBufSize )
        {
            pu8_Dst[i] = Value;
        }
        else
        {
            break;
        }
    }

    return i;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Convert terminated Hex string text buffer into binary array
int p_StringUtils_HexStringTermToBin( const char* hexString, u8* pu8_Dst, u32 u32_DstBufSize )
{
    return p_StringUtils_HexStringToBin( hexString, strlen( hexString ), pu8_Dst, u32_DstBufSize );
}

#endif // VAR_ON_CMND
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Add unit friendly name to buffer
bool p_StringUtils_AddDeviceFriendlyName(           INOUT   u8*         pu8_Dst,
                                                            u8          u8_DstSize,
                                                            u8          u8_UnitId,
                                                            const char* pc_UnitName,
                                                    OUT     u8*         pu8_DstUsedLen )
{
    bool ok = false;
    u8 u8_UnitCnt = pu8_Dst[0];
    u8 u8_UnitNameLen = STATIC_CAST( u8, strlen(pc_UnitName) );
    u8* pu8_Pos = &pu8_Dst[1];
    u8 u8_AvalibleBufferLen = 0;
    u8 u8_NewUnitBufferLen = 0;

    do // as breakable sequence
    {
        // find the end of FriendlyName
        i16 i;
        for ( i=0; i<u8_UnitCnt; i++)
        {
            u8 len = 0;
            pu8_Pos++;
            len = *pu8_Pos;

            if ( (pu8_Pos - pu8_Dst) + len+1 >= u8_DstSize )
            {
                break;
            }

            pu8_Pos += len+1;
        }

        if ( pu8_DstUsedLen )
        {
            *pu8_DstUsedLen = (pu8_Pos - pu8_Dst);
        }

        u8_AvalibleBufferLen = u8_DstSize - (pu8_Pos - pu8_Dst);
        u8_NewUnitBufferLen = (sizeof(u8_UnitId) + sizeof(u8_UnitNameLen) + u8_UnitNameLen);

        // check available buffer length
        if ( u8_AvalibleBufferLen < u8_NewUnitBufferLen )
        {
            break;
        }

        pu8_Dst[0]++;

        *pu8_Pos = u8_UnitId;
        pu8_Pos++;

        if ( pu8_DstUsedLen )
        {
            *pu8_DstUsedLen += (2 + u8_UnitNameLen + 1 - 1);
        }

        p_StringUtils_MakeString( pu8_Pos, u8_AvalibleBufferLen, pc_UnitName );
        ok = true;
    }
    while ( false );

    return ok;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
