#include <iostream>
#include <ktest.hpp>

int test_func(int a, int b)
{
    return a + b;
}

TEST(sample_test_case, test_0)
{
    int c = test_func(3, 4);
    EXPECT_EQ(c, 5);
    ASSERT_EQ(7, c);
    ASSERT_NE(7, c);
}

TEST(sample_test_case, test_1)
{
    int c = test_func(3, 4);
    ASSERT_TRUE(c == 7);
}

TEST(sample_test_case2, test_0)
{
    int c = test_func(3, 4);
    ASSERT_FALSE(c == 7);
}

TEST(sample_test_case2, test_1)
{
    int c = test_func(3, 4);
    ASSERT_EQ(5, c);
}

TEST(sample_test_case3, test_0)
{
    int c = test_func(3, 4);
    EXPECT_FALSE(c == 7);
}

TEST(sample_test_case3, test_1)
{
    int c = test_func(3, 4);
    EXPECT_EQ(5, c);
}

class StdOutStream : public ktest::OutputStream
{
public:
    StdOutStream() = default;
    void write(const ktest::ConstZString &zs) override;
};

void StdOutStream::write(const ktest::ConstZString &zs)
{
    std::cout << zs.get_data();
}

int main(int argc, char **argv) {
    StdOutStream sout;
    ::ktest::init_ktest(argc, argv, &sout);
    RUN_ALL_TESTS();
    return 0;
}