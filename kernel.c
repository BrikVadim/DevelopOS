//
//	Brik.OS Project
//	DevelopOS A1
//	
//	kernel.c
//
//	Brik Vadim @ Brik Inc.
//

#include "printf.h"
#include "screen.h"
#include "types.h"
#include "pci.h"
#include "desc.h"
#include "callrealmode.h"

struct segdesc g_GDT[5];

void SetupDescTables(struct segdesc *GDT_base);

int kernel_main(struct multiboot *mboot_ptr) {
	clear_screen();
	printf(">>> Brik.OS Project\n>>  DevelopOS A1 v0.1\n>Brik Vadim @ Brik Inc.\n\n");

	printf("PCI List:\n");
	PCIScan();
	
	printf("\nRAM List:\n");
	SetupDescTables(g_GDT);
    
	u64 ram_size = GetAvalibleRAMSize ();
	printf("\nram_size = %llu(%lluMb)\n", ram_size, (ram_size / 0x100000) + 1);
	printf("Hallo, Welt!");
	return 0xDEADBABA;
}
