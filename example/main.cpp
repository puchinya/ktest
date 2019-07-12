#include <iostream>
#include <ktest.hpp>

int test_func(int a, int b)
{
    return a + b;
}

TEST(sample_test_case, test_0)
{
    int c = test_func(3, 4);
    ASSERT_EQ(c, 7);
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