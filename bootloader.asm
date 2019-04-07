[org 0x7C00] ; bios maps us here
[bits 16] ; we start in real mode

%define CENTRY 0x7E00
; dont forget this is also defined in memory.h
%define MEMMAP 0x9000
%define PMAPL4 0xA000

; todo: more compatability in enabling the a20 line 

; fake BIOS parameter block so this works on a real machine
boot:
    jmp start
    TIMES 3-($-$$) DB 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

    ; Dos 4.0 EBPB 1.44MB floppy
    OEMname:           db    "mkfs.fat"  ; mkfs.fat is what OEMname mkdosfs uses
    bytesPerSector:    dw    512
    sectPerCluster:    db    1
    reservedSectors:   dw    1
    numFAT:            db    2
    numRootDirEntries: dw    224
    numSectors:        dw    2880
    mediaType:         db    0xf0
    numFATsectors:     dw    9
    sectorsPerTrack:   dw    18
    numHeads:          dw    2
    numHiddenSectors:  dd    0
    numSectorsHuge:    dd    0
    driveNum:          db    0
    reserved:          db    0
    signature:         db    0x29
    volumeID:          dd    0x2d7e5a1a
    volumeLabel:       db    "NO NAME    "
    fileSysType:       db    "FAT12   "


start:
    ; enable a20 line via BIOS
    ; TODO: more compatability by trying different a20 enabling methods 
    mov ax, 0x2403
    int 15h

    jmp 0x0000:init ; far jump to reload cs

init:
    xor ax, ax

    ; Set up segment registers.
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; place stack above since it grows away from us
    mov sp, boot

    ; load program into memory
    mov bx, CENTRY
    mov dh, 15
    ; dl should still contain the boot drive
    call read_disk

    ; load memory map
    mov di, MEMMAP
    call do_e820

    cld

    mov edi, PMAPL4
    jmp switch_longmode

%include "memorymap.asm"
%include "realdisk.asm"
%include "long.asm"

; we in it boiz
[bits 64]
longmode:
    jmp CENTRY

; Pad out file.
times 510 - ($-$$) db 0
dw 0xAA55