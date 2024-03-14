/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_LIB_DATA_H
#define _CMND_LIB_DATA_H

#include "TypeDefs.h"
#include "LibInstance.h"
#include "CmndPacketDetector.h"
#include "CmndHibernationMng.h"

extern_c_begin

typedef struct
{
    t_st_PubSub_Channel     st_RxPubSub;
    u8                      au8_RxPubSubBuffer[RX_PUBSUB_BUFFER_SIZE];

    t_Handle                h_Port;
    tpf_IoRead              pf_IoRead;
    tpf_IoWrite             pf_IoWrite;

    bool                    b_Initialized;
    t_stReceiveData         st_RecvData;
    t_st_Msg                st_RcvdMsg;
    t_st_CmndHibernationMng st_HibernationMng;
} t_st_CmndLibData;

extern_c_end

#endif // _CMND_LIB_DATA_H
