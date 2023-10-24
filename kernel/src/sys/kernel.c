#include <kernel/kernel.h>
#include <kernel/limine.h>
#include <kernel/string.h>
#include <kernel/log.h>
#include <kernel/page_alloc.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};

void _start(void)
{
  if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count == 0)
  {
    panic("Expected a framebuffer");
  }

  if (memmap_request.response == NULL || memmap_request.response->entry_count == 0)
  {
    panic("Expected a memory map");
  }

  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
  LOG_INFO("Framebuffer width: %u, height: %u, pitch: %u", framebuffer->width, framebuffer->height, framebuffer->pitch);

  page_alloc_init(memmap_request.response, memmap_request.response->entry_count);

  // TODO: Theres an issue where palloc() given an unsuable address and qemu crashes

  // void *test = palloc();
  // while (test != NULL)
  {
    // test = palloc();
    // LOG_INFO("Test: 0x%p", test);
    // memset(test, 0xFFFFFFFF, PAGE_SIZE);
  }
  // pfree(test);

  LOG_INFO("Finished kernel routine");
  asm("cli");
  asm("hlt");
}
