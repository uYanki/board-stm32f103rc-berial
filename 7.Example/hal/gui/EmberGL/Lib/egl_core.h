//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#ifndef EGL_CORE_H
#define EGL_CORE_H
//----------------------------------------------------------------------------

//============================================================================
// interface
//============================================================================
// external
#ifdef ARDUINO
// #include <Arduino.h>
#else
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "stdint.h"
#include "string.h"
// #include <cstdint>
// #include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <alloca.h>
#endif
#define EGL_NAMESPACE_BEGIN  namespace egl {
#define EGL_NAMESPACE_END    }
#define EGL_NAMESPACE_PREFIX egl::
#define EGL_USING_NAMESPACE  using namespace egl;
EGL_NAMESPACE_BEGIN

// new
#define EGL_DEBUG  // build with most verbose and comprehensive validation
// #define EGL_RELEASE  // build with basic validation
// #define EGL_RETAIL   // fastest build without validation

// build config
#ifdef EGL_DEBUG
// debug build config
#define EGL_BUILDOP_ASSERTS    1  // asserts
#define EGL_BUILDOP_ASSERT_LOC 1  // assert locations (filename + line)
#define EGL_BUILDOP_ASSERT_PED 1  // pedantic asserts
#define EGL_BUILDOP_ERRORS     1  // errors
#define EGL_BUILDOP_WARNINGS   1  // warnings
#define EGL_BUILDOP_WARN_LOC   1  // warning locations (filename + line)
#define EGL_BUILDOP_DEBUG_LOGS 1  // debug logging
#define EGL_BUILDOP_LOGS       1  // logging
#define EGL_BUILDOP_VERIFY_MSG 1  // verification messages
#elif defined(EGL_RELEASE)
// release build config
#define EGL_BUILDOP_ASSERTS    1  // asserts
#define EGL_BUILDOP_ASSERT_LOC 0  // assert locations (filename + line)
#define EGL_BUILDOP_ASSERT_PED 0  // pedantic asserts
#define EGL_BUILDOP_ERRORS     1  // errors
#define EGL_BUILDOP_WARNINGS   1  // warnings
#define EGL_BUILDOP_WARN_LOC   0  // warning locations (filename + line)
#define EGL_BUILDOP_DEBUG_LOGS 1  // debug logging
#define EGL_BUILDOP_LOGS       1  // logging
#define EGL_BUILDOP_VERIFY_MSG 1  // verification messages
#elif defined(EGL_RETAIL)
// retail build config
#define EGL_BUILDOP_ASSERTS    0  // asserts
#define EGL_BUILDOP_ASSERT_LOC 0  // assert locations (filename + line)
#define EGL_BUILDOP_ASSERT_PED 0  // pedantic asserts
#define EGL_BUILDOP_ERRORS     0  // errors
#define EGL_BUILDOP_WARNINGS   0  // warnings
#define EGL_BUILDOP_WARN_LOC   0  // warning locations (filename + line)
#define EGL_BUILDOP_DEBUG_LOGS 0  // debug logging
#define EGL_BUILDOP_LOGS       1  // logging
#define EGL_BUILDOP_VERIFY_MSG 1  // verification messages
#endif

// compiler defines
#ifdef _MSC_VER
#define EGL_COMPILER_MSVC
#define EGL_COMPILER_STR msvc
#elif defined(__GNUC__)
#define EGL_COMPILER_GCC
#define EGL_COMPILER_STR gcc
#endif

// MSVC compiler specific
#ifdef EGL_COMPILER_MSVC
#define EGL_INLINE                          __forceinline
#define EGL_RESTRICT                        __restrict
#define EGL_ALIGN(v__)                      __declspec(align(v__))
#define EGL_ALLOCA                          _alloca
#define EGL_SPRINTF(buf__, buf_size__, ...) sprintf_s(buf__, buf_size__, __VA_ARGS__)
#ifdef EGL_DEBUG
#define EGL_ABORT()     \
    {                   \
        __debugbreak(); \
    }
#else
#define EGL_ABORT() \
    {               \
        exit(-1);   \
    }
#endif
// disable warnings
#pragma warning(disable : 4293)  // "warning C4293: '<<': shift count negative or too big, undefined behavior"
#pragma warning(disable : 4127)  // "warning C4127: conditional expression is constant"
#endif

// GCC compiler specific
#ifdef EGL_COMPILER_GCC
#define EGL_INLINE                          __attribute__((always_inline)) inline
#define EGL_RESTRICT                        __restrict
#define EGL_ALIGN(v__)                      __attribute__((aligned(v__)))
#define EGL_ALLOCA                          alloca
#define EGL_SPRINTF(buf__, buf_size__, ...) sprintf(buf__, __VA_ARGS__)
#ifdef EGL_DEBUG
#define EGL_ABORT()       \
    {                     \
        __builtin_trap(); \
    }
#else
#define EGL_ABORT() \
    {               \
        exit(-1);   \
    }
#endif
#endif

// platform definitions
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)
// Teensy 4.0
#define EGL_PLATFORM_TEENSY
#define EGL_PLATFORM_TEENSY4X
#define EGL_PLATFORM_NUM_SPI 3
#define EGL_PLATFORM_NUM_DMA 32
#elif defined(__MK66FX1M0__)
// Teensy 3.6
#define EGL_PLATFORM_TEENSY
#define EGL_PLATFORM_TEENSY36
#define EGL_PLATFORM_NUM_SPI 3
#define EGL_PLATFORM_NUM_DMA 32
#elif defined(__MK64FX512__)
// Teensy 3.5
#define EGL_PLATFORM_TEENSY
#define EGL_PLATFORM_TEENSY35
#define EGL_PLATFORM_NUM_SPI 3
#define EGL_PLATFORM_NUM_DMA 16
#elif defined(__MK20DX256__)
// Teensy 3.1/3.2
#define EGL_PLATFORM_TEENSY
#define EGL_PLATFORM_TEENSY32
#define EGL_PLATFORM_NUM_SPI 1
#define EGL_PLATFORM_NUM_DMA 16
#elif defined(__MK20DX128__)
// Teensy 3.0
#define EGL_PLATFORM_TEENSY
#define EGL_PLATFORM_TEENSY30
#define EGL_PLATFORM_NUM_SPI 1
#define EGL_PLATFORM_NUM_DMA 0
#elif defined(__MKL26Z64__)
// Teensy LC
#define EGL_PLATFORM_TEENSY
#define EGL_PLATFORM_TEENSYLC
#define EGL_PLATFORM_NUM_SPI 2
#define EGL_PLATFORM_NUM_DMA 0
// Win32
#elif defined(EGL_PLATFORM_WIN32)
#define EGL_PLATFORM_STR     win32
#define EGL_PLATFORM_SRC_STR win
#define EGL_PLATFORM_32BIT
// Win64
#elif defined(EGL_PLATFORM_WIN64)
#define EGL_PLATFORM_STR     win64
#define EGL_PLATFORM_SRC_STR win
#define EGL_PLATFORM_64BIT
// Linux32
#elif defined(EGL_PLATFORM_LINUX32)
#undef linux
#define EGL_PLATFORM_STR     linux32
#define EGL_PLATFORM_SRC_STR linux
#define EGL_PLATFORM_32BIT
// Linux64
#elif defined(EGL_PLATFORM_LINUX64)
#undef linux
#define EGL_PLATFORM_STR     linux64
#define EGL_PLATFORM_SRC_STR linux
#define EGL_PLATFORM_64BIT
#endif

// config
#if !defined(EGL_PLATFORM_NUM_SPI) && defined(ARDUINO)
#define EGL_PLATFORM_NUM_SPI 1
#endif
#if !defined(EGL_PLATFORM_NUM_DMA) && defined(ARDUINO)
#define EGL_PLATFORM_NUM_DMA 0
#endif

// logging
void init_serial();
#if EGL_BUILDOP_LOGS == 1
#ifdef ARDUINO
#define EGL_LOG(msg__)       \
    {                        \
        Serial.print(msg__); \
    }
#define EGL_LOGF(...)                               \
    {                                               \
        char buf[64];                               \
        EGL_SPRINTF(buf, sizeof(buf), __VA_ARGS__); \
        Serial.print(buf);                          \
    }
#elif defined(EGL_COMPILER_MSVC)
#define EGL_LOG(msg__)            \
    {                             \
        OutputDebugString(msg__); \
    }
#define EGL_LOGF(...)                               \
    {                                               \
        char buf[256];                              \
        EGL_SPRINTF(buf, sizeof(buf), __VA_ARGS__); \
        OutputDebugString(buf);                     \
    }
#else
#define EGL_LOG(msg__) \
    {                  \
        printf(msg__); \
    }
#define EGL_LOGF(...)        \
    {                        \
        printf(__VA_ARGS__); \
    }
#endif
#else
#define EGL_LOG(msg__) \
    {                  \
    }
#define EGL_LOGF(...) \
    {                 \
    }
#endif

// debug logging
#if EGL_BUILDOP_DEBUG_LOGS == 1
#define EGL_DEBUG_LOG(msg__) EGL_LOG(msg__)
#define EGL_DEBUG_LOGF(...)  EGL_LOGF(__VA_ARGS__)
#else
#define EGL_DEBUG_LOG(msg__) \
    {                        \
    }
#define EGL_DEBUG_LOGF(...) \
    {                       \
    }
#endif

// low-level memory management
EGL_INLINE void mem_copy(void*, const void*, size_t num_bytes_);
EGL_INLINE void mem_move(void*, const void*, size_t num_bytes_);
EGL_INLINE void mem_zero(void*, size_t num_bytes_);
EGL_INLINE void mem_set(void*, unsigned char, size_t num_bytes_);

// min/max
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
template <typename T>
EGL_INLINE T min(T, T);
template <typename T>
EGL_INLINE T min(T, T, T);
template <typename T>
EGL_INLINE T min(T, T, T, T);
template <typename T>
EGL_INLINE T max(T, T);
template <typename T>
EGL_INLINE T max(T, T, T);
template <typename T>
EGL_INLINE T max(T, T, T, T);
template <typename T>
struct minmax_res {
    T min, max;
};
template <typename T>
EGL_INLINE minmax_res<T> minmax(T, T);
template <typename T>
EGL_INLINE minmax_res<T> minmax(T, T, T);
template <typename T>
EGL_INLINE minmax_res<T> minmax(T, T, T, T);

// misc
struct version_str {
    char str[8];
};
version_str bcd16_version_str(uint16_t version_);
//----------------------------------------------------------------------------

//============================================================================
// pre-compiler macros
//============================================================================
// stringify, e.g. EGL_STR(x) => "x"
#define EGL_STR(x__)    EGL_STR_DO(x__)
#define EGL_STR_DO(x__) #x__
// concatenation of 2-10 values, e.g. EGL_CAT3(a, b, c) => abc
#define EGL_CAT2(a__, b__)                                             EGL_CAT2_DO(a__, b__)
#define EGL_CAT2_DO(a__, b__)                                          a__##b__
#define EGL_CAT3(a__, b__, c__)                                        EGL_CAT3_DO(a__, b__, c__)
#define EGL_CAT3_DO(a__, b__, c__)                                     a__##b__##c__
#define EGL_CAT4(a__, b__, c__, d__)                                   EGL_CAT4_DO(a__, b__, c__, d__)
#define EGL_CAT4_DO(a__, b__, c__, d__)                                a__##b__##c__##d__
#define EGL_CAT5(a__, b__, c__, d__, e__)                              EGL_CAT5_DO(a__, b__, c__, d__, e__)
#define EGL_CAT5_DO(a__, b__, c__, d__, e__)                           a__##b__##c__##d__##e__
#define EGL_CAT6(a__, b__, c__, d__, e__, f__)                         EGL_CAT6_DO(a__, b__, c__, d__, e__, f__)
#define EGL_CAT6_DO(a__, b__, c__, d__, e__, f__)                      a__##b__##c__##d__##e__##f__
#define EGL_CAT7(a__, b__, c__, d__, e__, f__, g__)                    EGL_CAT7_DO(a__, b__, c__, d__, e__, f__, g__)
#define EGL_CAT7_DO(a__, b__, c__, d__, e__, f__, g__)                 a__##b__##c__##d__##e__##f__##g__
#define EGL_CAT8(a__, b__, c__, d__, e__, f__, g__, h__)               EGL_CAT8_DO(a__, b__, c__, d__, e__, f__, g__, h__)
#define EGL_CAT8_DO(a__, b__, c__, d__, e__, f__, g__, h__)            a__##b__##c__##d__##e__##f__##g__##h__
#define EGL_CAT9(a__, b__, c__, d__, e__, f__, g__, h__, i__)          EGL_CAT9_DO(a__, b__, c__, d__, e__, f__, g__, h__, i__)
#define EGL_CAT9_DO(a__, b__, c__, d__, e__, f__, g__, h__, i__)       a__##b__##c__##d__##e__##f__##g__##h__##i__
#define EGL_CAT10(a__, b__, c__, d__, e__, f__, g__, h__, i__, j__)    EGL_CAT10_DO(a__, b__, c__, d__, e__, f__, g__, h__, i__, j__)
#define EGL_CAT10_DO(a__, b__, c__, d__, e__, f__, g__, h__, i__, j__) a__##b__##c__##d__##e__##f__##g__##h__##i__##j__
//----------------------------------------------------------------------------

//============================================================================
// types
//============================================================================
// sized types
typedef uint16_t float16_t;
typedef uint16_t ufloat16_t;
typedef float    float32_t;
typedef float    ufloat32_t;
typedef double   float64_t;
typedef double   ufloat64_t;
// ranged types
typedef float  ufloat_t;    // [0, +inf]
typedef float  float1_t;    // [-1, 1]
typedef float  ufloat1_t;   // [0, 1]
typedef double udouble_t;   // [0, +inf]
typedef double double1_t;   // [-1, 1]
typedef double udouble1_t;  // [0, 1]
//----------------------------------------------------------------------------

//============================================================================
// asserts/warnings/errors
//============================================================================
// compile-time asserts (CTF=function and CTC=class scope asserts)
#define EGL_STATIC_ASSERT(e__)            static_assert(e__, #e__);
#define EGL_STATIC_ASSERT_MSG(e__, msg__) static_assert(e__, #msg__);
#define EGL_STATIC_ERROR(type__, msg__)   \
    struct cterror_##__LINE__ {           \
        char msg__ : sizeof(type__) == 0; \
    };
// assert/warning prefix
#if EGL_BUILDOP_ASSERT_LOC == 1
#define EGL_ASSERT_PREFIX(str__)                              \
    {                                                         \
        EGL_DEBUG_LOG(__FILE__ "(" EGL_STR(__LINE__) ") : "); \
        EGL_DEBUG_LOG(str__);                                 \
    }
#else
#define EGL_ASSERT_PREFIX(str__) \
    {                            \
        EGL_DEBUG_LOG(str__);    \
    }
#endif
#if EGL_BUILDOP_WARN_LOC == 1
#define EGL_WARN_PREFIX(str__)                                \
    {                                                         \
        EGL_DEBUG_LOG(__FILE__ "(" EGL_STR(__LINE__) ") : "); \
        EGL_DEBUG_LOG(str__);                                 \
    }
#else
#define EGL_WARN_PREFIX(str__) \
    {                          \
        EGL_DEBUG_LOG(str__);  \
    }
#endif
// run-time asserts
#if EGL_BUILDOP_ASSERTS == 1
#define EGL_ASSERT(e__)                                        \
    {                                                          \
        if (!(e__)) {                                          \
            EGL_ASSERT_PREFIX("assert failed : " #e__ "\r\n"); \
            EGL_ABORT();                                       \
        }                                                      \
    }
#define EGL_ASSERT_MSG(e__, ...)                   \
    {                                              \
        if (!(e__)) {                              \
            EGL_ASSERT_PREFIX("assert failed : "); \
            EGL_DEBUG_LOGF(__VA_ARGS__);           \
            EGL_ABORT();                           \
        }                                          \
    }
#else
#define EGL_ASSERT(e__) \
    {                   \
    }
#define EGL_ASSERT_MSG(e__, ...) \
    {                            \
    }
#endif
// pedantic asserts
#if EGL_BUILDOP_ASSERT_PED == 1
#define EGL_ASSERT_PEDANTIC(e__)          EGL_ASSERT(e__)
#define EGL_ASSERT_PEDANTIC_MSG(e__, ...) EGL_ASSERT_MSG(e__, __VA_ARGS__)
#else
#define EGL_ASSERT_PEDANTIC(e__) \
    {                            \
    }
#define EGL_ASSERT_PEDANTIC_MSG(e__, ...) \
    {                                     \
    }
#endif
// warnings
#if EGL_BUILDOP_WARNINGS == 1
#define EGL_WARN(msg__)                \
    {                                  \
        EGL_WARN_PREFIX("warning : "); \
        EGL_DEBUG_LOG(msg__);          \
    }
#define EGL_WARNF(...)                 \
    {                                  \
        EGL_WARN_PREFIX("warning : "); \
        EGL_DEBUG_LOGF(__VA_ARGS__);   \
    }
#else
#define EGL_WARN(msg__) \
    {                   \
    }
#define EGL_WARNF(...) \
    {                  \
    }
#endif
// errors
#if EGL_BUILDOP_ERRORS == 1
#define EGL_ERROR(msg__)               \
    {                                  \
        EGL_ASSERT_PREFIX("error : "); \
        EGL_DEBUG_LOG(msg__);          \
        EGL_ABORT();                   \
    }
#define EGL_ERRORF(...)                \
    {                                  \
        EGL_ASSERT_PREFIX("error : "); \
        EGL_DEBUG_LOGF(__VA_ARGS__);   \
        EGL_ABORT();                   \
    }
#define EGL_ERROR_NOT_IMPL()                                             \
    {                                                                    \
        EGL_ASSERT_PREFIX("error : Functionality not implemented.\r\n"); \
        EGL_ABORT();                                                     \
    }
#else
#define EGL_ERROR(msg__) \
    {                    \
    }
#define EGL_ERRORF(...) \
    {                   \
    }
#define EGL_ERROR_NOT_IMPL() \
    {                        \
    }
#endif
// verify messages
#if EGL_BUILDOP_VERIFY_MSG == 1
#define EGL_VERIFY(e__)                           \
    {                                             \
        if (!(e__)) {                             \
            EGL_ASSERT_PREFIX("check failed : "); \
            EGL_LOG(#e__);                        \
            EGL_LOG("\r\n");                      \
            EGL_ABORT();                          \
        }                                         \
    }
#define EGL_VERIFY_MSG(e__, ...)                  \
    {                                             \
        if (!(e__)) {                             \
            EGL_ASSERT_PREFIX("check failed : "); \
            EGL_LOGF(__VA_ARGS__);                \
            EGL_LOG("\r\n");                      \
            EGL_ABORT();                          \
        }                                         \
    }
#else
#define EGL_VERIFY(e__) \
    {                   \
        (e__);          \
    }
#define EGL_VERIFY_MSG(e__, ...) \
    {                            \
        (e__);                   \
    }
#endif
//----------------------------------------------------------------------------

//============================================================================
//============================================================================
// inline & template implementations
//============================================================================
//============================================================================

//============================================================================
// memory management & new/delete
//============================================================================
EGL_NAMESPACE_END
#if defined(ARDUINO) && !defined(CORE_TEENSY)  // Arduino (except Teensy) doesn't have "new" header to define placement new operator
EGL_INLINE void* operator new(size_t, void* ptr_) throw()
{
    return ptr_;
}
#else
#include <new>
#endif
EGL_NAMESPACE_BEGIN
#define EGL_MEM_ALLOC(bytes__)    malloc(bytes__)
#define EGL_NEW(type__)           new (malloc(sizeof(type__))) type__
#define EGL_MEM_FREE(ptr__)       free(ptr__)
#define EGL_DELETE(ptr__)         delete ptr__
#define EGL_PNEW(ptr__)           new (ptr__)
#define EGL_STACK_MALLOC(bytes__) EGL_ALLOCA(bytes__)
//----------------------------------------------------------------------------

//============================================================================
// low-level memory management
//============================================================================
EGL_INLINE void mem_copy(void* dst_, const void* src_, size_t num_bytes_)
{
    EGL_ASSERT_PEDANTIC(!num_bytes_ || (dst_ && src_));
    EGL_ASSERT_PEDANTIC(!num_bytes_ || dst_ == src_ || (dst_ < src_ && ((char*)dst_) + num_bytes_ <= src_) || (dst_ > src_ && ((char*)src_) + num_bytes_ <= dst_));
    ::memcpy(dst_, src_, num_bytes_);
}
//----

EGL_INLINE void mem_move(void* dst_, const void* src_, size_t num_bytes_)
{
    EGL_ASSERT_PEDANTIC(!num_bytes_ || (dst_ && src_));
    ::memmove(dst_, src_, num_bytes_);
}
//----

EGL_INLINE void mem_zero(void* p_, size_t num_bytes_)
{
    EGL_ASSERT_PEDANTIC(!num_bytes_ || p_);
    ::memset(p_, 0, num_bytes_);
}
//----

EGL_INLINE void mem_set(void* p_, unsigned char c_, size_t num_bytes_)
{
    EGL_ASSERT_PEDANTIC(!num_bytes_ || p_);
    ::memset(p_, c_, num_bytes_);
}
//----------------------------------------------------------------------------

//============================================================================
// min/max
//============================================================================
template <typename T>
EGL_INLINE T min(T v0_, T v1_)
{
    // return minimum of the two
    return v0_ < v1_ ? v0_ : v1_;
}
//----

template <typename T>
EGL_INLINE T min(T v0_, T v1_, T v2_)
{
    // return minimum of the three
    T v = v0_ < v1_ ? v0_ : v1_;
    return v < v2_ ? v : v2_;
}
//----

template <typename T>
EGL_INLINE T min(T v0_, T v1_, T v2_, T v3_)
{
    // return minimum of the four
    T v = v0_ < v1_ ? v0_ : v1_;
    v   = v < v2_ ? v : v2_;
    return v < v3_ ? v : v3_;
}
//----

template <typename T>
EGL_INLINE T max(T v0_, T v1_)
{
    // return maximum of the two
    return v1_ < v0_ ? v0_ : v1_;
}
//----

template <typename T>
EGL_INLINE T max(T v0_, T v1_, T v2_)
{
    // return maximum of the three
    T v = v1_ < v0_ ? v0_ : v1_;
    return v2_ < v ? v : v2_;
}
//----

template <typename T>
EGL_INLINE T max(T v0_, T v1_, T v2_, T v3_)
{
    // return maximum of the four
    T v = v1_ < v0_ ? v0_ : v1_;
    v   = v2_ < v ? v : v2_;
    return v3_ < v ? v : v3_;
}
//----

template <typename T>
EGL_INLINE minmax_res<T> minmax(T v0_, T v1_)
{
    minmax_res<T> res;
    if (v0_ < v1_) {
        res.min = v0_;
        res.max = v1_;
    } else {
        res.min = v1_;
        res.max = v0_;
    }
    return res;
}
//----

template <typename T>
EGL_INLINE minmax_res<T> minmax(T v0_, T v1_, T v2_)
{
    minmax_res<T> res;
    if (v0_ < v1_) {
        res.min = v0_;
        res.max = v1_;
    } else {
        res.min = v1_;
        res.max = v0_;
    }
    if (v2_ < res.min)
        res.min = v2_;
    else if ((res.max) < v2_)
        res.max = v2_;
    return res;
}
//----

template <typename T>
EGL_INLINE minmax_res<T> minmax(T v0_, T v1_, T v2_, T v3_)
{
    minmax_res<T> res;
    if (v0_ < v1_) {
        res.min = v0_;
        res.max = v1_;
    } else {
        res.min = v1_;
        res.max = v0_;
    }
    if (v2_ < res.min)
        res.min = v2_;
    else if ((res.max) < v2_)
        res.max = v2_;
    if (v3_ < res.min)
        res.min = v3_;
    else if ((res.max) < v3_)
        res.max = v3_;
    return res;
}
//----------------------------------------------------------------------------

//============================================================================
EGL_NAMESPACE_END
#endif
