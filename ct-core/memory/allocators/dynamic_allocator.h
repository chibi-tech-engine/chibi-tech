#pragma once

#include "types.h"
#include "allocator.h"

struct dynamic_allocator;

void dynamic_allocator_initialize(u64 total_size, u64* memory_requirement, void* memory, dynamic_allocator** self);
void dynamic_allocator_shutdown(dynamic_allocator* self);
void dynamic_allocator_report_allocations(dynamic_allocator* self);
void* dynamic_allocator_allocate(dynamic_allocator* self, u64 size);
void dynamic_allocator_free(dynamic_allocator* self, void* ptr);

allocator_i dynamic_allocator_get_interface(dynamic_allocator* self);
