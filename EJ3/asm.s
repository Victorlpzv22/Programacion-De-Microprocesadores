        AREA    RESET, DATA, READONLY
        EXPORT  __Vectors
__Vectors
        DCD     0x10001800     ; Dirección de inicio del stack
        DCD     Reset_Handler  ; Vector de reset

        ALIGN
        AREA    MI_PROGRAMA, CODE, READONLY
        EXPORT  Reset_Handler
Reset_Handler
        LDR     R0, =0x10000008
        MOVS    R1, #8
        MOVS    R2, #0
        MOVS    R3, #0
        LDR     R4, =0x10000010
loop
        ADD     R0, R2
        LDRB    R5, [R0]
        LSRS    R5, #1
        BCC     tail2
tail1
        SUBS    R1, #1
        BEQ     forever
        ADDS    R2, #1
        B       loop
tail2
        ADDS    R3, #1
        STRB    R3, [R4]
        B       tail1
forever
        B       forever

        END