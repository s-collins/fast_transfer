#ifndef FAST_TRANSFER_CONVERT_H
#define FAST_TRANSFER_CONVERT_H
#include "ft.config.h"
#ifdef __cplusplus
extern "C" {
#endif

void slice_index_bytes (Index_t, uint8_t *);
void slice_data_bytes (Data_t, uint8_t *);
Index_t combine_index_bytes (uint8_t const *);
Data_t combine_data_bytes (uint8_t const *);

#ifdef __cplusplus
}
#endif
#endif // FAST_TRANSFER_CONVERT_H