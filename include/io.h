//
//	Brik.OS Project
//	DevelopOS A1
//	
//	io.h
//
//	Brik Vadim @ Brik Inc.
//

#ifndef _IO_H
#define _IO_H

typedef u16 ioport_t;
typedef struct {
	ioport_t	port;
	union {
		u16	type;
		struct {
			u8	size;
			u8	dir;
		} __attribute__ ((packed));
	};
} core_io_t;

enum core_io_dir {
	CORE_IO_DIR_IN  = 0,
	CORE_IO_DIR_OUT = 1,
};

enum core_io_type {
	CORE_IO_TYPE_IN8   = 0x0001,
	CORE_IO_TYPE_IN16  = 0x0002,
	CORE_IO_TYPE_IN32  = 0x0004,
	CORE_IO_TYPE_OUT8  = 0x0101,
	CORE_IO_TYPE_OUT16 = 0x0102,
	CORE_IO_TYPE_OUT32 = 0x0104,
};

enum core_io_ret {
	CORE_IO_RET_DEFAULT,
	CORE_IO_RET_NEXT,
	CORE_IO_RET_DONE,	
	CORE_IO_RET_BLOCK,	
	CORE_IO_RET_INVALID,	
};
enum core_io_prio {
	CORE_IO_PRIO_HIGH,
	CORE_IO_PRIO_APPEND,
	CORE_IO_PRIO_EXCLUSIVE,
};

typedef int (*core_io_handler_t) (core_io_t ioaddr, union mem *data, void *arg);

static inline void in8 (ioport_t port, u8 *data) { 
	asm volatile ("inb %%dx, %%al" : "=a" (*data) : "d" (port)); 
}

static inline void in16 (ioport_t port, u16 *data) { 
	asm volatile ("inw %%dx, %%ax" : "=a" (*data) : "d" (port)); 
}

static inline void in32 (ioport_t port, u32 *data) { 
	asm volatile ("inl %%dx, %%eax" : "=a" (*data) : "d" (port)); 
}

static inline void ins8 (ioport_t port, u8 *buf, u32 count) { 
	asm volatile ("cld; rep insb" : "=c" (count), "=D" (buf)
		: "c" (count), "d" (port), "D" (buf)); 
}

static inline void ins16 (ioport_t port, u16 *buf, u32 count) { 
	asm volatile ("cld; rep insw" : "=c" (count), "=D" (buf)
		: "c" (count), "d" (port), "D" (buf)); 
}

static inline void ins32 (ioport_t port, u32 *buf, u32 count) { 
	asm volatile ("cld; rep insl" : "=c" (count), "=D" (buf)
		: "c" (count), "d" (port), "D" (buf)); 
}

static inline void insn (ioport_t port, void *buf, int unit_size, u32 total_size) {
	if (unit_size == 4)
		ins32 (port, buf, total_size/4);
	else if (unit_size == 2)
		ins16 (port, buf, total_size/2);
	else if (unit_size == 1)
		ins8 (port, buf, total_size);
}

static inline void out8 (ioport_t port, u8 data) { 
	asm volatile ("outb %%al, %%dx" : : "a" (data), "d" (port)); 
}

static inline void out16 (ioport_t port, u16 data) { 
	asm volatile ("outw %%ax, %%dx" : : "a" (data), "d" (port)); 
}

static inline void out32 (ioport_t port, u32 data) { 
	asm volatile ("outl %%eax, %%dx" : : "a" (data), "d" (port)); 
}

static inline void outs8 (ioport_t port, u8 *buf, u32 count) { 
	asm volatile ("cld; rep outsb" : "=c" (count), "=S" (buf)
		: "c" (count), "d" (port), "S" (buf)); 
}

static inline void outs16 (ioport_t port, u16 *buf, u32 count) { 
	asm volatile ("cld; rep outsw" : "=c" (count), "=S" (buf)
		: "c" (count), "d" (port), "S" (buf)); 
}

static inline void outs32 (ioport_t port, u32 *buf, u32 count) { 
	asm volatile ("cld; rep outsl" : "=c" (count), "=S" (buf)
		: "c" (count), "d" (port), "S" (buf)); 
}

static inline void outsn (ioport_t port, void *buf, int unit_size, u32 total_size) {
	if (unit_size == 4)
		outs32 (port, buf, total_size/4);
	else if (unit_size == 2)
		outs16 (port, buf, total_size/2);
	else if (unit_size == 1)
		outs8 (port, buf, total_size);
}

#endif
