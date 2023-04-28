        AREA    RESET, DATA, READONLY
        EXPORT  __Vectors
__Vectors
        DCD     0x10001800     ; Dirección de inicio del stack
        DCD     Reset_Handler  ; Vector de reset

        ALIGN
        AREA    MI_PROGRAMA, CODE, READONLY
        EXPORT  Reset_Handler
Reset_Handler
        LDR     R0, =0x10000000
        LDR     R1, =0x10000020
        MOVS    R2, #2
        MOVS    R3, #0
        MOVS    R4, #4
        MOV     R7, R3
loop
        LDRSH   R5, [R0, R3]
        LDRSH   R6, [R0, R2]
        MULS    R5, R6, R5
        ADDS    R7, R5
        STR     R5, [R0, #0X10]
        ADDS    R0, #4
        SUBS    R4, #1
        CMP     R4, #0
        BNE     loop
        STR     R7, [R1]
forever
        B       forever

        END