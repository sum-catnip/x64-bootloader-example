[bits 16]

read_disk:
    mov ah, 0x02 ; read sector function
    mov al, dh   ; read dh sectors
    mov ch, 0x00 ; read from cyl 0
    mov dh, 0x00 ; read from head 0
    mov cl, 0x02 ; start from sec 2
    ; since the bootloader is at sec 1

    int 0x13 ; drive interrupt
    
    ret