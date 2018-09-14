#include "ft.parser.h"
#include "ft.buffer.h"
#include "ft.crc.h"
#include "ft.convert.h"
#include <stdio.h>

#define ERROR     1
#define SUCCESS   0
#define when      break;case
#define otherwise break;default

//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------
static void reset();
static void write_message_data ();
static int message ();
static int header ();
static int packet ();
static int frame ();
static int arr_index ();
static int data ();
static int match (uint8_t);

// Constants to represent terminal nodes
enum { START1, START2, SENDER, RECEIVER, LENGTH, INDEX, DATA, CRC };

//------------------------------------------------------------------------------
// Private Variables ('s' prefix means static variable)
//------------------------------------------------------------------------------

static Buffer * s_buffer;
static Data_t * s_array;
static int      s_offset;     // Index of next byte in buffer to be parsed
static int      s_pkt_sz;     // Num. bytes in packet (excluding header and crc)
static int      s_msg_index;
static uint8_t  s_msg [MAX_PCKT_SZ];
static int      s_not_enough_in_buffer;

//------------------------------------------------------------------------------
// Public Parsing Function
//------------------------------------------------------------------------------

static void reset ()
{
    s_offset    = 0;
    s_pkt_sz    = 0;
    s_msg_index = 0;
}

void parse (Buffer * input_buffer, Data_t * destination)
{
    s_buffer = input_buffer;
    s_array = destination;
    s_not_enough_in_buffer = 0;
    reset();

    // Parse as long as the buffer has bytes
    while (s_offset < getSize(s_buffer))
    {
        if (message())
        {
            if (s_not_enough_in_buffer)
                return;
            else
                pop(s_buffer);
        }
        else
            write_message_data();
        reset();
    }
}

static void write_message_data ()
{
    for (int i = 0; i < s_pkt_sz; i += FRAME_SZ)
    {
        Index_t array_index = combine_index_bytes(&s_msg[i]);
        Data_t  data        = combine_data_bytes (&s_msg[i + INDEX_SZ]);
        if (array_index < ARRAY_SZ)
            s_array[array_index] = data;
    }
    for (int i = 0; i < s_offset; ++i)
        pop(s_buffer);
}

//------------------------------------------------------------------------------
// Non-terminals
//------------------------------------------------------------------------------
//
//    message -> header packet crc
//    header  -> start1 start2 address address length
//    packet  -> frame * N
//    frame   -> index byte byte
//
//------------------------------------------------------------------------------

static int message ()
{
    if (header() || packet() || match(CRC))
        return ERROR;
    if (s_pkt_sz != s_msg_index)
        return ERROR;
    return SUCCESS;
}

static int header ()
{
    if (match(START1) || match(START2) || match(SENDER) || match(RECEIVER) || match(LENGTH))
        return ERROR;
    return SUCCESS;
}

static int packet ()
{
    int num_frames = s_pkt_sz / FRAME_SZ;
    for (int i = 0; i < num_frames; ++i)
        if (frame())
            return ERROR;
    return SUCCESS;
}

static int frame ()
{
    if (arr_index() || data())
        return ERROR;
    return SUCCESS;
}

static int arr_index ()
{
    for (int i = 0; i < INDEX_SZ; ++i)
        if (match(INDEX))
            return ERROR;
    return SUCCESS;
}

static int data ()
{
    for (int i = 0; i < DATA_SZ; ++i)
        if (match(DATA))
            return ERROR;
    return SUCCESS;
}

//------------------------------------------------------------------------------
// Terminals
//------------------------------------------------------------------------------
//
//    start1  -> 0x06
//    start2  -> 0x85
//    address -> 0x00 | 0x01 | ... | MX_ADDRESS
//    length  -> 0x00 | 0x01 | ... | MX_PCKT_SZ
//    index   -> 0x00 | 0x01 | ... | 0xFF
//    data    -> 0x00 | 0x01 | ... | 0xFF
//    crc     -> calculateCRC(packet_data, N)
//
//------------------------------------------------------------------------------

static int match (uint8_t terminal)
{
    if (s_offset >= getSize(s_buffer))
    {
        s_not_enough_in_buffer = 1;
        return ERROR;
    }
    uint8_t next = getValue(s_buffer, s_offset);
    int status = SUCCESS;
    switch (terminal)
    {
        case START1:   status = (next == 0x06 ? SUCCESS : ERROR);
        when START2:   status = (next == 0x85 ? SUCCESS : ERROR);
        when SENDER:   status = SUCCESS;
        when RECEIVER: status = (next == LOCAL_ADDR ? SUCCESS : ERROR);
        when LENGTH:   s_pkt_sz = next;
                       status = SUCCESS;
        when INDEX:    s_msg[s_msg_index++] = next;
                       status = SUCCESS;
        when DATA:     s_msg[s_msg_index++] = next;
                       status = SUCCESS;
        when CRC:      status = (next == crc(s_msg, s_pkt_sz) ? SUCCESS : ERROR);
    }
    if (status == SUCCESS)
        ++s_offset;
    return status;
}