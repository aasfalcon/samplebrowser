#include <climits>
#include "gtest/gtest.h"

#include "ring.h"

TEST(SharedRingTest, ctor)
{
    // not less than two (1) items
    EXPECT_ANY_THROW(Ring<int>(unsigned(-1)));
    EXPECT_ANY_THROW(Ring<int>(unsigned(-100)));
    EXPECT_ANY_THROW(Ring<int>(unsigned(-INT_MAX)));
    EXPECT_ANY_THROW(Ring<int>(0));
    EXPECT_ANY_THROW(Ring<int>(1));
    EXPECT_NO_THROW(Ring<int>(2));

    for (unsigned i = 3; i < 10; i++) {
        EXPECT_NO_THROW(Ring<int> ring(i));
    }

    // types
    //EXPECT_ANY_THROW(Ring<bool>(5)); // static_assert()
    EXPECT_NO_THROW(Ring<char>(5));
    EXPECT_NO_THROW(Ring<int>(5));
    EXPECT_NO_THROW(Ring<float>(5));
    EXPECT_NO_THROW(Ring<double>(5));
    EXPECT_NO_THROW(Ring<void*>(5));
    EXPECT_NO_THROW(Ring<std::string>(5));

    struct TestStruct {
        double b;
        int a;
        void* c;
        std::string d;
    };

    EXPECT_NO_THROW(Ring<TestStruct>(21));

    class TestClass {
    public:
        TestClass() {}
    protected:
        int a, b, c;
    };

    EXPECT_NO_THROW(Ring<TestClass>(7));
    EXPECT_NO_THROW(Ring<TestClass*>(20));
    EXPECT_NO_THROW(Ring<std::shared_ptr<TestClass>>(11));
}

TEST(SharedRingTest, count)
{
    // 2(1) items
    Ring<float> ring(2);
    EXPECT_EQ(2, ring.count());

    ring.push(1);
    EXPECT_EQ(2, ring.count());
    ring.pop();
    EXPECT_EQ(2, ring.count());

    // 3(2) items
    ring = Ring<float>(3);
    EXPECT_EQ(3, ring.count());

    ring.push(1);
    EXPECT_EQ(3, ring.count());
    ring.push(2);
    EXPECT_EQ(3, ring.count());

    ring.pop();
    EXPECT_EQ(3, ring.count());
    ring.pop();
    EXPECT_EQ(3, ring.count());

    // 4(3) items
    ring = Ring<float>(4);
    EXPECT_EQ(4, ring.count());

    ring.push(1);
    EXPECT_EQ(4, ring.count());
    ring.push(2);
    EXPECT_EQ(4, ring.count());
    ring.push(3);
    EXPECT_EQ(4, ring.count());

    ring.pop();
    EXPECT_EQ(4, ring.count());
    ring.pop();
    EXPECT_EQ(4, ring.count());
    ring.pop();
    EXPECT_EQ(4, ring.count());

    // 10 (9) iitems
    ring = Ring<float>(10);
    EXPECT_EQ(10, ring.count());

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.push(i);
        EXPECT_EQ(10, ring.count());
    }

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.pop();

        EXPECT_EQ(10, ring.count());
    }
}

TEST(SharedRingTest, push)
{
    // 2(1) items
    Ring<int> ring(2);
    EXPECT_NO_THROW(ring.push(1));
    EXPECT_ANY_THROW(ring.push(2));

    // 3(2) items
    ring = Ring<int>(3);
    EXPECT_NO_THROW(ring.push(1));
    EXPECT_NO_THROW(ring.push(2));
    EXPECT_ANY_THROW(ring.push(3));

    // 4(3) items
    ring = Ring<int>(4);
    EXPECT_NO_THROW(ring.push(1));
    EXPECT_NO_THROW(ring.push(2));
    EXPECT_NO_THROW(ring.push(3));
    EXPECT_ANY_THROW(ring.push(4));

    // 10(9) items
    ring = Ring<int>(10);

    for (int i = 1; i < 10; i++) {
        EXPECT_NO_THROW(ring.push(i));
    }

    EXPECT_ANY_THROW(ring.push(10));
}

TEST(SharedRingTest, pop)
{
    Ring<double> ring(2);

    // 2(1) items
    ring.push(1. / 3);
    double value;

    EXPECT_NO_THROW(value = ring.pop());
    EXPECT_EQ(value, 1 / 3.);

    EXPECT_ANY_THROW(ring.pop());

    // 3(2) items
    ring = Ring<double>(3);

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.push(i / 9.);
    }

    EXPECT_NO_THROW(value = ring.pop());
    EXPECT_EQ(1 / 9., value);

    EXPECT_NO_THROW(value = ring.pop());
    EXPECT_EQ(2 / 9., value);

    EXPECT_ANY_THROW(ring.pop());

    // 10 (9) items
    ring = Ring<double>(10);

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.push(1. / i);
    }

    for (unsigned i = 9; i >= 1; i--) {
        EXPECT_NO_THROW(value = ring.pop());
        EXPECT_EQ(1 / (10. - i), value);
    }

    EXPECT_ANY_THROW(ring.pop());
}

TEST(SharedRingTest, loaded)
{
    // 2(1) items
    Ring<float> ring(2);
    EXPECT_EQ(0, ring.loaded());

    ring.push(1);
    EXPECT_EQ(1, ring.loaded());
    ring.pop();
    EXPECT_EQ(0, ring.loaded());

    // 3(2) items
    ring = Ring<float>(3);
    EXPECT_EQ(0, ring.loaded());

    ring.push(1);
    EXPECT_EQ(1, ring.loaded());
    ring.push(2);
    EXPECT_EQ(2, ring.loaded());

    ring.pop();
    EXPECT_EQ(1, ring.loaded());
    ring.pop();
    EXPECT_EQ(0, ring.loaded());

    // 4(3) items
    ring = Ring<float>(4);
    EXPECT_EQ(0, ring.loaded());

    ring.push(1);
    EXPECT_EQ(1, ring.loaded());
    ring.push(2);
    EXPECT_EQ(2, ring.loaded());
    ring.push(3);
    EXPECT_EQ(3, ring.loaded());

    ring.pop();
    EXPECT_EQ(2, ring.loaded());
    ring.pop();
    EXPECT_EQ(1, ring.loaded());
    ring.pop();
    EXPECT_EQ(0, ring.loaded());

    // 10 (9) iitems
    ring = Ring<float>(10);
    EXPECT_EQ(0, ring.loaded());

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.push(i);
        EXPECT_EQ(i, ring.loaded());
    }

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.pop();

        EXPECT_EQ(ring.count() - 1 - i, ring.loaded());
    }
}

TEST(SharedRingTest, isHalfFull)
{
    // 2(1) items
    Ring<unsigned> ring(2);
    EXPECT_FALSE(ring.isHalfFull());

    ring.push(1);
    EXPECT_TRUE(ring.isHalfFull());
    ring.pop();
    EXPECT_FALSE(ring.isHalfFull());

    // 3(2) items
    ring = Ring<unsigned>(3);
    EXPECT_FALSE(ring.isHalfFull());

    ring.push(1);
    EXPECT_TRUE(ring.isHalfFull());
    ring.push(2);
    EXPECT_TRUE(ring.isHalfFull());

    ring.pop();
    EXPECT_TRUE(ring.isHalfFull());
    ring.pop();
    EXPECT_FALSE(ring.isHalfFull());

    // 4(3) items
    ring = Ring<unsigned>(4);
    EXPECT_FALSE(ring.isHalfFull());

    ring.push(1);
    EXPECT_FALSE(ring.isHalfFull());
    ring.push(2);
    EXPECT_TRUE(ring.isHalfFull());
    ring.push(3);
    EXPECT_TRUE(ring.isHalfFull());

    ring.pop();
    EXPECT_TRUE(ring.isHalfFull());
    ring.pop();
    EXPECT_FALSE(ring.isHalfFull());
    ring.pop();
    EXPECT_FALSE(ring.isHalfFull());

    // 10 (9) iitems
    ring = Ring<unsigned>(10);
    EXPECT_FALSE(ring.isHalfFull());

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.push(i);

        if (i < 5) {
            EXPECT_FALSE(ring.isHalfFull());
        } else {
            EXPECT_TRUE(ring.isHalfFull());
        }
    }

    EXPECT_TRUE(ring.isFull());

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.pop();

        if (i < 5) {
            EXPECT_TRUE(ring.isHalfFull());
        } else {
            EXPECT_FALSE(ring.isHalfFull());
        }
    }
}

TEST(SharedRingTest, isEmpty)
{
    // 2(1) items
    Ring<unsigned> ring(2);
    EXPECT_TRUE(ring.isEmpty());

    ring.push(1);
    EXPECT_FALSE(ring.isEmpty());
    ring.pop();
    EXPECT_TRUE(ring.isEmpty());

    // 3(2) items
    ring = Ring<unsigned>(3);
    EXPECT_TRUE(ring.isEmpty());

    ring.push(1);
    EXPECT_FALSE(ring.isEmpty());
    ring.push(2);
    EXPECT_FALSE(ring.isEmpty());

    ring.pop();
    EXPECT_FALSE(ring.isEmpty());
    ring.pop();
    EXPECT_TRUE(ring.isEmpty());

    // 4(3) items
    ring = Ring<unsigned>(4);
    EXPECT_TRUE(ring.isEmpty());

    ring.push(1);
    EXPECT_FALSE(ring.isEmpty());
    ring.push(2);
    EXPECT_FALSE(ring.isEmpty());
    ring.push(3);
    EXPECT_FALSE(ring.isEmpty());

    ring.pop();
    EXPECT_FALSE(ring.isEmpty());
    ring.pop();
    EXPECT_FALSE(ring.isEmpty());
    ring.pop();
    EXPECT_TRUE(ring.isEmpty());

    // 10 (9) iitems
    ring = Ring<unsigned>(10);
    EXPECT_TRUE(ring.isEmpty());

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.push(i);
        EXPECT_FALSE(ring.isEmpty());
    }

    for (unsigned i = 1; i < ring.count() - 1; i++) {
        ring.pop();
        EXPECT_FALSE(ring.isEmpty());
    }

    ring.pop();
    EXPECT_TRUE(ring.isEmpty());
}

TEST(SharedRingTest, isFull)
{
    // 2(1) items
    Ring<unsigned> ring(2);
    EXPECT_FALSE(ring.isFull());

    ring.push(1);
    EXPECT_TRUE(ring.isFull());
    ring.pop();
    EXPECT_FALSE(ring.isFull());

    // 3(2) items
    ring = Ring<unsigned>(3);
    EXPECT_FALSE(ring.isFull());

    ring.push(1);
    EXPECT_FALSE(ring.isFull());
    ring.push(2);
    EXPECT_TRUE(ring.isFull());

    ring.pop();
    EXPECT_FALSE(ring.isFull());
    ring.pop();
    EXPECT_FALSE(ring.isFull());

    // 4(3) items
    ring = Ring<unsigned>(4);
    EXPECT_FALSE(ring.isFull());

    ring.push(1);
    EXPECT_FALSE(ring.isFull());
    ring.push(2);
    EXPECT_FALSE(ring.isFull());
    ring.push(3);
    EXPECT_TRUE(ring.isFull());

    ring.pop();
    EXPECT_FALSE(ring.isFull());
    ring.pop();
    EXPECT_FALSE(ring.isFull());
    ring.pop();
    EXPECT_FALSE(ring.isFull());

    // 10 (9) iitems
    ring = Ring<unsigned>(10);
    EXPECT_FALSE(ring.isFull());

    for (unsigned i = 1; i < ring.count() - 1; i++) {
        ring.push(i);
        EXPECT_FALSE(ring.isFull());
    }

    ring.push(9);
    EXPECT_TRUE(ring.isFull());

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.pop();
        EXPECT_FALSE(ring.isFull());
    }
}

TEST(SharedRingTest, space)
{
    // 2(1) items
    Ring<float> ring(2);
    EXPECT_EQ(1, ring.space());

    ring.push(1);
    EXPECT_EQ(0, ring.space());
    ring.pop();
    EXPECT_EQ(1, ring.space());

    // 3(2) items
    ring = Ring<float>(3);
    EXPECT_EQ(2, ring.space());

    ring.push(1);
    EXPECT_EQ(1, ring.space());
    ring.push(2);
    EXPECT_EQ(0, ring.space());

    ring.pop();
    EXPECT_EQ(1, ring.space());
    ring.pop();
    EXPECT_EQ(2, ring.space());

    // 4(3) items
    ring = Ring<float>(4);
    EXPECT_EQ(3, ring.space());

    ring.push(1);
    EXPECT_EQ(2, ring.space());
    ring.push(2);
    EXPECT_EQ(1, ring.space());
    ring.push(3);
    EXPECT_EQ(0, ring.space());

    ring.pop();
    EXPECT_EQ(1, ring.space());
    ring.pop();
    EXPECT_EQ(2, ring.space());
    ring.pop();
    EXPECT_EQ(3, ring.space());

    // 10 (9) iitems
    ring = Ring<float>(10);
    EXPECT_EQ(9, ring.space());

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.push(i);
        EXPECT_EQ(ring.count() - 1 - i, ring.space());
    }

    for (unsigned i = 1; i < ring.count(); i++) {
        ring.pop();

        EXPECT_EQ(i, ring.space());
    }
}

TEST(SharedRingTest, clear)
{
    Ring<const char *> ring(7);

    ring.push("this");
    ring.push("is");
    ring.push("a");
    ring.push("test");

    ring.clear();

    EXPECT_EQ(7, ring.count());
    EXPECT_EQ(0, ring.loaded());
    EXPECT_EQ(6, ring.space());
    EXPECT_TRUE(ring.isEmpty());
    EXPECT_FALSE(ring.isHalfFull());
    EXPECT_FALSE(ring.isFull());
    EXPECT_ANY_THROW(ring.pop());
}
