#include <kernel/port.h>

uint8_t port_byte_in(uint16_t port)
{
  uint8_t result;
  asm volatile("in %%dx, %%al" : "=a"(result) : "d"(port));
  return result;
}

void port_byte_out(uint16_t port, uint8_t data)
{
  asm volatile("out %%al, %%dx" : : "a"(data), "d"(port));
}

uint16_t port_word_in(uint16_t port)
{
  uint16_t result;
  asm volatile("in %%dx, %%ax" : "=a"(result) : "d"(port));
  return result;
}

void port_word_out(uint16_t port, uint16_t data)
{
  asm volatile("out %%ax, %%dx" : : "a"(data), "d"(port));
}

uint32_t port_dword_in(uint16_t port)
{
  uint32_t result;
  asm volatile("inl %%dx, %%eax" : "=a"(result) : "d"(port));
  return result;
}

void port_dword_out(uint16_t port, uint32_t data)
{
  asm volatile("outl %%eax, %%dx" : : "a"(data), "d"(port));
}
