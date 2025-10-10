main:
	push 1
	push 2
	add
	push 0
	jeqz main:
not_main:
	push -1
	push -2
	add
	jmp main:
