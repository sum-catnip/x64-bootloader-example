[bits 16]

%define PAGE_PRESENT (1 << 0)
%define PAGE_WRITE   (1 << 1)
%define PAGE_SIZE    (1 << 7) 

%define CODE_SEG     0x0008
%define DATA_SEG     0x0010

; an empty interrupt descriptor table
idt_null:
    .Length       dw 0
    .Base         dd 0

; Function to switch directly to long mode from real mode.
; es:di    Should point to a valid page-aligned 16KiB buffer, for the PML4, PDPT, PD and a PT.
; ss:sp    Should point to memory that can be used as a small (1 uint32_t) stack

switch_longmode:
    ; Zero out the 16KiB buffer.
    ; Since we are doing a rep stosd, count should be bytes/4.   
    push di                           ; REP STOSD alters DI.
    mov ecx, 0x1000
    xor eax, eax
    cld
    rep stosd
    pop di                            ; Get DI back.


    ; Build the Page Map Level 4.
    ; es:di points to the Page Map Level 4 table.
    lea eax, [es:di + 0x1000]         ; Put the address of the Page Directory Pointer Table in to EAX.
    or eax, PAGE_PRESENT | PAGE_WRITE ; Or EAX with the flags - present flag, writable flag.
    mov [es:di], eax                  ; Store the value of EAX as the first PML4E.


    ; Build the Page Directory Pointer Table.
    lea eax, [es:di + 0x2000]         ; Put the address of the Page Directory in to EAX.
    or eax, PAGE_PRESENT | PAGE_WRITE ; Or EAX with the flags - present flag, writable flag.
    mov [es:di + 0x1000], eax         ; Store the value of EAX as the first PDPTE.


    push di                           ; Save DI for the time being.
    lea di, [di + 0x2000]             ; Point DI to the page directory.
    mov eax, PAGE_PRESENT | PAGE_WRITE | PAGE_SIZE ; Move the flags into EAX - and point it to 0x0000.

    .build_pagedirectory:
        mov [es:di], eax
        add eax, 0x200000
        add di, 8
        cmp eax, 0x40000000 ; map 1 gb
        jb .build_pagedirectory

        pop di                            ; Restore DI.

        ; Disable IRQs
        mov al, 0xFF                      ; Out 0xFF to 0xA1 and 0x21 to disable all IRQs.
        out 0xA1, al
        out 0x21, al

        nop
        nop

        lidt [idt_null]                    ; Load a zero length IDT so that any NMI causes a triple fault.

        ; Enter long mode.
        mov eax, 10100000b                ; Set the PAE and PGE bit.
        mov cr4, eax

        mov edx, edi                      ; Point CR3 at the PML4.
        mov cr3, edx

        mov ecx, 0xC0000080               ; Read from the EFER MSR. 
        rdmsr    

        or eax, 0x00000100                ; Set the LME bit.
        wrmsr

        mov ebx, cr0                      ; Activate long mode -
        or ebx,0x80000001                 ; - by enabling paging and protection simultaneously.
        mov cr0, ebx                    

        lgdt [gdt_descriptor]                ; Load GDT.Pointer defined below.

        jmp CODE_SEG:prepare_lm             ; Load CS with 64 bit segment and flush the instruction cache

%include "longgdt.asm"

[bits 64]      
prepare_lm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp longmode