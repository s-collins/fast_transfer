#include "ft.convert.h"

// Extracts the bytes from index, with most significant byte getting
// the smallest index in the output array
void slice_index_bytes (Index_t index, uint8_t * bytes)
{
    for (int i = 0; i < INDEX_SZ; ++i)
    {
        int shift = (INDEX_SZ - i - 1) * 8;
        bytes[i] = (uint8_t)(index >> shift);
    }
}

// Extracts the bytes from data, with most significant byte getting
// the smallest index in the output array
void slice_data_bytes (Data_t data, uint8_t * bytes)
{
    UnsignedData_t udata = (UnsignedData_t)data;
    for (int i = 0; i < DATA_SZ; ++i)
    {
        int shift = (DATA_SZ - i - 1) * 8;
        bytes[i] = (uint8_t)(data >> shift);
    }
}

// Combines the array of bytes into a single index
Index_t combine_index_bytes (uint8_t const * bytes)
{
    Index_t index = 0;
    for (int i = 0; i < INDEX_SZ; ++i)
    {
        Index_t byte = bytes[i];
        int shift = (INDEX_SZ - i - 1) * 8;
        index |= (byte << shift);
    }
    return index;
}

// Combines the array of bytes into a data value
Data_t combine_data_bytes (uint8_t const * bytes)
{
    UnsignedData_t data = 0;
    for (int i = 0; i < DATA_SZ; ++i)
    {
        UnsignedData_t byte = bytes[i];
        int shift = (DATA_SZ - i - 1) * 8;
        data |= (byte << shift);
    }
    return data;
}