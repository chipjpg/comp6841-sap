// I've used IDA Pro's Python API to count the most common x86-64 instructions in one of the CrackMes I have completed
// Top 20 most common instructions with explanation + pseudocode, ordered from most to least used 
/*

instruction mnemonic: number of uses in one of the crackmes
mov: 6714
push: 3887
pop: 1811
call: 1788
cmp: 1466
jz: 1366
test: 1142
jmp: 933
jnz: 874
lea: 861
add: 766
retn: 746
xor: 732
and: 558
sub: 506
or: 404
inc: 391
leave: 182
shr: 166
movzx: 155

*/

/*
	The generic mov instruction copies data that is in the second operand, into the first operand.
	Can be used to:
	-Copy memory into register
	-Copy register to memory/register
	-Copy a constant to register/memory
	
	Can't be used to:
	-Copy memory to another memory location
	
	Example - copy value in eax into ebx:
	mov ebx, eax
*/
void mov(void *dest, register source) {
	// cast dest depending on size of source register
	
	*dest = source;
}
void mov(register dest, void *source) {
	// cast source depending on size of dest register
	
	dest = *source;
}
void mov(register dest, register source) {
	dest = source;
}
void mov(register dest, int immediate) {
	dest = immediate;
}
void mov(void *dest, int immediate) {
	*dest = immediate;
}

/*
	The push instruction pushes a value onto the top of the stack.
	This is accomplished by decrementing the stack pointer register, and then storing the value there.
	
	Can be used to:
	-Push data at a memory address onto the stack
	-Push a register value onto the stack
	-Push a constant/immediate value onto the stack
	
	Commonly used for:
	-Pushing arguments of function calls onto the stack
	
	Example:
	push	rax - push value in register rax onto the stack (64 bits because using rax register)
	push	qword [rax] - push value at address eax onto the stack (64 bits because qword pointer)
*/
void push(register reg) {
	// number decremented from the stack pointer depends on the size of register used
	// cast also depends on the size of the register used
	
	stack_pointer -= sizeof(reg);
	*(type*)stack_pointer = reg;
}
void push(constant) {
	// number decremented from the stack pointer depends on the size of constant used
	// cast also depends on the size of the constant
	
	stack_pointer -= sizeof(constant);
	*(type*)stack_pointer = constant;
}
void push(void *value_pointer) {
	// number decremented from the stack pointer depends on the type of data pointer
	// cast also depends on the type of data pointer
	
	stack_pointer -= sizeof(value);
	*(type*)stack_pointer = *value_pointer;
}

/*
	The pop instruction pops the value off the top of the stack.
	This is accomplished deferencing the top of stack, and then incrementing the stack pointer register.
	
	Can be used to:
	-Pop data from top of the stack into a register
	-Pop data from top of the stack and store it at a memory location

	Commonly used for:
	-Popping arguments of function calls from the stack
	
	Example:
	pop		rax - push value in register rax onto the stack (64 bits because using rax register)
	pop		qword [rax] - push value at address eax onto the stack (64 bits because qword pointer)
*/
void pop(register) {
	// number incremented to stack pointer depends on the size of destination register
	// cast also depends on the size of the size destination register
	
	register = *(type*)stack_pointer;
	stack_pointer += sizeof(register);
}
void pop(memory_address) {
	// number incremented to stack pointer depends on the size of destination
	// cast also depends on the size of the size destination
	
	*(type*)memory_address = *(type*)stack_pointer;
	stack_pointer += sizeof(type);
}

/*
	The call instruction performs an unconditional jump to a subroutine/function. The return address is also pushed onto the stack.
	
	Can be used to:
	-Call functions/subroutines relative to the current location
	-Call functions/subroutines from an absolute address
	
	Example using a label:
	call	MyFunc
	
	Example using a register:
	call	eax
*/
void call(code_address) {
	// set the EIP/RIP register
	push(return_address);
	instruction_pointer = code_address;
}


/*
	The cmp instruction is used to compare to operands. The result of the comparison is stored in the EFLAGS register.
	This instruction is equivalent to the sub instruction (see below), but it doesn't update the first operand.
	
	Can be used to:
	-Compare a register value with another register value
	-Compare a register value with memory, vice versa
	-Compare a register with a constant
	-Compare memory with a constant
	
	Example comparing a 64 bit register with memory:
	cmp		rax, QWORD PTR [rdx]
*/
void cmp(register, memory_address) {
	// Perform a comparison by doing a subtraction, and then setting the appropriate condition flags based on the result
	eflags_register = register - *memory_address;
}

/*
	The j_condition instruction is used to jump to a label based on the conditions set by the last arithmetic operation.
	
	
	Example:
	je		<label>		(jump when equal i.e. the last result was zero, so this is the same thing as jz)
	jne		<label>		(jump when not equal i.e. the last result was not zero, so this is the same thing as jnz)
	jz 		<label>		(jump when last result was zero, same thing as je)
	jnz 	<label>		(jump when last result was zero, same thing as jnz)
	jg 		<label>		(jump when greater than)
	jge 	<label>		(jump when greater than or equal to)
	jl 		<label>		(jump when less than)
	jle 	<label>		(jump when less than or equal to)
*/
void jz(code_address) {
	// Jump to the code_address if the zero flag is set
	if (ZF == 1)
		instruction_pointer = code_address;
}
void jnz(code_address) {
	// Jump to the code_address if the zero flag is not set
	if (ZF == 0)
		instruction_pointer = code_address;
}


/*
	The test instruction performs a bitwise AND on two operands.
	The SF, ZF, PF flags are set appropriately, while the AND is discarded.
	
	The the two operands are commonly the same i.e. the same register.
	
	Example:
	test	eax, eax   		set ZF to 1 if eax == 0
*/
void test(register1, register1) {
	// Perform a test by doing a bitwise AND, and then setting the appropriate condition flags based on the result
	
	OF = 0
	CF = 0
	
	// set zero flag and sign flag
	ZF = (register1 & register1) == 0;
	SF = significant_bit(register1 & register1);
	
	PF = (count_bits_set(XNOR((register1 & register1) & 0xFF)) % 2) == 0;
}

/*
	The jmp instruction is an unconditional jump.
	
	
	Examples:
	jmp		<label>
	
	jmp		rdx
*/
void jmp(code_address) {
	instruction_pointer = code_address;
}

/*
	The lea loads the effective address from the second operand, into the first operand.
	
	
	Examples:
	lea		rax, [ebx+4*esi]
	lea		rax, [ebx]
*/
void lea(register, effective_address) {
	register = effective_address;
}

/*
	The add instruction adds two operands together, and stores the result in the first operand
	
	Examples:
	add		eax, 5
	add		eax, esi
	add		BYTE PTR [eax], 10
*/
void add(operand1, operand2) {
	operand1 += operand2;
}

/*
	The retn (return) instruction is used to jump back to the the code that called this function.
	The value at the top of the stack is popped off and placed into the instruction_pointer
	
	Example:
	retn
*/
void retn() {
	pop(ret_addr);
	instruction_pointer = ret_addr;
}

/*
	The xor instruction performs a bitwise XOR on two operands together, and stores the result in the first operand
	
	Examples:
	xor		eax, 5
	xor		eax, esi
	xor		BYTE PTR [eax], 10
*/
void xor(operand1, operand2) {
	operand1 ^= operand2;
}

/*
	The and instruction performs a bitwise AND on two operands together, and stores the result in the first operand
	
	Examples:
	and		eax, 5
	and		eax, esi
	and		BYTE PTR [eax], 10
*/
void and(operand1, operand2) {
	operand1 &= operand2;
}

/*
	The sub instruction subtracts the second operand from the first, and stores the result in the first operand
	
	Examples:
	sub		eax, 5
	sub		eax, esi
	sub		BYTE PTR [eax], 10
*/
void sub(operand1, operand2) {
	operand1 -= operand2;
}

/*
	The or instruction performs a bitwise OR on two operands together, and stores the result in the first operand
	
	Examples:
	or		eax, 5
	or		eax, esi
	or		BYTE PTR [eax], 10
*/
void or(operand1, operand2) {
	operand1 |= operand2;
}

/*
	The inc instruction increments an operand
	
	Examples:
	inc		eax, 5
*/
void inc(operand) {
	operand += 1;
}

/*
	The leave instruction is sometimes used at the end of functions, which deallocates the stack frame
	
	Examples:
	leave
*/
void leave() {
	stack_pointer = base_pointer;
	pop(base_pointer); // pop item off the stack, into the base_pointer register
}

/*
	The shr instruction takes the value in the first operand, and shifts the bits by the number in the second operand.
	The result is stored in the first operand.
	
	Examples:
	shr		eax, 31
	shr		eax, cl
*/
void shr(operand1, operand2) {
	operand1 = operand1 >> operand2;
}

/*
	The movzx (move, zero extended) instruction copies data that is in the second operand, into the first operand
	 and then zero extends the first operand.
	
	It is commonly used to copy an 8bit register value to a larger register, but clearing the top bits of the larger register.
	
	Examples:
	movzx	esi, cl
*/
void movzx(operand1, operand2) {
	// Invisible in C code
	// Imagine operand2 is 8bits, and operand1 is 32bits
	
	operand1 = operand2;
}