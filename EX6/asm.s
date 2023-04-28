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
        MOVS    R3, #32
copiar
        LDRB    R2, [R0]
        CMP     R2, #0
        BEQ     forever
        CMP     R3, #0
        BEQ     forever
        STRB    R2, [R1]
        ADDS    R0, #1
        ADDS    R1, #1
        SUBS    R3, #1
        B       copiar
forever
        B       forever

        END