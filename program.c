#include "print.h"
#include "memory.h"

//TODO: the vidchar get/set's are not working u skid
//TODO: printnl() does not scroll
//TODO: colors are messed up for a moment when scrolling

void print_memmap();
void test_prints();
void test_memory();

void main() {
    init_screen();
    
    print_memmap();
}

void print_memmap() {
    printstr_col("{{ memory map }}\n", light_red, blue);
    printstr("base             | length           | attrib   | type\n");
    for(int i = 0; i < *memory_map_size; i++) {
        printhex8(memory_map[i].base);
        printstr(" | ");

        printhex8(memory_map[i].length);
        printstr(" | ");

        printhex4(memory_map[i].acpi_attributes);
        printstr(" | ");
        
        switch ((region_type)memory_map[i].type) {
            case usable: 
                printstr("USABLE");
                break;
        
            case reserved: 
                printstr("RESERVED");
                break;

            case acpi_reclaimable: 
                printstr("ACPI_RECLAIMABLE");
                break;

            case acpi_nvs:
                printstr("ACPI_NVS");
                break;

            case bad_memory:
                printstr("BAD_MEMORY");
                break;
            
            default:
                printstr("UNRECOGNIZED");
                break;
        }

        printnl();
    }
}

void print_tests() {
    printstr("test10\n");
    printstr_col("{{ testing prints }}\n", light_red, blue);
    printhex8(0xFEEDFACECAFEBEEF);
    printnl();
    printhex4(0xDEADBEEF);
    printnl();
    printhex2(0x1337);
    printnl();
    printhex1(0xAB);
    printnl();
}

void test_memory() {
    printstr_col("{{ checking virtual memory }}\n", light_red, blue);
    const int64_t* check_amount = (int64_t*)0x40000000;
    volatile long garbage;
    for(int64_t* c = 0; c < check_amount; c++ /*memed*/)
        garbage = *c;
    printhex8((intptr_t)check_amount);
    printstr(" bytes checked successfully :3\n");
}