# Instruction Set of AM4
Each instruction consists of an OPCODE and an optional argument.

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
0x10 | C

## Arithmetics
### Add
0x20

### Sub
0x30

### Mul
0x40

## Boolean Operations
### Eq
0xa0

### LT
0xa1

### LE
0xa2

### GT
0xa3

### GE
0xa4

### AND
0xaa

### OR
0xab

### NEG
0xb0

## Memory
### Fetch
0xc0 | VA

### Store
0xc1 | VA
