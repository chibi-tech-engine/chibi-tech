#pragma once

#include "../../types.h"
#include "allocator.h"

typedef struct block_header block_header;
struct block_header {
    u32 block_size; // includes the block_size member. true allocation size is block_size - sizeof(u32)
    block_header* next_ptr;
};

typedef struct dynamic_allocator dynamic_allocator;
struct dynamic_allocator {
    // todo: explore alternatives than embedding blocks into the memory
    // todo: explore using a slab allocator
    block_header* free_blocks;
    u64 used_memory;
    u64 free_memory;
    u64 num_allocations;
    void* base_ptr;
    u64 total_size;
};

void dynamic_allocator_initialize(u64 total_size, u64* memory_requirement, void* memory, dynamic_allocator** self);
void dynamic_allocator_shutdown(dynamic_allocator* self);
void dynamic_allocator_report_allocations(dynamic_allocator* self);
void* dynamic_allocator_allocate(dynamic_allocator* self, u64 size);
void dynamic_allocator_free(dynamic_allocator* self, void* ptr);

allocator_i dynamic_allocator_get_interface(dynamic_allocator* self);
