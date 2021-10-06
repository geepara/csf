/*
 * Assembly language implementation of the main function
 * for the asm_textsearch_fns program
 */

	.section .rodata
sNumArgsError: .string "Incorrect number of arguments\n"
sOpenFileError: .string "Could not open file\n"
sInvalidArgs: .string "Invalid arguments\n"
sC: .string "-c"
sR: .string "r"
sPrintLineFormat: .string "%s\n"
sPrintOccurrencesFormat: .string "%d occurrence(s)\n"

	.section .text

	.globl main
main:
	subq $8, %rsp /* align stack pointer */
	pushq %rbp
	pushq %r12 /* preserve value of callee-saved register */
	pushq %r13 /* preserve value of callee-saved register */
	pushq %r14 /* preserve value of callee-saved register */
	pushq %r15 /* preserve value of callee-saved register */

	pushq %rsi /* preserve value of 2nd arg */
	movq $sNumArgsError, %rsi /* set up error message */
	cmp $3, %rdi /* check if argc < 3 */
	jl .LmainReturn1 /* if so, return 1 */
	cmp $4, %rdi /* check if argc > 4 */
	jg .LmainReturn1 /* if so, return 1 */
	popq %rsi /* restore value of 2nd arg */

	pushq %rdi /* preserve value of 1st arg */
	pushq %rsi /* preserve value of 2nd arg */
	movq $8(%rsi), %rdi /* put argv[1] into 1st arg position */
	movq $sC, %rsi /* move "-c" into 2nd arg position */
	call strings_equal /* compare argv[1] to "-c" */
	popq %rsi /* restore value of 2nd arg */
	popq %rdi /* restore value of 1st arg */

	movq $0, %r12 /* indicate "-c" wasn't used as a 0 in r12  (BY DEFAULT)*/

	cmp $1, %rax /* check if argv[1] == "-c" */
	jne .LmainOpenFileNoCFlag /* if not, open argv[2] as file */
	movq $1, %r12 /* if yes, indicate a 1 in r12 (referenced later) */
	movq %rdi, %r10 /* preserve value of argc in r10 */
	movq $sInvalidArgs, %rdi /* set up error message */
	cmp $3, %r10 /* check if argc > 3 */
	jg .LmainReturn1 /* if so, return 1 */
	movq %r10, %rdi /* restore value of argc */

.LmainOpenFileCFlag:
	movq $24(%rsi), %rdi /* move argv[3] into 1st arg position */
	jmp .LmainOpenFile /* open file with argv[3] */

.LmainOpenFileNoCFlag:
	movq $16(%rsi), %rdi /* move argv[2] into 1st arg position */

.LmainOpenFile:
	pushq %rsi /* preserve value of argv */
	movq $sR, %rsi /* move "r" into 2nd arg position */
	call fopen /* fopen(argv[2 or 3], "r") */
	movq %rax, %r13 /* store file pointer in r13 */
	popq %rsi /* restore value of argv */

.LmainCheckFile:
	movq $sOpenFileError, %rdi /* set up error message. not preserving 
																rdi bc no use for argc anymore */
	cmp $0, %r13 /* check if file pointer is null */
	je .LmainReturn1 /* if so, return 1 */

.LmainTargetStr:
	cmp $1, %r12 /* check if "-c" was used */
	jne .LmainTargetStrCFlag /* if not, targetStr = argv[1] */
	movq $16(%rsi), %r14 /* if yes, store argv[2] as targetStr in r14 */
	jmp .LmainTextSearch /* go to text search */

.LmainTargetStrCFlag:
	movq $8(%rsi), %r14 /* store argv[1] as targetStr in r14 */

.LmainTextSearch:
	subq $511, %rsp /* allocate 511 bytes */
	movq %rsp, %rbp /* set base pointer to stack pointer */
	movq $0, %r15 /* set occurrences counter to 0 */

.LmainLoopCondition:
	movq %r13, %rdi
	movq %rbp, %rsi
	call read_line
	cmp $0, %rax
	je .LmainPrintOccurrences

.LmainLoop:
	movq %rbp, %rdi
	movq %r14, %rsi
	call count_occurrences
	addl %eax, %r15d
	cmpb $0, %eax
	jg .LmainLoopCheckCArg
	jmp .LmainLoopCondition

.LmainLoopCheckCArg
	cmp $1, %r12
	je .LmainLoopCondition
	movq $sPrintLineFormat, %rdi
	movq %rbp, %rsi
	call printf
	jmp .LmainLoopCondition

.LmainPrintOccurrences:
	cmp $1, %r12
	jne .LmainReturn0
	movq $sPrintOccurrencesFormat, %rdi
	movq %r15, %rsi
	call printf

.LmainReturn0:
	movl $0, %eax
	jmp .LmainReturn

.LmainReturn1:
	movl $1, %eax

.LmainReturn:
	movq stderr(%rip), %rdi
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	pop %rbp
	subq $8, %rsp
	ret
