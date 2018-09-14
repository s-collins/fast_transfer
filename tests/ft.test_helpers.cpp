#include "../ft.h"
#include "../ft.convert.h"
#include "../ft.crc.h"
#include "../ft.config.h"
#include "ft.test_helpers.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <stack>

static std::deque<uint8_t> rx_buffer;
static std::deque<uint8_t> tx_buffer;

////////////////////////////////////////////////////////////////////////////////
// Generating data
////////////////////////////////////////////////////////////////////////////////

// Generate a random byte
uint8_t random_byte () {
    static int i = 0;
    if (i == 0) {
        i = 1;
        srand(time(0));
    }
    return (uint8_t)(rand() % 256);
}

// Generate a header
std::vector<uint8_t> create_header ()
{
    return {
        0x06,
        0x85,
        0xFF,
        LOCAL_ADDR,
        random_byte() % MAX_PCKT_SZ / FRAME_SZ * FRAME_SZ
    };
}

// Generate a random valid message that does not write to any indices
// that were not written to previously
std::vector<uint8_t> random_disjoint_packet (int length)
{
    static std::vector<Index_t> indices;
    std::vector<uint8_t> packet;
    for (int i = 0; i < length; i += FRAME_SZ)
    {
        // Generate index that hasn't been written to yet
        uint8_t index_bytes [INDEX_SZ];
        bool loop = true;
        while (loop)
        {
            for (int j = 0; j < INDEX_SZ; ++j)
                index_bytes[j] = random_byte();
            Index_t random_index = combine_index_bytes(index_bytes);
            if (std::find(indices.begin(), indices.end(), random_index) == indices.end())
            {
                loop = false;
                indices.push_back(random_index);
            }
        }
        for (int j = 0; j < INDEX_SZ; ++j)
            packet.push_back(index_bytes[j]);
        
        // Generate random data
        for (int j = 0; j < DATA_SZ; ++j)
            packet.push_back(random_byte());
    }
    return packet;
}

std::vector<uint8_t> random_garbage ()
{
    std::vector<uint8_t> output;
    for (int i = 0; i < random_byte(); ++i)
        output.push_back(random_byte());
    return output;
}

// Generate a random valid message
std::vector<uint8_t> random_valid_message ()
{
    // Generate message components
    auto header = create_header();
    auto packet = random_disjoint_packet(header.back());
    uint8_t crc_value = crc(packet.data(), packet.size());

    // Populate the message
    std::vector<uint8_t> message;
    std::copy(header.begin(), header.end(), std::back_inserter(message));
    std::copy(packet.begin(), packet.end(), std::back_inserter(message));
    message.push_back(crc_value);

    return message;
}

////////////////////////////////////////////////////////////////////////////////
// Checking if a message has been written
////////////////////////////////////////////////////////////////////////////////

// Check that the array contains the data in the given message
bool array_contains_message_data (std::vector<uint8_t> const & message)
{
    for (int i = 5, n = message.at(4) + 5; i < n; i += FRAME_SZ)
    {
        Index_t index = combine_index_bytes(&message.data()[i]);
        Data_t data = combine_data_bytes(&message.data()[i + INDEX_SZ]);
        if (data != FT_ReadArray(index))
            return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Simulating Transmit and Receive Buffers
////////////////////////////////////////////////////////////////////////////////

void receive (std::vector<uint8_t> const & message) {
    std::copy(message.begin(), message.end(), std::back_inserter(rx_buffer));
}

uint8_t get () {
    uint8_t byte = rx_buffer.front();
    rx_buffer.pop_front();
    return byte;
}

void put (uint8_t byte) {
    tx_buffer.push_back(byte);
}

bool empty () {
    return rx_buffer.empty();
}