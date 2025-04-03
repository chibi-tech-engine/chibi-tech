#pragma once

#include "types.h"
#include "memory/ct_memory.h"
#include "optional.h"

struct hashtable
{
    allocator_i allocator;
    u32  count;
    u32  capacity;
    u64 *key_table;
    u32 *index_table;
};

CT_API u32  hash_u32(const void *key, const int key_len);
CT_API u64  hash_u64(const void *key, const int key_len);
CT_API u128 hash_u128(const void *key, const int key_len);

CT_API size_t hash_combine_size_t(const size_t seed, const size_t value);
CT_API u32    hash_combine_u32(const u32 seed, const u32 value);
CT_API u64    hash_combine_u64(const u64 seed, const u64 value);

CT_API void hashtable_init(hashtable *self, u32 initial_capacity, optional<allocator_i*> optional_allocator);
CT_API void hashtable_deinit(hashtable *hashtable);
CT_API bool hashtable_insert(hashtable *hashtable, u64 key, u32 value_index);
CT_API bool hashtable_get(hashtable *hashtable, u64 key, u32 *value_index);
