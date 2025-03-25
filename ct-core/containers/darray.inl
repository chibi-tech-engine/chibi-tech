#include "containers/optional.h"
#include "darray.h" //remove when done

template<typename type>
darray<type> darray_init(u32 capacity, optional<allocator_i*> optional_allocator)
{
    darray<type> result{};

    if (!optional_allocator.has_value())
    { //get heap allocator
    }
    else
    {
        result.allocator = *optional_allocator.get();
    }

    return result;
}

template<typename type>
void darray_free(darray<type>* array)
{
}

template<typename type>
void darray_insert(darray<type>* array, int index, type&& value)
{
}

template<typename type>
void darray_ordered_remove(darray<type>* array, int index)
{
}

template<typename type>
void darray_unordered_remove(darray<type>* array, int index)
{
}

template<typename type>
void darray_set_len(void **ptr, int new_len)
{
}

template<typename type>
void darray_set_capacity(void **ptr, int new_cap)
{
}
