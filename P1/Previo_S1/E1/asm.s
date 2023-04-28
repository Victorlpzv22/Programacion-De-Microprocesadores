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
        MOVS    R1, #0
        LDR    R2, [R0, R1]
        MOVS    R1, #4
        LDR    R3, [R0, R1]
        ADDS    R3, R2
        MOVS    R1, #0x10
        STR    R3, [R0, R1]
forever
        B       forever

        END
