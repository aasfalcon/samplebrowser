#include <gtest/gtest.h>

#include "shared/bus.h"

TEST(SharedBusTest, ctor)
{
    EXPECT_ANY_THROW(Bus<int> bus(0));
    EXPECT_ANY_THROW(Bus<int> bus(1));
    EXPECT_ANY_THROW(Bus<int> bus(1, 0));
    EXPECT_ANY_THROW(Bus<int> bus(1, 1));
    EXPECT_ANY_THROW(Bus<int> bus(2, 0));
    EXPECT_NO_THROW(Bus<int> bus(2, 1));
    EXPECT_NO_THROW(Bus<int> bus(3, 2));
    EXPECT_ANY_THROW(Bus<int> bus(unsigned(-1)));
    EXPECT_ANY_THROW(Bus<int> bus(5, unsigned(-1)));
}

TEST(SharedBusTest, held)
{
    Bus<int> bus(5, 1);

    for (int i = 1; i < 5; i++) {
        bus.push(i);
    }

    EXPECT_EQ(bus.held(), 0);
    EXPECT_TRUE(bus.isFull());
    EXPECT_EQ(bus.space(), 0);
    EXPECT_EQ(bus.loaded(), 4);

    bus.push(5);

    EXPECT_EQ(bus.held(), 1);
    EXPECT_TRUE(bus.isFull());
    EXPECT_EQ(bus.lost(), 0);

    EXPECT_EQ(bus.pop(), 1);

    EXPECT_EQ(bus.held(), 1);
    EXPECT_FALSE(bus.isFull());
    EXPECT_EQ(bus.lost(), 0);

    EXPECT_NO_THROW(bus.load());

    EXPECT_EQ(bus.held(), 0);
    EXPECT_TRUE(bus.isFull());
    EXPECT_EQ(bus.lost(), 0);

    for (int i = 6; i < 10; i++) {
        EXPECT_NO_THROW(bus.push(i));
    }

    EXPECT_EQ(bus.held(), 4);
    EXPECT_TRUE(bus.isFull());
    EXPECT_EQ(bus.lost(), 0);

    while (!bus.isEmpty()) {
        bus.pop();
    }

    EXPECT_EQ(bus.held(), 4);
    EXPECT_TRUE(bus.isEmpty());

    EXPECT_ANY_THROW(bus.pop());
}

TEST(SharedBusTest, lost)
{
    Bus<int> bus(10, 2);

    int space = int(bus.count() - 1);
    int heldSpace = int(bus.count() * 2 - 1);
    int max = space + heldSpace;
    EXPECT_EQ(max, 28);

    for (int i = 1; i <= space + heldSpace; i++) {
        bus.push(i);
    }

    EXPECT_EQ(bus.lost(), 0);
    EXPECT_TRUE(bus.isFull());
    EXPECT_EQ(bus.held(), heldSpace);

    EXPECT_THROW(bus.push(29), std::overflow_error);
    EXPECT_EQ(bus.lost(), 1);
    EXPECT_TRUE(bus.isFull());
    EXPECT_EQ(bus.held(), heldSpace);

    int firstOut = bus.pop();
    EXPECT_EQ(firstOut, 1);

    int lastOut;

    while (bus.held()) {
        bus.load();

        while (bus.loaded()) {
            lastOut = bus.pop();
        }
    }

    EXPECT_EQ(lastOut, 28);
    EXPECT_TRUE(bus.isEmpty());
    EXPECT_EQ(bus.held(), 0);
    EXPECT_EQ(bus.lost(), 1); // was 29

    bus.clearLost();
    EXPECT_EQ(bus.lost(), 0);

    for (int i = 1; i <= 50; i++) {
        if (i < 29) {
            EXPECT_NO_THROW(bus.push(i));
        } else {
            EXPECT_THROW(bus.push(i), std::overflow_error);
        }
    }

    EXPECT_EQ(bus.lost(), 50 - 28);
    EXPECT_TRUE(bus.isFull());
    EXPECT_EQ(bus.held(), 19);

    EXPECT_NO_THROW(bus.clear());
    EXPECT_TRUE(bus.isEmpty());
    EXPECT_EQ(bus.lost(), 0);
    EXPECT_EQ(bus.held(), 0);
}

TEST(SharedBusTest, flush)
{
    Bus<double> bus(16, 5);

    // 1 item
    for (int i = 1; i <= 16; i++) {
        EXPECT_NO_THROW(bus.push(i));
    }

    EXPECT_EQ(bus.held(), 1);

    for (int i = 1; i < 5; i++) {
        EXPECT_NO_THROW(bus.pop());
    }

    EXPECT_EQ(bus.space(), 4);
    EXPECT_EQ(bus.held(), 1);

    EXPECT_NO_THROW(bus.load());

    EXPECT_EQ(bus.held(), 0);
    EXPECT_EQ(bus.space(), 3);

    double lastOut = 0;

    while (!bus.isEmpty()) {
        EXPECT_NO_THROW(lastOut = bus.pop());
    }

    EXPECT_EQ(lastOut, 16);

    // 25 (10 held) items
    bus.clear();

    for (int i = 1; i <= 25; i++) {
        EXPECT_NO_THROW(bus.push(i));
    }

    EXPECT_EQ(bus.held(), 10);

    for (int i = 1; i <= 5; i++) {
        EXPECT_EQ(bus.pop(), i);
    }

    EXPECT_EQ(bus.held(), 10);
    EXPECT_EQ(bus.space(), 5);

    EXPECT_NO_THROW(bus.load());

    EXPECT_EQ(bus.held(), 5);
    EXPECT_TRUE(bus.isFull());

    EXPECT_EQ(bus.pop(), 6);

    while(!bus.isEmpty()) {
        EXPECT_NO_THROW(bus.pop());
    }

    bus.load();

    EXPECT_EQ(bus.held(), 0);
    EXPECT_EQ(bus.loaded(), 5);

    while(!bus.isEmpty()) {
        EXPECT_NO_THROW(lastOut = bus.pop());
    }

    EXPECT_EQ(lastOut, 25);

    // 50 (35 held) items
    for (int i = 1; i <= 50; i++) {
        EXPECT_NO_THROW(bus.push(i));
    }

    EXPECT_EQ(bus.held(), 35);

    while (!bus.isEmpty()) {
        EXPECT_NO_THROW(bus.pop()); // [1..15]
    }

    EXPECT_NO_THROW(bus.load());
    EXPECT_EQ(bus.held(), 20);
    EXPECT_TRUE(bus.isFull());

    for (int i = 1; i <= 7; i++) {
        EXPECT_NO_THROW(bus.pop()); // [16..22]
    }

    EXPECT_NO_THROW(bus.load());
    EXPECT_EQ(bus.held(), 13);
    EXPECT_TRUE(bus.isFull());
    EXPECT_EQ(bus.pop(), 23); // [23]

    while (!bus.isEmpty()) {
        EXPECT_NO_THROW(bus.pop()); // [24..37]
    }

    bus.load();
    EXPECT_EQ(bus.loaded(), 13);
    EXPECT_EQ(bus.held(), 0);
    EXPECT_EQ(bus.pop(), 38); // [38]
}
