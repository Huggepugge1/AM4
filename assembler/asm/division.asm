// y / x
push 3
store x
push 15
store y

push 0
store z
fetch x
store r

loop:
    fetch r
    fetch y
    le
    jeqz end:
    fetch y
    fetch r
    sub
    store y
    push 1
    fetch z
    add
    store z
    jmp loop:

end:
fetch z
