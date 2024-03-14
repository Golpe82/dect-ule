/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef C_HAN_LOGGER_H
#define C_HAN_LOGGER_H
#include "TypeDefs.h"
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

///////////////////////////////////////////////////////////////////////////////
/// @brief      This enum is logging level mask
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    LOG_LEVEL_NOTSET    = 0,        //!< No one log level enabled
    LOG_LEVEL_DEBUG     = 1 << 0,   //!< bit mask of DEBUG log level
    LOG_LEVEL_TRACE     = 1 << 1,   //!< bit mask of TRACE log level
    LOG_LEVEL_INFO      = 1 << 2,   //!< bit mask of INFO log level
    LOG_LEVEL_WARN      = 1 << 3,   //!< bit mask of WARN log level
    LOG_LEVEL_ERROR     = 1 << 4,   //!< bit mask of ERROR log level
    LOG_LEVEL_PORTIO    = 1 << 5,   //!< bit mask of PORTIO log level

    LOG_LEVEL_ALL       = ~0,       //!< All log levels enabled
} t_en_hanLogLevel;

#include "CmndLib_Config.h"

#define LOG_DEBUG(format,...)   WRITE_LOG_LINE( LOG_LEVEL_DEBUG,  format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...)  WRITE_LOG_LINE( LOG_LEVEL_TRACE,  format, ##__VA_ARGS__)
#define LOG_INFO(format,...)    WRITE_LOG_LINE( LOG_LEVEL_INFO,   format, ##__VA_ARGS__)
#define LOG_WARN(format,...)    WRITE_LOG_LINE( LOG_LEVEL_WARN,   format, ##__VA_ARGS__)
#define LOG_ERROR(format,...)   WRITE_LOG_LINE( LOG_LEVEL_ERROR,  format, ##__VA_ARGS__)
#define LOG_PORTIO(format,...)  WRITE_LOG_LINE( LOG_LEVEL_PORTIO, format, ##__VA_ARGS__)


#define IS_LOG_LEVEL_USED(level) ( (u32)CMNDLIB_LOG_LEVEL & (level) )

#define WRITE_LOG_LINE( en_LogLevel, format, ... )  \
    do\
    {\
        if( IS_LOG_LEVEL_USED( en_LogLevel ) )\
        {\
            printf(format "\n", ##__VA_ARGS__ ); \
        }\
    } while ( 0 )

#define LOG_BUFFER( en_LogLevel, buffer, buffer_size, format, ... ) \
    do\
    {\
        if( IS_LOG_LEVEL_USED( en_LogLevel ) )\
        {\
            unsigned int i = 0; \
            printf(format "", ##__VA_ARGS__ ); \
            printf("[%u] ", (unsigned int)buffer_size ); \
            for ( i = 0; i < buffer_size; i++ ) \
            { \
                printf( "%02x ", ((u8*)buffer)[i] ); \
            } \
            printf( "\n" ); \
        }\
    } while ( 0 )

extern_c_end

#endif // C_HAN_LOGGER_H
