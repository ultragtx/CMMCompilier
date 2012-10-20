.section .rodata
str_literal_0: .asciz "please input your student number:"
str_literal_1: .asciz "%d"
str_literal_2: .asciz "the score of student number %d is %f higher than 60.\n"
str_literal_3: .asciz "the score of student number %d is %f lower than 60.\n"
.section .text
.globl main
main:
subl $24, %esp
movl $0, %edx
movl $76, -24(%ebp, %edx, 4)
movl $1, %edx
movl $82, -24(%ebp, %edx, 4)
movl $2, %edx
movl $90, -24(%ebp, %edx, 4)
movl $3, %edx
movl $86, -24(%ebp, %edx, 4)
movl $4, %edx
movl $79, -24(%ebp, %edx, 4)
movl $5, %edx
movl $62, -24(%ebp, %edx, 4)
subl $24, %esp
movl $0, %edx
movl $2, -48(%ebp, %edx, 4)
movl $1, %edx
movl $2, -48(%ebp, %edx, 4)
movl $2, %edx
movl $1, -48(%ebp, %edx, 4)
movl $3, %edx
movl $2, -48(%ebp, %edx, 4)
movl $4, %edx
movl $2, -48(%ebp, %edx, 4)
movl $5, %edx
movl $3, -48(%ebp, %edx, 4)
subl $4, %esp
subl $4, %esp
subl $4, %esp
subl $4, %esp
subl $4, %esp
pushl $str_literal_0
call printf
addl $4, %esp
;pushl $0
;pushl $str_literal_1
;call scanf
;addl $8, %esp
movl $0, -60(%ebp)
movl $0, -64(%ebp)
movl $0, -68(%ebp)
for_begin_0:
movl -68(%ebp), %eax
movl $6, %ebx
cmp %eax, %ebx
jb for_st_0
jmp for_end_0
for_st_0:
movl -68(%ebp), %edx
movl -48(%ebp, %edx, 4), %eax
movl %eax, %ebx
movl -68(%ebp), %edx
movl -24(%ebp, %edx, 4), %eax
mull %ebx
movl -60(%ebp), %ebx
addl %ebx, %eax
movl %eax, -60(%ebp)
movl -68(%ebp), %edx
movl -48(%ebp, %edx, 4), %eax
movl -64(%ebp), %ebx
addl %ebx, %eax
movl %eax, -64(%ebp)
addl $1, -68(%ebp)
jmp for_begin_0
for_end_0:
movl -60(%ebp), %eax
movl -60(%ebp), %ebx
divl %ebx
movl %eax, -56(%ebp)
movl -56(%ebp), %eax
movl $60, %ebx
cmp %eax, %ebx
jae if_part_0
movl -56(%ebp), %ebx
movl $60, %eax
subl %ebx, %eax
movl %eax, -56(%ebp)
pushl 0(%ebp)
pushl 0(%ebp)
pushl $str_literal_3
call printf
addl $12, %esp
jmp if_end_0
if_part_0:
movl -56(%ebp), %eax
movl $60, %ebx
subl %ebx, %eax
movl %eax, -56(%ebp)
pushl 0(%ebp)
pushl 0(%ebp)
pushl $str_literal_2
call printf
addl $12, %esp
if_end_0:
movl $1, %eax
movl $0, %ebx
int $0x80