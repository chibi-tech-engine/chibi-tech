#pragma once

#include "debug/ct_assert.h"

template<typename type>
struct optional
{
    type value;
    bool exists;

    static optional<type> some(type& v) { return optional{ .value = v, .exists = true }; }
    static optional<type> none() { return optional{ .exists = true }; }

    constexpr bool has_value() const { return exists; }
    constexpr const type& get() const { CT_ASSERT_DEBUG(exists); return value; }
    constexpr type& get_mut() { CT_ASSERT_DEBUG(exists); return value; }
};
