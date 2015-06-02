#include "Memory.h"
#include "gtest/gtest.h"
TEST(MemoryTest, Basic) {
    using namespace drawing;
    Memory<char> empty;
    Memory<char> tenBytes(10);
    const char message[] = "HELLO WORLD";
    ConstMemory<char> messageMemory(message, strlen(message) + 1, nullptr, nullptr);
    EXPECT_EQ(empty.size(), 0u);
    EXPECT_EQ(tenBytes.size(), 10u);
    EXPECT_EQ(empty.get(), nullptr);
    EXPECT_TRUE(tenBytes.get());
    EXPECT_TRUE(tenBytes);
    EXPECT_FALSE(empty);
    EXPECT_EQ(tenBytes.get(), &tenBytes[0]);

    Memory<char> tenBytes2(tenBytes);
    Memory<char> tenBytes3;
    tenBytes3 = tenBytes;
    EXPECT_EQ(tenBytes.get(), tenBytes2.get());
    EXPECT_EQ(tenBytes.get(), tenBytes3.get());

    Memory<char> empty2(empty);
    Memory<char> empty3;
    empty3 = empty;
    EXPECT_FALSE(empty2);
    EXPECT_FALSE(empty3);
}
