/*
	Developer: Benjamin Michaud
	Date: January 17, 2022
	File Name: GenericBufferObject.h
	Version: 1.07
	IDE: Visual Studio 2010 Professional
	Compiler: C89

	Description:
	This file contains the definition of the GENERIC_BUFFER
	data structure.  This file was created just to 
	further seperate out the structure of the library.
*/

#ifndef GENERIC_BUFFER_OBJECT_H
	#define GENERIC_BUFFER_OBJECT_H

#include "GenericTypeDefs.h"

/*
	The following struct is the definition
	of the GENERIC_BUFFER.  This holds all 
	the necessary data to manipulate the 
	GENERIC_BUFFER.
*/
struct _GenericBuffer
{
	/*
		The current write position of the GENERIC_BUFFER.  The location that
		this pointer points to is where the next byte that will be written to
		the GENERIC_BUFFER will be placed.
	*/
	BYTE *CurrentWritePosition;

	/*
		The current read position of the GENERIC_BUFFER.  The location that
		this pointer points to is where the next byte that will be read from
		the GENERIC_BUFFER.
	*/
	BYTE *CurrentReadPosition;

	/*
		This variable denotes how full the GENERIC_BUFFER is in bytes.  This
		value cannot be larger than BufferCapacity.
	*/
	UINT32 BufferSize;

	/*
		This variable denotes how many bytes at most a GENERIC_BUFFER can hold.
	*/
	UINT32 BufferCapacity;

	/*
		This is a pointer to the area in RAM that will actually hold the data
		that the GENERIC_BUFFER can write and read.
	*/
	BYTE *Buffer;
};

typedef struct _GenericBuffer GENERIC_BUFFER;


#endif // end of GENERIC_BUFFER_OBJECT_H
