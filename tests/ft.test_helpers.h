#ifndef FAST_TRANSFER_TEST_HELPERS_H
#define FAST_TRANSFER_TEST_HELPERS_H
#include <vector>
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// Generating data
////////////////////////////////////////////////////////////////////////////////

// Create a valid message from random sender, filled with random indices and
// random data.
std::vector<uint8_t> random_valid_message ();

// Generate random number of random garbage bytes
std::vector<uint8_t> random_garbage ();

////////////////////////////////////////////////////////////////////////////////
// Checking if a message has been written
////////////////////////////////////////////////////////////////////////////////

bool array_contains_message_data (std::vector<uint8_t> const &message);

////////////////////////////////////////////////////////////////////////////////
// Simulating Transmit and Receive Buffers
////////////////////////////////////////////////////////////////////////////////

// Populates the receive buffer
void receive (std::vector<uint8_t> const & message);

// Empty receive buffer
void empty_receive ();

// Get a byte from the receive buffer
uint8_t get ();

// Push a byte to the transmit buffer
void put (uint8_t);

// Query whether or not the receive buffer is empty
bool empty ();

#endif // FAST_TRANSFER_TEST_HELPERS_H