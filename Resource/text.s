.section .data
cmpa: .int 0
cmpb: .int 0
.section .rodata
msg: .asciz "%d\n"
.section .text
.globl main
main:
pushl %ebp
movl %esp,%ebp
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
subl $4, %esp
movl $0, %eax
movl %eax, -60(%ebp)
movl $0, %eax
movl %eax, -64(%ebp)
movl $0, %eax
movl %eax, -68(%ebp)
for_start:
movl -68(%ebp), %eax
movl %eax, cmpa
movl $6, %eax
movl %eax, cmpb
movl cmpa, %eax
movl cmpb, %ebx
cmp %eax, %ebx
jbe for_fail
movl -60(%ebp), %eax
movl %eax, %ecx
movl -68(%ebp), %edx
movl -24(%ebp, %edx, 4), %eax
movl %eax, %ebx
movl -68(%ebp), %edx
movl -48(%ebp, %edx, 4), %eax
mull %ebx
addl %ecx, %eax
movl %eax, -60(%ebp)
movl -64(%ebp), %eax
movl %eax, %ecx
movl -68(%ebp), %edx
movl -48(%ebp, %edx, 4), %eax
addl %ecx, %eax
movl %eax, -64(%ebp)
pushl -60(%ebp)
pushl $msg
call printf
addl $8, %esp
pushl -64(%ebp)
pushl $msg
call printf
addl $8, %esp
addl $1, -68(%ebp)
movl -68(%ebp), %eax
movl %eax, cmpa
jmp for_start
for_fail:
pushl -60(%ebp)
pushl $msg
call printf
addl $8, %esp
movl -60(%ebp), %eax
movl %eax, %ebx
movl -64(%ebp), %eax
movl %eax, %edx
movl %ebx, %eax
movl %edx, %ebx
movl $0, %edx
divl %ebx
movl %eax, -56(%ebp)
pushl -56(%ebp)
pushl $msg
call printf
addl $8, %esp
movl -56(%ebp), %eax
movl %eax, cmpa
movl $60, %eax
movl %eax, cmpb
movl cmpa, %eax
movl cmpb, %ebx
cmp %eax, %ebx
ja if_fail
movl -56(%ebp), %eax
movl %eax, %ecx
movl $60, %eax
subl %eax, %ecx
movl %ecx, %eax
movl %eax, -56(%ebp)
jmp selection_end
if_fail:
movl $60, %eax
movl %eax, %ecx
movl -56(%ebp), %eax
subl %eax, %ecx
movl %ecx, %eax
movl %eax, -56(%ebp)
selection_end:
pushl -56(%ebp)
pushl $msg
call printf
addl $8, %esp
movl $1, %eax
movl $0, %ebx
int $0x80
