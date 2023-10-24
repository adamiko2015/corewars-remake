section .text

; TODO: load survivor's flags register before changing flags
global flags_8_bit_add
flags_8_bit_add:

    add cl, dl ; adding the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_add
flags_16_bit_add:

    add cx, dx ; adding the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret