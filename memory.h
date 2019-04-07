#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

// dont forget this is dependent on testloader.asm
#define MMAPSZ 0x9000
#define MEMMAP 0x9004

const unsigned char FLAG_IGNORE_REGION = 1 << 0;
const unsigned char FLAG_NON_VOLATILE  = 1 << 1;

typedef enum {
    usable           = 1,
    reserved         = 2,
    acpi_reclaimable = 3,
    acpi_nvs         = 4,
    bad_memory       = 5
} region_type;

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_attributes;
} __attribute__((packed)) memory_region;

uint32_t* memory_map_size = (uint32_t*)MMAPSZ;
memory_region* memory_map = (memory_region*)MEMMAP;

// really fucking inefficient memcopy
void memcpy(unsigned char* src, unsigned char* dst, int count) {
    for(; count > 0; --count) *dst++ = *src++;
}

void memset(unsigned char* dst, unsigned char byte, int count) {
    for(; count > 0; --count) *dst++ = byte;
}

#endif