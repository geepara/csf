/*
 * Assembly language implementation of the main function
 * for the asm_textsearch_fns program
 */

	.section .rodata
sNumArgsError: .string "Incorrect number of arguments\n"
sOpenFileError: .string "Could not open file\n"
sC: .string "-c"
sR: .string "r"
sPrintLineFormat: .string "%s\n"
sPrintOccurrencesFormat: .string "%d occurrence(s)\n"

	.section .text

	.globl main
main:
	pushq %r12 /* preserve value of callee-saved register */
	pushq %r13 /* preserve value of callee-saved register */
	pushq %r14 /* preserve value of callee-saved register */
	pushq %r15 /* preserve value of callee-saved register */
	pushq %rbp /* preserve value of frame pointer */
	subq $8, %rsp /* align stack pointer */
	subq $520, %rsp /* allocate 520 bytes for "line" buffer */
	movq %rsp, %rbp /* set up rbp to be able to access "line" buffer */

	movq %rsi, %r10 /* preserve value of 2nd arg */
	movq $sNumArgsError, %rsi /* set up error message */
	cmpl $3, %edi /* check if argc < 3 */
	jl .LmainReturn1 /* if so, return 1 */
	cmpl $4, %edi /* check if argc > 4 */
	jg .LmainReturn1 /* if so, return 1 */
	movq %r10, %rsi /* restore value of 2nd arg */

	pushq %rdi /* preserve value of 1st arg */
	pushq %rsi /* preserve value of 2nd arg */
	movq 8(%rsi), %rdi /* put argv[1] into 1st arg position */
	movq $sC, %rsi /* move "-c" into 2nd arg position */
	call strings_equal /* compare argv[1] to "-c" */
	popq %rsi /* restore value of 2nd arg */
	popq %rdi /* restore value of 1st arg */

	movl $0, %r12d /* indicate "-c" wasnt used as a 0 in r12 (BY DEFAULT) */

	cmpl $0, %eax /* check if argv[1] == "-c" */
	je .LmainOpenFileNoCFlag /* if not, open argv[2] as file */
	movl $1, %r12d /* if yes, indicate a 1 in r12 (referenced later) */

.LmainOpenFileCFlag:
	movq 24(%rsi), %rdi /* move argv[3] into 1st arg position */
	jmp .LmainOpenFile /* open file with argv[3] */

.LmainOpenFileNoCFlag:
	movq %rsi, %r10 /* preserve value of 2nd arg */
	movq $sNumArgsError, %rsi /* set up error message */
	cmpl $3, %edi /* check if argc > 3 */
	jg .LmainReturn1 /* if yes, return 1 */
	movq %r10, %rsi /* restore value of 2nd arg */
	movq 16(%rsi), %rdi /* move argv[2] into 1st arg position */

.LmainOpenFile:
	pushq %rsi /* preserve value of argv */
	movq $sR, %rsi /* move "r" into 2nd arg position */
	call fopen /* fopen(argv[2 or 3], "r") */
	movq %rax, %r13 /* store file pointer in r13 */
	popq %rsi /* restore value of argv */

.LmainCheckFile:
	movq %rsi, %r10 /* preserve value of argv */
	movq $sOpenFileError, %rsi /* set up error message */
	cmp $0, %r13 /* check if file pointer is null */
	je .LmainReturn1 /* if so, return 1 */
	movq %r10, %rsi /* restore value of argv */

.LmainTargetStr:
	cmpl $1, %r12d /* check if "-c" was used */
	jne .LmainTargetStrNoCFlag /* if not, targetStr = argv[1] */
	movq 16(%rsi), %r14 /* if yes, store argv[2] as targetStr in r14 */
	jmp .LmainTextSearch /* go to text search */

.LmainTargetStrNoCFlag:
	movq 8(%rsi), %r14 /* store argv[1] as targetStr in r14 */

.LmainTextSearch:
	movl $0, %r15d /* set occurrences counter to 0 */

.LmainLoopCondition:
	movq %r13, %rdi /* move file into 1st arg position */
	movq %rbp, %rsi /* move "line" buffer into 2nd arg position */
	call read_line
	cmpl $0, %eax /* check if EOF was reached */
	je .LmainPrintOccurrences /* if yes, exit loop */

.LmainLoop:
	movq %rbp, %rdi /* move "line" buffer into 1st arg position */
	movq %r14, %rsi /* move target string into 2nd arg position */
	call count_occurrences /* find the # occurrences of target string in "line" buffer */
	addl %eax, %r15d /* add the # occurrences to the overall occurrences count */
	cmpl $0, %eax /* check if there were any occurrences of target str in "line" buffer */
	jg .LmainLoopCheckCArg /* if yes, check if "-c" flag was used */
	jmp .LmainLoopCondition /* if no, iterate again */

.LmainLoopCheckCArg:
	cmpl $1, %r12d /* check if "-c" flag was used */
	je .LmainLoopCondition /* if yes, continue the loop */
	movq $sPrintLineFormat, %rdi /* set up "line" buffer to be printed */
	movq %rbp, %rsi /* set up "line" buffer to be printed */
	call printf /* print "line" buffer */
	movq $0, %rsi /* make %rbp unaccessable through %rsi */
	jmp .LmainLoopCondition /* iterate again */

.LmainPrintOccurrences:
	movq %r13, %rdi /* move file pointer into 1st arg position */
	call fclose /* close "in" file */
	cmpl $0, %r12d /* check if "-c" flag was used*/
	je .LmainReturn0 /* if not, return 0 */
	movq $sPrintOccurrencesFormat, %rdi /* if yes, set up occurrences to be printed */
	movl %r15d, %esi /* set up occurrences to be printed */
	movl $0, %eax /* set return reg to 0 */
	call printf /* print occurrences */

.LmainReturn0:
	movl $0, %eax /* move 0 into return reg */
	jmp .LmainReturn /* return 0 */

.LmainReturn1:
	movq stderr(%rip), %rdi /* setup fprintf for stderr */
	movl $0, %eax /* set return reg to 0 */
	call fprintf /* print error */
	movl $1, %eax /* move 1 into return reg */

.LmainReturn:
	addq $520, %rsp /* deallocate "line" buffer */
	popq %rbp /* restore value of frame pointer */
	popq %r15 /* restore value of callee-saved register */
	popq %r14 /* restore value of callee-saved register */
	popq %r13 /* restore value of callee-saved register */
	popq %r12 /* restore value of callee-saved register */
	addq $8, %rsp /* align stack pointer */
	ret
