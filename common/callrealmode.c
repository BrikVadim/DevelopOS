//
//	Brik.OS Project
//	DevelopOS A1
//	
//	callrealmode.c
//
//	Brik Vadim @ Brik Inc.
//

#include "printf.h"
#include "types.h"
#include "string.h"
#include "segment.h"
#include "callrealmode_asm.h"

static void callrealmode_Call(struct callrealmode_Data *p_param)
{
    u16 sp16;
    u32 sp32;
    

    memcpy ((u8*)CALLREALMODE_OFFSET, &callrealmode_start, 
            &callrealmode_end - &callrealmode_start);

    sp16 = CALLREALMODE_OFFSET - sizeof(*p_param);
    memcpy ((void*)(u32)sp16, p_param, sizeof(*p_param));
    
    __asm__ volatile (
        "mov %%esp,%0\n"    // Сохраняем ресистр ESP в переменную sp32
        "mov %1,%%ds \n"    // Загружаем 16ти битный селектор данных в 
        "mov %1,%%es \n"    // регистры DS, ES, FS, GS, SS
        "mov %1,%%fs \n"    //
        "mov %1,%%gs \n"    //
        "mov %1,%%ss \n"    //
        "mov %2,%%esp\n"    // Переходим на 16ти битный стек по адресу sp16
        "pusha       \n"    // Сохраняем регистры общего назначения
        "lcall %3,%4 \n"    // Загружаем 16ти битный селектор кода в CS 
                            // и переходим на адрес CALLREALMODE_OFFSET.
                            // На стеке сохраняется текущий CS и EIP,
                            // которые будут использованны инструкцией 
                            // lretl для возврата в 32х битный код
        "popa        \n"    // востанавливаем сохраненные регистры общего назначения
        "mov %5,%%ds \n"    // Загружаем 32х битный селектор данных в 
        "mov %5,%%es \n"    // регистры DS, ES, FS, GS, SS
        "mov %5,%%fs \n"    //
        "mov %5,%%gs \n"    //
        "mov %5,%%ss \n"    //
        "mov %0,%%esp\n"    // Переходим на 32х битный стек, адрес
                            // которого сохранен в переменной sp32
        : "=&a" (sp32)              // %0 – Input
        : "b" ((u32)SEG_SEL_DATA16) // %1 - Output
        , "c" ((u32)sp16)           // %2 - Output
        , "i" ((u32)SEG_SEL_CODE16) // %3 - Output
        , "i" (CALLREALMODE_OFFSET) // %4 - Output
        , "d" ((u32)SEG_SEL_DATA32) // %5 - Output
        );

    memcpy (p_param, (void*)(u32)sp16, sizeof(*p_param));
}

u64 GetAvalibleRAMSize()
{
    struct callrealmode_Data param; 
    u64 avalible_ram_sz = 0;

    param.func = CALLREALMODE_FUNC_GETSYSMEMMAP;
    param.getsysmemmap.next_num = 0;
    do
    {
        param.getsysmemmap.num = param.getsysmemmap.next_num;
        callrealmode_Call(&param);  
                
        if (SYSMEMMAP_TYPE_AVAILABLE == param.getsysmemmap.type)
        {
            avalible_ram_sz += param.getsysmemmap.len;
        }
        
        printf("n 0x%08X nn 0x%08X b 0x%08llX l 0x%08llX(%lldMb) t 0x%08X\n",
            param.getsysmemmap.num, 
            param.getsysmemmap.next_num,
            param.getsysmemmap.base, 
            param.getsysmemmap.len, 
            param.getsysmemmap.len / 0x100000, 
            param.getsysmemmap.type);
    }
    while (param.getsysmemmap.next_num);

    return avalible_ram_sz;
}
