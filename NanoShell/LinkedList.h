#ifndef LINKED_LIST_H
	#define LINKED_LIST_H
/*
	Developer: Benjamin Michaud
	Date: July 6, 2020
	File Name: LinkedList.h
	Version: 1.05
	IDE: Visual Studio 2019
	Compiler: C89

	Description:
		This file contains all the the method headers for interfacing to the
		Linked List library.  This should be included in any file that references
		a LINKED_LIST.  All methods and macro's have their documentation above
		their declaration.
*/

/*! \mainpage Linked List Library
 *  \brief This is a Library written in C for manipulating a Linked List Data Structure.
 *  \author Benjamin Michaud
 *  \version 1.05
 *  \date   July 6, 2020
 */

#include "GenericTypeDefs.h"
#include "LinkedListObject.h"
#include "LinkedListConfig.h"

/*
	July 6, 2020 v1.05
	  - LinkedListAddFirst() method was tested and confirmed.

	July 2, 2020 v1.04.01
	  - LinkedListAddFirst() method was added but not yet tested.

	Future Anticipated Release Notes:
	  - Methods to merge 2 LINKED_LIST's.
	  - Methods to partition a LINKED_LIST. (Based on either the Iterator or an Index)
	  - Method to sort a LINKED_LIST when merging a new LINKED_LIST.
	  - Method to add a new LINKED_LIST_NODE at a specified Index.
	  - Methods for returning the min and max value held in the LINKED_LIST.
	  - Method for peeking the next LINKED_LIST_NODE based on the current Index of the Iterator. (use -,0,+)
	  - Method for peeking the previous LINKED_LIST_NODE based on the current Index of the Iterator.
	  - Method for the Iterator to iterate to the next occurence.
	  - Method for the Iterator to iterate to the previous occurence.
	  - Method for the Iterator to delete the current node it's at.
	  - Method for getting the number of nodes left infront of the Iterator, backwards too.
	  - Method for sorting the list in ascending or descending order.
	  - Method for getting the index of the last occurence of data.
	  - Methods for List Dump and Restore.
*/


/*
	Function: LINKED_LIST *CreateLinkedList(LINKED_LIST *LinkedListToCreate, INT32 (*CompareTo)(const void *Data1, const void *Data2), void (*CustomFreeMethod)(void *Data))

	Parameters: 
		LINKED_LIST *LinkedList - The address at which the LINKED_LIST will be inititalized.
		If NULL is passed in then this method will create a LINKED_LIST out of a
		the heap with a call to MemAlloc()(User Supplied Define).

		INT32 (*CompareTo)(const void *Data1, const void *Data2) - A pointer to a method that
		the user defines for comparing items in the LINKED_LIST.  This method is called 
		everytime the user calls any sorting routine for the LINKED_LIST or a contains method.
		Essentially the method in its easiest implementation would be return Data1 - Data2.

		void(*CustomFreeMethod)(void *Data) - A pointer to a method that the
		user defines.  Each time a LINKED_LIST_NODE is deleted this method will
		be called to delete the data at the LINKED_LIST_NODE.

	Returns:
		LINKED_LIST* - The address at which the newly initialized LINKED_LIST resides
		in memory.  If a new LINKED_LIST could not be created then (LINKED_LIST*)NULL is returned.

	Description: Creates a new LINKED_LIST in memory.

	Notes: None
*/
/**
		* @brief Initializes a LINKED_LIST, and can create a LINKED_LIST.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST or a NULL LINKED_LIST 
		pointer to create a LINKED_LIST from MemAlloc().
		* @param *CompareTo - A pointer to a method that will be called when the LINKED_LIST
		does any kind of sorting.
		* @param *CustomFreeMethod - A method called by the LINKED_LIST Library to delete
		a piece of data that a LINKED_LIST_NODE points to.
		* @return *LINKED_LIST - The address of the LINKED_LIST in memory.  If a LINKED_LIST could
		not be allocated, returns a NULL LINKED_LIST pointer.
		* @note None
		* @sa MemAlloc()
		* @since v1.0
*/
LINKED_LIST *CreateLinkedList(LINKED_LIST *LinkedListToCreate, INT32 (*CompareTo)(const void *Data1, const void *Data2), void (*CustomFreeMethod)(void *Data));

/*
	Function: BOOL LinkedListAddFirst(LINKED_LIST *LinkedList, const void *Data)

	Parameters:
		LINKED_LIST *LinkedList - The LINKED_LIST that will have the data added to it.

		const void *Data - The data that will be stored at the new node in the LINKED_LIST.

	Returns:
		BOOL - Returns TRUE if the data was added successfully.  FALSE otherwise.

	Description: Adds a new item to the beginning of a LINKED_LIST.  This method calls MemAlloc() to
		create a new LINKED_LIST_NODE.

	Notes: USING_LINKED_LIST_ADD_FIRST_METHOD must be defined as 1 in LinkedListConfig.h to
		use this method.
*/
/**
		* @brief Adds a new item to the beginning of a LINKED_LIST at the beginning (index 1).
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @param *Data - The data that will be added to the LINKED_LIST.
		* @return BOOL - TRUE if the operation was successfull.  FALSE otherwise.
		* @note USING_LINKED_LIST_ADD_FIRST_METHOD must be defined as 1 in LinkedListConfig.h to
		use this method.  This method will call MemAlloc() to create a new LINKED_LIST_NODE.
		* @sa MemAlloc()
		* @since v1.04.01
*/
#if	(USING_LINKED_LIST_ADD_FIRST_METHOD == 1)
	BOOL LinkedListAddFirst(LINKED_LIST* LinkedList, const void* Data);
#endif // end of #if	(USING_LINKED_LIST_ADD_FIRST_METHOD == 1)

/*
	Function: BOOL LinkedListAddLast(LINKED_LIST *LinkedList, const void *Data)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will have the data added to it.

		const void *Data - The data that will be stored at the new node in the LINKED_LIST.

	Returns:
		BOOL - Returns TRUE if the data was added successfully.  FALSE otherwise.

	Description: Adds a new item to the LINKED_LIST end of the LINKED_LIST.  This method calls
		MemAlloc() to create a new LINKED_LIST_NODE.

	Notes: USING_LINKED_LIST_ADD_METHOD or USING_LINKED_LIST_ADD_LAST_METHOD must be defined 
		as 1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief Adds a new item to the end of a LINKED_LIST.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @param *Data - The data that will be added to the LINKED_LIST.
		* @return BOOL - TRUE if the operation was successfull.  FALSE otherwise.
		* @note USING_LINKED_LIST_ADD_METHOD or USING_LINKED_LIST_ADD_LAST_METHOD must 
		* be defined as 1 in LinkedListConfig.h to use this method.  This method will 
		* call MemAlloc() to create a new LINKED_LIST_NODE.
		* @sa MemAlloc()
		* @since v1.00
*/
#if (USING_LINKED_LIST_ADD_METHOD == 1 || USING_LINKED_LIST_ADD_LAST_METHOD == 1)
	#define LinkedListAdd(LinkedList, Data)									LinkedListAddLast(LinkedList, Data)

	// Tries to add the data specified by void *Data to the end of the LINKED_LIST.
	BOOL LinkedListAddLast(LINKED_LIST *LinkedList, const void *Data);
#endif // end of USING_LINKED_LIST_ADD_METHOD

/*
	Function: void *LinkedListRemoveNode(LINKED_LIST *LinkedList, UINT32 Index)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose LINKED_LIST_NODE will be removed.

		UINT32 Index - The 1 based index at which the LINKED_LIST_NODE will be removed.

	Returns:
		void* - Returns the data stored at the LINKED_LIST_NODE that was removed.  NULL otherwise.

	Description: Deletes a LINKED_LIST_NODE from the LINKED_LIST and returns the data.  It is 
		the users responsibility to free anything that the LINKED_LIST_NODE was pointing to.

	Notes: USING_LINKED_LIST_REMOVE_NODE_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.
*/
/**
		* @brief Removes and returns the data at the specified node.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @param Index - The 1 based index at which node will be removed.
		* @return void* - The value at the specified node.  NULL otherwise.
		* @note USING_LINKED_LIST_REMOVE_NODE_METHOD must be defined as 1 in LinkedListConfigto
		use this method.h.  This method will call MemDealloc() to delete the LINKED_LIST_NODE.  
		It is the users responsibility to free any memory that the returned void pointer is 
		pointing to.
		* @sa MemDealloc()
		* @since v1.00
*/
#if (USING_LINKED_LIST_REMOVE_NODE_METHOD == 1)
	// Removes and returns the specified Node in the LINKED_LIST.
	void *LinkedListRemoveNode(LINKED_LIST *LinkedList, UINT32 Index);

#define LINKED_LIST_NODE_REMOVAL					1
#define LINKED_LIST_ITERATOR_CHECK					1

#endif // end of USING_LINKED_LIST_REMOVE_NODE_METHOD

/*
	Function: BOOL LinkedListDeleteNode(LINKED_LIST *LinkedList, UINT32 Index)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will have the node and its data deleted from it.

		UINT32 Index - The 1 based index at which the LINKED_LIST_NODE will be deleted.  

	Returns:
		BOOL - Returns TRUE if the LINKED_LIST_NODE and its data was successfully deleted.  FALSE otherwise.

	Description: Deletes a LINKED_LIST_NODE from the LINKED_LIST and deletes the data that the
		deleted node was pointing at.

	Notes: USING_LINKED_LIST_DELETE_NODE_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.  If the LINKED_LIST already has a method for deleteing data then this 
		method will automatically get called.
*/
/**
		* @brief Deletes a LINKED_LIST_NODE and the data at the specified node.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @param Index - The 1 based index at which node will be deleted.
		* @return BOOL - Returns TRUE if the LINKED_LIST_NODE that was successfully deleted.  FALSE otherwise.
		* @note USING_LINKED_LIST_DELETE_NODE_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.  If the LINKED_LIST already has a method for deleteing data then this 
		method will automatically get called.
		* @sa MemDealloc()
		* @since v1.00
*/
#if (USING_LINKED_LIST_DELETE_NODE_METHOD == 1)
	BOOL LinkedListDeleteNode(LINKED_LIST *LinkedList, UINT32 Index);

#define LINKED_LIST_NODE_REMOVAL					1
#define LINKED_LIST_ITERATOR_CHECK					1
#define LINKED_LIST_FREE_METHOD						1

#endif // end of USING_LINKED_LIST_DELETE_NODE_METHOD

/*
	Function: BOOL LinkedListDeleteAll(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will have all the nodes and data deleted from it.
	
	Returns:
		BOOL - Returns TRUE if the all the LINKED_LIST_NODEs and their data were successfully deleted.  
			FALSE otherwise.  If this method returns FALSE, no LINKED_LIST_NODE's inside the LINKED_LIST
			were deleted.

	Description: Deletes all the LINKED_LIST_NODE's from the LINKED_LIST and deletes the data that the
		deleted LINKED_LIST_NODE's were pointing at.

	Notes: USING_LINKED_LIST_DELETE_ALL_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.  If the LINKED_LIST already has a method for deleteing data then this 
		method will automatically get called.
*/
/**
		* @brief Deletes all LINKED_LIST_NODE's and the data each one point to.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @return BOOL - Returns TRUE if all the LINKED_LIST_NODE's and their data were successfully 
		deleted.  FALSE otherwise.
		* @note USING_LINKED_LIST_DELETE_ALL_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.  If the LINKED_LIST already has a method for deleteing data then this 
		method will automatically get called.
		* @sa MemDealloc()
		* @since v1.0
*/
#if (USING_LINKED_LIST_DELETE_ALL_METHOD == 1)
	// Deletes all Nodes inside the LINKED_LIST.
	BOOL LinkedListDeleteAll(LINKED_LIST *LinkedList);

#define LINKED_LIST_ITERATOR_CHECK					1
#define LINKED_LIST_FREE_METHOD						1

#endif // end of USING_LINKED_LIST_REMOVE_ALL_METHOD

/*
	Function: void *LinkedListGetData(LINKED_LIST *LinkedList, UINT32 Index)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose LINKED_LIST_NODE will have it's data
			returned.

		UINT32 Index - The 1 based index at which the LINKED_LIST_NODE will have it's data 
			returned.

	Returns:
		void* - Returns the data stored at the LINKED_LIST_NODE.  NULL otherwise.

	Description: Returns a LINKED_LIST_NODE's data from the LINKED_LIST.

	Notes: USING_LINKED_LIST_GET_DATA_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.
*/
/**
		* @brief Returns the data at the specified node.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @param Index - The 1 based index at which node will have it's data returned.
		* @return void* - The value at the specified node.  NULL otherwise.
		* @note USING_LINKED_LIST_GET_DATA_METHOD must be defined as 1 in LinkedListConfig.h to
		use this method.
		* @sa None
		* @since v1.00
*/
#if (USING_LINKED_LIST_GET_DATA_METHOD == 1)
	// Returns the data at the specified index, otherwise (void*)NULL
	void *LinkedListGetData(LINKED_LIST *LinkedList, UINT32 Index);
#endif // end of USING_LINKED_LIST_GET_DATA_METHOD

/*
	Function: UINT32 LinkedListContains(LINKED_LIST *LinkedList, const void *Data, BOOL MultipleOccurences, UINT32 ReturnOnOccurence)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose contents will be searched.

		const void *Data - A pointer to the data that will be passed into the LINKED_LIST's 
			CompareTo routine.

		BOOL MultipleOccurences - Used to count how many times void *Data occures in the LINKED_LIST.

		UINT32 ReturnOnOccurence - When LinkedListContains() finds the ReturnOnOccurence'th time that
			void *Data is found, it will return the index of the ReturnOnOccurence'th instance of void *Data.

	Returns:
		UINT32 - MultipleOccurences = FALSE;
			ReturnOnOccurence = 0;

			This method will return the Index of the first match of Data.

			MultipleOccurences = TRUE;
			ReturnOnOccurence = 0;

			This method will return how many times it found (void*)Data in the LINKED_LIST.

			MultipleOccurences = FALSE;
			ReturnOnOccurence > 0;

			This method will return the Index of the ReturnOnOccurence'th time it found (void*)Data in the LINKED_LIST.
			If there are less instances of void *Data than ReturnOnOccurence is requesting, then this method will
			return 0.

			MultipleOccurences = TRUE;
			ReturnOnOccurence > 0;

			Invalid Input, returns 0.

	Description: Returns how many of a certain data exist in a LINKED_LIST, or the index of the
		last occurence of a certain piece of data, or the index of the first occurence of a piece of 
		data.

	Notes: USING_LINKED_LIST_CONTAINS_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.  The LINKED_LIST must have been initialized with a CompareTo method in
		order for it to be searched.
*/
/**
		* @brief Returns how many of a certain data exist in a LINKED_LIST, or the index of the
		last occurence of a certain piece of data, or the index of the first occurence of a piece of 
		data.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @param *Data - A pointer to the data that will be sent to the CompareTo method.
		* @param MultipleOccurences - Returns how many times const void *Data occured in the LINKED_LIST.
		* @param ReturnOnOccurence - Returns the index of the ReturnOnOccurence'th time that
		const void *Data was in the LINKED_LIST.  If there are not ReturnOnOccurence instances
		of const void *Data, then this method will return 0.
		* @return UINT32 - MultipleOccurences = FALSE;
		ReturnOnOccurence = 0;

		This method will return the Index of the first match of Data.

		MultipleOccurences = TRUE;
		ReturnOnOccurence = 0;

		This method will return how many times it found (void*)Data in the LINKED_LIST.

		MultipleOccurences = FALSE;
		ReturnOnOccurence > 0;

		This method will return the Index of the ReturnOnOccurence'th time it found (void*)Data in the LINKED_LIST.
		If there are less instances of void *Data than ReturnOnOccurence is requesting, then this method will
		return 0.

		MultipleOccurences = TRUE;
		ReturnOnOccurence > 0;

		Invalid Input, returns 0.
		* @note USING_LINKED_LIST_GET_DATA_METHOD must be defined as 1 in LinkedListConfig.h to
		use this method.  The LINKED_LIST must have been initialized with a CompareTo method when
		being created.
		* @sa CreateLinkedList()
		* @since v1.00
*/
#if (USING_LINKED_LIST_CONTAINS_METHOD == 1)
	// Returns the position of the specified data if it exists, 0 otherwise
	UINT32 LinkedListContains(LINKED_LIST *LinkedList, const void *Data, BOOL MultipleOccurences, UINT32 ReturnOnOccurence);
#endif // end of USING_LINKED_LIST_CONTAINS_METHOD

/*
	Function: UINT32 LinkedListGetSize(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose LINKED_LIST_NODE will have it's size
			returned.

	Returns:
		UINT32 - Returns the current size of the LINKED_LIST.  0 otherwise.

	Description: Returns the size of a LINKED_LIST.

	Notes: USING_LINKED_LIST_GET_SIZE_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.
*/
/**
		* @brief Returns the size of a LINKED_LIST.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @return UINT32 - The current size of a LINKED_LIST.  0 otherwise.
		* @note USING_LINKED_LIST_GET_SIZE_METHOD must be defined as 1 in LinkedListConfig.h to
		use this method.
		* @sa None
		* @since v1.00
*/
#if (USING_LINKED_LIST_GET_SIZE_METHOD == 1)
	// Returns the current size of the LINKED_LIST.
	UINT32 LinkedListGetSize(LINKED_LIST *LinkedList);
#endif // end of USING_LINKED_LIST_GET_SIZE_METHOD

#if (USING_LINKED_LIST_SORTING == 1)

/*
	Function: BOOL LinkedListSort(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will be sorted.

	Returns:
		BOOL - Returns TRUE if the LINKED_LIST was successfully sorted.  FALSE otherwise.

	Description: Sorts a LINKED_LIST in ascending order.

	Notes: USING_SORT_LINKED_LIST_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.  The LINKED_LIST's Iterator is reset upon successful sorting of the
		LINKED_LIST.  The LINKED_LIST must have been initialized with a CompareTo method in
		order for it to be sorted.
*/
/**
		* @brief Sorts a LINKED_LIST in ascending order.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @return BOOL - Returns TRUE if the LINKED_LIST was successfully sorted.  FALSE otherwise.
		* @note USING_SORT_LINKED_LIST_METHOD must be defined as 1 in LinkedListConfig.h to
		use this method.  The LINKED_LIST's Iterator is reset upon successful sorting of the
		LINKED_LIST.  The LINKED_LIST must have been initialized with a CompareTo method in
		order for it to be sorted.
		* @sa CreateLinkedList()
		* @since v1.00
*/
#if (USING_SORT_LINKED_LIST_METHOD == 1)
	// Sorts the LINKED_LIST based on the CompareTo method the user supplies
	BOOL LinkedListSort(LINKED_LIST *LinkedList);	
#endif // end of USING_SORT_LINKED_LIST_METHOD

/*
	Function: BOOL LinkedListAddToSortedLinkedList(LINKED_LIST *LinkedList, const void *Data)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will be sorted.
		const void *Data - The data to add to the sorted LINKED_LIST.

	Returns:
		BOOL - Returns TRUE if the data was successfully added.  FALSE otherwise.

	Description: Adds a node of data to the assumingly assorted LINKED_LIST.

	Notes: USING_ADD_TO_SORTED_LINKED_LIST_METHOD must be defined at 1 in LinkedListConfig.h to
		use this method.  The LINKED_LIST must have been initialized with a CompareTo
		method in order for it to be sorted.
*/
/**
		* @brief Adds a node to a LINKED_LIST that is assumingly sorted.
		* @param *LinkedList - A pointer to an already allocate LINKED_LIST.
		* @param *Data - A pointer to the data that will be added to the LINKED_LIST.
		* @return BOOL - Returns TRUE if the data was successfully added.  FALSE otherwise.
		* @note USING_ADD_TO_SORTED_LINKED_LIST_METHOD must be defined as 1 in LinkedListConfig.h to
		use this method.  The LINKED_LIST must have been initialized with a CompareTo method in
		order for it to be sorted.
		* @sa CreateLinkedList(), LinkedListSort()
		* @since v1.00
*/
#if (USING_ADD_TO_SORTED_LINKED_LIST_METHOD == 1)
	// Adds the specified data to a LINKED_LIST in a sorted manner.
	BOOL LinkedListAddToSortedLinkedList(LINKED_LIST *LinkedList, const void *Data);
#endif // end of USING_ADD_TO_SORTED_LINKED_LIST_METHOD

#endif // end of USING_LINKED_LIST_SORTING



#if (USING_LINKED_LIST_ITERATOR == 1)

/*
	Function: BOOL LinkedListIteratorHasNext(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will have it's ITERATOR check for a next node.

	Returns:
		BOOL - Returns TRUE if the ITERATOR has a node infront of it.  FALSE otherwise.

	Description: Checks to see if the ITERATOR of a LINKED_LIST has a node infront of it to iterate to.

	Notes: USING_ITERATOR_HAS_NEXT_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief Checks to see if the ITERATOR of a LINKED_LIST has a node infront of it to iterate to.
		* @param *LinkedList - The LINKED_LIST that will have it's ITERATOR check for a next node.
		* @return BOOL - Returns TRUE if the ITERATOR has a node infront of it.  FALSE otherwise.
		* @note USING_ITERATOR_HAS_NEXT_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa LinkedListIteratorNext()
		* @since v1.00
*/
#if (USING_ITERATOR_HAS_NEXT_METHOD == 1)
	// Returns TRUE if there is another Node next in the LINKED_LIST.
	BOOL LinkedListIteratorHasNext(LINKED_LIST *LinkedList);
#endif // end of USING_ITERATOR_HAS_NEXT_METHOD

/*
	Function: BOOL LinkedListIteratorDelete(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will have it's current ITERATOR node and data 
			deleted.

	Returns:
		BOOL - Returns TRUE if the ITERATOR's current node and data were deleted.  FALSE otherwise.

	Description: Attempts to delete the node and data the ITERATOR is currently pointing to.

	Notes: USING_ITERATOR_DELETE_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief Attempts to delete the node and data the ITERATOR is currently pointing to.
		* @param *LinkedList - The LINKED_LIST that will have it's current ITERATOR node and data deleted.
		* @return BOOL - Returns TRUE if the ITERATOR's current node and data were deleted.  FALSE otherwise.
		* @note USING_ITERATOR_DELETE_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa None
		* @since v1.00
*/
#if (USING_ITERATOR_DELETE_METHOD == 1)
	BOOL LinkedListIteratorDelete(LINKED_LIST *LinkedList);

#define LINKED_LIST_FREE_METHOD						1
#define LINKED_LIST_NODE_REMOVAL					1
#define LINKED_LIST_ITERATOR_CHECK					1

#endif // end of USING_ITERATOR_REMOVE_METHOD

/*
	Function: void *LinkedListIteratorRemove(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will have it's current ITERATOR node removed.

	Returns:
		BOOL - Returns TRUE if the ITERATOR's current node and data were deleted.  FALSE otherwise.

	Description: Attempts to delete the node and data the ITERATOR is currently pointing to.

	Notes: USING_ITERATOR_DELETE_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief Attempts to delete the node and data the ITERATOR is currently pointing to.
		* @param *LinkedList - The LINKED_LIST that will have it's current ITERATOR node removed.
		* @return BOOL - Returns TRUE if the ITERATOR's current node and data were deleted.  FALSE otherwise.
		* @note USING_ITERATOR_DELETE_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa None
		* @since v1.00
*/
#if (USING_ITERATOR_REMOVE_METHOD == 1)
	// Removes the current Node in the LINKED_LIST and returns the data from it.
	void *LinkedListIteratorRemove(LINKED_LIST *LinkedList);

#define LINKED_LIST_NODE_REMOVAL					1
#define LINKED_LIST_ITERATOR_CHECK					1

#endif // end of USING_ITERATOR_REMOVE_METHOD

/*
	Function: void *LinkedListIteratorGetData(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will it's current ITERATOR node data returned.

	Returns:
		void* - Returns the current data pointed to by the LINKED_LIST's ITERATOR.  (void*)NULL otherwise.

	Description: Returns the current data pointed to by the LINKED_LIST's ITERATOR.

	Notes: USING_ITERATOR_GET_DATA_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief Returns the current data pointed to by the LINKED_LIST's ITERATOR.
		* @param *LinkedList - The LINKED_LIST that will it's current ITERATOR node data returned.
		* @return void* - Returns the current data pointed to by the LINKED_LIST's ITERATOR.  (void*)NULL otherwise.
		* @note USING_ITERATOR_GET_DATA_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa None
		* @since v1.00
*/
#if (USING_ITERATOR_GET_DATA_METHOD == 1)
	void *LinkedListIteratorGetData(LINKED_LIST *LinkedList);
#endif // end of USING_ITERATOR_GET_DATA_METHOD

/*
	Function: void *LinkedListIteratorNext(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST that will it's current ITERATOR incremented and node data returned.

	Returns:
		void* - Returns the current data pointed to by the LINKED_LIST's ITERATOR after the iteration of the
			ITERATOR to the next node.  (void*)NULL otherwise.  If no next node exists, the ITERATOR remains
			unchanged.

	Description: Iterates the LINKED_LIST's ITERATOR, and then returns the data pointed to by the ITERATOR.

	Notes: USING_ITERATOR_NEXT_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief Iterates the LINKED_LIST's ITERATOR, and then returns the data pointed to by the ITERATOR.
		* @param *LinkedList - The LINKED_LIST that will it's current ITERATOR incremented and node data returned.
		* @return void* - Returns the current data pointed to by the LINKED_LIST's ITERATOR after the iteration of the
		ITERATOR to the next node.  (void*)NULL otherwise.  If no next node exists, the ITERATOR remains
		unchanged.
		* @note USING_ITERATOR_NEXT_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa LinkedListIteratorHasNext()
		* @since v1.00
*/
#if (USING_ITERATOR_NEXT_METHOD == 1)
	// Iterates the ITERATOR by one and returns the data at the new Node in the LINKED_LIST.
	void *LinkedListIteratorNext(LINKED_LIST *LinkedList);
#endif // end of USING_ITERATOR_NEXT_METHOD

/*
	Function: BOOL LinkedListIteratorReset(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose ITERATOR will be reset.

	Returns:
		BOOL - Returns TRUE if the operation was successfull, FALSE otherwise.

	Description: Returns the LINKED_LIST's ITERATOR to the beginning of the LINKED_LIST.  A call
		to LinkedListIteratorNext() will place the ITERATOR at node 1 of the LINKED_LIST.

	Notes: USING_ITERATOR_RESET_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief  Returns the LINKED_LIST's ITERATOR to the beginning of the LINKED_LIST.  A call
		to LinkedListIteratorNext() will place the ITERATOR at node 1 of the LINKED_LIST.
		* @param *LinkedList - The LINKED_LIST whose ITERATOR will be reset.
		* @return BOOL - Returns TRUE if the operation was successfull, FALSE otherwise.
		* @note USING_ITERATOR_RESET_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa None
		* @since v1.00
*/
#if (USING_ITERATOR_RESET_METHOD == 1)
	// Sets the ITERATOR to the beginning of the LINKED_LIST.
	BOOL LinkedListIteratorReset(LINKED_LIST *LinkedList);
#endif // end of USING_ITERATOR_RESET_METHOD

/*
	Function: BOOL LinkedListIteratorHasBack(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose ITERATOR will be checked to see if it has a
			node previous to it.

	Returns:
		BOOL - Returns TRUE if the operation was successfull, FALSE otherwise.

	Description: Checks to see if there is a node previous to the LINKED_LIST's ITERATOR.

	Notes: USING_ITERATOR_HAS_BACK_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief  Checks to see if there is a node previous to the LINKED_LIST's ITERATOR.
		* @param *LinkedList - The LINKED_LIST whose ITERATOR will be checked to see if it has a
		node previous to it.
		* @return void* - Returns TRUE if the operation was successfull, FALSE otherwise.
		* @note USING_ITERATOR_HAS_BACK_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa LinkedListIteratorBack()
		* @since v1.00
*/
#if(USING_ITERATOR_HAS_BACK_METHOD == 1)
	BOOL LinkedListIteratorHasBack(LINKED_LIST *LinkedList);
#endif // end of USING_ITERATOR_HAS_BACK_METHOD

/*
	Function: void *LinkedListIteratorBack(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose ITERATOR will be moved back 1 and data returned.

	Returns:
		void* - Returns the data of the new node, otherwise this returns (void*)NULL.

	Description: Iterates the LINKED_LIST's ITERATOR to the previous node if there is one, and then returns
		the data at the current index of the ITERATOR.  If there is no node behind the ITERATOR, this method
		will not change the status of the ITERATOR.

	Notes: USING_ITERATOR_BACK_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief  Iterates the LINKED_LIST's ITERATOR to the previous node if there is one, and then returns
		the data at the current index of the ITERATOR.  If there is no node behind the ITERATOR, this method
		will not change the status of the ITERATOR.
		* @param *LinkedList - The LINKED_LIST whose ITERATOR will be moved back 1 and data returned.
		* @return void* - Returns the data of the new node, otherwise this returns (void*)NULL.
		* @note USING_ITERATOR_BACK_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa LinkedListIteratorHasBack()
		* @since v1.00
*/
#if(USING_ITERATOR_BACK_METHOD == 1)
	void *LinkedListIteratorBack(LINKED_LIST *LinkedList);
#endif // end of USING_ITERATOR_BACK_METHOD

/*
	Function: UINT32 LinkedListIteratorGetIndex(LINKED_LIST *LinkedList)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose ITERATOR's index will be returned.

	Returns:
		UINT32 - Returns the current index of the ITERATOR.  If 0 is returned, the ITERATOR is currently
			in it's initialized state and not pointing to any nodes within the LINKED_LIST.

	Description: Returns the index of the LINKED_LIST's ITERATOR.

	Notes: USING_ITERATOR_GET_INDEX_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief  Returns the index of the LINKED_LIST's ITERATOR.
		* @param *LinkedList - The LINKED_LIST whose ITERATOR's index will be returned.
		* @return UINT32 - Returns the current index of the ITERATOR.  If 0 is returned, the ITERATOR 
		is currently in it's initialized state and not pointing to any nodes within the LINKED_LIST.
		* @note USING_ITERATOR_GET_INDEX_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa None
		* @since v1.00
*/
#if (USING_ITERATOR_GET_INDEX_METHOD == 1)
	// Returns the current position of the ITERATOR.
	UINT32 LinkedListIteratorGetIndex(LINKED_LIST *LinkedList);
#endif // end of USING_ITERATOR_GET_INDEX_METHOD

#endif // end of USING_LINKED_LIST_ITERATOR

/*
	Function: UINT32 LinkedListGetSizeInBytes(LINKED_LIST *LinkedList, UINT32 DataSizeInBytes)

	Parameters: 
		LINKED_LIST *LinkedList - The LINKED_LIST whose size will be returned.
		UINT32 DataSizeInBytes - The size of the data pointed to by each node.  Set to 0 if the
			node's do not point to data.

	Returns:
		UINT32 - Returns the current size of the LINKED_LIST in bytes.  This includes the size of
			the data each node points to, the size of a LINKED_LIST_NODE and the size of a 
			LINKED_LIST.

	Description: Returns the current size in bytes of a LINKED_LIST and all it's data.

	Notes: USING_LINKED_LIST_GET_SIZE_IN_BYTES_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
*/
/**
		* @brief  Returns the current size in bytes of a LINKED_LIST and all it's data
		* @param LINKED_LIST *LinkedList - The LINKED_LIST whose size will be returned.
		* @param UINT32 DataSizeInBytes - The size of the data pointed to by each node.  
		Set to 0 if the node's do not point to data.
		* @return UINT32 - Returns the current size of the LINKED_LIST in bytes.  This includes the size of
		the data each node points to, the size of a LINKED_LIST_NODE and the size of a LINKED_LIST.
		* @note USING_LINKED_LIST_GET_SIZE_IN_BYTES_METHOD and USING_LINKED_LIST_ITERATOR must be defined as
		1 in LinkedListConfig.h to use this method.
		* @sa None
		* @since v1.00
*/
#if (USING_LINKED_LIST_GET_SIZE_IN_BYTES_METHOD == 1)
	// Returns the size in bytes of the entire Linked List, including the head
	UINT32 LinkedListGetSizeInBytes(LINKED_LIST *LinkedList, UINT32 DataSizeInBytes);
#endif // end of USING_LINKED_LIST_GET_SIZE_IN_BYTES_METHOD

/*
	Method: const BYTE *LinkedListGetLibraryVersion(void)

	Parameters: 
		None

	Returns:
		const BYTE* - The current Linked List Library version number.  This string
		is (BYTE)NULL terminated.

	Description: Returns the current Linked List Library version number.

	Notes: None
*/
/**
		* @brief  Returns the current Linked List Library version number.
		* @return const BYTE* - The current Linked List Library version number.
		* @note The returned string is (BYTE)NULL terminated.
		* @sa None
		* @since v1.02
*/
#if (USING_LINKED_LIST_GET_LIBRARY_VERSION == 1)
	const BYTE *LinkedListGetLibraryVersion(void);
#endif // end of USING_LINKED_LIST_GET_LIBRARY_VERSION

#define LinkedListGetSizeOfNodeInBytes(Size)		(UINT32)(Size + sizeof(LINKED_LIST_NODE))
#define LinkedListIsEmpty(LinkedList)				(LinkedList->Size == (UINT32)0)
#define LinkedListIsNull(LinkedList)				LinkedList == (LINKED_LIST*)NULL

#endif // end of LINKED_LIST_H
