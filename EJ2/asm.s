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
        LDR     R1, =0x10000005
        LDRB    R2, [R1]
        LSLS    R2, #1
        ADD     R0, R2
        LSRS    R0, #28
        ADDS    R1, #3
        MOV     R0, R1
        LDRH    R3, [R0]
        ORRS    R2, R0
        MOVS    R4, #0X0C
        EORS    R1, R4
        STR     R3, [R2]
        STR     R3, [R0]
        STRH    R0, [R1]
forever
        B       forever

        END