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
        EORS    R1, R1
        EORS    R2, R2
loop
        LDRSB   R3, [R0, R1]
        ORRS    R3, R3
        BPL     tail
        ADDS    R2, #0x01
tail
        ADDS    R1, #0x01
        CMP     R1, #0x08
        BLO     loop
        STRB    R2, [R0, R1]
forever
        B       forever

        END