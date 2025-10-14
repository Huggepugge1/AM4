main:
	push 1
	push 2
	add
	push 0
	jeqz not_main:
not_main:
	push -1
	push -2
	add
	jmp main:
