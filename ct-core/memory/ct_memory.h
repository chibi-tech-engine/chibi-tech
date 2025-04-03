#pragma once

#include "types.h"
#include "allocators/allocator.h"

enum class allocator_type {
    dynamic,
    linear,
};

struct memory_system_config
{
    u64 total_alloc_size;
};

CT_API bool memory_system_initialize(memory_system_config config);
CT_API void memory_system_shutdown();

CT_API allocator_i ct_get_heap_allocator();
// if the parent allocator is null, then the global heap allocator is used.
CT_API allocator_i ct_create_allocator(allocator_i* parent_allocator, u64 size, memory::tag tag);

CT_API void* ct_allocate(u64 size, memory::tag tag);
CT_API void* ct_allocate_aligned(u64 size, u16 alignment, memory::tag tag);
CT_API void ct_free(void* ptr, memory::tag tag);

CT_API void* ct_reallocate(void* block, u64 old_size, u64 new_size, memory::tag tag);
CT_API void* ct_reallocate_aligned(void* block, u64 old_size, u64 new_size, u16 alignment, memory::tag tag);

CT_API void ct_zero_memory(void* block, u8 value, u64 size);
CT_API void ct_zero_memory(void* block, u64 size);
CT_API void ct_copy_memory(void* dest, const void* source, u64 size);

CT_API u32 pack_u8_into_u32(u8 x, u8 y, u8 z, u8 w);
CT_API void unpack_u8_from_u32(u32 n, u8& x, u8& y, u8& z, u8& w);

template<typename Type>
Type* ct_alloc_type(memory::tag tag)
{
    Type* result = (Type*)ct_allocate(sizeof(Type), tag);
}

template<typename Type>
void ct_free_type(Type* ptr, memory::tag tag)
{
    ct_free((void*)ptr, tag);
}

template<typename Type>
span<Type> ct_alloc_array(u64 count, memory::tag tag)
{
    Type* result = (Type*)ct_allocate(sizeof(Type) * count, tag);
    return {
        .list = result,
        .count = count,
    };
}

template<typename Type>
void ct_free_array(span<Type>& array, memory::tag tag)
{
    ct_free((void*)array, tag);
}

template<typename Type>
void ct_zero_type(Type* ptr)
{
    ct_zero_memory((void*)ptr, sizeof(Type));
}

template<typename Type>
void ct_zero_array(span<Type>& ptr)
{
    ct_zero_memory((void*)ptr.list, ptr.count * sizeof(Type));
}

template<typename Type>
void ct_copy_type(Type* dst, Type* src)
{
    ct_copy_memory((void*)dst, (void*)src, sizeof(Type));
}

template<typename Type>
Type* ct_clone_type(Type* src, memory::tag tag)
{
    Type* clone = ct_alloc_type<Type>(tag);
    ct_copy_type(clone, src);
    return clone;
}

template<typename Type>
void ct_copy_array(span<Type>& dst_array, span<Type>& src_array, u64 count, u64 dst_offset = 0, u64 src_offset = 0)
{
    ct_copy_memory((void*)(dst_array.list + dst_offset), (void*)(src_array.list + src_offset), count * sizeof(Type));
}

template<typename Type>
span<Type> ct_clone_array(span<Type>& src, memory::tag tag)
{
    span<Type> clone = ct_alloc_array(src.count, tag);
    ct_copy_array(clone, src);
    return clone;
}
