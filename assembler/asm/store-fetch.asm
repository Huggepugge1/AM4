main:
	push 5
	store x
	fetch x

pushsomething:
	store y
	fetch y
	jmp main:
