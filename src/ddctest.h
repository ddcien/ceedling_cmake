#ifndef DDCTEST_H
#define DDCTEST_H

#include "pp_iter.h"

#include "unity.h"

#define __STR(x) #x
#define __STRZ(x) __STR(x)
#define __JOIN(...) PP_CAT(_, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

struct ddctest_suit;

typedef void (*v_func)(void);
typedef void (*runner_func)(struct ddctest_suit const* suit);

struct ddctest_test {
    char const* const file;
    unsigned int line;
    char const* const name;
    runner_func const runner;
};

struct ddctest_suit {
    char const* const file;
    unsigned int line;
    char const* const name;
    v_func const suit_setup;
    v_func const suit_teardown;
    v_func const test_setup;
    v_func const test_teardown;
    struct ddctest_test const* const* const start;
    struct ddctest_test const* const* const stop;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#define __TEST_SUIT_ATTR() __attribute__((used, section(__STRZ(dd_test))))
#define __TEST_SUIT_START() __JOIN(__start, dd_test)
#define __TEST_SUIT_STOP() __JOIN(__stop, dd_test)

#define __TEST_SUIT_SETUP(suit) __JOIN(_, suit, suit_setup)
#define __TEST_SUIT_TEARDOWN(suit) __JOIN(_, suit, suit_teardown)
#define __TEST_SUIT_TEST_SETUP(suit) __JOIN(_, suit, test_setup)
#define __TEST_SUIT_TEST_TEARDOWN(suit) __JOIN(_, suit, test_teardown)

#define __TEST_SUIT_TEST_FUNC(suit, test) __JOIN(_, suit, test, func)
#define __TEST_SUIT_TEST_RUNNER(suit, test, repeat, ...) __JOIN(_, suit, test, runner, repeat, PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define __TEST_TEST_ATTR(suit) __attribute__((used, section(__STRZ(__JOIN(dd_test, suit)))))
#define __TEST_TEST_START(suit) __JOIN(__start, __JOIN(dd_test, suit))
#define __TEST_TEST_STOP(suit) __JOIN(__stop, __JOIN(dd_test, suit))

#define __TEST_SUIT_NAME(suit) __JOIN(_, suit)
#define __TEST_SUIT_TEST_NAME(suit, test) __JOIN(_, suit, test, __COUNTER__)

#define TEST_SUIT(suit)                                                                                                                    \
    extern struct ddctest_test const* const __TEST_TEST_START(suit);                                                                       \
    extern struct ddctest_test const* const __TEST_TEST_STOP(suit);                                                                        \
    static void __TEST_SUIT_SETUP(suit)(void);                                                                                             \
    static void __TEST_SUIT_TEARDOWN(suit)(void);                                                                                          \
    static void __TEST_SUIT_TEST_SETUP(suit)(void);                                                                                        \
    static void __TEST_SUIT_TEST_TEARDOWN(suit)(void);                                                                                     \
    struct ddctest_suit const* const __TEST_SUIT_NAME(suit) __TEST_SUIT_ATTR() = &((struct ddctest_suit const){                            \
        .file = __FILE__,                                                                                                                  \
        .line = __LINE__,                                                                                                                  \
        .name = __STRZ(suit),                                                                                                              \
        .suit_setup = __TEST_SUIT_SETUP(suit),                                                                                             \
        .suit_teardown = __TEST_SUIT_TEARDOWN(suit),                                                                                       \
        .test_setup = __TEST_SUIT_TEST_SETUP(suit),                                                                                        \
        .test_teardown = __TEST_SUIT_TEST_TEARDOWN(suit),                                                                                  \
        .start = &(__TEST_TEST_START(suit)),                                                                                               \
        .stop = &(__TEST_TEST_STOP(suit)),                                                                                                 \
    })

#define TEST_SUIT_SETUP(suit) static void __TEST_SUIT_SETUP(suit)(void)
#define TEST_SUIT_TEARDOWN(suit) static void __TEST_SUIT_TEARDOWN(suit)(void)
#define TEST_SUIT_TEST_SETUP(suit) static void __TEST_SUIT_TEST_SETUP(suit)(void)
#define TEST_SUIT_TEST_TEARDOWN(suit) static void __TEST_SUIT_TEST_TEARDOWN(suit)(void)

#define TEST_CASE_DEC(suit, test, ...) static inline __attribute__((always_inline)) void __TEST_SUIT_TEST_FUNC(suit, test)(__VA_ARGS__)

#define TEST_CASE_RUN(suit, test, repeat, ...)                                                                                             \
    static void __TEST_SUIT_TEST_RUNNER(suit, test, repeat, __VA_ARGS__)(struct ddctest_suit const* suit)                                  \
    {                                                                                                                                      \
        for (int i = 0; i < (repeat); ++i) {                                                                                               \
            Unity.TestFile = __FILE__;                                                                                                     \
            Unity.CurrentTestName = __STRZ(test);                                                                                          \
            Unity.CurrentTestLineNumber = __LINE__;                                                                                        \
            Unity.NumberOfTests++;                                                                                                         \
            if (TEST_PROTECT()) {                                                                                                          \
                suit->test_setup();                                                                                                        \
                __TEST_SUIT_TEST_FUNC(suit, test)(__VA_ARGS__);                                                                            \
            }                                                                                                                              \
            if (TEST_PROTECT()) {                                                                                                          \
                suit->test_teardown();                                                                                                     \
            }                                                                                                                              \
            if (Unity.CurrentTestIgnored) {                                                                                                \
                Unity.TestIgnores++;                                                                                                       \
                Unity.CurrentTestIgnored = 0;                                                                                              \
                UNITY_PRINT_EOL();                                                                                                         \
            } else if (Unity.CurrentTestFailed) {                                                                                          \
                Unity.TestFailures++;                                                                                                      \
                Unity.CurrentTestFailed = 0;                                                                                               \
                UNITY_PRINT_EOL();                                                                                                         \
            }                                                                                                                              \
        }                                                                                                                                  \
    }                                                                                                                                      \
    static struct ddctest_test const* const __TEST_SUIT_TEST_NAME(suit, test) __TEST_TEST_ATTR(suit) = &((struct ddctest_test const){      \
        .file = __FILE__,                                                                                                                  \
        .line = __LINE__,                                                                                                                  \
        .name = __STRZ(test),                                                                                                              \
        .runner = __TEST_SUIT_TEST_RUNNER(suit, test, repeat, __VA_ARGS__),                                                                \
    })

#define TEST_CASE_RUN_ONCE(suit, test, ...) TEST_CASE_RUN(suit, test, 1, __VA_ARGS__)

extern struct ddctest_suit const* const __TEST_SUIT_START();
extern struct ddctest_suit const* const __TEST_SUIT_STOP();

#define __TEST_RUN_SUIT(suit)                                                                                                              \
    ({                                                                                                                                     \
        __auto_type const _suit = (suit);                                                                                                  \
        if (TEST_PROTECT()) {                                                                                                              \
            _suit->suit_setup();                                                                                                           \
        }                                                                                                                                  \
        for (struct ddctest_test const* const* test = _suit->start; test < _suit->stop; ++test) {                                          \
            (*test)->runner(_suit);                                                                                                        \
        }                                                                                                                                  \
        if (TEST_PROTECT()) {                                                                                                              \
            _suit->suit_teardown();                                                                                                        \
        }                                                                                                                                  \
    })

#define TEST_RUN_SUIT(suit) __TEST_RUN_SUIT(&(__TEST_SUIT_NAME(suit)))

#define TEST_RUN_ALL()                                                                                                                     \
    ({                                                                                                                                     \
        UnityBegin(NULL);                                                                                                                  \
        for (struct ddctest_suit const* const* suit = &__TEST_SUIT_START(); suit < &__TEST_SUIT_STOP(); ++suit) {                          \
            __TEST_RUN_SUIT(*suit);                                                                                                        \
        }                                                                                                                                  \
        UnityEnd();                                                                                                                        \
    })

#endif /* DDCTEST_H */
