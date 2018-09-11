#include "ft.config.h"

//------------------------------------------------------------------------------
// Grammar of FastTransfer messages
//------------------------------------------------------------------------------
//
//    Non-Terminal Symbols
//    --------------------
//    message -> header packet crc
//    header  -> start1 start2 address address length
//    packet  -> frame * N
//    frame   -> index byte byte
//
//    Terminal Symbols
//    ----------------
//    start1  -> 0x06
//    start2  -> 0x85
//    address -> 0x00 | 0x01 | ... | MX_ADDRESS
//    length  -> 0x00 | 0x01 | ... | MX_PCKT_SZ
//    index   -> 0x00 | 0x01 | ... | 0xFF
//    data    -> 0x00 | 0x01 | ... | 0xFF
//    crc     -> calculateCRC(packet_data, N)
//
// NOTES
//
// - The bodies of the productions for the "packet" and the "crc" nonterminals
//   are dependent on values corresponding to the "length" and "packet" symbols.
// - The terminals for "address", "length", and "index" depend on constants
//   that are configurable by the user of the library.
//------------------------------------------------------------------------------

// Constants for terminal symbols
static enum
{
    START1,
    START2,
    ADDRESS,
    LENGTH,
    INDEX,
    DATA,
    CRC
};

//------------------------------------------------------------------------------
// Data structure to track parser's state
//------------------------------------------------------------------------------

struct Parser
{
    Index_t index_buffer [];
    Data_t  data_buffer [];
    uint8_t buffer_index;
};

//------------------------------------------------------------------------------
// Initialization
//------------------------------------------------------------------------------

void
init_parser (bool(*get)())
{
    // Provide the parser with a function for reading next byte from the input
    // stream
}

void run_parser (Parser * parser)
{
    int parsing = 1;
    while (parsing)
    {
        message();
        if (parser->error) {
            trash_first_byte(parser);
            parser->error = false;
        }
    }
}

//------------------------------------------------------------------------------
// Parse non-terminal symbols
//------------------------------------------------------------------------------

void message ()
{
    header();
    packet();
    match(CRC);
}

void header ()
{
    match(START1);
    match(START2);
    match(ADDRESS);
    match(LENGTH);
}

void packet ()
{
    int num_frames = parser->packet_sz / (INDEX_SZ + DATA_SZ);
    for (int i = 0; i < num_frames; ++i)
        frame();
}

void frame ()
{
    //-------------------------------------------------------------------------
    // TODO: INDEX and DATA should actually be treated as non-terminal nodes
    //       with their own functions because they may consist of multiple
    //       bytes.  That way, I can verify that they are within the valid
    //       ranges.  This will require me to add a new terminal called "byte"
    //-------------------------------------------------------------------------
    for (int i = 0; i < INDEX_SZ; ++i)
        match(INDEX);
    for (int i = 0; i < DATA_SZ; ++i)
        match(DATA);
}

//------------------------------------------------------------------------------
// Parse terminal symbols
//------------------------------------------------------------------------------

#define when break;case
#define otherwise break;default

void match (uint8_t terminal)
{
    switch (terminal)
    {
        case START1:  lookahead == 0x06         ? advance() : flag_error();
        when START2:  lookahead == 0x85         ? advance() : flag_error();
        when ADDRESS: lookahead  < 255          ? advance() : flag_error();
        when LENGTH:  lookahead  < 255          ? advance() : flag_error();
        when INDEX:   isValidIndex(lookahead)   ? advance() : flag_error();
        when DATA:    ; // TODO: implement this (transfer bytes and advance)
        when CRC:     isCorrectCRC(lookahead)   ? advance() : flag_error();
        otherwise:    flag_error();
    }
}
