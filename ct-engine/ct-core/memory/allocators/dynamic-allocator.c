#include "dynamic-allocator.h"
#include "pch/pch.h"

#include "debug/ct-assert.h"

static constexpr u64 c_min_block_allocation_size = 2 * sizeof(void*);
static constexpr u64 c_block_header_size = sizeof(u32);
static constexpr u64 c_min_allocation_size = c_block_header_size + c_min_block_allocation_size;

CT_INLINE u32
get_user_allocation_size(block_header* self)
{
    return self->block_size - c_block_header_size;
}

CT_INLINE u32
get_true_allocation_size(u64 user_size)
{
    user_size = CT_MAX(user_size, c_min_block_allocation_size);
    return user_size + c_block_header_size;
}

CT_INLINE block_header*
user_ptr_to_block_ptr(void* user_ptr)
{ //@assume: user_ptr has already been validated
    return (block_header*)((u8*)user_ptr - c_block_header_size);
}

CT_INLINE void*
block_ptr_to_user_ptr(block_header* header)
{ //@assume: user_ptr has already been validated
    return (void*)((u8*)header + c_block_header_size);
}

// Merges two adjacent blocks of memory into the left block.
// If the memory is not adjacent, false is returned.
//
// | Left | ... | Right | <- blocks have a gap, no merge, return false
// | Left | Right | <- blocks are adjacent, merge: | Left+Right |
//
static bool
merge_block_left(block_header* left_header, block_header* right_header)
{
    if ((u8*)left_header + left_header->block_size == (u8*)right_header) {
        // consume the right block in its entirety
        left_header->block_size += right_header->block_size;
        // right header is no longer valid
        right_header->next_ptr = nullptr;
        right_header->block_size = 0;
        return true;
    }

    return false;
}

void
dynamic_allocator_initialize(u64 total_size, u64* memory_requirement, void* memory, dynamic_allocator** out)
{
    CT_ASSERT(total_size > 0);
    CT_ASSERT(memory_requirement); // must provide a valid pointer

    *memory_requirement = total_size + sizeof(dynamic_allocator);

    if (!memory) { // just fetching the system size, early out
        return;
    }

    CT_ASSERT(out);

    *out = (dynamic_allocator*)memory;
    dynamic_allocator* self = *out;
    self->used_memory = 0;
    self->free_memory = total_size;
    self->num_allocations = 0;
    self->base_ptr = (void*)(self + 1);
    self->total_size = total_size;
    self->free_blocks = user_ptr_to_block_ptr(self->base_ptr);
    self->free_blocks->next_ptr = nullptr;
    self->free_blocks->block_size = total_size;
}

void
dynamic_allocator_reset(dynamic_allocator* self)
{
    self->num_allocations = 0;
    self->free_memory = self->total_size;
    self->used_memory = 0;
    self->free_blocks = user_ptr_to_block_ptr(self->base_ptr);
    self->free_blocks->next_ptr = nullptr;
    self->free_blocks->block_size = self->total_size;
}

void
dynamic_allocator_report_allocations(dynamic_allocator* self)
{ // todo:
}

void
dynamic_allocator_shutdown(dynamic_allocator* self)
{
    dynamic_allocator_report_allocations(self);

    self->used_memory = 0;
    self->free_memory = 0;
    self->num_allocations = 0;
    self->base_ptr = nullptr;
    self->total_size = 0;
    self->free_blocks = nullptr;
}

void*
dynamic_allocator_allocate(dynamic_allocator* self, u64 size)
{
    const u64 real_size = get_true_allocation_size(size);

    if (real_size > self->free_memory) { // not enough memory
        return nullptr;
    }

    if (!self->free_blocks) { // there is enough memory, but allocations are too fragmented.
        return nullptr;
    }

    // find a free block large enough to fit the allocation in, this is first fit search
    block_header* prev_block = nullptr;
    block_header* iter = self->free_blocks;
    while (iter != nullptr) {
        if (real_size <= iter->block_size) { // found a block the allocation will fit into
            break;
        }

        prev_block = iter;
        iter = iter->next_ptr;
    }

    if (!iter) { // couldn't find a block of memory to satisfy the user's request
        return nullptr;
    }

    // split the block if possible, don't want to return a large allocation if the request was much smaller
    {
        u64 leftover_size = iter->block_size - real_size;
        if (leftover_size >= c_min_allocation_size) { // the leftover space is large enough to split the block.
            block_header* split_block = iter;
            block_header* new_block = (block_header*)((uintptr_t)iter + real_size);

            // update the old block first to avoid corrupting the original block's pointers
            new_block->block_size = leftover_size;
            new_block->next_ptr = split_block->next_ptr;

            // now we can safely update the split block
            split_block->next_ptr = new_block;
            split_block->block_size = real_size;
        }
    }

    // Remove the block from the free list
    if (iter == self->free_blocks) { // this is just the front of the list
        self->free_blocks = iter->next_ptr;
    } else { // we're in the middle or end of the list
        prev_block->next_ptr = iter->next_ptr;
    }

    // make absolutely sure this block can't accidentally point back to the free list
    iter->next_ptr = nullptr;

    self->used_memory += iter->block_size;
    self->free_memory -= iter->block_size;
    self->num_allocations += 1;

    CT_ASSERT(self->used_memory <= self->total_size); // did we somehow alloc too much memory?

    return block_ptr_to_user_ptr(iter);
}

void
dynamic_allocator_free(dynamic_allocator* self, void* ptr)
{
    // make sure we own the allocation.
    CT_ASSERT((uintptr_t)ptr >= (uintptr_t)self->base_ptr && (uintptr_t)ptr < ((uintptr_t)self->base_ptr + self->total_size));

    block_header* block = user_ptr_to_block_ptr(ptr);
    block->next_ptr = nullptr;

    // update tracking metrics
    self->free_memory += block->block_size;
    CT_ASSERT(self->free_memory <= block->block_size); // was there a corruption and the size value is invalid?
    CT_ASSERT(self->used_memory >= block->block_size); // are we about to free too much memory?
    self->used_memory -= block->block_size;
    self->num_allocations -= 1;

    // is this block at the front of the list?
    if (!self->free_blocks) {
        self->free_blocks = block;
    } else if (block < self->free_blocks) {
        block->next_ptr = self->free_blocks;
        self->free_blocks = block;
    } else {
        // free list is sorted by increasing address. find the block before the one to add
        block_header* iter = self->free_blocks;
        while (iter != nullptr && iter->next_ptr != nullptr) {
            if ((uintptr_t)block < (uintptr_t)iter->next_ptr) { // found a block the allocation will fit into
                break;
            }
            iter = iter->next_ptr;
        }

        // insert the new block into the free list
        block_header* prev_block = iter;
        block_header* next_block = iter->next_ptr;

        // verify the user ptr does not overlap the previous and next_ptr blocks
        CT_ASSERT((uintptr_t)block >= (uintptr_t)prev_block + prev_block->block_size);
        CT_ASSERT((uintptr_t)next_block >= (uintptr_t)block + block->block_size);

        // can the user pointer be merged to the previous block?
        if (merge_block_left(prev_block, block)) {
            if (next_block && merge_block_left(prev_block, next_block)) { // could we, perhaps, merge one more time?
                prev_block->next_ptr = next_block->next_ptr;
            }
        } else if (next_block &&
                   merge_block_left(block, next_block)) { // ok, well, can we merge the next_ptr block into the user block?
            prev_block->next_ptr = block;
            block->next_ptr = next_block->next_ptr;
        } else { // can't merge, so just insert the block
            prev_block->next_ptr = block;
            if (next_block) {
                block->next_ptr = next_block;
            }
        }
    }
}

static void*
allocate_helper(allocator_i* allocator, u64 size)
{
    CT_ASSERT(allocator->internal_state);
    return dynamic_allocator_allocate((dynamic_allocator*)allocator->internal_state, size);
}

static void
free_helper(allocator_i* allocator, void* block)
{
    CT_ASSERT(allocator->internal_state);
    dynamic_allocator_free((dynamic_allocator*)allocator->internal_state, block);
}

static void
free_all_helper(allocator_i* allocator)
{
    CT_ASSERT(allocator->internal_state);
    dynamic_allocator_reset((dynamic_allocator*)allocator->internal_state);
}

allocator_i
dynamic_allocator_get_interface(dynamic_allocator* self)
{
    allocator_i result;
    result.internal_state = (void*)self;
    result.allocate = allocate_helper;
    result.free = free_helper;
    result.free_all = free_all_helper;
    return result;
}
