#ifndef FT_CONFIG_H
#define FT_CONFIG_H

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              Library Settings                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define ARRAY_SZ 256
// - number of elements in array
// - shall be less than or equal to 2^8 (256) because index is uint8_t

#define MAX_PCKT_SZ 99
// - maximum number of bytes in a packet

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//              Validation (prevents compilation if bad settings)             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#if ARRAY_SZ > 256
  #error Fast-trasfer configuration: Array size exceeds maximum value of 256.
#endif

#endif // FT_CONFIG_H
