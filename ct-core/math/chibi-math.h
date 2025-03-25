#pragma once

#include "../chibi-types.h"

constexpr bool cUseStdRand = true;

// Returns a random number between [0, 1]
auto randomF32() -> f32;

// Returns a random number between [min, max]
auto randomF32InRange(f32 min, f32 max) -> f32;
auto randomIntInRange(s32 min, s32 max) -> s32;

// --------------------------------------------------------------------
// NOTE(enlynn): This is a lazy attempt at addressing FP-error
//
// Source: https://isocpp.org/wiki/faq/newbie#floating-point-arith

// This function is not symmetrical - F32IsEqual(Left, Right) might not always equal F32IsEqual(Right, Left)
// TODO(enlynn): properly handle floating point error.
auto areFloatsEqual(const f32 left, const f32 right) -> bool;
auto isF32Zero(const f32 v) -> bool;

