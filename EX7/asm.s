        AREA    RESET, DATA, READONLY
        EXPORT  __Vectors
__Vectors
        DCD     0x10001800     ; Dirección de inicio del stack
        DCD     Reset_Handler  ; Vector de reset

        ALIGN
        AREA    MI_PROGRAMA, CODE, READONLY
        EXPORT  Reset_Handler
Reset_Handler
        LDR     R0, =0x10000010
        LDR     R1, =0x10000000
        LDR     R2, =0x00001FFE
loop
        LDRB    R3, [R0]
        ADDS    R0, #1
        SUBS    R2, #1
        CMP     R3, #0
        BEQ     forever
        CMP     R2, #0
        BEQ     forever
        CMP     R3, R4
        BHS     mayor
        B       loop
mayor
        MOVS    R4, R3
        STRB    R4, [R1]
        B       loop
forever
        B       forever

        END