#ifndef LINKED_LIST_OBJECT_H
	#define LINKED_LIST_OBJECT_H

/*
	Developer: Benjamin Michaud
	Date: July 6, 2020
	File Name: LinkedList.h
	Version: 1.05
	IDE: Visual Studio 2019
	Compiler: C89

	Description: This file contains the definition of the LINKED_LIST
	data structure.  This file was created just to 
	further seperate out the structure of the library.

*/

#include "GenericTypeDefs.h"
#include "LinkedListConfig.h"

/*
	This struct is a node which comprises the LINKED_LIST.  It points
	to the data of the node, and the next and previous nodes.
*/
struct _LinkedListNode
{
	/*
		The data the the node will be pointing to.  This address is used
		to delete and compare data at nodes.
	*/
	void *Data;

	/*
		A pointer to the next node in the LINKED_LIST.  If no next node exists
		then this will point to NULL.
	*/
	struct _LinkedListNode *Next;

	/*
		A pointer to the previous node in the LINKED_LIST.  If no previous node exists
		then this will point to NULL.
	*/
	struct _LinkedListNode *Previous;
};

typedef struct _LinkedListNode LINKED_LIST_NODE;


#if (USING_LINKED_LIST_ITERATOR == 1)

/*
	This structure is used to iterate through a LINKED_LIST.  Different methods
	allow the insertion or removal of nodes based on the current index of the 
	ITERATOR.  When the ITERATOR starts off, it is pointing to NULL and has an
	index of 0.
*/
struct _Iterator
{
	// Attributes

	/*
		The current node the ITERATOR is pointing to.  If it has yet to iterate, then
		the ITERATOR will be pointing to NULL.
	*/
	LINKED_LIST_NODE *CurrentNode;		// A pointer to the current Node the ITERATOR is at.

	/*
		The current position of the ITERATOR.  When the LINKED_LIST is empty or just initialized
		or the ITERATOR has yet to be iterated, then this number will be 0.
	*/
	UINT32 Position;					// The numeric position the inside the LINKED_LIST the ITERATOR is at.
										// The first position the ITERATOR is at is considered to be Position 1.
};

typedef struct _Iterator ITERATOR;

#endif  // end of USING_LINKED_LIST_ITERATOR

/*
	The following is the LINKED_LIST structure itself.  This structure will hold all attributes
	of the LINKED_LIST, like the head and tail or CompareTo methods.  It will not hold the actual
	data.  That is done on a node by node basis.
*/
struct LinkedList
{
	// Attributes

	/*
		The number of nodes the LINKED_LIST contains.  0 if empty.
	*/
	UINT32 Size;					// The current Size of the LINKED_LIST.

	/*
		A pointer to the first node in the LINKED_LIST.  If no nodes exist, then this will
		point to NULL.
	*/
	LINKED_LIST_NODE *Head;			// A pointer to the beginning of the LINKED_LIST.

	/*
		A pointer to the last node in the LINKED_LIST.  If no nodes exists, then this will
		point to NULL.
	*/
	LINKED_LIST_NODE *Tail;			// A pointer to the end of the LINKED_LIST.

	/*
		The following will allow the use of an ITERATOR within a LINKED_LIST.  This can
		be achieved through the LinkedListConfig.h file.  When using ITERATORS, all LINKED_LIST's
		have an ITERATOR whether they use it or not is up to the user.
	*/
	#if (USING_LINKED_LIST_ITERATOR == 1)
		ITERATOR Iterator;				// An ITERATOR for the LINKED_LIST.
	#endif // end of USING_LINKED_LIST_ITERATOR

	// Methods

	/*
		The CompareTo method pointer is used to sort a linked list, or to find the
		max/min of a LINKED_LIST's stored data, or a certain piece of data.
		This must be implemented by the user if used.
	*/
	#if (USING_LINKED_LIST_SORTING == 1 || USING_LINKED_LIST_CONTAINS_METHOD == 1)
		INT32 (*CompareTo)(const void *Data1, const void *Data2);
		/*
			If Data2 is smaller than Data1 then this method should return > 0.
			If they are equal it should return 0.
			If Data2 is larger than Data1 then this method should return < 0.
		*/
	#endif // end of USING_LINKED_LIST_SORTING

	/*
		The following method is used to delete data that a LINKED_LIST_NODE points
		to.  This must be implemented by the user if used.  This method is useful if
		each LINKED_LIST_NODE points to data of the following:

		struct AStruct
		{
			BYTE *A_NULLTerminatedPointer;
			BYTE *AnotherNULLTerminatedPointer;
			struct MysteryStruct *MysteryStructPointer;
			UINT32 SillyNumberHolder;
		};

		The above structure could not properly be deleted with a simple call to 
		free or the user supplied free method.  This is where custom free methods
		come in handy so that there are no memory leaks from deleting data that a
		node points to.
	*/
	#if(USING_LINKED_LIST_DEPENDENT_FREE_METHOD == 1)
		void (*LinkedListFreeMethod)(void *Data);
	#endif // USING_LINKED_LIST_DEPENDENT_FREE_METHOD
};

typedef struct LinkedList LINKED_LIST;

#endif // end of LINKED_LIST_OBJECT_H
