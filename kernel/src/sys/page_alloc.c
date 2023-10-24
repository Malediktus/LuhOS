#include <kernel/page_alloc.h>
#include <kernel/string.h>
#include <kernel/log.h>

// TODO: Use memory mapping to prevent fragmentation
// TODO: Implement a buddy allocator

static uint64_t *bitmap = NULL;
static uint64_t num_pages = 0;

static uint64_t last_index = 0;

static void bit_set(uint64_t *bitmap, uint64_t index)
{
  uint64_t array_index = index / 64;
  uint64_t bit_offset = index % 64;
  bitmap[array_index] |= (1ULL << bit_offset);
}

static void bit_clear(uint64_t *bitmap, uint64_t index)
{
  uint64_t array_index = index / 64;
  uint64_t bit_offset = index % 64;
  bitmap[array_index] &= ~(1ULL << bit_offset);
}

static bool bit_get(const uint64_t *bitmap, uint64_t index)
{
  uint64_t array_index = index / 64;
  uint64_t bit_offset = index % 64;
  return (bitmap[array_index] & (1ULL << bit_offset)) != 0;
}

void page_alloc_init(struct limine_memmap_response *mmap_entries, uint64_t num_mmap_entries)
{
  uint64_t max_addr = 0;

  for (uint64_t i = 0; i < num_mmap_entries; i++)
  {
    uint64_t end_addr = mmap_entries->entries[i]->base + mmap_entries->entries[i]->length;
    if (end_addr > max_addr)
      max_addr = end_addr;
  }

  num_pages = max_addr / PAGE_SIZE;
  uint64_t bitmap_size = (num_pages + 8 - 1) / 8;

  for (uint64_t i = 0; i < num_mmap_entries; i++)
  {
    if (mmap_entries->entries[i]->type != LIMINE_MEMMAP_USABLE)
      continue;

    if (mmap_entries->entries[i]->length < bitmap_size)
      continue;

    bitmap = (uint64_t *)mmap_entries->entries[i]->base;
    mmap_entries->entries[i]->base += bitmap_size;
    mmap_entries->entries[i]->length -= bitmap_size;
  }

  if (bitmap == NULL)
  {
    // TODO: Kernel panic
    LOG_ERROR("No space for bitmap found");
    return;
  }

  memset(bitmap, 0xFFFFFFFF, bitmap_size); // 0: Free, 1: Reserved or used

  for (uint64_t i = 0; i < num_mmap_entries; i++)
  {
    if (mmap_entries->entries[i]->type != LIMINE_MEMMAP_USABLE)
      continue;

    uint64_t start_page = (mmap_entries->entries[i]->base + PAGE_SIZE - 1) / PAGE_SIZE;

    for (uint64_t j = 0; j < mmap_entries->entries[i]->length / PAGE_SIZE; j++)
    {
      uint64_t page_index = start_page + j;
      bit_clear(bitmap, page_index);
    }
  }
}

void *palloc()
{
  for (uint64_t i = last_index; i < num_pages; i++)
  {
    if (bit_get(bitmap, i))
      continue;

    bit_set(bitmap, i);
    last_index = i;

    return (void *)(i * PAGE_SIZE);
  }

  return NULL;
}

void pfree(void *mem)
{
  // TODO: Assert if memory is not allocated
  uint64_t index = (uintptr_t)mem / PAGE_SIZE;
  bit_clear(bitmap, index);
  last_index = index;
}
