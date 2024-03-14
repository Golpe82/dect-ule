/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_API_HOST_H_
#define _CMND_API_HOST_H_

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "CmndApiExported.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma pack(1)
#endif

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////

// CMND API protocol constants
enum
{
    CMND_API_PROTOCOL_SIZE_HEADER               = 4,                            //!< Size of CMND API protocol header (start code and message length)
    CMND_API_PROTOCOL_SIZE_MANDATORY_FIELDS     = 10,                           //!< Size of CMND API mandatory fields
    CMND_API_PROTOCOL_SIZE_WITHOUT_DATA         = 6,                            //!< Message size without Header and data
    CMND_API_PROTOCOL_COOKIE_POS                = 0,                            //!< Position where message cookie placed
    CMND_API_PROTOCOL_UNITID_POS                = 1,                            //!< Position where UnitId placed
    CMND_API_PROTOCOL_SERVICEID_POS             = 2,                            //!< Position where CMND Service Id placed
    CMND_API_PROTOCOL_MESSAGEID_POS             = 4,                            //!< Position where CMND Message Id placed
    CMND_API_PROTOCOL_CHECKSUM_POS              = 5,                            //!< Position where message checksum placed
    CMND_API_PROTOCOL_DATASTART_POS             = 6,                            //!< Position of data begin
    CMND_API_PROTOCOL_CHECKSUM_POS_WITH_HEADERS = (CMND_API_PROTOCOL_CHECKSUM_POS + CMND_API_PROTOCOL_SIZE_HEADER), //!< Checksum position with header

    //CMND_API_PAYLOAD_MAX_LENGTH                   = HHANAPP_CMND_DATA_PAYLOAD_MAX_LENGTH, //!< Maximum size of CMND API message data payload
};


///////////////////////////////////////////////////////////////////////////////
// Attribute Id
///////////////////////////////////////////////////////////////////////////////
typedef u8 t_u8_AttributeId;

// An attribute id values
typedef enum
{
    ATTRIBUTE_ID_MIN = 0x00,    //!< A minimal attribute id value
    ATTRIBUTE_ID_MAX = 0xFE,    //!< A maximal attribute id value
    ATTRIBUTE_ID_ALL = 0xFF,    //!< A broadcast attribute id value
}
t_en_AttributeId;

///////////////////////////////////////////////////////////////////////////////
// Report Id
///////////////////////////////////////////////////////////////////////////////

// A Report Id with type contains Report Type(U1) and Report id(U7)
typedef u8 t_ReportIdWithType;
#define REPORT_KEY_ID_MASK 0x7F
#define REPORT_KEY_FREE 0xFF

STATIC_ASSERT( sizeof( t_ReportIdWithType ) == 1, sizeof_t_ReportIdWithType_must_be_1 );

// Unique report Id (U7), a part of t_ReportIdWithType
typedef u8 t_en_ReportId;
#define REPORT_ID_MIN 0         //!< A minimal report id value
#define REPORT_ID_MAX 0x7e      //!< A maximal report id value
#define REPORT_ID_ALL  0x7F     //!< Special report id value, which selects all reports
                                //!< (It is used only to remove all reports)

// Report type, a part of t_ReportIdWithType
typedef enum
{
    REPORT_TYPE_PERIODIC    = 0x00, //!< A periodic report type
    REPORT_TYPE_EVENT       = 0x01, //!< An event report type
    REPORT_TYPE_MASK        = 0x80, //!< A report type mask
}
t_en_ReportType;


// Macro for combination of Report type and report id to report Id with type
#define REPORT_KEY_COMBINE( en_ReportType, en_ReportId )    ( ( t_ReportIdWithType ) \
    ( ( ( ( en_ReportType ) & 0x01 ) << 7 ) | ( ( en_ReportId ) & REPORT_KEY_ID_MASK ) ) )
// Macro for validating a value of ReportId with type
#define REPORT_KEY_IS_VALID(reportIdWithType) ( REPORT_KEY_ID( reportIdWithType ) <= REPORT_ID_MAX )

// Macro for allocation of report type from Report Id with type
#define REPORT_KEY_TYPE( u8_ReportKey ) ( ( t_en_ReportType ) ( ( ( u8_ReportKey ) >> 7 ) & 0x01 ) )
#define REPORT_KEY_ID( u8_ReportKey )   ( ( t_en_ReportId ) ( ( u8_ReportKey ) & REPORT_KEY_ID_MASK ) )

///////////////////////////////////////////////////////////////////////////////
// Device Interface Id
///////////////////////////////////////////////////////////////////////////////

// An interface addr value contained of attribute id type(U1) and attribute id(U15)
typedef u16 t_u16_InterfaceAddr;

// An interface id values
typedef enum
{
    INTERFACE_MIN_VALUE = 0x0000,   //!< A minimal interface id value
    INTERFACE_MAX_VALUE = 0x7FFE,   //!< A maximal interface id value
    INTERFACE_ALL       = 0x7FFF,   //!< A broadcast interface id value
    INTERFACE_MASK      = 0x7FFF,   //!< An interface id value mask
}
t_en_Interface;

// An interface id type
typedef enum
{
    INTERFACE_TYPE_CLIENT = 0,      //!< A client interface id
    INTERFACE_TYPE_SERVER = 1,      //!< A server interface id
}
t_en_InterfaceType;

// Macro for combination of attribute id type and attribute id to attribute address
#define INTERFACE_COMBINE( en_InterfaceType, en_Interface ) ( ( t_InterfaceId ) \
    ( ( ( ( en_InterfaceType ) & 0x01 ) << 15 ) | ( ( en_Interface ) & 0x7FFF ) ) )

// Macro for allocation of attribute type from attribute address
#define INTERFACE_TYPE( InterfaceId )   ( ( t_en_InterfaceType ) ( ( ( InterfaceId ) >> 15 ) & 0x01 ) )

// Macro for allocation of attribute id from attribute address
#define INTERFACE_ID( InterfaceId ) ( ( t_en_Interface ) ( ( InterfaceId ) & 0x7FFF ) )

///////////////////////////////////////////////////////////////////////////////
// Device Address Id
///////////////////////////////////////////////////////////////////////////////

// A Full Device address contained of device address type(U1) and device address(U15)
typedef u16 t_u16_DeviceAddressFull;

// A Device address type
typedef enum
{
    DEVICE_ADDRESS_TYPE_INDIVIDUAL  = 0,    //!< An individual device address type
    DEVICE_ADDRESS_TYPE_GROUP       = 1,    //!< A group device address type
}
t_en_DeviceAddressType;

// Macro for combination of device address type and device address to a full device address
#define DEVICE_ADDRESS_FULL ( en_DeviceAddressType, en_DeviceAddress) ( ( t_u16_DeviceAddressFull )\
    ( ( ( ( en_DeviceAddressType ) & 0x01 ) << 15 ) | ( ( en_DeviceAddress ) & 0x7FFF ) ) )

// Macro for allocation of device address type from full device address
#define  DEVICE_ADDRESS_TYPE ( u16_DeviceAddressFull ) \
    ( ( t_en_DeviceAddressType ) ( ( ( u16_DeviceAddressFull ) >> 15 ) & 0x01 ) )

// Macro for allocation of device address from full device address
#define DEVICE_ADDRESS( u16_DeviceAddressFull ) \
    ( ( t_en_DeviceAddress ) ( ( u16_DeviceAddressFull ) & 0x7FFF ) )

///////////////////////////////////////////////////////////////////////////////
// Unit Id
///////////////////////////////////////////////////////////////////////////////

// A Unit id
typedef u8 t_u8_UnitId;

// A Unit id values
typedef enum
{
    UNIT_ID_MIN  = 0x0,     //!< A minimal unit id value
    UNIT_ID_MAX  = 0xFE,    //!< A maximal unit id value
    UNIT_ID_ALL  = 0xFF,    //!< A broadcast unit id value
}
t_en_UnitId;

///////////////////////////////////////////////////////////////////////////////
// Attribute Pack types
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    ATTRIBUTE_PACK_MANDATORY = 0x00,    //!< A mandatory attribute pack type
    ATTRIBUTE_PACK_ALL       = 0xFE,    //!< An all attribute pack type
    ATTRIBUTE_PACK_DYNAMIC   = 0xFF,    //!< A dynamic attribute pack type
}
t_en_AttributePackUid;

///////////////////////////////////////////////////////////////////////////////
// A report entries
///////////////////////////////////////////////////////////////////////////////

/// Event Report Entry for attribute packs except the special Attribute
/// Pack ID 0xFF - Dynamic.
typedef PACK_STRUCT
{
    t_u8_UnitId         u8_UnitId;          //!< An entry unit id
    t_u16_InterfaceAddr u16_Interface;      //!< An entry interface key
    u8                  u8_AttributePackId; //!< An entry attribute pack
    u8                  u8_TypeOfReporting; //!< An entry type of reporting
}
t_st_EvRepEntry;

/// Event Report Entry for the special Attribute Pack ID 0xFF - Dynamic.
typedef PACK_STRUCT
{
    t_u8_UnitId         u8_UnitId;              //!< An entry unit id
    t_u16_InterfaceAddr u16_Interface;          //!< An entry interface key
    u8                  u8_AttributePackId;     //!< An entry attribute pack
    u8                  u8_NumberOfAttributes;  //!< An entry number of attribute conditions
    u8                  u8_AttrList[1];         //!< A pointer to entry's attribute conditions
}
t_st_EvDynRepEntry;

/// An Attribute condition for Change of Value(COV)
typedef PACK_STRUCT
{
    t_u8_AttributeId    u8_AttributeId;     //!< An attribute condition attribute id
    u8                  u8_TypeOfReporting; //!< An attribute condition type of reporting
}
t_st_EvDynRepCovEntryAttr;

/// An attribute condition for not COV condition
typedef PACK_STRUCT
{
    t_u8_AttributeId    u8_AttributeId;         //!< An attribute condition attribute id
    u8                  u8_TypeOfReporting;     //!< An attribute condition type of reporting
    u8                  u8_AttributeSize;       //!< An attribute condition value size
    u8                  u8_AttributeValue[1];   //!< A pointer to the attribute condition value
}
t_st_EvDynRepNonCovEntryAttr;

/// Periodic Report Entry for attribute packs except the special Attribute
/// Pack ID 0xFF - Dynamic.
typedef PACK_STRUCT
{
    t_u8_UnitId         u8_UnitId;          //!< An entry unit id
    t_u16_InterfaceAddr u16_Interface;      //!< An entry interface key
    u8                  u8_AttributePackId; //!< An entry attribute pack
}
t_st_PerRepEntry;

/// Periodic Report Entry for the special Attribute Pack ID 0xFF - Dynamic.
typedef PACK_STRUCT
{
    t_u8_UnitId         u8_UnitId;              //!< An entry unit id
    t_u16_InterfaceAddr u16_Interface;          //!< An entry interface key
    u8                  u8_AttributePackId;     //!< An entry attribute pack
    u8                  u8_NumberOfAttributes;  //!< An entry number of attribute ids
    u8                  u8_AttributeIds[1];     //!< A pointer to entry's attribute ids
}
t_st_PerDynRepEntry;

///////////////////////////////////////////////////////////////////////////////
// TODO: SUOTA Here?

#define CMND_IE_IMAGE_TYPE_FW_TAG_STR

typedef enum
{
    CMND_IE_IMAGE_TYPE_UNKNOWN      = 0,
    CMND_IE_IMAGE_TYPE_FW           = 1,
    CMND_IE_IMAGE_TYPE_NODE_HOST    = 2,
}
t_en_hanCmndIeImageType;

#define CMND_IE_IMAGE_TYPE_FW_TAG           "DECT"  //!< Image for DECT device
#define CMND_IE_IMAGE_TYPE_NODE_HOST_TAG    "HOST"  //!< Image for MCU Node

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_REGISTRATION
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8 u8_NumOfUnitEntries;
    u8 u8_LengthOfUnitEntry;
    u8 u8_UnitId;
    u16 u16_UnitType;
    u16 u16_NumOptionalClient;
}
t_st_hanCmndIeRegistrationUnitInfo;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma pack()
#endif

#endif  // _CMND_API_EXT_H_
