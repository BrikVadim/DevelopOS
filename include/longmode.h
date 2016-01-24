//
//	Brik.OS Project
//	DevelopOS A1
//	
//	longmode.h
//
//	Brik Vadim @ Brik Inc.
//

#ifdef AS

	.section .gomi
1:
	inc	%eax
2:

.if 2b-1b == 1
	longmode = 0
.else
	longmode = 1
.endif

#endif
