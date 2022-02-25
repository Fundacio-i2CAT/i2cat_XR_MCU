#pragma once

#include "DDKLogger.h"
#include "ddk_crash.h"
#include "ddk_flagset.h"

#define EXPAND_CLASS_TEMPLATE(_CLASS,...) \
    _CLASS<__VA_ARGS__>


#if defined(WIN32)

    //pragma macros
    #define __TODO(_FILE,_LINE,_TEXT) __pragma(message(_FILE"("#_LINE") : " #_TEXT))
    #define _TODO(_FILE,_LINE,_TEXT) __TODO(_FILE,_LINE,_TEXT)
    #define TODO(text) _TODO(__FILE__,__LINE__,text)
    #define UNUSED(_x) (void)(_x);

    //annotations
    #define NO_DISCARD_RETURN [[ nodiscard ]]

    #define DDK_THROW(__cond,__exception,...) \
        { \
            if(static_cast<bool>(__cond) == false) \
            { \
                throw __exception{__VA_ARGS__}; \
            } \
        }

    #ifdef DDK_DEBUG

        #include "ddk_assert.h"

        #define DDK_ASSERT(__cond,__msg) \
            { \
                if(static_cast<bool>(__cond) == false) \
                { \
                    ddk::detail::Assert assertion(#__cond,__FILE__,__LINE__,"Assert",__msg); \
                    if (assertion.raise() == ddk::detail::Assert::Break) \
                    { \
                        __debugbreak(); \
                    } \
                } \
            }

        #define DDK_FAIL(__msg) \
            { \
                ddk::detail::Assert assertion(__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ddk::detail::Assert::Break) \
                { \
                    __debugbreak(); \
                } \
            }
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            DDK_ASSERT(__cond,__msg)
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_FAIL(__msg)

    #else

        #define DDK_ASSERT(cond,msg)
        #define DDK_FAIL(msg)
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if(static_cast<bool>(__cond) == false) \
                { \
                    DDK_LOG_ERROR(__msg); \
                } \
            }
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_LOG_ERROR(__msg);

    #endif

#elif defined(__LINUX__)

    #define PRAGMA(p)		_Pragma(#p)
    #define TODO(something) PRAGMA(message(something));
    #define UNUSED(_x) (void)_x;

    //annotations
    #define NO_DISCARD_RETURN [[ nodiscard ]]

    #ifdef DDK_DEBUG

        #include <signal.h>
        #include "ddk_assert.h"

        #define DDK_ASSERT(__cond,__msg) \
            if(static_cast<bool>(__cond) == false) \
            { \
                ddk::detail::Assert assertion(#__cond,__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ddk::detail::Assert::Break) \
                { \
                    raise(SIGTRAP); \
                } \
            }

        #define DDK_FAIL(__msg) \
            { \
                ddk::detail::Assert assertion(__FILE__,__LINE__,"Assert",__msg); \
                if (assertion.raise() == ddk::detail::Assert::Break) \
                { \
                    raise(SIGTRAP); \
                } \
            }

        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            DDK_ASSERT(__cond,__msg)
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_FAIL(__msg)

    #else

        #define DDK_ASSERT(__cond,__msg)
        #define DDK_FAIL(__msg)
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if(static_cast<bool>(__cond) == false) \
                { \
                    DDK_LOG_ERROR(__msg); \
                } \
            }
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_LOG_ERROR(__msg);

    #endif

#elif defined(__APPLE__)

    #define PRAGMA(p) _Pragma(#p)
    #define TODO(something) PRAGMA(message(something));
    #define UNUSED(_x) (void)_x;

    #ifdef DDK_DEBUG

        #include <signal.h>

        #define DDK_ASSERT(__cond,__msg) \
            { \
                if(static_cast<bool>(__cond) == false) \
                { \
                    raise(SIGTRAP); \
                } \
            }
        #define DDK_FAIL(__msg) \
            { \
                raise(SIGTRAP); \
            }
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            DDK_ASSERT(__cond,__msg)
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_FAIL(__msg)

    #else

        #define DDK_ASSERT(cond,msg)
        #define DDK_FAIL(msg)
        #define DDK_ASSERT_OR_LOG(__cond,__msg) \
            { \
                if(static_cast<bool>(__cond) == false) \
                { \
                    DDK_LOG_ERROR(__msg); \
                } \
            }
        #define DDK_FAIL_OR_LOG(__msg) \
            DDK_LOG_ERROR(__msg);

    #endif

#else

#define PRAGMA(p)		_Pragma(#p)
#define TODO(something) PRAGMA(message(something));
#define UNUSED (void)
#endif

#ifndef NULL
	#define NULL 0
#endif

#define MAKE_IT_CRASH \
		DDK_LOG_ERROR("Crashing at file: " << __FILE__ << ", line: " << __LINE__); \
		*reinterpret_cast<volatile int*>(0xDEAD);

#define CRASH_ON_RETURN(_TYPE) \
		DDK_LOG_ERROR("Crashing at file: " << __FILE__ << ", line: " << __LINE__); \
		return ddk::crash_on_return<_TYPE>::value();

#define EXPAND_ARGS(_ARGS) _ARGS
#define _NUM_ARGS(X100, X99, X98, X97, X96, X95, X94, X93, X92, X91, X90, X89, X88, X87, X86, X85, X84, X83, X82, X81, X80, X79, X78, X77, X76, X75, X74, X73, X72, X71, X70, X69, X68, X67, X66, X65, X64, X63, X62, X61, X60, X59, X58, X57, X56, X55, X54, X53, X52, X51, X50, X49, X48, X47, X46, X45, X44, X43, X42, X41, X40, X39, X38, X37, X36, X35, X34, X33, X32, X31, X30, X29, X28, X27, X26, X25, X24, X23, X22, X21, X20, X19, X18, X17, X16, X15, X14, X13, X12, X11, X10, X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ...) N
#define NUM_ARGS(...) EXPAND_ARGS(_NUM_ARGS(__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

#define FOREACH(MACRO,  ...)    FOREACH_(MACRO,__VA_ARGS__)
#define FOREACH_(MACRO,  ...)   FOREACH__(NUM_ARGS(__VA_ARGS__), MACRO,  __VA_ARGS__)
#define FOREACH__(N, M,  ...)   FOREACH___(N, M,  __VA_ARGS__)
#define FOREACH___(N, M,  ...)  FOREACH_##N(M,  __VA_ARGS__)
#define FOREACH_0(M, ...)
#define FOREACH_1(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_0(M)
#define FOREACH_2(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_1(M,__VA_ARGS__)
#define FOREACH_3(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_2(M,__VA_ARGS__)
#define FOREACH_4(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_3(M,__VA_ARGS__)
#define FOREACH_5(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_4(M,__VA_ARGS__)
#define FOREACH_6(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_5(M,__VA_ARGS__)
#define FOREACH_7(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_6(M,__VA_ARGS__)
#define FOREACH_8(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_7(M,__VA_ARGS__)
#define FOREACH_9(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_8(M,__VA_ARGS__)
#define FOREACH_10(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_9(M,__VA_ARGS__)
#define FOREACH_11(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_10(M,__VA_ARGS__)
#define FOREACH_12(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_11(M,__VA_ARGS__)
#define FOREACH_13(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_12(M,__VA_ARGS__)
#define FOREACH_14(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_13(M,__VA_ARGS__)
#define FOREACH_15(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_14(M,__VA_ARGS__)
#define FOREACH_16(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_15(M,__VA_ARGS__)
#define FOREACH_17(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_16(M,__VA_ARGS__)
#define FOREACH_18(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_17(M,__VA_ARGS__)
#define FOREACH_19(M, FIRST_ARG, ...)  M(FIRST_ARG) FOREACH_18(M,__VA_ARGS__)
#define FOREACH_20(M, FIRST_ARG, ...) M(FIRST_ARG) FOREACH_19(M,__VA_ARGS__)

#define FOREACH_ARG(MACRO,ARG,  ...)    FOREACH_ARG_(MACRO,ARG,__VA_ARGS__)
#define FOREACH_ARG_(MACRO,ARG,  ...)   FOREACH_ARG__(NUM_ARGS(__VA_ARGS__),MACRO,ARG,__VA_ARGS__)
#define FOREACH_ARG__(N, M,ARG,  ...)   FOREACH_ARG___(N,M,ARG,__VA_ARGS__)
#define FOREACH_ARG___(N, M,ARG,  ...)  EXPAND_ARGS(FOREACH_ARG_##N(M,ARG,__VA_ARGS__))
#define FOREACH_ARG_1(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG)
#define FOREACH_ARG_2(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_1(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_3(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_2(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_4(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_3(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_5(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_4(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_6(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_5(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_7(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_6(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_8(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_7(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_9(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_8(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_10(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_9(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_11(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_10(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_12(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_11(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_13(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_12(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_14(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_13(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_15(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_14(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_16(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_15(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_17(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_16(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_18(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_17(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_19(M,ARG,FIRST_ARG, ...)  M(ARG,FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_18(M,ARG,##__VA_ARGS__))
#define FOREACH_ARG_20(M,ARG,FIRST_ARG, ...)  M(ARG, FIRST_ARG) EXPAND_ARGS(FOREACH_ARG_19(M,ARG,##__VA_ARGS__))
