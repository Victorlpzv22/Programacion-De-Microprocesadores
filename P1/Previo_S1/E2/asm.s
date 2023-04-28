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
        EORS    R2, R2
loop
        SUBS    R1, #1
        BMI     exit
        LDRB    R3, [R0, R1]
        LSRS    R3, #1
        BCC     loop
        ADDS    R2, #1
        B       loop
exit
        MOVS    R1, #0x08
        STRB    R2, [R0, R1]
forever
        B       forever

        END