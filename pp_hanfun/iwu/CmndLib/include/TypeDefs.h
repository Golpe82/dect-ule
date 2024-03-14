/*
 * Copyright (c) 2016-2018 DSP Group, Inc.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _TYPE_DEFS_H
#define _TYPE_DEFS_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER

#define FLASHSTR(x) x
typedef __int8              i8;
typedef __int16             i16;
typedef __int32             i32;
typedef __int64             i64;

typedef unsigned __int8     u8;
typedef unsigned __int16    u16;
typedef unsigned __int32    u32;
typedef unsigned __int64    u64;

#elif ARDUINO
#include <avr/pgmspace.h>
#include <stdint.h>
#define FLASHSTR(str)           PSTR(str)
typedef uint64_t                u64;
typedef int8_t                  i8;
typedef int16_t                 i16;
typedef int32_t                 i32;
typedef uint8_t                 u8;
typedef short unsigned int      u16;
typedef uint32_t                u32;


#else
#define FLASHSTR(x) x
#include <stdint.h>

typedef uint8_t                 u8;
typedef uint16_t                u16;
typedef uint32_t                u32;
typedef uint64_t                u64;

typedef int8_t                  i8;
typedef int16_t                 i16;
typedef int32_t                 i32;
typedef int64_t                 i64;

#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef VAR_ON_CMND
#ifndef bool

    typedef u16                     bool_type;

    #define true                    (1)
    #define false                   (0)
    #define bool                    bool_type
#endif // #ifndef bool
#else
#ifndef bool
#include <cg0type.h>
#endif
#endif // VAR_ON_CMND

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL                    (0)
#endif // NULL

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef  VAR_ON_CMND

#define MIN(a, b)   ((a) < (b) ? (a): (b))

#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @brief  DECLARE_OPAQUE macro definition. Use in order to export a (void*) type with compiler's
/// @n      type checking support.
///////////////////////////////////////////////////////////////////////////////
#ifndef DECLARE_OPAQUE
#define DECLARE_OPAQUE(name) typedef struct name##__ __##name##__; \
    typedef __##name##__ * name
#endif

///////////////////////////////////////////////////////////////////////////////
/// @brief A macros to easy declare extern "C" { ... } block
/// Usage example:
///
/// extern_c_begin
/// void declare(some* thing);
/// extern_c_end
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
#define extern_c_begin  extern "C" {
#define extern_c_end    }
#else
#define extern_c_begin
#define extern_c_end
#endif

///////////////////////////////////////////////////////////////////////////////
/// @brief IN, OUT and INOUT macros
///////////////////////////////////////////////////////////////////////////////
#ifndef IN
#define IN
#endif // IN

#ifndef OUT
#define OUT
#endif // OUT

#ifndef INOUT
#define INOUT
#endif // INOUT

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif // _MSC_VER

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// brief    Macro which defines a bitfield mask in u8/u16/u32 value
//
// param    FieldName       Variable prefix
//          Type            u8, u16, u32
//          BitOffset       The offset of the bitfield in the value
//          BitWidth        Number of bits in the bitfield
///////////////////////////////////////////////////////////////////////////////
#define DEFINE_BITFIELD( FieldName, Type, BitOffet, BitWidth )                          \
    enum                                                                                \
{                                                                                   \
    FieldName##_OFS         = (BitOffet),                                           \
    FieldName##_WIDTH       = (BitWidth),                                           \
    FieldName##_MASK        = (Type)((( (Type)1<<(BitWidth))-1) << (BitOffet)),     \
};


#define GET_BITFIELD_VAL( Var, FieldName )                                              \
    ( ((Var) & FieldName##_MASK ) >> FieldName##_OFS )


#define SET_BITFIELD_VAL( Var, FieldName, NewValue )                                    \
    ( ((Var) & (~FieldName##_MASK)) | ((NewValue) << FieldName##_OFS) )
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// return new value of x which is rounded to the closes devision of y
#define ROUNDUP(x, y) ( ((x) + (y)-1)  & (~((y)-1)) )

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define SEC_TO_MS(sec)  ((sec) * 1000)
#define MS_TO_SEC(ms)   ((ms) / 1000)


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define XSTR(x) #x
#define STR(x)  XSTR(x)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Aliases for minimum and maximum for types
#define U8_MAX  ( 0xFF )
#define U8_MIN  ( 0x00 )
#define U16_MAX ( 0xFFFF )
#define U16_MIN ( 0x0000 )
#define U32_MAX ( 0xFFFFFFFF )
#define U32_MIN ( 0x00000000 )

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// @brief      Size in bytes
///////////////////////////////////////////////////////////////////////////////
#define SIZEOF_B(x)     ( sizeof(x) )

///////////////////////////////////////////////////////////////////////////////
/// @brief      Size in chars
///////////////////////////////////////////////////////////////////////////////
#define SIZEOF_C(x)     ( sizeof(x) )

///////////////////////////////////////////////////////////////////////////////
/// @brief      Size in words
///////////////////////////////////////////////////////////////////////////////
#define SIZEOF_W(x)     ( ( SIZEOF_C(x) + 1 ) / SIZEOF_C(u16) )

///////////////////////////////////////////////////////////////////////////////
/// @brief      Size in ints
///////////////////////////////////////////////////////////////////////////////
#define SIZEOF_I(x)     ( ( SIZEOF_C(x) + 3 ) / SIZEOF_C(u32) )

///////////////////////////////////////////////////////////////////////////////
/// @brief      Char size to words
///////////////////////////////////////////////////////////////////////////////
#define CHAR_SIZE_TO_W(x)       ( ( (x) + 1 ) / SIZEOF_C(u16) )

///////////////////////////////////////////////////////////////////////////////
/// @brief      Char size to ints
///////////////////////////////////////////////////////////////////////////////
#define CHAR_SIZE_TO_I(x)       ( ( (x) + 3 ) / SIZEOF_C(u32) )

///////////////////////////////////////////////////////////////////////////////
/// @brief      Char size to ints
///////////////////////////////////////////////////////////////////////////////
#define WORD_SIZE_TO_I(x)       ( ( (x) + 1 ) / SIZEOF_W(u32) )

///////////////////////////////////////////////////////////////////////////////
/// @brief  ARGUSED macro. Use in order to prevent compiler warning because of unused function parameter.
///////////////////////////////////////////////////////////////////////////////
#define ARGUSED(x) (void)(x)

///////////////////////////////////////////////////////////////////////////////
/// @brief  LENGTHOF macro. Use in order to get number of items in array.
///////////////////////////////////////////////////////////////////////////////
#define LENGTHOF(x) (sizeof(x) / sizeof((x)[0]))

#define SIZEOF_MEMBER(type, member) sizeof(((type *)0)->member)

///////////////////////////////////////////////////////////////////////////////
/// @brief  SIZE_IN_BYTES macro. Use in order to get the size of a variable/type in bytes.
///////////////////////////////////////////////////////////////////////////////
#define SIZE_IN_BYTES(x) (sizeof(x)*CHAR_BIT/8)

///////////////////////////////////////////////////////////////////////////////
/// @brief  ROUND_UP macros
///////////////////////////////////////////////////////////////////////////////
#define ROUND_UP(x,n)           ( ( (u32)(x) + (u32)(n) - 1 ) & ( ~((n)-1)) )
#define ROUND_UP_TO_TYPE(x,n)   ( ( (u32)(x) + SIZE_IN_BYTES(n) - 1) & ~(SIZE_IN_BYTES(n)-1) )

///////////////////////////////////////////////////////////////////////////////
/// @brief  ROUND_DOWN macros
///////////////////////////////////////////////////////////////////////////////
#define ROUND_DOWN(x,n)         ( (u32)(x) & ~((n)-1) )
#define ROUND_DOWN_TO_TYPE(x,n) ( (u32)(x)  & ~(SIZE_IN_BYTES(n)-1) )

///////////////////////////////////////////////////////////////////////////////
/// @brief  FIELD_MASK macro. Use in order to create a mask of bits in which bits Low..High are set.
///////////////////////////////////////////////////////////////////////////////
#define FIELD_MASK( Low, High ) ( ((1 << ((High)-(Low)+1)) - 1) << (Low) )

///////////////////////////////////////////////////////////////////////////////
/// @brief  OFFSET_OF macro. Use in order to get the offset of a field in a struct. Should not be
/// @n      used on packed structs.
///////////////////////////////////////////////////////////////////////////////
#define OFFSET_OF( StructName, FieldName ) \
    ((int)(&(((StructName*)0)->FieldName)))

///////////////////////////////////////////////////////////////////////////////
/// @brief  UPWARD_CAST macro. Use in order to implement C++ using C.
///////////////////////////////////////////////////////////////////////////////
#define UPWARD_CAST( BaseClassName, DerivedClassName, BaseClassMemberName, BaseClassPtr ) \
    ((DerivedClassName*)(((int)BaseClassPtr) - OFFSET_OF( DerivedClassName, BaseClassMemberName )))

///////////////////////////////////////////////////////////////////////////////
/// @brief  STATIC_CAST macro. Implementaion of static_cast<> in C
/// Usage example: u16_Len = STATIC_CAST( u16, strlen( "bla" ) )
///////////////////////////////////////////////////////////////////////////////
#define STATIC_CAST( typeto, what ) ( (typeto)(what) )

///////////////////////////////////////////////////////////////////////////////
/// @brief  CSTRING_TO_HEX macro. Use in order get unsigned long hex from string
///////////////////////////////////////////////////////////////////////////////
#define CSTRING_TO_HEX( str ) (strtoul( str, NULL, 16 ) )


///////////////////////////////////////////////////////////////////////////////
/// @brief      Gets a u8 number, returns its equivalent bit mask
///////////////////////////////////////////////////////////////////////////////
#define NUM_TO_BITMASK( x )     ( 1 << ( (u32) ( x - 1 ) ) )


///////////////////////////////////////////////////////////////////////////////
/// @brief  Kb macro. Use to specify 1024 multiplier.
///////////////////////////////////////////////////////////////////////////////
#define Kb  *1024

///////////////////////////////////////////////////////////////////////////////
/// @brief  Mb macro. Use to specify 1024*1024 multiplier.
///////////////////////////////////////////////////////////////////////////////
#define Mb  *1024 Kb

///////////////////////////////////////////////////////////////////////////////
/// @brief  Gb macro. Use to specify 1024*1024*1024 multiplier.
///////////////////////////////////////////////////////////////////////////////
#define Gb  *1024 Mb

///////////////////////////////////////////////////////////////////////////////
/// @brief Obsolete PACKED macro. Use PACKED_STRUCT instead (see below)
///////////////////////////////////////////////////////////////////////////////
#ifndef VAR_ON_CMND
#ifndef PACKED
    #define PACKED "Error: Use PACKED_STRUCT instead!!!"
#endif // PACKED
#endif

///////////////////////////////////////////////////////////////////////////////
/// @brief  Macro to be used when declaring "packed" struct
///
/// Use this macro when you want to declare a "packed" struct. Usage:
///     typedef PACKED_STRUCT struct _MyPackedStruct
///     {
///         u32 FourBytes;
///         u16 Twobytes;
///         u8  Onebyte;
///     } RESTORE_PACK(MyPackedStruct);
///
/// @note   Microsoft Visual Studio compiler is not supported. If you want to use a "packed" struct
///         in a code that should be compiled for _MSC_VER, take care of it
///         in the code. Example:
///
///     #ifdef _MSC_VER
///     #pragma pack(1)
///     #endif
///     typedef PACKED_STRUCT struct    _MyPackedStruct
///     {
///         u32 FourBytes;
///         u16 Twobytes;
///         u8  Onebyte;
///     } RESTORE_PACK(MyPackedStruct);
///     #ifdef _MSC_VER
///     #pragma pack()
///     #endif
///////////////////////////////////////////////////////////////////////////////
#ifndef PACK_STRUCT
#ifndef VAR_ON_CMND
    #if defined(_MSC_VER)
        #define PACK_STRUCT struct
    #elif defined(__GNUC__)
        #define PACK_STRUCT struct __attribute__((packed))
    #elif defined(__CC_ARM) // ARM_ADS
        #define PACK_STRUCT __packed struct
    #elif defined(__ICCARM__) // ARM_IAR
        #define PACK_STRUCT __packed struct
    #endif
#else
#define PACK_STRUCT __packed struct
#endif
#endif // PACK_STRUCT

///////////////////////////////////////////////////////////////////////////////
/// @brief  PACK_UNION macro. To be used when decalring "packed" union (same as for packed struct)
///////////////////////////////////////////////////////////////////////////////
#ifndef PACK_UNION
    #if defined(_MSC_VER)
        #define PACK_UNION union
    #elif defined(__GNUC__)
        #define PACK_UNION union __attribute__((packed))
    #elif defined(__CC_ARM) // ARM_ADS
        #define PACK_UNION __packed union
    #elif defined(__ICCARM__) // ARM_IAR
        #define PACK_UNION __packed union
    #endif
#endif // PACK_UNION

#ifndef RESTORE_PACK
    #if defined(_MSC_VER)
        #define RESTORE_PACK(Name) Name
    #elif defined(__GNUC__)
        #define RESTORE_PACK(Name) Name
    #elif defined(__CC_ARM) // ARM_ADS
        #define RESTORE_PACK(Name) Name
    #elif defined(__ICCARM__) // ARM_IAR
        #define RESTORE_PACK(Name) Name
    #endif
#endif


///////////////////////////////////////////////////////////////////////////////
/// @brief  Macros for compiling functions to specific sections:
/// @n      RAM_FUNC        = code which MUST be placed in RAM
/// @n      FAST_FUNC       = code which should run FAST
/// @n      VERY_FAST_FUNC  = code which should run VERY FAST (very small amount of code)
/// @n      END_FUNC        = back to default code section
/// @n
/// @n      NOTE: each function you want to place in the non-default section, should be
/// @n      defined wrapped by XXX_CODE and DEFAULT_CODE
/// @n
/// @n      Example usage:
/// @n      FAST_FUNC
/// @n      void foo( void )
/// @n      {
/// @n          ...
/// @n      }
/// @n      END_FUNC            // MUST after each function!!! Can't wrap 2 function...
///////////////////////////////////////////////////////////////////////////////
#if defined(__ICCARM__)         // IAR

#define NORETURN        __noreturn

#define RAM_FUNC        _Pragma("location=\"CODE_I\"")

// FAST functions are being compiled for ARM state
// NOTE: for DB56, it will be valuable only if the code is located on RAM... (running ARM instructions from
// external 16bits flash is slower than running THUMB instruction from the same flash)
#define FAST_FUNC       _Pragma("location=\"CODE_FAST\"")       ARM_FUNC
#define VERY_FAST_FUNC  _Pragma("location=\"CODE_VERY_FAST\"")  ARM_FUNC
#define END_FUNC

#elif defined(__CC_ARM)         // ADS

#define NORETURN

#define RAM_FUNC        _Pragma("arm section code=\"CODE_I\"")
#define FAST_FUNC       _Pragma("arm section code=\"CODE_FAST\"")       ARM_FUNC
#define VERY_FAST_FUNC  _Pragma("arm section code=\"CODE_VERY_FAST\"")  ARM_FUNC
#define END_FUNC        _Pragma("arm section code")

#else                           // Windows + TeakLite

#define NORETURN

#define RAM_FUNC
#define FAST_FUNC
#define VERY_FAST_FUNC
#define END_FUNC

#endif


/// Does break if condition is true
#define BREAK_IF( condition ) if( ( condition ) ) break

/// Performs compile-time assertion of constant expression with message
///
/// @param cond constant expression
/// @param msg message to be added to compiler error. Should meet to the symbol naming rule in C
/// @return none
/// @note
///   If cond is false (or 0) then the compiler will fail with error
///   If cond is true (non 0) then the static_assert will be ignored
///
/// @code
/// STATIC_ASSERT( sizeof(int) == 8, 64_bit_platform_is_not_supported );
/// @endcode
///
#ifndef STATIC_ASSERT
#define STATIC_ASSERT(cond, msg) extern char static_assertion__##msg[2*(!!(cond))-1]
#endif

#endif // _TYPE_DEFS_H
