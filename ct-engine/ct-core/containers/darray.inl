#include "containers/optional.h"
#include "darray.h" //remove when done

template<typename type>
darray<type> darray_init(u32 capacity, optional<allocator_i*> optional_allocator)
{
    darray<type> result{};

    if (!optional_allocator.has_value())
    { //get heap allocator
        result.allocator = ct_get_heap_allocator();
    }
    else
    {
        CT_ASSERT(false); //todo: actually support
        result.allocator = *optional_allocator.get();
    }

    result.size = 0;
    result.cap = (capacity == 0) ? 10 : capacity;
    result.elements = (type*)result.allocator.allocate(&result.allocator, result.cap * sizeof(type));

    return result;
}

template<typename type>
void darray_free(darray<type>* array)
{
    result.allocator.free(&result.allocator, result.elements);
    result.elements = nullptr;
    result.size = 0;
    result.cap = 0;
    result.allocator = {};
}

template<typename type> static void
darray_grow(darray<type>* array, int new_cap)
{
    const int cap = array->cap;
    if (new_cap <= cap)
    {
        return;
    }

    type* current_base = array->elements;
    const int len = array->size;

    type* new_array = (type*)array->allocator.allocate(&array->allocator, new_cap * sizeof(type));
    ct_copy_array(
        {current_base, len}, //span<Type>& dst_array,
        {new_array, len}, //span<Type>& src_array,
        len);

    array->elements = new_array;
    array->cap = new_cap;

    array->allocator.free(&array->allocator, current_base);
}

template<typename type> void
darray_insert(darray<type>* array, int index, type&& value)
{
    const int len = array->size;
    const int cap = array->cap;
    assert(index <= len);

    if (len + 1 > cap) darray_grow(ptr, cap * 2);

    const type* endp = array->elements + index;
    for (type* iter = array->elements + len; iter > endp; --iter)
    {
        ct_copy_type(iter, iter - 1);
    }

    ct_copy_type(array->elements + index, &value);
    array->size = len + 1;
}

template<typename type>
void darray_ordered_remove(darray<type>* array, int index)
{
    if (array->size == 0 || index >= array->size)
    {
        return;
    }

    const type* startp = array->elements + (index + 1);
    const type* endp = array->elements + (array->size);

    u8 *iter = (u8*)ptr;
    for (type* iter = startp; iter < endp; ++del_idx)
    {
        ct_copy_type(iter - 1, iter);
    }

    array->size -= 1;
}

template<typename type>
void darray_unordered_remove(darray<type>* array, int index)
{
    if (header->count == 0 || index >= header->count)
    {
        return;
    }

    if (header->count > 1)
    {
        array->size -= 1;
        ct_copy_type(array->elements + index, array->elements + array->size);
    }
}

template<typename type>
void darray_set_len(darray<type>* array, int new_len)
{
    darray_grow(array, new_len);
    array->size = new_len;
}

template<typename type>
void darray_set_capacity(darray<type>* array, int new_cap)
{
    ct_array_grow(ptr, new_cap);
    array->cap = new_cap;
    array->size = CT_MIN(array->size, array->cap);
}
