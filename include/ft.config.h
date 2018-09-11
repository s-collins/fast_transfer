#ifndef FAST_TRANSFER_CONFIG_H
#define FAST_TRANSFER_CONFIG_H
#include "stdint.h"

//------------------------------------------------------------------------------
// Configuration (user inputs library settings here)
//------------------------------------------------------------------------------

#define INDEX_SZ    2      // Size of index type [bytes]
#define DATA_SZ     2      // Size of data value [bytes]
#define MAX_PCKT_SZ 100    // Max size of packet [bytes]

//------------------------------------------------------------------------------
// Validation (emits compiler messages if settings are not valid)
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
    #define Data_t uint8_t
#elif DATA_SZ == 2
    #define Data_t uint16_t
#else
    #error FT_CONFIG: Invalid DATA_SZ
#endif

// Max packet size must be multiple of (index size + data size)
#if MAX_PCKT_SZ % (INDEX_SZ + DATA_SZ) != 0
    #error FT_CONFIG: MAX_PCKT_SZ must be multiple of (INDEX_SZ + DATA_SZ)
#endif

// Max packet size must not exceed 255
#if MAX_PCKT_SZ > 255
    #error FT_CONFIG: MAX_PCKT_SZ must not exceed 255
#endif

#endif // FAST_TRANSFER_CONFIG_H