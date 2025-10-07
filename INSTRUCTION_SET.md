# Instruction Set of AM4
## Definitions
### OPCODE
8 bit number representing the current instruction

### C
Constant, 24 bits

### VA
Variable address, 24 bit number.

### IA
Instruction address. 24 bit number.

## NOOP
0x00

## Jump Instructions
### Jump
0x01 | IA

### Jump if equal to zero
0x02 | IA

## Push
### Int
0xa0 | C

### True
0xaa

### False
0xab

## Arithmetics
### Add
0xc0

### Sub
0xc1

### Mul
0xc3

## Boolean Operations
### Eq
0xd0

### LE
0xd1

### AND
0xd2

### NEG
0xd3

## Memory
### Fetch
0xe0 | VA

### Store
0xe1 | VA
