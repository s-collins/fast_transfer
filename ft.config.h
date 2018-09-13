#ifndef FAST_TRANSFER_CONFIG_H
#define FAST_TRANSFER_CONFIG_H
#include <stdint.h>

//------------------------------------------------------------------------------
// Configuration (input your settings for the library here)
//------------------------------------------------------------------------------

#define INDEX_SZ    2                // Size of index type [bytes]
#define DATA_SZ     2                // Size of data value [bytes]
#define MAX_PCKT_SZ 200              // Max size of packet [bytes]
#define ARRAY_SZ    1<<(INDEX_SZ*8)  // Length of array
#define LOCAL_ADDR  1

//------------------------------------------------------------------------------
// Validation (emits compiler messages if your settings are not valid)
//------------------------------------------------------------------------------

// Sets type for fast transfer indices
#if INDEX_SZ == 1
    #define Index_t uint8_t
#elif INDEX_SZ == 2
    #define Index_t uint16_t
#else
    #error FT_CONFIG: Invalid INDEX_SZ
#endif

// Sets type for fast transfer data
#if DATA_SZ == 1
    #define Data_t int8_t
    #define UnsignedData_t uint8_t
#elif DATA_SZ == 2
    #define Data_t int16_t
    #define UnsignedData_t uint16_t
#else
    #error FT_CONFIG: Invalid DATA_SZ
#endif

// Max packet size must not exceed 256
#if MAX_PCKT_SZ > 256
    #error FT_CONFIG: MAX_PCKT_SZ must not exceed 256
#endif

// Array must be small enough that every position has corresponding index
#if ARRAY_SZ > (1 << (INDEX_SZ * 8UL))
    #error FT_CONFIG: ARRAY_SZ is too large for INDEX_SZ
#endif

#endif // FAST_TRANSFER_CONFIG_H