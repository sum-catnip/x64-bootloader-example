; todo split the dq's to be more descriptive
gdt_start:

gdt_null:
    dq 0x0000000000000000             ; Null Descriptor - should be present.

gdt_code:
    dq 0x00209A0000000000             ; 64-bit code descriptor (exec/read).
    dq 0x0000920000000000             ; 64-bit data descriptor (read/write).

align 4
    dw 0                              ; Padding to make the "address of the GDT" field aligned on a 4-byte boundary

gdt_descriptor:
    dw $ - gdt_start - 1 ; size
    dd gdt_start         ; base