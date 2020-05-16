/**
 *  K Test framework
 */
#pragma once
#ifndef KTEST_HPP
#define KTEST_HPP

#include <new>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cinttypes>
#include <cstdio>

#define KTEST_TEST_CLASS_NAME_(test_suite_name, test_name)  test_suite_name##_##test_name##_Test

#define KTEST_TEST(test_case_name, test_name) \
    class KTEST_TEST_CLASS_NAME_(test_case_name, test_name) : public ktest::Test { \
    public: \
        KTEST_TEST_CLASS_NAME_(test_case_name, test_name)() = default; \
        ~KTEST_TEST_CLASS_NAME_(test_case_name, test_name)() = default; \
    private: \
        void test_body() override; \
        static ::ktest::TestInfo * const m_test_info; \
    }; \
    ::ktest::TestInfo * const KTEST_TEST_CLASS_NAME_(test_case_name, test_name)::m_test_info = \
    make_and_register_test_info(#test_case_name, #test_name, ::ktest::CodeLocation(__FILE__, __LINE__), \
    new(std::nothrow) ::ktest::TestFactoryImpl<KTEST_TEST_CLASS_NAME_(test_case_name, test_name)>()); \
    void KTEST_TEST_CLASS_NAME_(test_case_name, test_name)::test_body()

#define RUN_ALL_TESTS() (ktest::get_unit_test()->run())
#define TEST(test_case_name, test_name)    KTEST_TEST(test_case_name, test_name)
#define EXPECT_TRUE(actual)  ((actual) ? (void)0 : \
    ::ktest::expect_true(::ktest::CodeLocation(__FILE__, __LINE__), #actual, actual), 0)
#define EXPECT_FALSE(actual)  (!(actual) ? (void)0 : \
    ::ktest::expect_false(::ktest::CodeLocation(__FILE__, __LINE__), #actual, actual), 0)
#define EXPECT_EQ(expected, actual)  ((expected) == (actual) ? (void)0 : \
    ::ktest::expect_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " == " #actual, expected, actual), 0)
#define EXPECT_NE(expected, actual)  ((expected) != (actual) ? (void)0 : \
    ::ktest::expect_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " != " #actual, expected, actual), 0)
#define EXPECT_LT(expected, actual)  ((expected) < (actual) ? (void)0 : \
    ::ktest::expect_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " < " #actual, expected, actual), 0)
#define EXPECT_LE(expected, actual)  ((expected) <= (actual) ? (void)0 : \
    ::ktest::expect_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " <= " #actual, expected, actual), 0)
#define EXPECT_GT(expected, actual)  ((expected) > (actual) ? (void)0 : \
    ::ktest::expect_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " > " #actual, expected, actual), 0)
#define EXPECT_GE(expected, actual)  ((expected) >= (actual) ? (void)0 : \
    ::ktest::expect_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " >= " #actual, expected, actual), 0)
#define EXPECT_STREQ(expected, actual)  (strcmp((expected), (actual) == 0 ? (void)0 : \
    ::ktest::expect_compare_string(::ktest::CodeLocation(__FILE__, __LINE__), #expected " == " #actual, expected, actual), 0)
#define EXCPECT_STRNE(expected, actual)  (strcmp((expected), (actual) != 0 ? (void)0 : \
    ::ktest::expect_compare_string(::ktest::CodeLocation(__FILE__, __LINE__), #expected " != " #actual, expected, actual), 0)

#define ASSERT_TRUE(actual)  ((actual) ? (void)0 : \
    ::ktest::assert_true(::ktest::CodeLocation(__FILE__, __LINE__), #actual, actual), 0)
#define ASSERT_FALSE(actual)  (!(actual) ? (void)0 : \
    ::ktest::assert_false(::ktest::CodeLocation(__FILE__, __LINE__), #actual, actual), 0)

#define ASSERT_EQ(expected, actual)  ((expected) == (actual) ? (void)0 : \
    ::ktest::assert_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " == " #actual, expected, actual), 0)
#define ASSERT_NE(expected, actual)  ((expected) != (actual) ? (void)0 : \
    ::ktest::assert_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " != " #actual, expected, actual), 0)
#define ASSERT_LT(expected, actual)  ((expected) < (actual) ? (void)0 : \
    ::ktest::assert_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " < " #actual, expected, actual), 0)
#define ASSERT_LE(expected, actual)  ((expected) <= (actual) ? (void)0 : \
    ::ktest::assert_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " <= " #actual, expected, actual), 0)
#define ASSERT_GT(expected, actual)  ((expected) > (actual) ? (void)0 : \
    ::ktest::assert_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " > " #actual, expected, actual), 0)
#define ASSERT_GE(expected, actual)  ((expected) >= (actual) ? (void)0 : \
    ::ktest::assert_compare(::ktest::CodeLocation(__FILE__, __LINE__), #expected " >= " #actual, expected, actual), 0)
#define ASSERT_STREQ(expected, actual)  (strcmp((expected), (actual) == 0 ? (void)0 : \
    ::ktest::assert_compare_string(::ktest::CodeLocation(__FILE__, __LINE__), #expected " == " #actual, expected, actual), 0)
#define ASSERT_STRNE(expected, actual)  (strcmp((expected), (actual) != 0 ? (void)0 : \
    ::ktest::assert_compare_string(::ktest::CodeLocation(__FILE__, __LINE__), #expected " != " #actual, expected, actual), 0)

namespace ktest {

    using namespace std;

    class Test;
    class TestCase;
    class TestInfo;
    class UnitTest;

    struct CodeLocation {
        CodeLocation(const char *a_file, uint32_t a_line)
        : file(a_file), line(a_line)
        {
        }

        const char *file;
        uint32_t line;
    };

    class ListNode {
    public:
        ListNode()
        : m_next(nullptr), m_prev(nullptr)
        {

        }

        ListNode *m_next;
        ListNode *m_prev;
    };

    class ZString;
    class ConstZString;

    class ZString {
        friend class ConstZString;
    public:
        ZString()
        : m_value(nullptr)
        {

        }

        ZString(char *value)
        : m_value(value)
        {

        }

        ZString(const ZString &zs) = default;

        char operator [](size_t index)
        {
            return m_value[index];
        }

        explicit operator char * () const {
            return m_value;
        }

        size_t get_length() const {
            return strlen(m_value);
        }
    private:
        char *m_value;
    };

    class ConstZString {
    public:
        ConstZString()
        : m_value(nullptr)
        {

        }
        ConstZString(const char *value)
                : m_value(value)
        {
        }

        ConstZString(const ConstZString &z) = default;
        ConstZString(const ZString &z)
        : m_value(z.m_value)
        {

        }

        char operator [](size_t index)
        {
            return m_value[index];
        }

        ConstZString &operator = (const ZString &z)
        {
            m_value = z.m_value;
            return *this;
        }

        bool operator == (const ConstZString &z)
        {
            return ::strcmp(m_value, z.m_value) == 0;
        }

        bool operator == (const char *s)
        {
            return ::strcmp(m_value, s) == 0;
        }

        bool operator != (const ConstZString &z)
        {
            return ::strcmp(m_value, z.m_value) != 0;
        }

        bool operator < (const ConstZString &z)
        {
            return ::strcmp(m_value, z.m_value) < 0;
        }

        bool operator <= (const ConstZString &z)
        {
            return strcmp(m_value, z.m_value) <= 0;
        }

        bool operator > (const ConstZString &z)
        {
            return strcmp(m_value, z.m_value) > 0;
        }

        bool operator >= (const ConstZString &z)
        {
            return strcmp(m_value, z.m_value) >= 0;
        }

        explicit operator const char *() const {
            return m_value;
        }

        const char *get_data() const {
            return m_value;
        }

        size_t get_length() const {
            return strlen(m_value);
        }
    private:
        const char *m_value;
    };

    template <class T> class List;

    template <class T> class ListIterator {
    private:
        friend class List<T>;
        ListNode *m_item;
        explicit ListIterator(ListNode *item)
        : m_item(item)
        {

        }
    public:
        T * operator -> () {
            return (T*)m_item;
        }
        ListIterator operator ++ () {
            m_item = m_item->m_next;
            return ListIterator(m_item);
        }
        ListIterator operator -- () {
            m_item = m_item->m_prev;
            return ListIterator(m_item);
        }
        bool operator == (const ListIterator &item) {
            return m_item == item.m_item;
        }
        bool operator != (const ListIterator &item) {
            return m_item != item.m_item;
        }

        operator T *() {
            return reinterpret_cast<T*>(m_item);
        }
    };

    template <class T> class List {
    private:
        ListNode *at_impl(int index);
        void add_impl(ListNode *item);
    public:
        List();

        void add(T *item) {
            add_impl(static_cast<ListNode *>(item));
        }

        T *at(uint32_t index)
        {
            return reinterpret_cast<T*>(at_impl(index));
        }

        uint32_t get_count() const {
            return m_count;
        }

        ListIterator<T> begin() {
            return ListIterator<T>(m_root.m_next);
        }

        ListIterator<T> end() {
            return ListIterator<T>(&m_root);
        }

    private:
        ListNode m_root;
        uint32_t m_count;
    };

    class TestFactoryBase {
    public:
        virtual ~TestFactoryBase() = default;

        virtual Test *create_test() = 0;
    protected:
        TestFactoryBase() = default;
    };

    template <class T>
    class TestFactoryImpl : public TestFactoryBase {
    public:
        Test *create_test() override {
            return new T();
        }
    };

    class TestInfo : public ListNode {
    public:
        TestInfo(const char *test_case_name, const char *test_name,
                 const CodeLocation &location, TestFactoryBase *test_factory);

        ~TestInfo();

        ConstZString get_test_case_name() const {
            return m_test_case_name;
        }

        ConstZString get_name() const {
            return m_name;
        }

    private:
        friend class TestCase;
        friend class UnitTest;
        ConstZString m_test_case_name;
        ConstZString m_name;
        CodeLocation m_location;
        TestFactoryBase *m_test_factory;
    };

    class OutputStream
    {
    protected:
        OutputStream();
        void set_error() {
            m_is_error = true;
        }
    public:
        virtual ~OutputStream();

        virtual void write(const ConstZString &zs) = 0;

        OutputStream &operator << (const ConstZString &value)
        {
            write(value);
            return *this;
        }

        OutputStream &operator << (int32_t value);

        bool is_error() const {
            return m_is_error;
        }

        void clear_error() {
            m_is_error = false;
        }
    private:
        bool m_is_error;
    };

    class Test {
    public:
        virtual ~Test();

        void run();

    protected:
        Test();

        virtual void set_up();
        virtual void tear_down();
        virtual void test_body() = 0;

    private:

    };

    class TestCase : public ListNode {
    public:
        explicit TestCase(const ConstZString &name)
         : m_name(name), m_test_infos()
        {

        }

        ~TestCase() = default;

        ConstZString get_name() const {
            return m_name;
        }

        void add_test_info(TestInfo *test_info);

    private:

        friend class UnitTest;
        ConstZString m_name;
        List<TestInfo> m_test_infos;
    };

    class UnitTest {
    public:
        UnitTest() = default;

        static UnitTest *get_instance();

        bool run();

        uint32_t get_successful_test_case_count() const
        {
            return m_successful_test_case_count;
        }
        uint32_t get_failed_test_case_count() const
        {
            return m_failed_test_case_count;
        }
        uint32_t get_total_test_case_count() const
        {
            return m_total_test_case_count;
        }

        TestCase *get_test_case(const ConstZString &test_case_name);

        void add_test_info(TestInfo *test_info);
    public:

        void set_cur_test_case(TestCase *test_case) {
            m_cur_test_case = test_case;
        }

        void set_cur_test_failed() {
            m_cur_test_successful = false;
        }

        uint32_t m_successful_test_case_count = 0;
        uint32_t m_failed_test_case_count = 0;
        uint32_t m_total_test_case_count = 0;
        uint32_t m_successful_test_count = 0;
        uint32_t m_cur_test_case_successful_count = 0;
        uint32_t m_cur_test_case_failed_count = 0;
        bool m_test_case_init_error = false;
        bool m_cur_test_successful = false;
        List<TestCase> m_test_cases;
        OutputStream *m_output_stream = nullptr;
        TestCase *m_cur_test_case = nullptr;
    };

    static inline UnitTest *get_unit_test()
    {
        return UnitTest::get_instance();
    }

    void init_ktest(int argc, char **argv, OutputStream *os);

    TestInfo *make_and_register_test_info(const char *test_case_name,
                                const char *name,
                                const CodeLocation &code_location,
                                TestFactoryBase *test_factory) noexcept;

    [[noreturn]] void fatal_error();

    [[noreturn]] void assertion_impl(const CodeLocation &code_location,
                                     const ConstZString &expr,
                                     const ConstZString &expected,
                                     const ConstZString &actual);

    [[noreturn]] static void assert_true(const CodeLocation &code_location,
                                                          const ConstZString &expr,
                                                          bool actual)
    {
        assertion_impl(code_location, expr, "true", actual ? "true" : "false");
    }

    [[noreturn]] static void assert_false(const CodeLocation &code_location,
                                  const ConstZString &expr,
                                  bool actual)
    {
        assertion_impl(code_location, expr, "false", actual ? "true" : "false");
    }

    void expect_impl(const CodeLocation &code_location,
                                  const ConstZString &expr,
                                  const ConstZString &expected,
                                  const ConstZString &actual);

   static void expect_true(const CodeLocation &code_location,
                                         const ConstZString &expr,
                                         bool actual)
    {
        expect_impl(code_location, expr, "true", actual ? "true" : "false");
    }

    static void expect_false(const CodeLocation &code_location,
                                          const ConstZString &expr,
                                          bool actual)
    {
        expect_impl(code_location, expr, "false", actual ? "true" : "false");
    }

    static void to_string(int8_t from, char *to, size_t to_size)
    {
        ::sprintf(to, "%d", from);
    }

    static void to_string(uint8_t from, char *to, size_t to_size)
    {
        ::sprintf(to, "%d", from);
    }

    static void to_string(int16_t from, char *to, size_t to_size)
    {
        ::sprintf(to, "%d", from);
    }

    static void to_string(uint16_t from, char *to, size_t to_size)
    {
        ::sprintf(to, "%d", from);
    }

    static void to_string(int32_t from, char *to, size_t to_size)
    {
        ::sprintf(to, "%d", from);
    }

    static void to_string(uint32_t from, char *to, size_t to_size)
    {
        ::sprintf(to, "%d" PRIu32, from);
    }

    static void to_string(int64_t from, char *to, size_t to_size)
    {
        ::sprintf(to, "%" PRIi64, from);
    }

    static void to_string(uint64_t from, char *to, size_t to_size)
    {
        ::sprintf(to, "%" PRIu64, from);
    }

    static void to_string(void *from, char *to, size_t to_size)
    {
        auto from_value = reinterpret_cast<uintptr_t>(from);

        if(sizeof(uintptr_t) > 4) {
            ::sprintf(to, "0x%016" PRIx64, static_cast<uint64_t>(from_value));
        } else {
            ::sprintf(to, "0x%08lx", from_value);
        }
    }

    static void to_string(bool from, char *to, size_t to_size)
    {
        strcpy(to, from ? "true" : "false");
    }

    template <typename T> struct MaxStringLength
    {

    };

    template <> struct MaxStringLength<int8_t>
    {
        static constexpr size_t value = 4;
    };

    template <> struct MaxStringLength<uint8_t>
    {
        static constexpr size_t value = 3;
    };

    template <> struct MaxStringLength<int16_t>
    {
        static constexpr size_t value = 6;
    };

    template <> struct MaxStringLength<uint16_t>
    {
        static constexpr size_t value = 5;
    };

    template <> struct MaxStringLength<int32_t>
    {
        static constexpr size_t value = 11;
    };

    template <> struct MaxStringLength<uint32_t>
    {
        static constexpr size_t value = 10;
    };

    template <> struct MaxStringLength<int64_t>
    {
        static constexpr size_t value = 20;
    };

    template <> struct MaxStringLength<uint64_t>
    {
        static constexpr size_t value = 20;
    };

    template <> struct MaxStringLength<void *>
    {
        static constexpr size_t value = 10;
    };

    template <> struct MaxStringLength<bool>
    {
        static constexpr size_t value = 5;
    };

    template <typename T> [[noreturn]] static void assert_compare(const CodeLocation &code_location,
                                        const ConstZString &expr,
                                        const T &expected,
                                        const T &actual)
    {
        char expected_str[MaxStringLength<T>::value + 1];
        char actual_str[MaxStringLength<T>::value + 1];

        to_string(expected, expected_str, sizeof(expected_str));
        to_string(actual, actual_str, sizeof(actual_str));

        assertion_impl(code_location, expr, expected_str, actual_str);
    }

    [[noreturn]] static void assert_compare_string(const CodeLocation &code_location,
                                                                  const ConstZString &expr,
                                                                  const char *expected,
                                                                  const char *actual)
    {
        assertion_impl(code_location, expr, expected, actual);
    }

    template <typename T> static void expect_compare(const CodeLocation &code_location,
                                                                  const ConstZString &expr,
                                                                  const T &expected,
                                                                  const T &actual)
    {
        char expected_str[MaxStringLength<T>::value + 1];
        char actual_str[MaxStringLength<T>::value + 1];

        to_string(expected, expected_str, sizeof(expected_str));
        to_string(actual, actual_str, sizeof(actual_str));

        expect_impl(code_location, expr, expected_str, actual_str);
    }

    static void expect_compare_string(const CodeLocation &code_location,
                                                   const ConstZString &expr,
                                                   const char *expected,
                                                   const char *actual)
    {
        expect_impl(code_location, expr, expected, actual);
    }
}

#endif
