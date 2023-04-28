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
        LDR     R1, =0x10000001
        LDRB    R0, [R0]
        LDRB    R1, [R1]
        MULS    R0, R1, R0
        LSRS    R0, #1
        LDR     R2, =0x10000008
        STRB    R0, [R2]
forever
        B       forever