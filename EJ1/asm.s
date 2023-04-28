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
        LDRB    R1, [R0]
        ADDS    R0, #8
        LDR     R2, [R0]
        ORRS    R2, R1
        LDR     R3, =0x10000002
        STRH    R2, [R3]
        LDRB    R1, [R0]
        BICS    R2, R1
        LSRS    R2, #2
        BCC     no_more
        STR     R3, [R0]
forever
        B       forever
no_more
        ADDS    R0, #4
        STR     R1, [R0]
        B       forever
        
        END