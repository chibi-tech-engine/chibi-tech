#include "chibi-math.h"

#include <cassert>
#include <random>

auto randomF32() -> f32
{
    if constexpr (cUseStdRand)
    {
        thread_local static std::mt19937 generator(8340978);
        thread_local static std::uniform_real_distribution distribution(0.0f, 1.0f);
        return distribution(generator);
    }
    else
    {
        // todo:
        assert(false);
        return 0.0f;
    }
}

auto randomF32InRange(const f32 min, const f32 max) -> f32 { return min + (max - min) * randomF32(); }

auto randomIntInRange(const s32 min, const s32 max) -> s32
{
    thread_local static std::mt19937 generator(89258);
    thread_local static std::uniform_int_distribution distribution(min, max);
    return distribution(generator);
}

auto areFloatsEqual(const f32 left, const f32 right) -> bool
{ // TODO(enlynn):  max - min test to make sure there aren't perf implication...
    return std::abs(left - right) <= (std::numeric_limits<f32>::epsilon() * std::abs(left));
}

auto isF32Zero(const f32 v) -> bool { return areFloatsEqual(v, 0.0f); }
