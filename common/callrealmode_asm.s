    
    SEG_SEL_CODE_REAL   = 0x0000
    SEG_SEL_DATA_REAL   = 0x0000
    CALLREALMODE_OFFSET = 0x5000
    
    CALLREALMODE_FUNC_GETSYSMEMMAP = 0x0

    SEG_SEL_CODE32 = (1 * 8)
    SEG_SEL_DATA32 = (2 * 8)
    SEG_SEL_CODE16 = (3 * 8)
    SEG_SEL_DATA16 = (4 * 8)

    .globl callrealmode_start
    .globl callrealmode_end
    
    .text

    .code16        
callrealmode_start:
    push %ebp       
    mov  %sp,%bp

    call protection_off   
    call callrealmode_switch   
    call protection_on          
    
    mov  %bp,%sp    
    pop  %ebp  
    lretl           


callrealmode_switch:
    OFF_FUNC = 44  
                  

    # Which function?
    movw OFF_FUNC(%bp),%ax
    cmp  $CALLREALMODE_FUNC_GETSYSMEMMAP,%ax
    je   getsysmemmap
    ret

getsysmemmap:
    OFF_GETSYSMEMMAP_EBX      = 50 
    OFF_GETSYSMEMMAP_EBX_RET  = 54  
    OFF_GETSYSMEMMAP_E820DATA = 58  

    push %bp
    mov %sp,%bp

    stc         

    mov    %ss,%ax
    mov    %ax,%es
    lea    OFF_GETSYSMEMMAP_E820DATA(%bp),%di
    
    mov    $0xE820,%eax
    mov    $0x534D4150,%edx
    mov    %ss:OFF_GETSYSMEMMAP_EBX(%bp),%ebx 
    mov    $20,%ecx                             
    
    int    $0x15  

    mov    $1,%ax
    jc     1f     
    xor    %ax,%ax
1:
    # Save next %ebx
    mov    %ebx,%ss:OFF_GETSYSMEMMAP_EBX_RET(%bp)

    pop %bp
    ret

protection_on:
    cli

    lgdt saved_gdtr - callrealmode_start + CALLREALMODE_OFFSET
    lidt saved_idtr - callrealmode_start + CALLREALMODE_OFFSET

    mov %cr0,%ebx
    orl $0x1,%ebx
    mov %ebx,%cr0

    ljmp $SEG_SEL_CODE16,$prot_mode - callrealmode_start + CALLREALMODE_OFFSET

prot_mode: .code16
    mov $SEG_SEL_DATA16,%bx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %ss
    mov %bx, %fs
    mov %bx, %gs
    
    ret

    .code16
protection_off:
    sgdt saved_gdtr - callrealmode_start + CALLREALMODE_OFFSET
    sidt saved_idtr - callrealmode_start + CALLREALMODE_OFFSET

    mov %cr0,%ebx
    andl $0xFFFFFFFE,%ebx
    mov %ebx,%cr0

    ljmp $SEG_SEL_CODE_REAL,$real_mode - callrealmode_start + CALLREALMODE_OFFSET

real_mode:
    mov $SEG_SEL_DATA_REAL,%ax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    mov %ax,%ss

    lidt real_mode_idtr - callrealmode_start + CALLREALMODE_OFFSET
    sti
    
    ret
    
real_mode_idtr: 
    .word 0x3ff,0,0
saved_gdtr:    
    .word 0x0
    .long 0x0
saved_idtr:   
    .word 0x0
    .long 0x0
callrealmode_end:
