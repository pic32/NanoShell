/*
	Developer: Benjamin Michaud
	Date: January 17, 2022
	File Name: GenericBufferConfig.h
	Version: 1.07
	IDE: Visual Studio 2010 Professional
	Compiler: C89

	Description:
	This file configures the options built into the GenericBuffer Library.
	The below set of defines allows the user to enable certain methods
	located inside the GenericBuffer library.

	It also allows the user to define how the GenericBuffer library will
	allocate and deallocate memory for new nodes.

	All methods are thread safe(re-entrant), with the potential exception 
	of GenericBufferCreate, GenericBufferDelete.  The reason
	for this is that it all depends on how the user defines the way the 
	GenericBuffer library will allocate, deallocate memory.
*/

#ifndef GENERIC_BUFFER_CONFIG_H
	#define GENERIC_BUFFER_CONFIG_H

/**
	*Set USING_GENERIC_BUFFER_PEEK_METHOD to 1 to enable the
	GenericBufferPeek() method.
*/
#define USING_GENERIC_BUFFER_PEEK_METHOD					1

/**
*Set USING_GENERIC_BUFFER_CONTAINS_METHOD to 1 to enable the
GenericBufferContains() method.
*/
#define USING_GENERIC_BUFFER_CONTAINS_METHOD				0

/**
 Set USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD to 1 to 
 enable the GenericBufferContainsSequence() method.
*/
#define USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD		1

/**
 Set USING_GENERIC_BUFFER_SEEK_METHOD to 1 to enable the
 GenericBufferSeek() method.
*/
#define USING_GENERIC_BUFFER_SEEK_METHOD					0

/**
Set USING_GENERIC_REMOVE_LAST_BYTE_WRITTEN_METHOD to 1 to enable
GenericBufferRemoveLastByteWritten() method.
*/
#define USING_GENERIC_REMOVE_LAST_BYTE_WRITTEN_METHOD		1

/**
	*Set USING_GENERIC_BUFFER_FLUSH_METHOD to 1 to enable the
	GenericBufferFlush() method.
*/
#define USING_GENERIC_BUFFER_FLUSH_METHOD					1

/**
	*Set USING_GENERIC_BUFFER_GET_SIZE_METHOD to 1 to enable the
	GenericBufferGetSize() method.
*/
#define USING_GENERIC_BUFFER_GET_SIZE_METHOD				1

/**
	*Set USING_GENERIC_BUFFER_GET_CAPACITY_METHOD to 1 to enable the
	GenericBufferGetCapacity() method.
*/
#define USING_GENERIC_BUFFER_GET_CAPACITY_METHOD			0

/**
	*Set USING_GENERIC_BUFFER_DELETE_METHOD to 1 to enable the
	GenericBufferDelete() method.
*/
#define USING_GENERIC_BUFFER_DELETE_METHOD					0

/**
	*Set USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD to 1 to enable the
	GenericBufferGetRemainingBytes() method.
*/
#define USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD		0

/**
	*Set USING_GENERIC_BUFFER_GET_LIBRARY_VERSION to 1 to endable the 
	GenericBufferGetLibraryVersion() method.
*/

#define USING_GENERIC_BUFFER_GET_LIBRARY_VERSION			0

/**
	*Set GENERIC_BUFFER_SAFE_MODE to 1 to enable the portions of code
	inside the GENERIC_BUFFER Library that check to make sure all passed
	in parameters are of a valid nature.
*/
#define GENERIC_BUFFER_SAFE_MODE							1

/**
	*This defines what method the GenericBuffer library will use to 
	allocate memory and deallocate memory.  This allows the GenericBuffer 
	library to be integrated into an OS.
*/
#define BufferMemAlloc(Mem)									malloc(Mem)

/**
	*The method used for freeing up memory.
*/
#define BufferMemDealloc(Mem)								free(Mem)

/**
	*Define USE_MALLOC as 1 to enable the stdlib.h file included
	with GenericBuffer.c.
*/
#define GENERIC_BUFFER_USE_MALLOC							1

/**
	*If the user isn't using malloc then include the file that will
	be needed for accessing whatever method MemAlloc() and MemDealloc()
	implement, below.
*/
//#include "Your memory file here.h"

#endif // end of GENERIC_BUFFER_CONFIG_H
