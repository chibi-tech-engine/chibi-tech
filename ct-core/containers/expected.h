#pragma once

#include "optional.h"
#include "debug/ct_assert.h"

template<typename type, typename error>
struct expected
{
    bool is_err;
    union
    {
        type value;
        error error_value;
    };

    static expected ok(type t) {
        return expected<type, error>{
            .is_err = false,
            .value = t,
        };
    }

    static expected err(error err) {
        return expected<type, error>{
            .is_err = true,
            .error_value = err,
        };
    }

    bool is_error() const { return is_err; }
    bool is_ok() const { return !is_err; }

    const type& get() const { CT_ASSERT_DEBUG(!is_err); return value; }
    type& get_mut() { CT_ASSERT_DEBUG(!is_err); return value; }

    const error& get_err() const { CT_ASSERT_DEBUG(is_err); return error_value; }

    type unwrap_assume_valid()   { CT_ASSERT_DEBUG(!is_err); return std::move(value); }
    type unwrap_or(type&& other) { CT_ASSERT_DEBUG(!is_err); return is_err ? std::forward(other) : std::move(value); }
    optional<type> unwrap()      { CT_ASSERT_DEBUG(!is_err); return is_err ? optional<type>::none() : optional<type>::some(std::move(value)); }
};
