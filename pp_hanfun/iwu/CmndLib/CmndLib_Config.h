/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMNDLIB_CONFIG_H
#define _CMNDLIB_CONFIG_H
#include "Logger.h"

// constants
enum
{
    CMNDLIB_SUBSCRIBERS_CAPACITY            = 5,    //!< Maximum subscribers available with p_CmndTransport_Subscribe
    CMNDLIB_DATA_PAYLOAD_MAX_LENGTH         = 167,  //!< Maximum size of CMND data payload
    CMNDLIB_API_PACKET_MAX_SIZE             = 250,  //!< Maximum size of CMND API message
    CMNDLIB_LOG_LEVEL                       = (LOG_LEVEL_ALL & ~LOG_LEVEL_TRACE), //!< A bit mask of enabled log levels. See t_en_hanLogLevel.
    //CMNDLIB_LOG_LEVEL    = LOG_LEVEL_NOTSET, //!< Logs disabled
};


#endif // _CMNDLIB_CONFIG_H
