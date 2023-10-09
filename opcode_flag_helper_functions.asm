section .text

global flags_8_bit_add
flags_8_bit_add:
    add cl, dl
    pushf
    pop rax

    ret

global flags_16_bit_add
flags_16_bit_add:
    add cx, dx
    pushf
    pop rax

    ret