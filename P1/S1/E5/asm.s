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
        LDRH    R2, [R0, R1]
        LDR     R3, =0x00000000
        ADDS    R1, #2
        LDRB    R3, [R0, R1]
forever
        B       forever