.text

.globl __ctx_record
__ctx_record:
movq (%rsp), %rcx
movq %rcx,(%rdi)
movq %rbp,8(%rdi)

//leaq 8(%rsp), %rcx
//movq %rcx,16(%rdi)
movq %rsp,16(%rdi)

xorq %rax,%rax
ret

// TODO

.globl __ctx_recover
__ctx_recover:
movq %rsi,%rax
movq 8(%rdi),%rbp
movq 16(%rdi),%rsp
movq (%rdi),%rcx
movq %rcx,(%rsp)

movq 24(%rdi),%rdi

//jmp *%rcx
ret

// TODO

