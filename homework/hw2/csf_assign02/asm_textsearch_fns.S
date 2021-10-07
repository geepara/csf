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
	popq %r13 /* restore value of callee-saved register */
	popq %r12 /* restore value of callee-saved register */

	addq $8, %rsp /* align stack pointer */
	ret

.LreadLineReturn0:
	movq $0, %rax /* return 0 */
	popq %r13 /* restore value of callee-saved register */
	popq %r12 /* restore value of callee-saved register */

	addq $8, %rsp /* align stack pointer */
	ret

/* implementation of find_string_length function */

	.globl count_occurrences
count_occurrences:
	subq $8, %rsp /* align stack pointer */
	pushq %r12 /* preserve value of callee-saved register */
	pushq %r13 /* preserve value of callee-saved register */
	pushq %r14 /* preserve value of callee-saved register */
	pushq %r15 /* preserve value of callee-saved register */
	
	/* check if line == NULL || str == NULL */
	movq $0, %rax /* set return value to 0 */
	cmp $0, %rdi /* check if 1st argument is null */
	je .LcountOccurrencesReturn /* if so, return 0 */
	cmp $0, %rsi /* check if 2nd argument is null */
	je .LcountOccurrencesReturn /* if so, return 0 */

	/* find_string_length(line) */
	pushq %rdi /* preserve value of 1st argument */
	pushq %rsi /* preserve value of 2nd argument */
	call find_string_length /* get the length of 1st arg */
	movl %eax, %r12d /* store the length of 1st arg in r12 */
	popq %rsi /* restore value of 2nd argument */
	popq %rdi /* restore value of 1st argument */

	/* find_string_length(str) */
	pushq %rdi /* preserve value of 1st argument */
	pushq %rsi /* preserve value of 2nd argument */
	movq %rsi, %rdi /* move 2nd arg into position of 1st arg */
	call find_string_length /* get the length of 2nd arg */
	movl %eax, %r13d /* store the length of 2nd arg in r13 */
	popq %rsi /* restore value of 2nd argument */
	popq %rdi /* restore value of 1st argument */

	movl $0, %eax /* set return value to 0 */
	cmpl $0, %r12d /* check if size of 1st arg is 0 */
	je .LcountOccurrencesReturn /* if so, return 0 */
	cmpl $0, %r13d /* check if size of 2nd arg is 0 */
	je .LcountOccurrencesReturn /* if so, return 0 */

	subl %r13d, %r12d /* len(1st arg) - len(2nd arg) + 1*/
	addl $1, %r12d
	movl $0, %r10d /* set counter i to 0 */

.LcountOccurrencesLoop1Condition:
	cmpl %r10d, %r12d /* check if end of "line" was reached */
	jz .LcountOccurrencesReturn /* if so, exit loop */

	movl $0, %r11d /* 2nd loop counter set to 0 */
	pushq %rdi /* preserve position in line */
	pushq %rsi /* preserve position in str */

.LcountOccurrencesLoop2Condition:
	cmpl %r11d, %r13d /* check if end of str was reached */
	jz .LcountOccurrencesLoop2Match /* if so, there was a match */

.LcountOccurrencesLoop2:
	movb (%rdi), %r14b /* store char of line in r14b */
	movb (%rsi), %r15b /* store char of str in r15b */
	cmpb %r14b, %r15b /* compare char of line to str */
	jne .LcountOccurrencesLoop2Exit /* if they don't match, exit the loop */
	incq %rdi /* increment line pointer */
	incq %rsi /* increment str pointer */
	inc %r11d /* increment 2nd loop counter */
	jmp .LcountOccurrencesLoop2Condition

.LcountOccurrencesLoop2Exit:
	popq %rsi /* restore value of line pointer */
	popq %rdi /* restore value of str pointer */
	incq %rdi /* progress through line */
	inc %r10d /* increment loop counter */
	jmp .LcountOccurrencesLoop1Condition /* iterate again */

.LcountOccurrencesLoop2Match:
	inc %eax /* occurrence found, increment counter */
	jmp .LcountOccurrencesLoop2Exit /* continue with loop */

.LcountOccurrencesReturn:
	popq %r15 /* preserve value of callee-saved register */
	popq %r14 /* preserve value of callee-saved register */
	popq %r13 /* preserve value of callee-saved register */ 
	popq %r12 /* preserve value of callee-saved register */ 
	addq $8, %rsp /* align stack pointer */
	ret

/* implementation of find_string_length function */

	.globl find_string_length
find_string_length:
	subq $8, %rsp /* align stack pointer */
	movq $0, %rax /* set 0 as return value */
	cmp $0, %rdi /* check if string is null */
	jz .LfindStringLengthReturn /* if so, return 0 */

.LfindStringLengthLoopCondition:
	cmpb $0, (%rdi) /* check if null terminator reached */
	je .LfindStringLengthReturn /* if so, return string length */

.LfindStringLengthLoop:
	inc %rax /* increment string length counter */
	incq %rdi /* progress through string */
	jmp .LfindStringLengthLoopCondition /* iterate again */

.LfindStringLengthReturn:
	addq $8, %rsp /* align stack pointer */
	ret

/* implementation of starts_with function */

	.globl starts_with
starts_with:
	subq $8, %rsp /* align stack pointer */
	pushq %r12 /* preserve callee-saved register */
	pushq %r13 /* presrve callee-saved register */
	
	cmp $0, %rdi /* check if string is null */
	jz .LstartsWithReturn0 /* if so, return 0 */
	cmp $0, %rsi /* check if prefix is null */
	jz .LstartsWithReturn0 /* if so, return 0 */

	pushq %rdi /* preserve value of 1st arg */
	pushq %rsi /* preserve value of 2nd arg */
	movq %rsi, %rdi /* move 2nd arg in 1st arg position */
	call find_string_length /* get string length of prefix */
	movq %rax, %r10 /* store size of prefix in r10 */
	popq %rsi /* restore value of 2nd arg */
	popq %rdi /* restore value of 1st arg */

	cmp $0, %r10 /* check if size of prefix is zero */
	jz .LstartsWithReturn0 /* if so, return 0 */

	movq $0, %r11 /* set loop counter to 0 */

.LstartsWithLoopCondition:
	cmp %r11, %r10 /* check if end of  */
	jz .LstartsWithReturn1

.LstartsWithLoop:
	movb (%rdi), %r12b
	movb (%rsi), %r13b
	cmpb %r12b, %r13b
	jne .LstartsWithReturn0
	incq %rdi
	incq %rsi
	inc %r11
	jmp .LstartsWithLoopCondition

.LstartsWithReturn1:
	movq $1, %rax
	jmp .LstartsWithReturn	

.LstartsWithReturn0:
	movq $0, %rax

.LstartsWithReturn:
	popq %r13
	popq %r12
	addq $8, %rsp
	ret

/* implementation of strings_equal function */

	.globl strings_equal
strings_equal:
	subq $8, %rsp
	pushq %r12
	pushq %r13

	cmp $0, %rdi
	jz .LstringsEqualReturn0
	cmp $0, %rsi
	jz .LstringsEqualReturn0

	pushq %rdi
	pushq %rsi
	call find_string_length
	movq %rax, %r10
	popq %rsi
	popq %rdi

	pushq %rdi
	pushq %rsi
	movq %rsi, %rdi
	call find_string_length
	popq %rsi
	popq %rdi

	cmp %r10, %rax
	jne .LstringsEqualReturn0

	movq $0, %r11

.LstringsEqualLoopCondition:
	cmp %r11, %r10
	jz .LstringsEqualReturn1

.LstringsEqualLoop:
	movb (%rdi), %r12b
	movb (%rsi), %r13b
	cmpb %r12b, %r13b
	jne .LstringsEqualReturn0
	incq %rdi
	incq %rsi
	inc %r11
	jmp .LstringsEqualLoopCondition

.LstringsEqualReturn0:
	movq $0, %rax
	jmp .LstringsEqualReturn

.LstringsEqualReturn1:
	movq $1, %rax

.LstringsEqualReturn:
	popq %r13
	popq %r12
	addq $8, %rsp
	ret