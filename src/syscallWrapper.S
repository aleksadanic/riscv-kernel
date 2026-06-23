.global syscallWrapper
.type syscallWrapper, @function
syscallWrapper:
    ecall
    ret