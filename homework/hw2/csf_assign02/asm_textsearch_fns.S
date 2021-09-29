/*
 * Assembly language implementations of functions declared in
 * textsearch_fns.h
 */

	.section .text

/*
 * The following is just an example of how an assembly language
 * function would be defined
 */

/* TODO: implementation of read_line function */

	.globl read_line
read_line:
	subq $8, %rsp /* align stack pointer */
	pushq %r12 /* preserve value of callee-saved register */
	pushq %r13 /* preserve value of callee-saved register */
	movq $0, %r12 /* this is the counter i, setting to 0*/
	movq $0, %r13 /* this is the char c, setting to 0 */

.LreadLineLoopCondition:
	cmp $511, %r12 /* compare counter to MAXLINE */
	jz .LreadLineAfterLoop /* exit loop if counter = MAXLINE */
	test %r13, %r13 /* if not, compare c to EOF */
	js .LreadLineAfterLoop /* exit loop if c = EOF */

.LreadLineLoop:
	pushq %rdi /* preserve value of first argument */
	pushq %rsi /* preserve value of second argument */
	call fgetc /* call fgetc using first arg */
	popq %rsi /* restore value of first argument */
	popq %rdi /* restore value of second argument */
	movq %rax, %r13 /* copy result of fgetc into r13 */
	cmp $10, %r13 /* check if c is a new line char */
	jz .LreadLineAfterLoop /* exit loop */
	movq %r13, (%rsi) /* insert c into string buffer */
	incq %rsi /* increment string buffer pointer */
	inc %r12 /* increment counter i */
	jmp .LreadLineLoopCondition /* iterate again */

.LreadLineAfterLoop:
	movq $0, (%rsi) /* insert null terminator */
	test %eax, %eax /* check if last c was EOF */
	js .LreadLineReturn0 /* if it is, return 0 */
	movq $1, %rax /* otherwise, return 1 */
	popq %r12 /* restore value of callee-saved register */
	popq %r13 /* restore value of callee-saved register */

	addq $8, %rsp /* align stack pointer */
	ret

.LreadLineReturn0:
	movq $0, %rax /* return 0 */
	popq %r12 /* restore value of callee-saved register */
	popq %r13 /* restore value of callee-saved register */

	addq $8, %rsp /* align stack pointer */
	ret

/* implementation of find_string_length function */

	.globl find_string_length
find_string_length:
	subq $8, %rsp
	movq $0, %rax
	cmp $0, (%rdi)
	jz .LfindStringLengthReturn

.LfindStringLengthLoopCondition:
	cmp $0, (%rdi)
	jz .LfindStringLengthReturn

.LfindStringLengthLoop:
	inc %rax
	incq %rdi
	jmp .LfindStringLengthLoopCondition

.LfindStringLengthReturn:
	addq $8, %rsp
	ret