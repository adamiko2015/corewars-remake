section .text

global flags_8_bit_add
flags_8_bit_add:
    add cl, dl ; adding the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret