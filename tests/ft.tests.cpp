#include "../ft.h"
#include "../ft.crc.h"
#include "ft.test_helpers.h"
#include "gtest/gtest.h"

TEST (FT_Tests, ArrayValuesAreInitiallyZero)
{
    for (int i = 0; i < ARRAY_SZ; ++i)
        EXPECT_EQ(0, FT_ReadArray(i));
}

// Receive One Valid Message
TEST (FT_Tests, ReceiveOneValidMessage)
{
    FT_Initialize();
    auto port = FT_NewPort(put, get, empty);

    // Create 1 random message
    auto msg = random_valid_message();

    // Populate simulated receive buffer
    receive(msg);

    for (int i = 0; i < 100; ++i)
        FT_ScanPort(port);
    EXPECT_TRUE(array_contains_message_data(msg));
}

// Receive Two Valid Messages
TEST (FT_Tests, ReceiveTwoValidMessages)
{
    FT_Initialize();
    auto port = FT_NewPort(put, get, empty);

    // Create 1 random message
    auto msg1 = random_valid_message();
    auto msg2 = random_valid_message();

    // Populate simulated receive buffer
    receive(msg1);
    receive(msg2);

    for (int i = 0; i < 10; ++i)
        FT_ScanPort(port);
    EXPECT_TRUE(array_contains_message_data(msg1));
    EXPECT_TRUE(array_contains_message_data(msg2));
}

// Receive Two Valid Messages with Garbage in between
TEST (FT_Tests, ReceiveTwoValidMessagesWithGarbageBetween)
{
    FT_Initialize();
    auto port = FT_NewPort(put, get, empty);

    // Create 1 random message
    auto msg1 = random_valid_message();
    auto msg2 = random_valid_message();

    // Populate simulated receive buffer
    receive(random_garbage());
    receive(msg1);
    receive(random_garbage());
    receive(msg2);
    receive(random_garbage());

    for (int i = 0; i < 10; ++i)
        FT_ScanPort(port);
    EXPECT_TRUE(array_contains_message_data(msg1));
    EXPECT_TRUE(array_contains_message_data(msg2));
}


