#pragma once

#include "types.h"
#include "optional.h"
#include "memory/ct_memory.h"
#include "debug/ct_assert.h"

template <typename type>
struct darray
{
    allocator_i allocator;
    u32 size;
    u32 cap;
    type* elements;
};

template<typename type>
darray<type> darray_init(u32 capacity, optional<allocator_i*> allocator = optional<allocator_i*>::none());

template<typename type>
void darray_free(darray<type>* array);

template<typename type>
void darray_insert(darray<type>* array, int index, type&& value);

template<typename type>
void darray_ordered_remove(darray<type>* array, int index);

template<typename type>
void darray_unordered_remove(darray<type>* array, int index);

template<typename type>
void darray_set_len(darray<type>* array, int new_len);

template<typename type>
void darray_set_capacity(darray<type>* array, int new_cap);

#include "darray.inl"
