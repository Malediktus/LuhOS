#ifndef PAGE_ALLOC_H
#define PAGE_ALLOC_H

#include <kernel/types.h>
#include <kernel/limine.h>

#define PAGE_SIZE 4096 // 4 KiB

void page_alloc_init(struct limine_memmap_response *mmap_entries, uint64_t num_mmap_entries);

void *palloc();
void pfree(void *mem);

#endif // PAGE_ALLOC_H