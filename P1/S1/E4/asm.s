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
        LDR     R1, =0x00000000
        LDRSH   R2, [R0, R1]
        ADDS    R1, #4
        LDRSH   R3, [R0, R1]
        SUBS    R4, R2, R3
        LSRS    R4, #1
        ADDS    R1, #8
        ADDS    R1, #4
        STRH    R4, [R0, R1]
        MULS    R2, R3, R2
        LSRS    R2, #2
        ADDS    R1, #4
        STR     R2, [R0, R1]
forever
        B       forever