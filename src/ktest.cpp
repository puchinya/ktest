//
// Created by 鍋島雅貴 on 2019-06-20.
//

#include <ktest.hpp>
#include <new>
#include <setjmp.h>
#include <stdlib.h>

namespace ktest {

    template <class T> List<T>::List()
    : m_count(0)
    {
        m_root.m_next = &m_root;
        m_root.m_prev = &m_root;
    }

    template <class T> ListNode *List<T>::at_impl(int index)
    {
        if(index >= m_count) {
            return nullptr;
        }

        auto n = m_root.m_next;
        for(auto i = index; i > 0; --i)
        {
            n = n->m_next;
        }

        return n;
    }

    template <class T> void List<T>::add_impl(ListNode *item)
    {
        item->m_next = &m_root;
        item->m_prev = m_root.m_prev;
        m_root.m_prev->m_next = item;
        m_root.m_prev = item;
        m_count++;
    }

    static jmp_buf s_jmp_buf;

    template <class T, typename R>
    R handle_exception_in_method(T *obj, R (T::*method)())
    {
        int code = ::setjmp(s_jmp_buf);
        if(code == 0) {
            return (obj->*method)();
        } else {
            return static_cast<R>(0);
        }
    }

    void raise_exception(int code) {
        longjmp(s_jmp_buf, code);
    }

    OutputStream::OutputStream()
    : m_is_error(false)
    {

    }

    OutputStream::~OutputStream()
    {

    }

    constexpr size_t kI32StrMaxLength = 12; // included null

    static size_t i32_to_str(int32_t i, char s[kI32StrMaxLength])
    {
        char buf[12];
        int32_t abs_i = i;
        if(i < 0) {
            abs_i = -i;
        }
        int l = 0;
        if(abs_i == 0) {
            l = 1;
            buf[0] = '0';
        } else {
            while (abs_i > 0) {
                int32_t div_10 = abs_i / 10;
                int32_t mod_10 = abs_i - div_10 * 10;
                buf[l++] = (char)(mod_10 + '0');
                abs_i = div_10;
            }
        }
        char *p = s;
        if(i < 0) {
            *p++ = '-';
        }
        for(int c = l -1; c >= 0; c--)
        {
            *p++ = buf[c];
        }

        *p = 0;

        return (size_t)(p - s - 1);
    }

    OutputStream &OutputStream::operator << (int32_t value)
    {
        char s[kI32StrMaxLength];
        i32_to_str(value, s);

        write(s);

        return *this;
    }

    Test::Test()
    {

    }

    Test::~Test()
    {

    }

    void Test::set_up()
    {

    }

    void Test::tear_down()
    {

    }

    bool Test::run()
    {
        set_up();
        handle_exception_in_method(this, &Test::test_body);
        tear_down();

        return true; // @TODO
    }

    TestInfo::TestInfo(const char *test_case_name, const char *test_name,
             CodeLocation location, TestFactoryBase *test_factory)
            : m_test_case_name(test_case_name),
              m_name(test_name), m_location(location),
              m_test_factory(test_factory)
    {

    }

    TestInfo::~TestInfo()
    {
        delete m_test_factory;
    }

    void TestCase::add_test_info(TestInfo *test_info)
    {
        m_test_infos.add(test_info);
    }

    TestCase *UnitTest::get_test_case(const ConstZString &test_case_name)
    {
        for(auto it = m_test_cases.begin(); it != m_test_cases.end(); ++it)
        {
            auto name = it->get_name();
            if(name == test_case_name) {
                return it;
            }
        }

        auto test_case = new(std::nothrow) TestCase(test_case_name);

        if(test_case == nullptr) {
            fatal_error();
        }

        m_test_cases.add(test_case);

        return test_case;
    }

    UnitTest *UnitTest::get_instance()
    {
        static auto const instance = new(std::nothrow) UnitTest();
        return instance;
    }

    bool UnitTest::run()
    {
        auto test_cases_count = m_test_cases.get_count();
        auto total_test_count = 0;

        for(auto it = m_test_cases.begin(); it != m_test_cases.end(); ++it)
        {
            total_test_count += it->m_test_infos.get_count();
        }

        *m_output_stream << "[==========] Running " << total_test_count << " test from " << test_cases_count << " test case.\n";

        for(auto it = m_test_cases.begin(); it != m_test_cases.end(); ++it)
        {
            set_cur_test_case(&*it);
            m_cur_test_case_failed_count = 0;
            m_cur_test_case_successful_count = 0;

            auto test_case_name = it->get_name();
            auto test_count = it->m_test_infos.get_count();

            *m_output_stream << "[----------] " << test_count << " test from " << test_case_name << "\n";

            auto &test_infos = it->m_test_infos;

            for(auto it2 = test_infos.begin(); it2 != test_infos.end(); ++it2)
            {
                m_cur_test_sucessful = true;

                auto test_name = it2->get_name();

                *m_output_stream << "[ RUN      ] " << test_case_name << "." << test_name << "\n";

                auto test = it2->m_test_factory->create_test();
                auto status = test->run();
                delete test;

                if(status) {
                    m_cur_test_case_successful_count++;
                    *m_output_stream << "[       OK ] " << test_case_name << "." << test_name << " (0 ms)\n";
                } else {
                    m_cur_test_case_failed_count++;
                }
            }
            *m_output_stream << "[----------] " << test_count << " test from " << test_case_name << " (0 ms total)\n";

            set_cur_test_case(nullptr);
        }

        *m_output_stream << "[==========] " << total_test_count << " test from " << test_cases_count << " test case ran. (0 ms total)\n";

        *m_output_stream << "[  PASSED  ] " << get_successful_test_case_count() << " test.\n";

        return false;
    }

    void UnitTest::add_test_info(TestInfo *test_info)
    {
        auto test_case = get_test_case(test_info->get_test_case_name());
        test_case->add_test_info(test_info);
    }

    void init_ktest(int argc, char **argv, OutputStream *os)
    {
        get_unit_test()->m_output_stream = os;
    }

    TestInfo *make_and_register_test_info(const char *test_case_name,
                                          const char *name,
                                          CodeLocation code_location,
                                          TestFactoryBase *test_factory)
    {
        auto test_info = new TestInfo(test_case_name, name, code_location, test_factory);

        get_unit_test()->add_test_info(test_info);

        return test_info;
    }

    void fatal_error()
    {
        for(;;) {

        }
    }
}
