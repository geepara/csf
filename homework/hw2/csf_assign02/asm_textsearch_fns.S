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

	.globl count_occurrences
count_occurrences:
	subq $8, %rsp /* align stack pointer */
	pushq %r12 /* preserve value of callee-saved register */
	pushq %r13 /* preserve value of callee-saved register */
	movq $0, %rax /* set return value to 0 */
	cmp $0, %rdi /* check if 1st argument is null */
	je .LcountOccurrencesReturn /* if so, return 0 */
	cmp $0, %rsi /* check if 2nd argument is null */
	je .LcountOccurrencesReturn /* if so, return 0 */

	pushq %rdi /* preserve value of 1st argument */
	pushq %rsi /* preserve value of 2nd argument */
	call find_string_length /* get the length of 1st arg */
	movq %rax, %r12 /* store the length of 1st arg in r12 */
	popq %rdi /* restore value of 1st argument */
	popq %rsi /* restore value of 2nd argument */

	pushq %rdi /* preserve value of 1st argument */
	movq %rsi, %rdi /* move 2nd arg into position of 1st arg */
	pushq %rsi /* preserve value of 2nd argument */
	call find_string_length /* get the length of 2nd arg */
	movq %rax, %r13 /* store the length of 2nd arg in r13 */
	popq %rsi /* restore value of 1st argument */
	popq %rdi /* restore value of 2nd argument */

	movq $0, %rax /* set return value to 0 */
	cmp $0, %r13 /* check if size of 2nd arg is 0 */
	je .LcountOccurrencesReturn /* if so, return 0 */

	movq %r12, %r10 /* copy size of 1st arg into r10 */
	subq %r10, %r13 /* len(1st arg) - len(2nd arg) */
	movq $0, %r10 /* set counter i to 0 */

.LcountOccurencesLoop1Condition:
	cmp %r10, %r10 /* check if counter < len(1st arg) - len(2nd arg) */
	jge .LcountOccurrencesReturn
	movq $0, %r11

.LcountOccurencesLoop2Condition
	


.LcountOccurrencesReturn:
	popq %r13
	popq %r12
	addq $8, %rsp
	ret

/* implementation of find_string_length function */

	.globl find_string_length
find_string_length:
	subq $8, %rsp
	movq $0, %rax
	cmp $0, %rdi
	jz .LfindStringLengthReturn

.LfindStringLengthLoopCondition:
	cmpb $0, (%rdi)
	je .LfindStringLengthReturn

.LfindStringLengthLoop:
	inc %rax
	incq %rdi
	jmp .LfindStringLengthLoopCondition

.LfindStringLengthReturn:
	addq $8, %rsp
	ret
