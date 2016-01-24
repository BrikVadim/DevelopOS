//
//	Brik.OS Project
//	DevelopOS A1
//	
//	string.h
//
//	Brik Vadim @ Brik Inc.
//

#ifndef __CORE_STRING_H
#define __CORE_STRING_H

#include "types.h"

#define USE_BUILTIN_STRING

static inline void *
memset_slow (void *addr, int val, int len)
{
	char *p;

	p = addr;
	while (len--)
		*p++ = val;
	return addr;
}

static inline void *
memcpy_slow (void *dest, void *src, int len)
{
	char *p, *q;

	p = dest;
	q = src;
	while (len--)
		*p++ = *q++;
	return dest;
}

static inline int
strcmp_slow (char *s1, char *s2)
{
	int r, c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;
		r = c1 - c2;
	} while (!r && c1);
	return r;
}

static inline int
memcmp_slow (void *p1, void *p2, int len)
{
	int r, i;
	char *q1, *q2;

	q1 = p1;
	q2 = p2;
	for (r = 0, i = 0; !r && i < len; i++)
		r = *q1++ - *q2++;
	return r;
}

static inline int
strlen_slow (char *p)
{
	int len = 0;

	while (*p++)
		len++;
	return len;
}

#ifdef USE_BUILTIN_STRING
#	define memset(addr, val, len)	memset_builtin (addr, val, len)
#	define memcpy(dest, src, len)	memcpy_builtin (dest, src, len)
#	define strcmp(s1, s2)		strcmp_builtin (s1, s2)
#	define memcmp(p1, p2, len)	memcmp_builtin (p1, p2, len)
#	define strlen(p)		strlen_builtin (p)
#else  
#	define memset(addr, val, len)	memset_slow (addr, val, len)
#	define memcpy(dest, src, len)	memcpy_slow (dest, src, len)
#	define strcmp(s1, s2)		strcmp_slow (s1, s2)
#	define memcmp(p1, p2, len)	memcmp_slow (p1, p2, len)
#	define strlen(p)		strlen_slow (p)
#endif 

#ifdef USE_BUILTIN_STRING
static inline void *
memset_builtin (void *addr, int val, int len)
{
	return __builtin_memset (addr, val, len);
}

static inline void *
memcpy_builtin (void *dest, void *src, int len)
{
	return __builtin_memcpy (dest, src, len);
}

static inline int
strcmp_builtin (char *s1, char *s2)
{
	return __builtin_strcmp (s1, s2);
}

static inline int
memcmp_builtin (void *p1, void *p2, int len)
{
	return __builtin_memcmp (p1, p2, len);
}

static inline int
strlen_builtin (char *p)
{
	return __builtin_strlen (p);
}
#endif /* USE_BUILTIN_STRING */

#endif
