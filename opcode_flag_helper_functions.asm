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

global flags_8_bit_or
flags_8_bit_or:

    or cl, dl ; oring the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_or
flags_16_bit_or:

    or cx, dx ; oring the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_8_bit_adc
flags_8_bit_adc:

    adc cl, dl ; adcing the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_adc
flags_16_bit_adc:

    adc cx, dx ; adcing the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_8_bit_sbb
flags_8_bit_sbb:

    sbb cl, dl ; sbbing the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_sbb
flags_16_bit_sbb:

    sbb cx, dx ; sbbing the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_8_bit_and
flags_8_bit_and:

    and cl, dl ; anding the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_and
flags_16_bit_and:

    and cx, dx ; anding the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_8_bit_sub
flags_8_bit_sub:

    sub cl, dl ; subing the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_sub
flags_16_bit_sub:

    sub cx, dx ; subing the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_8_bit_xor
flags_8_bit_xor:

    xor cl, dl ; xoring the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_xor
flags_16_bit_xor:

    xor cx, dx ; xoring the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_8_bit_cmp
flags_8_bit_cmp:

    cmp cl, dl ; cmping the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_cmp
flags_16_bit_cmp:

    cmp cx, dx ; cmping the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_8_bit_mov
flags_8_bit_mov:

    mov cl, dl ; moving the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_mov
flags_16_bit_mov:

    mov cx, dx ; moving the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_8_bit_test
flags_8_bit_test:

    test cl, dl ; testing the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret

global flags_16_bit_test
flags_16_bit_test:

    test cx, dx ; testing the two inputs

    pushf ; moving Flags register to rax (output)
    pop rax

    ret