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
        EORS    R1, R1
        MOVS    R3, #0X01
        LSLS    R4, R3, #0X05
loop
        LDRB    R2, [R0, R1]
        MOVS    R2, R2
        BEQ     forever
        CMP     R2, #' '
        BNE     no_space
        MOVS    R3, #0X01
        B       next
no_space
        TST     R3, R3
        BEQ     char_done
        CMP     R2, #'a'
        BLO     char_done
        CMP     R2, #'z'
        BHI     char_done
        EORS    R2, R4
        STRB    R2, [R0, R1]
char_done
        EORS    R3, R3
next
        ADDS    R1, #0X01
        B       loop
forever
        B       forever

        END