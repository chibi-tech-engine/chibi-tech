#pragma once

#include "../chibi-types.h"

// todo: FP error handling

struct Float2
{
    union
    {
        struct
        {
            f32 x, y;
        };
        f32 d[2];
    };
};

struct Float3
{
    static auto zero() -> Float3 { return Float3(); }
    static auto one() -> Float3  { return Float3(1.0); }

    static auto random() -> Float3;
    static auto randomInRange(f32 min, f32 max) -> Float3;
    static auto randomInUnitSphere() -> Float3;
    static auto randomUnit() -> Float3;

    static auto sampleInSquare() -> Float3;
    // Finds a random vector pointing in the same direction as the normal.
    static auto sampleInHemisphere(const Float3& normal) -> Float3;

    explicit Float3() = default;
    explicit Float3(f32 v) : x(v), y(v), z(v) {}
    explicit Float3(f32 vx, f32 vy, f32 vz) : x(vx), y(vy), z(vz) {}

	Float3(const Float3&) = default;
    auto operator=(const Float3&) -> Float3& = default;

    auto operator==(const Float3& rhs) const -> bool;
    auto operator!=(const Float3& rhs) const -> bool;

    auto operator++() -> Float3&; // ++v
    auto operator++(int) -> Float3&; // v++

	auto operator--() -> Float3&; // --v
    auto operator--(int) -> Float3&; // v--

    auto operator+=(const Float3& rhs) -> Float3&;
    auto operator-=(const Float3& rhs) -> Float3&;
    auto operator*=(const Float3& rhs) -> Float3&;
    auto operator/=(const Float3& rhs) -> Float3&;
    auto operator/=(const f32& rhs) -> Float3&;

	[[nodiscard]] auto isZero() const -> bool;
    [[nodiscard]] auto dot(Float3 other) const -> f32;
    [[nodiscard]] auto cross(Float3 other) const -> Float3;
    [[nodiscard]] auto unit() const -> Float3;
    [[nodiscard]] auto magnitudeSq() const -> f32;
    [[nodiscard]] auto magnitude() const -> f32;
    
    // reflect the vector about a normal vector
    [[nodiscard]] auto reflect(const Float3& normal) const -> Float3;

    union
    {
        struct
        {
            f32 x, y, z;
        };

        struct
        {
            Float2 xy;
            f32 pad0;
        };

        struct
        {
            f32 pad1;
            Float2 yz;
        };

        f32 d[3];
    };
};

inline auto operator+(const Float3& lhs, const Float3& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs.x + rhs.x);
    result.y = (lhs.y + rhs.y);
    result.z = (lhs.z + rhs.z);

    return result;
}

inline auto operator+(const Float3& lhs, const f32& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs.x + rhs);
    result.y = (lhs.y + rhs);
    result.z = (lhs.z + rhs);

    return result;
}

inline auto operator+(const f32& lhs, const Float3& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs + rhs.x);
    result.y = (lhs + rhs.y);
    result.z = (lhs + rhs.z);

    return result;
}

inline auto operator*(const Float3& lhs, const Float3& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs.x * rhs.x);
    result.y = (lhs.y * rhs.y);
    result.z = (lhs.z * rhs.z);

    return result;
}

inline auto operator*(const Float3& lhs, const f32& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs.x * rhs);
    result.y = (lhs.y * rhs);
    result.z = (lhs.z * rhs);

    return result;
}

inline auto operator*(const f32& lhs, const Float3& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs * rhs.x);
    result.y = (lhs * rhs.y);
    result.z = (lhs * rhs.z);

    return result;
}

inline auto operator-(const Float3& lhs) -> Float3
{
    Float3 result{};

    result.x = -lhs.x;
    result.y = -lhs.y;
    result.z = -lhs.z;

    return result;
}

inline auto operator-(const Float3& lhs, const Float3& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs.x - rhs.x);
    result.y = (lhs.y - rhs.y);
    result.z = (lhs.z - rhs.z);

    return result;
}

inline auto operator-(const Float3& lhs, const f32& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs.x - rhs);
    result.y = (lhs.y - rhs);
    result.z = (lhs.z - rhs);

    return result;
}

inline auto operator/(const Float3& lhs, const Float3& rhs) -> Float3
{
    Float3 result{};

    result.x = (lhs.x / rhs.x);
    result.y = (lhs.y / rhs.y);
    result.z = (lhs.z / rhs.z);

    return result;
}

inline auto operator/(const Float3& lhs, const f32& rhs) -> Float3
{
    Float3 result{};

    if (rhs != 0.0f)
    {
        result.x = (lhs.x / rhs);
        result.y = (lhs.y / rhs);
        result.z = (lhs.z / rhs);
    }

    return result;
}
