#ifndef GENERIC_TYPE_DEFS_H
	#define GENERIC_TYPE_DEFS_H

/* get compiler defined type definitions (NULL, size_t, etc) */
#include <stddef.h> 

#define BOOL		INT32
#define FALSE		0
#define TRUE		!FALSE

#define PUBLIC                                  
#define PROTECTED
#define PRIVATE		static

/*
	char = 1
	short = 2
	int = 4
	long = 4
	long int = 4
	long long = 8
	float = 4
	double = 8
	long float = 8
	long double = 8
*/

typedef unsigned char		UBYTE;
typedef signed char			BYTE;

typedef unsigned short		UINT16;
typedef signed short		INT16;

typedef unsigned int		UINT32;
typedef signed int			INT32;

typedef unsigned long long	UINT64;
typedef signed long long	INT64;

typedef float				DECIMAL32;

typedef double				DECIMAL64;

#endif // end of GENERIC_TYPE_DEFS_H