0:
    push 0
    store i
1:
    fetch i
    push 0
    ge
    fetch i
    push 10
    le
    land
    jeqz 2:
    fetch i
    push 1
    add

    store i
    jmp 1:
2:
    push 0
    store i
3:
    fetch i
    push 0
    le
    fetch i
    push 10
    lt
    lor
    jeqz 4:
    fetch i
    push 1
    add

    store i
    jmp 3:
4:
    fetch i
    push 10
    eq
    jeqz 5:
    fetch i
    fetch i
    mul

    push 5
    sub

    store x
    fetch i
    push 2
    add

    store y
    fetch y
    store x
    jmp 6:
5:
    noop
6:
    fetch x
