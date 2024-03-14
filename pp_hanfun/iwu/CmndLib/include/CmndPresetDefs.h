/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CMND_PRESET_DEFS_H
#define _CMND_PRESET_DEFS_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern_c_begin

#ifdef WIN32
#pragma pack(1)
#endif

///////////////////////////////////////////////////////////////////////////////
/// Logical Event Names . Output - MSb = 0 (0x00 - 0x7F) , Input - Msb = 1 (0x80 - 0xFD)
/// if no specific GPIO is needed use eGPIO_HAN_DONT_CARE   (0xFE)
/// for  a non valid GPIO.. use eGPIO_HAN_INVALID           (0xFF).
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    eGPIO_OUT_DEFINE_START  = 0x0,
    eGPIO_READY_2_SLEEP     = 0x1,
    eGPIO_LED_ONE           = 0x2,
    eGPIO_LED_TWO           = 0x3,
    eGPIO_LED_THREE         = 0x4,
    eGPIO_LED_FOUR          = 0x5,
    eGPIO_LED_FIVE          = 0x6,
    eGPIO_LED_SIX           = 0x7,
    eGPIO_SWITCH_ONE        = 0x8,
    eGPIO_SWITCH_TWO        = 0x9,
    eGPIO_SWITCH_THREE      = 0xa,
    eGPIO_SWITCH_FOUR       = 0xb,
    eGPIO_SWITCH_FIVE       = 0xc,
    eGPIO_SWITCH_SIX        = 0xd,

    // add here new output GPIO events but before eGPIO_OUT_DEFINE_END

    eGPIO_OUT_DEFINE_END,

    // make sure all GPO are defined until here
    eGPIO_IN_DEFINE_START   = 0x80,
    eGPIO_IN_ONE            = 0x81,
    eGPIO_IN_TWO            = 0x82,
    eGPIO_IN_THREE          = 0x83,
    eGPIO_IN_FOUR           = 0x84,
    eGPIO_IN_FIVE           = 0x85,
    eGPIO_IN_SIX            = 0x86,
    eGPIO_IN_INTERVAL       = 0x87,
    eGPIO_REGISTER          = 0x88,
    eGPIO_TAMPER            = 0x89,
    eGPIO_ALARM             = 0x8a,
    eGPIO_SUPERVISION       = 0x8b,
    eGPIO_LOW_BAT           = 0x8c,
    eGPIO_AC_LOST           = 0x8d,
    eGPIO_EXT_WAKEUP        = 0x8e, // Wakeup from external MCU
    eGPIO_ALARM_TOGGLE      = 0x8f,
    eGPIO_DEREGISTER        = 0x90,
    eGPIO_IN_INTERVAL_ONE   = 0x91,
    eGPIO_IN_INTERVAL_TWO   = 0x92,

    eGPIO_IN_DEFINE_END,

    eGPIO_HAN_DONT_CARE = 0xFE,     // Not a specific gpio, used so the GPIO won't be referred as eGPIO_HAN_INVALID.

    eGPIO_HAN_INVALID =0xFF         // Max,
}
t_en_hanGpioLogicalEvent;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define HAN_GPIO_IS_OUTPUT(x)   ( (x) > eGPIO_OUT_DEFINE_START && (x) < eGPIO_OUT_DEFINE_END )
#define HAN_GPIO_IS_INPUT(x)    ( (x) > eGPIO_IN_DEFINE_START && (x) < eGPIO_IN_DEFINE_END )

///////////////////////////////////////////////////////////////////////////////
// Information Element: CMND_IE_ADD_REPORT_INFO
///////////////////////////////////////////////////////////////////////////////

// max 4 entries for registration
typedef PACK_STRUCT
{
    u8  u8_ReportIdWithType;
    u16 u16_ReceiverDevAddr;
    u8  u8_ReceiverUnitId;
    u32 u32_PeriodicInterval;
}
t_st_hanCmndCmndIeAttrReportTable;

// max 6 entries
typedef PACK_STRUCT
{
    u8      u8_ReportId;
    u8      u8_ReportEntryId;
    u8      u8_UnitId;
    u16     u16_InterfaceId;
    u8      u8_AttrPackId;
    u8      u8_TypeOfReporting;
}
t_st_hanCmndCmndIeAttrReportEntry;

// max 6 elements
typedef PACK_STRUCT
{
    u8      u8_ReportEntryId;
    u8      u8_AttrId;
    u8      u8_TypeOfReporting;
    u8      u8_AttrSize;
    u32     u32_AttrValue;
}
t_st_hanCmndCmndIeAttrReportDataField;

///////////////////////////////////////////////////////////////////////////////
/// Map/Route message for Unit + CMND API service id to specific Plugin
/// For example: map suota for unit 0 to CMND API uart plugin
///////////////////////////////////////////////////////////////////////////////
typedef PACK_STRUCT
{
    u8              UnitId;             //!< Which unit to map to plugin
    u16             InterfaceId;        //!< CMND API service id
    u8              eGpioName;          //!< see t_en t_en_hanGpioLogicalEvent, if no specific GPIO is needed use eGPIO_HAN_DONT_CARE,
                                        //!<        for  a non valid GPIO.. use eGPIO_HAN_INVALID.
    u8              PlugIn;             //!< see t_en_epPlugInSupported
    u8              PlugInParam;        //!< parameter based on plugin type
}
t_st_hanUnit2Plugin_Map;


//////////////////////////////////////////////////////////////////////////
// han\DeviceManager\hhanUnitManager.h
//////////////////////////////////////////////////////////////////////////


enum
{
    HAN_UNIT_MANAGER_MAX_OPTIONAL_INTERFACES                = 8,    /// Max optional interfaces when registering a device
    HAN_UNIT_MANAGER_UNIT_ENTRY_HEADER_SIZE                 = 3,    /// unit id, unit type (WITHOUT Number of optional Interfaces)
    HAN_UNIT_MANAGER_UNIT_ENTRY_HEADER_WITH_OPT_IFS_SIZE    = 4,    /// unit id, unit type and Number of optional Interfaces
};


/// Struct to read eeprom configuration of unit id and its type
typedef PACK_STRUCT
{
    u8              u8_UnitId;
    u16             u16_UnitType;
    u8              u8_NumOptIfs;
    u16             u16_OptionalIfs[HAN_UNIT_MANAGER_MAX_OPTIONAL_INTERFACES];
}t_st_hanUnitIdType;

enum
{
    EP_UNIT_INFO_TABLE_SIZE     = 160,
    HAN_EEPROM_UNIT_INFO_SIZE   = EP_UNIT_INFO_TABLE_SIZE/sizeof(t_st_hanUnitIdType),
};

/// This is enumeration of CMND API plugins
///
/// The next EEPROM parameter - EP_CMND_LINK_LAYER - is holding
/// which plugins are being used
///
/// UART - support only UART
/// GPIO - support only GPIO
/// UART_GPIO - support both
typedef enum
{
    HAN_UNIT_MANAGER_PLUGINID_UART              = 0x1,
    HAN_UNIT_MANAGER_PLUGINID_ULEGPIO           = 0x2,
    HAN_UNIT_MANAGER_PLUGINID_GPIO              = 0x3,
    HAN_UNIT_MANAGER_PLUGINID_SPI               = 0x4,
    HAN_UNIT_MANAGER_PLUGINID_SIGNLE_PIN        = 0x5,
    HAN_UNIT_MANAGER_PLUGINID_SIMPLE_PWR_MTR    = 0x6,
    HAN_UNIT_MANAGER_PLUGINID_HIBERNATE_CMND    = 0x7,
    HAN_UNIT_MANAGER_PLUGINID_INVALID           = 0x8,
    HAN_UNIT_MANAGER_PLUGINID_DBM_UART          = 0x9,
    HAN_UNIT_MANAGER_PLUGINID_TEMPLATE          = 0xa,
    HAN_UNIT_MANAGER_PLUGINID_SWS_BTN           = 0xb,
    HAN_UNIT_MANAGER_PLUGINID_EUROT             = 0xc,
}
t_en_epPlugInSupported;


/// We read from EEPROM  EP_DEMO_TYPE
typedef enum
{
    HAN_UNIT_MANAGER_HW_TYPE_DSPG           = 0x0,
    HAN_UNIT_MANAGER_HW_TYPE_CROW           = 0x1,
    HAN_UNIT_MANAGER_HW_TYPE_GNRGY          = 0x2,
    HAN_UNIT_MANAGER_HW_TYPE_TYCO           = 0x3,
    HAN_UNIT_MANAGER_HW_TYPE_SWS            = 0x4,
    HAN_UNIT_MANAGER_HW_TYPE_GAP            = 0x5,
    HAN_UNIT_MANAGER_HW_TYPE_GAP_SH         = 0x6,
    HAN_UNIT_MANAGER_HW_TYPE_LGIT           = 0x7,
    HAN_UNIT_MANAGER_HW_TYPE_EUROT          = 0x8,
}
t_en_epHWTypeSupported;


//////////////////////////////////////////////////////////////////////////
// han\PlugIns\hhanUleGpio.h
//////////////////////////////////////////////////////////////////////////


/// GPIO Configuration Defines
#define HAN_ULE_GPIO_EVENT_MAP_TABLE_SIZE           (6)     // size of table that maps ULE Gpio event to logical event
#define HAN_ULE_UNIT2PLUGIN_EVENT_MAP_TABLE_SIZE    (EP_UNIT_PLUGIN_MAP_END - EP_UNIT_PLUGIN_MAP)/sizeof(t_st_hanUnit2Plugin_Map)
#define HAN_ULE_GPIO_DEBOUNCER_MAP_TABLE_SIZE       (2)     // special debouncer mapping table size

// how much time to delay hibernation on first bootup or battery change
// in other words when waking up NOT form hibernation
#define ULE_GPIO_PLUGIN_DELAY_HBR_ON_BOOT_TIME_MS   (5000)

/// Error code for p_hanUleGpioPlugin_GetGpiState that signifies that the pin name provided was invalid.
#define ULE_GPIO_INVALID_PIN_NAME                   0xFF


/**
 * ULE GPIO pin names (both for input and output)
 */
typedef enum
{
    ULE_GPIO_PIN_NAME_ANA_IN1_DIG1_WU   = 0, // DBNC_DIG1_WU
    ULE_GPIO_PIN_NAME_ANA_IN2_DIG2_WU   = 1, // DBNC_DIG2_WU
    ULE_GPIO_PIN_NAME_AMP1_OUT_DIG3_WU  = 2, // DBNC_DIG3_WU
    ULE_GPIO_PIN_NAME_AMP2_OUT_DIG4_WU  = 3, // DBNC_DIG4_WU
    ULE_GPIO_PIN_NAME_DOUBCAP2P_DIG5_WU = 4, // DBNC_DIG5_WU
    ULE_GPIO_PIN_NAME_LEDSINK2_DIG6_WU  = 5, // DBNC_DIG6_WU
    ULE_SENSOR_COND_COMP1_WU = 6,            // DBNC_COMP1_WU
    ULE_SENSOR_COND_COMP2_WU = 7,            // DBNC_COMP2_WU
    ULE_GPIO_PIN_NAME_DCINPIN   = 8,

    ULE_GPIO_PIN_LAST,
}
t_en_hanUleGpio_PinName;


/**
 * Define ULE GPO state to be set during hibernation
 * Clear the relevant bitmask for opposite behavior
 * For example: To enable GPO when dhx91 in hibernation use opposite of ULE_GPIO_HBR_STATE_DISABLED) meaning 0x00
 */
typedef enum
{
    ULE_GPIO_HBR_STATE_DISABLED     = 0x01, // GPIO is disabled in hbr, if bit if OFF then gpio is enabled
    ULE_GPIO_HBR_PULL_ENABLED       = 0x02, // enable pulling in hbr, 0 in this bitmaks disables pulling
    ULE_GPIO_HBR_PULL_TYPE_UP       = 0x04, // enable pull up, if bit off then this is pull down. only relevant if ULE_GPIO_HBR_PULL_ENABLED bit is ON
}
t_en_hanUleGpio_GpoHibernateState;



/**
 *  This is a configurtion for ULE GPI (input) or GPO (output)
 *  dhx91 has 6 gpios overall, each gpio has a unique identifier when used for Input and when usef for output
 *  See table 1-71 in DHX TRM ver 0.3 "ULE GPIO Functionallity".
 *

    PIN NAME        INPUT SIGNAL          OUTPUT SIGNAL           ALTERNATE FUNCTION
    ---------------------------------------------------------------------------------
    ANA_IN1         ULE_DIG_WU1           ULE_GPO1                ULE_PWM0
    ANA_IN2         ULE_DIG_WU2           ULE_GPO2                N/A
    AMP1_OUT        ULE_DIG_WU3           ULE_GPO3                N/A
    AMP2_OUT        ULE_DIG_WU4           ULE_GPO4                SLW_CLK_OUT
    DOUBCAP2P       ULE_DIG_WU5           N/A                     N/A
    LEDSINK2        ULE_DIG_WU6           ULE_GPO6                ULE_PWM1 (PWM with LED sink capability)
    ---------------------------------------------------------------------------------

 *  Each record maps logical event (PinName) to inpout debouncer or output GPO resource.
 *
*/
typedef PACK_STRUCT
{
    u8              PinName;                    // ULE GPIO pin identifier. see t_en_hanUleGpio_PinName

    u8              Event;                      // ULE GPIO logical event name. each gpio must have a unique name. see t_en_hanGpioLogicalEvent
                                                // For input direction use input identifier from t_en_hanGpioLogicalEvent enum
                                                // For output direction use the output identifiers from t_en_hanGpioLogicalEvent enum

    u8              Resource;                   // An INPUT gpio must be asigned to ULE debouncer,
                                                // this is the HW that performs debouncing for ULE gpio signal and generates an interupt or triggers wakeup from hbr
                                                // Set this field to one of ULE debouncers from next enum t_en_DBNC_UNIT
                                                // Any ULE debouncer can be chosen, as long as each debouncer is mapped to a singler ULE gpio.

                                                // An OUTPUT gpio must be assigned to output driver, see next enum t_en_hanUleGpio_GpoTranslate
                                                // The output resource must be assigned according to DHX91 TRM based on selected PinName
                                                // For example: For PinName=ULE_GPIO_PIN_NAME_ANA_IN1_DIG1_WU use ULE_GPIO_TRANSLATE_GPO1
                                                //              For PinName=ULE_GPIO_PIN_NAME_AMP2_OUT_DIG4_WU use ULE_GPIO_TRANSLATE_GPO4

    u8              DestinationUnitId;          // Destination unit ID that will receive the logical event <Event>, values are 0, 1, 2, ...

    u8              Alternate;                  // OUTPUT gpio only. Select a Pulse Width Modulation resource for output gpio.
                                                // for OUTPUT gpios see t_en_ULE_GPO_PWM
                                                // for Input gpios use ULE_PWM_LAST
                                                // Use ULE_PWM0 for ULE_GPIO_PIN_NAME_ANA_IN1_DIG1_WU
                                                // Use ULE_PWM1 for ULE_GPIO_PIN_NAME_LEDSINK2_DIG6_WU

    u8              DefaultLevel;               // OUTPUT gpio only. Default output value for OUTPUT gpio. Values are 0 or 1. Not used for Input gpio
    u8              PwmHigh;                    // OUTPUT gpio only. Duration in high state in ms for alternation function
    u8              PwmLow;                     // OUTPUT gpio only. Duration in low state in ms for alternation function
    u8              IsInterrupt;                // Input gpio only. 0 or 1, whether event can be triggered as interrupt when device is UP (not wakeup from hbr)
    u8              Polarity;                   // Defines the polarity of gpio (both input and output)
    u8              Freq;                       // Input gpio only. Define ULE debouncer frequency. see t_en_DBNC_FREQ (speed at which ULE GPI is monitored)
    u8              Mode;                       // Input gpio only. see t_en_DBNC_MODE
    u8              Trig;                       // Input gpio only. Interrupt type for ULE debouncer. t_en_DBNC_TRIG

    u8              GpoControlBits;             // OUTPUT gpio only. t_en_hanUleGpio_GpoHibernateState - configure GPO enable/disable, pull up/down enalbe/disable
                                                // to configure "enable" set this

}
t_st_hanUleGpio_Map;


typedef  enum
{
    ULE_PWM0,
    ULE_PWM1,
    ULE_PMW_LAST
}t_en_ULE_GPO_PWM;


/**
 * For debouncer 0,1 you can use extended mode, configure here
 */
typedef PACK_STRUCT
{
    u8              Resource;
    u8              Mode;
    u16             Count0;
    u16             Count1;
    u32             extendedCompareVal;
    u16             compareVal;
    u16             periodicVal;
}
t_st_hanUleDebounceExt;


/**
 * ULE GPO (output) enum translation - as GPO defines are complicated for
 * user to use in our EEPROM
 */
typedef enum GPIO_TRANSLATE_GPO
{
    ULE_GPIO_TRANSLATE_GPO1 = 1,        // ANA_IN1         ULE_DIG_WU1
    ULE_GPIO_TRANSLATE_GPO2 = 2,        // ANA_IN2         ULE_DIG_WU2
    ULE_GPIO_TRANSLATE_GPO3 = 3,        // AMP1_OUT        ULE_DIG_WU3
    ULE_GPIO_TRANSLATE_GPO4 = 4,        // AMP2_OUT        ULE_DIG_WU4
    ULE_GPIO_TRANSLATE_GPO5 = 5,        // DOUBCAP2P       ULE_DIG_WU5
    ULE_GPIO_TRANSLATE_GPO6 = 6,        // LEDSINK2        ULE_DIG_WU6

    ULE_GPIO_TRANSLATE_GPO_LAST = 7,
}
t_en_hanUleGpio_GpoTranslate;


//////////////////////////////////////////////////////////////////////////
// han\PagingService\hhanPagingService.h
//////////////////////////////////////////////////////////////////////////


typedef PACK_STRUCT
{
    u8  u8_Enabled;         // Is paging enabled or not
    u32 u32_IntervalMs;     // Paging interval in millisec
}
t_st_hanPagingSvc_Settings;



//////////////////////////////////////////////////////////////////////////
// han\DeviceManager\hhanKeepAliveService.h
//////////////////////////////////////////////////////////////////////////


#define HHAN_KEEP_ALIVE_STYLE_SIMPLE        0x0     // Send FUN standard keep alive
#define HHAN_KEEP_ALIVE_STYLE_WITH_RSSI     0x1     // Send FUN standrard keep alive + rssi in same message


//////////////////////////////////////////////////////////////////////////
// han\Common\chanGeneral.h
//////////////////////////////////////////////////////////////////////////


#define HAN_GEN_FLAG_NOTIFY_ON_REGISTRATION (0x1)
#define HAN_GEN_FLAG_COIN_BAT_BEHAVIOR      (0x2)
#define HAN_GEN_FLAG_COMPATIBILLITY_1_1     (0x4)


//////////////////////////////////////////////////////////////////////////
// han\Tools\HanCmndTester\uartTest\uartTest\ConfWizOscTypeDlg.cpp
//////////////////////////////////////////////////////////////////////////


#define ULE_OSC32   0
#define ULE_RC      1


//////////////////////////////////////////////////////////////////////////
// han\DeviceManager\hhanSleepService.h
//////////////////////////////////////////////////////////////////////////

// Following defines are for EP_HAN_SYS_OFF_USED eeprom values, for setting hibernation mode

/// System off local - Sleep Manager will go to hibernate mode after processing all events, standalone
#define EP_HAN_SYS_OFF_USED_LOCALLY 0x10

/// System off MCU - Sleep Manager waits for an event from MCU in order to hibernate
#define EP_HAN_SYS_OFF_USED_MCU     0x11

/// System off MCU - Sleep Manager does not go to hibernation (for example AC Outlet)
#define EP_HAN_SYS_OFF_USED_NONE    0x12


// Use these timeout to count also for hibernation watchdog counter g_hibernationWatchdog
// Periodic check by Sleep Manager if system is ready to hibernate
#define HAN_SLEEP_SERVICE_DEFAULT_PERIODICAL_TIME   (10)            // default is to check every 10ms

// slow is to check ever 10sec
// NOTE: to allow entering Scorpion Idle mode (when using coincell for example)
// this value must be greater than HAN_POWER_NOTIFY_INTERVAL_BAT_LEVEL_TIME (hhanPower.c)
#define HAN_SLEEP_SERVICE_SLOW_PERIODICAL_TIME      (10000)


//////////////////////////////////////////////////////////////////////////
// han\ConnectivityServer\hhanConnectivityServer.h
//////////////////////////////////////////////////////////////////////////


enum
{
    EEPROM_REG_STATUS_REGISTERED                    = 5,
    EEPROM_REG_STATUS_NOT_REGISTERED                = 6,
};


//////////////////////////////////////////////////////////////////////////
// han\Registrar\hhanRegistrar.h
//////////////////////////////////////////////////////////////////////////


//Auto registration modes.
enum
{
    EP_REGISTRAR_ENABLE_AUTO_REGISTRATION_ENABLED               = 0x11, // Start registration on powerup/wakeup if not registered
    EP_REGISTRAR_ENABLE_AUTO_REGISTRATION_DISABLED              = 0x12, // Dont do anything if not registered on powerup/wakeup
    EP_REGISTRAR_ENABLE_AUTO_REGISTRATION_EN_ON_BAT_INSERTION   = 0x13, // Auto registration is enabled ONLY on battery insertion (powerup)
};


//////////////////////////////////////////////////////////////////////////
// han\RfTestMode\hhanRfTestMode.h
//////////////////////////////////////////////////////////////////////////


#define HHAN_RF_TEST_MODE_ON   0x01
#define HHAN_RF_TEST_MODE_OFF  0x00


//////////////////////////////////////////////////////////////////////////
// han\RfTestMode\hhanRfTestMode.h
//////////////////////////////////////////////////////////////////////////


// Define CMND API Service Id Internal
typedef enum
{
    CMND_SERVICE_ID_BIND_MANAGEMEN                  = 0x7F00,
    CMND_SERVICE_ID_GROUP_MANAGEMENT                = 0x7F01,
    CMND_SERVICE_ID_DEVICE_INFORMATION              = 0x7F02,
    CMND_SERVICE_ID_SIMPLE_POWER_METERING           = 0x7F03,
    CMND_SERVICE_ID_RF_TEST_MODE                    = 0x7F04,
    CMND_SERVICE_ID_EEPROM_ACCESS                   = 0x7F05,
    CMND_SERVICE_ID_CNTCT_BASE_IN_MULTIFRAME        = 0x7F06,
    CMND_SERVICE_ID_LED_INDICATION                  = 0x7F07,
    CMND_SERVICE_ID_DEVICE_STATISTICS               = 0x7F08,
    CMND_SERVICE_ID_VOICE_PROMPTER                  = 0x7F09,
    CMND_SERVICE_ID_TEMPERATURE                     = 0x7F0A,
    CMND_SERVICE_ID_POWER                           = 0x7F0B,
    CMND_SERVICE_ID_NOTIFICATION                    = 0x7F0C,
    CMND_SERVICE_ID_LOG_DATA                        = 0x7F0D,
    CMND_SERVICE_ID_BUTTON                          = 0x7F0E,
    CMND_SERVICE_ID_LED                             = 0x7F0F,
    CMND_SERVICE_ID_SWS_PRODUCTION                  = 0x7F10,
    CMND_SERVICE_ID_SIMPLE_BUTTON                   = 0x7F11,
    CMND_SERVICE_ID_SIMPLE_VISUAL_CONTROL           = 0x7F12,

    CMND_SERVICE_ID_INTERNAL_LAST,
}
t_en_hanCmndServiceIdInternal;


//////////////////////////////////////////////////////////////////////////
// han\...\cdrUleDbnc.h
//////////////////////////////////////////////////////////////////////////


typedef  enum
{
    DBNC_UNIT0 = 0,
    DBNC_UNIT1 = 1,
    DBNC_UNIT2 = 2,
    DBNC_UNIT3 = 3,
    DBNC_DCINS_UNIT = 4,
    DBNC_LAST_UNIT,
}
t_en_DBNC_UNIT;


typedef  enum
{
    DBNC_DIG1_WU = 0, // ANA_IN1
    DBNC_DIG2_WU = 1, // ANA_IN2
    DBNC_DIG3_WU = 2, // AMP1_OUT
    DBNC_DIG4_WU = 3, // AMP2_OUT
    DBNC_DIG5_WU = 4, // DOUBCAP2P
    DBNC_DIG6_WU = 5, // LEDSINK2
    DBNC_COMP1_WU = 6, //from sensor condition
    DBNC_COMP2_WU = 7, //from sensor condition
    DBNC_DCINS = 8, //only for DCINS unit allowed
}
t_en_DBNC_INPUT;


typedef  enum
{
    DBNC_POL_NORMAL = 0,
    DBNC_POL_INVERTED = 1,
}
t_en_DBNC_POL;


typedef  enum
{
    DBNC_FREQ_32768HZ = 0, //30.5us
    DBNC_FREQ_1024HZ = 1, //0.97ms
    DBNC_FREQ_512HZ = 2, //1.95ms
    DBNC_FREQ_256HZ = 3, //3.9ms
    DBNC_FREQ_128HZ = 4,//7.8ms
    DBNC_FREQ_64HZ = 5, //15.625ms
    DBNC_FREQ_32HZ = 6,//31.25ms
    DBNC_FREQ_16HZ = 7, //62.5ms
}t_en_DBNC_FREQ;


typedef  enum
{
    DBNC_NORMAL_MODE        = 0, // 3 consecutive clks
    DBNC_COMPARE_MODE       = 1, //UNIT 0 & 1
    DBNC_PERIODIC_MODE      = 2, //UNIT 0 & 1
    DBNC_EXT_COMPARE_MODE   = 3, //UNIT 1 only
    DBNC_TIME_WINDOW_MODE   = 4, //UNIT 1 only
    DBNC_BYPASS_MODE        = 5, // 1 clk
    DBNC_LATCH_MODE         = 6, // 1 clk or even less
}
t_en_DBNC_MODE;


typedef  enum
{
    DBNC_LEVEL_TRIG = 0,
    DBNC_EDGE_TRIG = 1,
}
t_en_DBNC_TRIG;


typedef  enum
{
    DBNC_OK = 0,
    DBNC_WRONG_PARAM = -1,
    DBNC_NOT_SUPPORTED = -2,
    DBNC_INPUT_ALREADY_USED = -3,
    DBNC_UNIT_OFF = -4,
    DBNC_INVALID = -5,
}
t_en_DBNC_ERROR_CODE;

#ifdef WIN32
#pragma pack()
#endif

extern_c_end

#endif  //_CMND_PRESET_DEFS_H
