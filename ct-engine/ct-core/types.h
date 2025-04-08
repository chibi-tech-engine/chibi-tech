#pragma once

#include "defines.h"

#ifndef NULL
#    define NULL (void*)0
#endif

#define nullptr NULL

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef union u128 u128;
union u128 {
    struct {
        u64 upper, lower;
    };
    u32 bits32[4];
    u64 bits64[2];
};

static_assert(sizeof(s8) == 1, "Expected s8 to be 1 byte.");
static_assert(sizeof(s16) == 2, "Expected s16 to be 2 bytes.");
static_assert(sizeof(s32) == 4, "Expected s32 to be 4 bytes.");
static_assert(sizeof(s64) == 8, "Expected s64 to be 8 bytes.");

static_assert(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
static_assert(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
static_assert(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
static_assert(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

static_assert(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
static_assert(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

constexpr u64 c_u64_max = 18446744073709551615UL;
constexpr u32 c_u32_max = 4294967295U;
constexpr u16 c_u16_max = 65535U;
constexpr u8 c_u8_max = 255U;
constexpr u64 c_u64_min = 0UL;
constexpr u32 c_u32_min = 0U;
constexpr u16 c_u16_min = 0U;
constexpr u8 c_u8_min = 0U;

constexpr s8 c_s8_max = 127;
constexpr s16 c_s16_max = 32767;
constexpr s32 c_i32_max = 2147483647;
constexpr s64 c_s64_max = 9223372036854775807L;
constexpr s8 c_s8_min = (-c_s8_max - 1);
constexpr s16 c_s16_min = (-c_s16_max - 1);
constexpr s32 c_s32_min = (-c_i32_max - 1);
constexpr s64 c_s64_min = (-c_s64_max - 1);

#define _KB(v) (v) * 1024
#define _MB(v) _KB(v) * 1024
#define _GB(v) _MB(v) * 1024

constexpr u64 _64KB = _KB(64);
constexpr u64 _1MB = _MB(1);
constexpr u64 _2MB = _MB(2);
constexpr u64 _4MB = _MB(4);
constexpr u64 _8MB = _MB(8);
constexpr u64 _16MB = _MB(16);
constexpr u64 _32MB = _MB(32);
constexpr u64 _64MB = _MB(64);
constexpr u64 _128MB = _MB(128);
constexpr u64 _256MB = _MB(256);
constexpr u64 _512MB = _MB(512);
constexpr u64 _1GB = _GB(1);

typedef struct range64 range64;
struct range64 {
    u64 offset;
    u64 size;
};

typedef struct range32 range32;
struct range32 {
    u32 offset;
    u32 size;
};

CT_INLINE u64
get_aligned(u64 operand, u64 granularity)
{
    return ((operand + (granularity - 1)) & ~(granularity - 1));
}

CT_INLINE range64
get_aligned_range(u64 offset, u64 size, u64 granularity)
{
    return (range64){get_aligned(offset, granularity), get_aligned(size, granularity)};
}

#define CT_CLAMP(value, min, max) ((value <= min) ? min : (value >= max) ? max : value)
#define CT_MIN(x, y) (x < y ? x : y)
#define CT_MAX(x, y) (x > y ? x : y)

// Indicates if the provided flag is set in the given flags int.
#define FLAG_GET(flags, flag) ((flags & flag) == flag)
#define FLAG_SET(flags, flag, enabled) (flags = (enabled ? (flags | flag) : (flags & ~flag)))

#define make_version(major, minor, patch) (major << 24) | (minor << 16) | (patch << 8)
