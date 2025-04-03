#include "free_list.h"
#include "types.h"

struct free_list_node
{
    u32 block_size; // includes the block_size member. true allocation size is block_size - sizeof(u32)
    u32 ptr_offset;
    free_list_node* next_ptr;
};

struct free_list
{

    free_list_node* free_block_list{nullptr};
    free_list_node* free_nodes{nullptr};
    u64 max_blocks{0};
    u64 used_blocks{0};
    u64 free_blocks{0};
};

static constexpr u64 c_min_block_allocation_size = sizeof(void*);
static constexpr u64 c_block_header_size = sizeof(u32);
static constexpr u64 c_min_allocation_size = c_block_header_size + c_min_block_allocation_size;

CT_API bool
free_list_initialize(u64 block_size, void* block_ptr, free_list* out_list, u64* required_size)
{
    const u64 num_elements = block_size / sizeof(free_list_node);

    return true;
}

CT_API void
free_list_release()
{
}

/*

u32 allocate(u32 allocation_size) {
    u64 aligned_offset = align(allocation_size);

    return ptr_offset;
}

void release(u32 ptr_offset) {

}

*/
