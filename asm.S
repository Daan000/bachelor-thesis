.data
.global ffe_instr
.global ffe_page_begin
.global ffe_page_end
.global ffe_next_page_end
.align 0x1000
ffe_page_begin:
    .fill 0xffe, 1, 0xaa
ffe_instr:
    .byte 0x41
    .byte 0x89
ffe_page_end:
    .byte 0x48
    .byte 0x3c
ffe_instr_end:
    .fill 0x1000-(ffe_instr_end-ffe_page_end), 1, 0xbb
ffe_next_page_end:
