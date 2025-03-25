#include "chibi-float3.h"
#include "chibi-math.h"

#include <cmath>

auto Float3::isZero() const -> bool { return isF32Zero(x) && isF32Zero(y) && isF32Zero(z); }
auto Float3::random() -> Float3 { return Float3(randomF32(), randomF32(), randomF32()); }
auto Float3::magnitudeSq() const -> f32 { return x * x + y * y + z * z; }
auto Float3::dot(const Float3 other) const -> f32 { return x * other.x + y * other.y + z * other.z; }
auto Float3::unit() const -> Float3 { return *this / magnitude(); }
auto Float3::operator!=(const Float3& rhs) const -> bool { return !(*this == rhs); }
auto Float3::magnitude() const -> f32 { return std::sqrt(magnitudeSq()); }

auto Float3::randomInRange(const f32 min, const f32 max) -> Float3
{
    return Float3(randomF32InRange(min, max), randomF32InRange(min, max), randomF32InRange(min, max));
}

auto Float3::randomInUnitSphere() -> Float3
{
    // Use "rejection" as a way of determining if the random point is within the sphere,
    // If the Length is outside the unit sphere, then it is an invalid point.

    // TODO: raytracing in a weekend mentions using analytical methods instead of using rejection

    u32 maxIterations = 100;
    for (u32 i = 0; i < maxIterations; ++i)
    {
        if (Float3 p = randomInRange(-1.0f, 1.0f); p.magnitudeSq() < 1.0f)
        {
            return p;
        }
    }

    return Float3::zero();
}

auto Float3::randomUnit() -> Float3
{
    const Float3 unitSphere = randomInUnitSphere();
    return unitSphere.unit();
}

auto Float3::sampleInSquare() -> Float3
{
    // Returns a vector to a random point in the [-0.5, -0.5] -> [0.5, 0.5] unit square
    return Float3(randomF32() - 0.5f, randomF32() - 0.5f, 0.0);
}

auto Float3::sampleInHemisphere(const Float3& normal) -> Float3
{
    // cos(theta) = Normal . random_vector()
    // cos(theta) > 0  -> vector is in the same hemisphere
    // cos(theta) <= 0 -> vector is tangent or on the other hemisphere (points against the normal)
    Float3 onUnitSphere = Float3::randomUnit();
    if (onUnitSphere.dot(normal) > 0.0f)
    {
        return onUnitSphere;
    }
    else
    {
        return -onUnitSphere;
    }
}

auto Float3::reflect(const Float3& normal) const -> Float3
{
    // (this) incoming_ray -> ray intersecting the surface
    //        normal       -> normal of the surface (assumed unit length)
    //
    // ir   n  or
    //  \   |   /|
    //   \  |  / | pr
    //    \ | /  |
    //   ---------
    //      \    |
    //       \   |
    //        \  |
    //      ir \ | pr
    //
    // ir - incoming_ray              (direction - downwards)
    // n  - normal                    (direction - upwards)
    // or - outgoing (reflected) ray  (direction - upwards)
    // pr - projected ray             (direction - upwards)
    //
    // Project the incoming Ray onto the normal vector:
    //   (incoming_ray . normal) * normal
    //
    // Using some quick maths:
    //   ir - 2*pr = or
    //   incoming_ray - 2 * ((incoming_ray . normal) * normal)
    return *this - (2.0f * dot(normal) * normal);
}

auto Float3::operator==(const Float3& rhs) const -> bool { return x == rhs.x && y == rhs.y && z == rhs.z; }

auto Float3::operator++() -> Float3&
{
    x += 1;
    y += 1;
    z += 1;
    return *this;
}

auto Float3::operator++([[maybe_unused]] int) -> Float3&
{
    x += 1;
    y += 1;
    z += 1;
    return *this;
}

auto Float3::operator--() -> Float3&
{
    x -= 1;
    y -= 1;
    z -= 1;
    return *this;
}

auto Float3::operator--([[maybe_unused]] int) -> Float3&
{
    x -= 1;
    y -= 1;
    z -= 1;
    return *this;
}

auto Float3::cross(const Float3 other) const -> Float3
{
    Float3 result;

    result.x = (y * other.z) - (z * other.y);
    result.y = (z * other.x) - (x * other.z);
    result.z = (x * other.y) - (y * other.x);

    return result;
}

auto Float3::operator+=(const Float3& rhs) -> Float3&
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

auto Float3::operator-=(const Float3& rhs) -> Float3&
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

auto Float3::operator*=(const Float3& rhs) -> Float3&
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

auto Float3::operator/=(const Float3& rhs) -> Float3&
{
    x = (rhs.x != 0.0f) ? x / rhs.x : 0.0f;
    y = (rhs.y != 0.0f) ? y / rhs.y : 0.0f;
    z = (rhs.z != 0.0f) ? z / rhs.z : 0.0f;
    return *this;
}

auto Float3::operator/=(const f32& rhs) -> Float3&
{
    if (rhs != 0.0f)
    {
        x = x / rhs;
        y = y / rhs;
        z = z / rhs;
    }
    else
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    return *this;
}
