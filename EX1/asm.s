        AREA    RESET, DATA, READONLY
        EXPORT  __Vectors
__Vectors
        DCD     0x10001800     ; Dirección de inicio del stack
        DCD     Reset_Handler  ; Vector de reset

        ALIGN
        AREA    MI_PROGRAMA, CODE, READONLY
        EXPORT  Reset_Handler
Reset_Handler
        LDRB    R6, [R5]
        ADDS    R7, R5, R6
        SUBS    R6, #1
        STRB    R6, [R5]
        ADDS    R5, #1
fun_loop
        LDRB    R0, [R5]
        LDRB    R2, [R5, #1]
        SUBS    R0, R2
        BPL     fun_reverse
        RSBS    R0, R0, #0
fun_reverse
        STRB    R0, [R5]
        ADDS    R5, #1
        CMP     R5, R7
        BNE     fun_loop
        MOVS    R5, R6
forever
        B       forever
        
        END