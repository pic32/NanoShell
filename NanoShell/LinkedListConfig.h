#ifndef LINKED_LIST_CONFIG_H
	#define LINKED_LIST_CONFIG_H

/*
	Developer: Benjamin Michaud
	Date: July 6, 2020
	File Name: LinkedListConfig.h
	Version: 1.05
	IDE: Visual Studio 2019
	Compiler: C89

	Description:  This file contains all the necessary definitions
	for configuring the Linked List Library.

*/

/*
	The following defines are for the core methods of manipulating
	a Linked List.
*/
#define USING_LINKED_LIST_ADD_METHOD					1
#define USING_LINKED_LIST_ADD_LAST_METHOD				1
#define USING_LINKED_LIST_ADD_FIRST_METHOD				0
#define USING_LINKED_LIST_REMOVE_NODE_METHOD			0
#define USING_LINKED_LIST_DELETE_NODE_METHOD			1
#define USING_LINKED_LIST_DELETE_ALL_METHOD				0
#define USING_LINKED_LIST_GET_DATA_METHOD				1
#define USING_LINKED_LIST_CONTAINS_METHOD				0
#define USING_LINKED_LIST_GET_SIZE_METHOD				1

/*
	Linked List sorting functionality and methods.
*/
// The below define must be 1 if any Linked List sorting is used.
#define USING_LINKED_LIST_SORTING						0

/*
	*Define the below as 1 if each LINKED_LIST is to store a custom
	method for freeing a LINKED_LIST_NODE.  This is useful when 
	deleting a node that has data that is referenced.  For instance,
	a node containing an object of type person.  Everything is fixed
	length except for a persons name.  If their name was allocated
	with a call to a malloc of sorts, then the nodes data gets erased
	the allocated memory for the persons name will not get erased and
	a memory leak is created.  This allows the user to specify a 
	method that will be called when deleting a node with such data.
	The user can first free the persons name, then the entire person
	object so no memory leaks are created.
*/
#define USING_LINKED_LIST_DEPENDENT_FREE_METHOD			1

/*
	If USING_SORT_LINKED_LIST_METHOD is defined as 1, then so must
	USING_ADD_TO_SORTED_LINKED_LIST_METHOD because LinkedListSort()
	makes multiple calls to LinkedListAddToSortedLinkedList().
*/
#define USING_SORT_LINKED_LIST_METHOD					0
#define USING_ADD_TO_SORTED_LINKED_LIST_METHOD			0

/*
	Linked List Iterator functionality and methods.
*/
// The below define must be 1 if any Linked List Iterator methods are used.
#define USING_LINKED_LIST_ITERATOR						0

#define USING_ITERATOR_HAS_NEXT_METHOD					0
#define USING_ITERATOR_REMOVE_METHOD					0
#define USING_ITERATOR_DELETE_METHOD					0
#define USING_ITERATOR_GET_DATA_METHOD					0
#define USING_ITERATOR_NEXT_METHOD						0
#define USING_ITERATOR_RESET_METHOD						0
#define USING_ITERATOR_HAS_BACK_METHOD					0
#define USING_ITERATOR_BACK_METHOD						0
#define USING_ITERATOR_GET_INDEX_METHOD					0

/*
	Linked List size in bytes method.
*/
#define USING_LINKED_LIST_GET_SIZE_IN_BYTES_METHOD		0

/*
	*If USING_LINKED_LIST_GET_LIBRARY_VERSION is set to 1 then
	LinkedListGetLibraryVersion() is available.
*/
#define USING_LINKED_LIST_GET_LIBRARY_VERSION			0

/**
	*Set LINKED_LIST_SAFE_MODE to 1 to enable the portions of code
	inside the LINKED_LIST Library that check to make sure all passed
	in parameters are of a valid nature.
*/
#define LINKED_LIST_SAFE_MODE							1

/**
	*This defines what method the LinkedList library will use to 
	allocate memory and deallocated.  This allows the LinkedList 
	library to be integrated into an OS.
*/
#define ListMemAlloc(Mem)								malloc(Mem)

/**
	*The method used for freeing up memory.
*/
#define ListMemDealloc(Mem)								free(Mem)

/**
	*Define USE_MALLOC as 1 to enable the stdlib.h file included
	with LinkedList.c.
*/
#define LINKED_LIST_USE_MALLOC							1

/**
	*If the user isn't using malloc then include the file that will
	be needed for accessing whatever method ListMemAlloc() and ListMemDealloc()
	implement, below.
*/
//#include "Your memory file here.h"

#endif // end of LINKED_LIST_CONFIG_H
