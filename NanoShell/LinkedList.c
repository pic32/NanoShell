/*
	Developer: Benjamin Michaud
	Date: July 6, 2020
	File Name: LinkedList.c
	Version: 1.05
	IDE: Visual Studio 2019
	Compiler: C89

	Description:  This file contains all the necessary code
	for manipulating the LINKED_LIST data structure.  An explanation
	of each function can be found in LinkedList.h.

*/

#include "LinkedList.h"
#include "LinkedListConfig.h"

#if (LINKED_LIST_USE_MALLOC == 1)
	#include "stdlib.h"
#endif // end of LINKED_LIST_USE_MALLOC

/*
	Creates a new LINKED_LIST, if LinkedListToCreate is NULL this just initializes it.
*/
LINKED_LIST *CreateLinkedList(LINKED_LIST *LinkedListToCreate, INT32 (*CompareTo)(const void *Data1, const void *Data2), void (*CustomFreeMethod)(void *Data))
{
	LINKED_LIST *NewLinkedList;

	if(LinkedListToCreate == (LINKED_LIST*)NULL)
	{
		NewLinkedList = (LINKED_LIST*)ListMemAlloc(sizeof(LINKED_LIST));
		
		if(NewLinkedList == (LINKED_LIST*)NULL)
		{
			return (LINKED_LIST*)NULL;
		}
	}
	else
	{
		NewLinkedList = (LINKED_LIST*)LinkedListToCreate;
	}

	#if (USING_LINKED_LIST_SORTING == 1 || USING_LINKED_LIST_CONTAINS_METHOD == 1)
		NewLinkedList->CompareTo = CompareTo;
	#endif // end of USING_LINKED_LIST_SORTING

	#if(USING_LINKED_LIST_DEPENDENT_FREE_METHOD == 1)
		NewLinkedList->LinkedListFreeMethod = CustomFreeMethod;
	#endif // USING_LINKED_LIST_DEPENDENT_FREE_METHOD

	NewLinkedList->Head = (LINKED_LIST_NODE*)NULL;
	NewLinkedList->Tail = (LINKED_LIST_NODE*)NULL;
	NewLinkedList->Size = (UINT32)0;

	#if (USING_LINKED_LIST_ITERATOR == 1)
		NewLinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)NULL;
		NewLinkedList->Iterator.Position = (UINT32)0;
	#endif // end of USING_LINKED_LIST_ITERATOR

	return (LINKED_LIST*)NewLinkedList;
}

#if	(USING_LINKED_LIST_ADD_FIRST_METHOD == 1)
	BOOL LinkedListAddFirst(LINKED_LIST* LinkedList, const void* Data)
	{
		LINKED_LIST_NODE* TempNode;

		#if (LINKED_LIST_SAFE_MODE == 1)
			if (LinkedListIsNull(LinkedList))
				return FALSE;
		#endif // end of LINKED_LIST_SAFE_MODE

		// allocate room for the LINKED_LIST_NODE
		if ((TempNode = ListMemAlloc(sizeof(LINKED_LIST_NODE))) == (LINKED_LIST_NODE*)NULL)
		{
			return FALSE;
		}

		// add the data
		TempNode->Data = (void*)Data;

		// is the list empty?
		if (LinkedList->Head == (LINKED_LIST_NODE*)NULL)
		{
			LinkedList->Tail = LinkedList->Head = (LINKED_LIST_NODE*)TempNode;

			TempNode->Next = TempNode->Previous = (LINKED_LIST_NODE*)NULL;
		}
		else
		{
			// now update the heads, and the next/previous
			LinkedList->Head->Previous = TempNode;
			TempNode->Next = LinkedList->Head;

			// now we can break the link from the head
			LinkedList->Head = TempNode;

			// since we're node number 1, our previous is non-existent
			TempNode->Previous = (LINKED_LIST_NODE*)NULL;

			#if (USING_LINKED_LIST_ITERATOR == 1)
				// increment the iterator position if it was active
				if (LinkedList->Iterator.Position != 0)
					LinkedList->Iterator.Position++;
			#endif // end of #if (USING_LINKED_LIST_ITERATOR == 1)
		}

		// now we will increment the size
		LinkedList->Size++;

		return TRUE;
	}
#endif // end of #if (USING_LINKED_LIST_ADD_FIRST_METHOD == 1)

#if (USING_LINKED_LIST_ADD_METHOD == 1 || USING_LINKED_LIST_ADD_LAST_METHOD == 1)
	// Tries to add the data specified by void *Data to the end of the LINKED_LIST.
	BOOL LinkedListAddLast(LINKED_LIST *LinkedList, const void *Data)
	{
		LINKED_LIST_NODE *TempNode;

		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;
		#endif // end of LINKED_LIST_SAFE_MODE
	
		if((TempNode = (LINKED_LIST_NODE*)ListMemAlloc(sizeof(LINKED_LIST_NODE))) == (LINKED_LIST_NODE*)NULL)
		{
			return (BOOL)FALSE;
		}
	
		TempNode->Data = (void*)Data;
	
		LinkedList->Size++;
	
		if(LinkedList->Tail == (LINKED_LIST_NODE*)NULL)
		{
			LinkedList->Tail = LinkedList->Head = (LINKED_LIST_NODE*)TempNode;
			TempNode->Next = TempNode->Previous = (LINKED_LIST_NODE*)NULL;
		}
		else
		{
			TempNode->Next = (LINKED_LIST_NODE*)NULL;
			TempNode->Previous = (LINKED_LIST_NODE*)(LinkedList->Tail);
			LinkedList->Tail->Next = (LINKED_LIST_NODE*)TempNode;
			LinkedList->Tail = (LINKED_LIST_NODE*)TempNode;
		}
	
		return (BOOL)TRUE;
	}
#endif // end of #if (USING_LINKED_LIST_ADD_METHOD == 1 || USING_LINKED_LIST_ADD_LAST_METHOD == 1)

#if (LINKED_LIST_FREE_METHOD == 1)
	static void LinkedListFreeNodeAndData(LINKED_LIST *LinkedList, LINKED_LIST_NODE *Node)
	{
		if(LinkedList->LinkedListFreeMethod)
			LinkedList->LinkedListFreeMethod((void*)(Node->Data));
	
		ListMemDealloc((void*)(Node));
	}
#endif // end of LINKED_LIST_FREE_METHOD

#if (LINKED_LIST_ITERATOR_CHECK == 1)
	#if (USING_LINKED_LIST_ITERATOR == 1)

		static void LinkedListIteratorRemovalCheck(LINKED_LIST *LinkedList, UINT32 Index)
		{
			if(LinkedList->Iterator.Position > (UINT32)Index)
			{
				LinkedList->Iterator.Position--;
			}
			else
			{
				if(LinkedList->Iterator.Position == (UINT32)Index)
				{
					if(LinkedList->Iterator.CurrentNode->Next == (LINKED_LIST_NODE*)NULL)
					{
						if(LinkedList->Iterator.CurrentNode->Previous == (LINKED_LIST_NODE*)NULL)
						{
							LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)(NULL);
							LinkedList->Iterator.Position = (UINT32)0;
						}
						else
						{
							LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)(LinkedList->Iterator.CurrentNode->Previous);
							LinkedList->Iterator.Position--;
						}
					}
					else
					{
						LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)(LinkedList->Iterator.CurrentNode->Next);
					}
				}
			}
		}

	#endif // end of USING_LINKED_LIST_ITERATOR
#endif // end of LINKED_LIST_ITERATOR_CHECK

#if (LINKED_LIST_NODE_REMOVAL == 1)
	static LINKED_LIST_NODE *LinkedListNodeRemoval(LINKED_LIST *LinkedList, UINT32 Index)
	{
		LINKED_LIST_NODE *TempNode;
		UINT32 i = (UINT32)1;
	
		TempNode = (LINKED_LIST_NODE*)(LinkedList->Head);
		
		while(i != (UINT32)Index)
		{
			TempNode = (LINKED_LIST_NODE*)(TempNode->Next);
			i++;
		}

		// Iterator checking
		#if (USING_LINKED_LIST_ITERATOR == 1)
			LinkedListIteratorRemovalCheck((LINKED_LIST*)LinkedList, (UINT32)Index);
		#endif // end of USING_LINKED_LIST_ITERATOR

		// Now we're at the Node they want to remove, so remove it!
		if(TempNode->Next != (LINKED_LIST_NODE*)NULL)
		{
			TempNode->Next->Previous = TempNode->Previous;
		}
		
		if(TempNode->Previous != (LINKED_LIST_NODE*)NULL)
		{
			TempNode->Previous->Next = (LINKED_LIST_NODE*)(TempNode->Next);
		}

		if(LinkedList->Head == (LINKED_LIST_NODE*)TempNode)
		{
			if(LinkedList->Tail == (LINKED_LIST_NODE*)TempNode)
				LinkedList->Head = LinkedList->Tail = (LINKED_LIST_NODE*)NULL;
			else
				LinkedList->Head = (LINKED_LIST_NODE*)(TempNode->Next);
		}
		else
		{
			if(LinkedList->Tail == (LINKED_LIST_NODE*)TempNode)
				LinkedList->Tail = (LINKED_LIST_NODE*)(TempNode->Previous);
		}
		
		return (LINKED_LIST_NODE*)TempNode;
	}
#endif // end of LINKED_LIST_REMOVE_NODE

#if (USING_LINKED_LIST_DELETE_NODE_METHOD == 1)
	BOOL LinkedListDeleteNode(LINKED_LIST *LinkedList, UINT32 Index)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;
		
			if(LinkedListIsEmpty(LinkedList))
				return (BOOL)TRUE;	
		
			if(LinkedList->Size < (UINT32)Index)
				return (BOOL)FALSE;
		
			if(Index == (UINT32)0)
				return (BOOL)FALSE;
		#endif // end of LINKED_LIST_SAFE_MODE

		LinkedListFreeNodeAndData((LINKED_LIST*)LinkedList, (LINKED_LIST_NODE*)LinkedListNodeRemoval((LINKED_LIST*)LinkedList, (UINT32)Index));

		LinkedList->Size--;

		return (BOOL)TRUE;
	}
#endif // end of USING_LINKED_LIST_DELETE_NODE_METHOD

#if (USING_LINKED_LIST_REMOVE_NODE_METHOD == 1)

	// Removes and returns the data at the specified Node in the LINKED_LIST.
	void *LinkedListRemoveNode(LINKED_LIST *LinkedList, UINT32 Index)
	{
		LINKED_LIST_NODE *TempNode;
		void *Data;
	
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (void*)NULL;
	
			if(LinkedListIsEmpty(LinkedList))
				return (void*)NULL;	
	
			if(LinkedList->Size < (UINT32)Index)
				return (void*)NULL;
	
			if(Index == (UINT32)0)
				return (void*)NULL;
		#endif // end of LINKED_LIST_SAFE_MODE

		TempNode = (LINKED_LIST_NODE*)LinkedListNodeRemoval((LINKED_LIST*)LinkedList, (UINT32)Index);

		LinkedList->Size--;

		Data = (void*)(TempNode->Data);

		ListMemDealloc((void*)TempNode);

		return (void*)Data;
	}
#endif // end of USING_LINKED_LIST_REMOVE_NODE_METHOD

#if (USING_LINKED_LIST_DELETE_ALL_METHOD == 1)
	// Deletes all Nodes inside the LINKED_LIST.
	BOOL LinkedListDeleteAll(LINKED_LIST *LinkedList)
	{
		LINKED_LIST_NODE *TempNode;
	
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;
	
			if(LinkedList->Size == (UINT32)0)
				return (BOOL)TRUE;
		#endif // end of LINKED_LIST_SAFE_MODE

		TempNode = (LINKED_LIST_NODE*)(LinkedList->Head);
	
		while(TempNode != (LINKED_LIST_NODE*)NULL)
		{
			TempNode = (LINKED_LIST_NODE*)(TempNode->Next);

			LinkedListFreeNodeAndData((LINKED_LIST*)LinkedList, (LINKED_LIST_NODE*)(LinkedList->Head));

			LinkedList->Head = (LINKED_LIST_NODE*)TempNode;
		}
	
		LinkedList->Size = (UINT32)0;
		LinkedList->Head = LinkedList->Tail = (LINKED_LIST_NODE*)NULL;
	
		#if (USING_LINKED_LIST_ITERATOR == 1)
			LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)NULL;
			LinkedList->Iterator.Position = (UINT32)0;
		#endif // end of USING_LINKED_LIST_ITERATOR

		return (BOOL)TRUE;
	}
#endif // end of USING_LINKED_LIST_REMOVE_ALL_METHOD

#if (USING_LINKED_LIST_GET_DATA_METHOD == 1)
	// Returns the specified data at the index specified.  
	void *LinkedListGetData(LINKED_LIST *LinkedList, UINT32 Index)
	{
		UINT32 i;
		LINKED_LIST_NODE *TempNode;
		
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(Index == (UINT32)0)
				return (void*)NULL;
	
			if(LinkedListIsNull(LinkedList))
				return (void*)NULL;
	
			if(LinkedListIsEmpty(LinkedList))
				return (void*)NULL;
	
			if(Index > (UINT32)(LinkedList->Size))
				return (void*)NULL;
		#endif // end of LINKED_LIST_SAFE_MODE

		TempNode = (LINKED_LIST_NODE*)(LinkedList->Head);

		for(i = (UINT32)1; i < Index; i++)
			TempNode = (LINKED_LIST_NODE*)(TempNode->Next);
	
		return (void*)(TempNode->Data);
	}
#endif // end of USING_LINKED_LIST_GET_DATA_METHOD

#if (USING_LINKED_LIST_CONTAINS_METHOD == 1)
	/*
		MultipleOccurences = FALSE;
		ReturnOnOccurence = 0;

		This method will return the Index of the first match of Data.

		MultipleOccurences = TRUE;
		ReturnOnOccurence = 0;

		This method will return how many times it found (void*)Data in the LINKED_LIST.

		MultipleOccurences = FALSE;
		ReturnOnOccurence > 0;

		This method will return the Index of the ReturnOnOccurence'th time it found (void*)Data in the LINKED_LIST.

		MultipleOccurences = TRUE;
		ReturnOnOccurence > 0;

		Invalid Input, returns 0.
	*/
	// Returns the position of the specified data if it exists, 0 otherwise
	UINT32 LinkedListContains(LINKED_LIST *LinkedList, const void *Data, BOOL MultipleOccurences, UINT32 ReturnOnOccurence)
	{
		LINKED_LIST_NODE *TempNode;
		UINT32 Occurences;
		UINT32 Index;
	
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (UINT32)0;
	
			if(LinkedListIsEmpty(LinkedList))
				return (UINT32)0;

			if(MultipleOccurences && ReturnOnOccurence)
				return (UINT32)0;
	
			if(ReturnOnOccurence > (UINT32)(LinkedList->Size))
				return (UINT32)0;
		#endif // end of LINKED_LIST_SAFE_MODE

		TempNode = (LINKED_LIST_NODE*)(LinkedList->Head);
		Occurences = (UINT32)0;
		Index = (UINT32)1;

		do
		{
			if(LinkedList->CompareTo((void*)(TempNode->Data), (void*)Data) == (UINT32)0)
			{
				if(MultipleOccurences || ReturnOnOccurence)
				{
					Occurences++;

					if(Occurences == ReturnOnOccurence)
						return (UINT32)Index;
				}
				else
					return (UINT32)Index;
			}
				
			TempNode = (LINKED_LIST_NODE*)(TempNode->Next);
			Index++;
		}
		while(TempNode != (LINKED_LIST_NODE*)NULL);
	
		if(MultipleOccurences)
			return (UINT32)Occurences;
		else
			return (UINT32)0;
	}	
#endif // end of USING_LINKED_LIST_CONTAINS_METHOD

#if (USING_LINKED_LIST_GET_SIZE_METHOD == 1)
	// Returns the current size of the LINKED_LIST.
	UINT32 LinkedListGetSize(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (UINT32)0;
		#endif // end of LINKED_LIST_SAFE_MODE

		return (UINT32)(LinkedList->Size);
	}
#endif // end of USING_LINKED_LIST_GET_SIZE_METHOD

#if (USING_LINKED_LIST_SORTING == 1)

	static void AddNodeToSortedLinkedList(LINKED_LIST *LinkedList, LINKED_LIST_NODE *Node)
	{
		#if (USING_LINKED_LIST_ITERATOR == 1)
			UINT32 Index = (UINT32)1;
		#endif // end of USING_LINKED_LIST_ITERATOR

		LINKED_LIST_NODE *TempNode = (LINKED_LIST_NODE*)(LinkedList->Head);

		do
		{
			if(LinkedList->CompareTo((void*)(TempNode->Data), (void*)(Node->Data)) > 0)
			{
				// we found a winner, we go before TempNode
				if(TempNode->Previous)
				{
					// we know we're not the first node in the list.
					Node->Previous = (LINKED_LIST_NODE*)(TempNode->Previous);
					Node->Next = (LINKED_LIST_NODE*)TempNode;
					Node->Previous->Next = (LINKED_LIST_NODE*)Node;
					TempNode->Previous = (LINKED_LIST_NODE*)Node;
				}
				else
				{
					// We're the beginning.
					Node->Next = (LINKED_LIST_NODE*)TempNode;
					TempNode->Previous = (LINKED_LIST_NODE*)Node;
					LinkedList->Head = (LINKED_LIST_NODE*)Node;
				}

				#if (USING_LINKED_LIST_ITERATOR == 1)
					if(LinkedList->Iterator.Position)
					{
						if(Index <= (UINT32)(LinkedList->Iterator.Position))
							LinkedList->Iterator.Position++;
					}
				#endif // end of USING_LINKED_LIST_ITERATOR

				return;
			}

			#if (USING_LINKED_LIST_ITERATOR == 1)
				Index++;
			#endif // end of USING_LINKED_LIST_ITERATOR

			TempNode = (LINKED_LIST_NODE*)(TempNode->Next);
		}
		while(TempNode != (LINKED_LIST_NODE*)NULL);

		// now were at the end of the LINKED_LIST
		LinkedList->Tail->Next = (LINKED_LIST_NODE*)Node;
		Node->Previous = (LINKED_LIST_NODE*)(LinkedList->Tail);
		LinkedList->Tail = (LINKED_LIST_NODE*)Node;
	}

#if (USING_SORT_LINKED_LIST_METHOD == 1)

	BOOL LinkedListSort(LINKED_LIST *LinkedList)
	{
		LINKED_LIST_NODE *TempNode1, *TempNode2;
	
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;

			if(LinkedList->CompareTo == NULL)
				return (BOOL)FALSE;

			if(LinkedListIsEmpty(LinkedList))
				return (BOOL)TRUE;
	
			if(LinkedList->Size == (UINT32)1)
				return (BOOL)TRUE;
		#endif // end of LINKED_LIST_SAFE_MODE

		#if (USING_LINKED_LIST_ITERATOR == 1)
			LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)NULL;
			LinkedList->Iterator.Position = (UINT32)0;
		#endif // end of USING_LINKED_LIST_ITERATOR

		LinkedList->Tail = (LINKED_LIST_NODE*)(LinkedList->Head);
		TempNode1 = (LINKED_LIST_NODE*)(LinkedList->Tail->Next);
		TempNode2 = (LINKED_LIST_NODE*)(TempNode1->Next);
		LinkedList->Head->Next = LinkedList->Head->Previous = (LINKED_LIST_NODE*)NULL;
		
		do
		{
			TempNode1->Next = TempNode1->Previous = (LINKED_LIST_NODE*)NULL;
			AddNodeToSortedLinkedList((LINKED_LIST*)LinkedList, (LINKED_LIST_NODE*)TempNode1);
			TempNode1 = (LINKED_LIST_NODE*)(TempNode2);

			if(TempNode2 != (LINKED_LIST_NODE*)NULL)
				TempNode2 = (LINKED_LIST_NODE*)(TempNode2->Next);
		}
		while(TempNode1);

		return (BOOL)TRUE;
	}

#endif // end of USING_SORT_LINKED_LIST_METHOD

#if (USING_ADD_TO_SORTED_LINKED_LIST_METHOD == 1)
	/*
		Must also see if the iterator needs to be adjusted!
	*/
	// Adds the specified data to a LINKED_LIST in a sorted manner.
	BOOL LinkedListAddToSortedLinkedList(LINKED_LIST *LinkedList, const void *Data)
	{
		LINKED_LIST_NODE *TempNode;
	
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;

			if(LinkedList->CompareTo == NULL)
				return (BOOL)FALSE;
		#endif // end of LINKED_LIST_SAFE_MODE

		if((TempNode = (LINKED_LIST_NODE*)ListMemAlloc(sizeof(LINKED_LIST_NODE))) == (LINKED_LIST_NODE*)NULL)
		{
			return (BOOL)FALSE;
		}
	
		TempNode->Data = (void*)Data;
		TempNode->Next = (LINKED_LIST_NODE*)NULL;
		TempNode->Previous = (LINKED_LIST_NODE*)NULL;

		if(LinkedList->Size)
		{
			AddNodeToSortedLinkedList((LINKED_LIST*)LinkedList, (LINKED_LIST_NODE*)TempNode);
		}
		else
		{
			LinkedList->Head = LinkedList->Tail = (LINKED_LIST_NODE*)TempNode;
		}

		LinkedList->Size++;

		return (BOOL)TRUE;
	}
#endif // end of USING_ADD_TO_SORTED_LINKED_LIST_METHOD

#endif // end of USING_LINKED_LIST_SORTING

#if (USING_LINKED_LIST_ITERATOR == 1)

#if (USING_ITERATOR_HAS_NEXT_METHOD == 1)
	// Returns TRUE if there is another Node next in the LINKED_LIST.
	BOOL LinkedListIteratorHasNext(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;
		
			if(LinkedListIsEmpty(LinkedList))
				return (BOOL)FALSE;
		#endif // end of LINKED_LIST_SAFE_MODE

		if(LinkedList->Iterator.Position == (UINT32)0)
			return (BOOL)TRUE;
		
		if(LinkedList->Iterator.CurrentNode->Next == (LINKED_LIST_NODE*)NULL)
			return (BOOL)FALSE;
		else
			return (BOOL)TRUE;
	}
#endif // end of USING_ITERATOR_HAS_NEXT_METHOD

#if (USING_ITERATOR_DELETE_METHOD == 1)
	// Removes the current Node in the LINKED_LIST and returns the data from it.
	BOOL LinkedListIteratorDelete(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;

			if(LinkedList->Iterator.Position == (UINT32)0)
				return (BOOL)FALSE;
		#endif // end of LINKED_LIST_SAFE_MODE

		LinkedListFreeNodeAndData((LINKED_LIST*)LinkedList, (LINKED_LIST_NODE*)LinkedListNodeRemoval((LINKED_LIST*)LinkedList, (UINT32)(LinkedList->Iterator.Position)));

		LinkedList->Size--;

		return (BOOL)TRUE;
	}
#endif // end of USING_ITERATOR_REMOVE_METHOD

#if (USING_ITERATOR_REMOVE_METHOD == 1)
	// Removes the current Node in the LINKED_LIST and returns the data from it.
	void *LinkedListIteratorRemove(LINKED_LIST *LinkedList)
	{
		void *Data;
		LINKED_LIST_NODE *TempNode;

		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (void*)NULL;

			if(LinkedList->Iterator.Position == (UINT32)0)
				return (void*)NULL;
		#endif // end of LINKED_LIST_SAFE_MODE

		TempNode = (LINKED_LIST_NODE*)LinkedListNodeRemoval((LINKED_LIST*)LinkedList, (UINT32)(LinkedList->Iterator.Position));

		LinkedList->Size--;

		Data = (void*)(TempNode->Data);

		ListMemDealloc((void*)TempNode);

		return (void*)Data;
	}
#endif // end of USING_ITERATOR_REMOVE_METHOD

#if (USING_ITERATOR_GET_DATA_METHOD == 1)
	void *LinkedListIteratorGetData(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (void*)NULL;

			if(LinkedList->Iterator.Position == (UINT32)0)
				return (void*)NULL;
		#endif // end of LINKED_LIST_SAFE_MODE

		return (void*)(LinkedList->Iterator.CurrentNode->Data);
	}
#endif // end of USING_ITERATOR_GET_DATA_METHOD

#if (USING_ITERATOR_NEXT_METHOD == 1)
	// Iterates the ITERATOR by one and returns the data at the new Node in the LINKED_LIST.
	void *LinkedListIteratorNext(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (void*)NULL;
	
			if(LinkedListIsEmpty(LinkedList))
				return (void*)NULL;
		#endif // end of LINKED_LIST_SAFE_MODE

		if(LinkedList->Iterator.Position == (UINT32)0)
		{
			LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)(LinkedList->Head);
			LinkedList->Iterator.Position = (UINT32)1;
			return (void*)(LinkedList->Head->Data);
		}
		
		// if its at the tail
		if(LinkedList->Iterator.CurrentNode->Next == (LINKED_LIST_NODE*)NULL)
			return (void*)NULL;
	
		LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)(LinkedList->Iterator.CurrentNode->Next);
		LinkedList->Iterator.Position++;
	
		return (void*)(LinkedList->Iterator.CurrentNode->Data);
	}
#endif // end of USING_ITERATOR_NEXT_METHOD

#if (USING_ITERATOR_RESET_METHOD == 1)
	// Sets the ITERATOR to the beginning of the LINKED_LIST.
	BOOL LinkedListIteratorReset(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;
		#endif // end of LINKED_LIST_SAFE_MODE

		LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)NULL;
		LinkedList->Iterator.Position = (UINT32)0;

		return (BOOL)TRUE;
	}
#endif // end of USING_ITERATOR_RESET_METHOD

#if (USING_ITERATOR_GET_INDEX_METHOD == 1)
	// Returns the current position of the ITERATOR.
	UINT32 LinkedListIteratorGetIndex(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (UINT32)0;
		#endif // end of LINKED_LIST_SAFE_MODE

		return (UINT32)(LinkedList->Iterator.Position);
	}
#endif // end of USING_ITERATOR_GET_INDEX_METHOD

#if(USING_ITERATOR_HAS_BACK_METHOD == 1)
	BOOL LinkedListIteratorHasBack(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (BOOL)FALSE;

			if(LinkedList->Iterator.CurrentNode == (LINKED_LIST_NODE*)NULL)
				return (BOOL)FALSE;
		#endif // end of LINKED_LIST_SAFE_MODE

		if(LinkedList->Iterator.CurrentNode->Previous)
			return (BOOL)TRUE;
		else
			return (BOOL)FALSE;
	}
#endif // end of USING_ITERATOR_HAS_BACK_METHOD

#if(USING_ITERATOR_BACK_METHOD == 1)
	void *LinkedListIteratorBack(LINKED_LIST *LinkedList)
	{
		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (void*)NULL;
		#endif // end of LINKED_LIST_SAFE_MODE

		if(LinkedList->Iterator.Position == (UINT32)1 || LinkedList->Iterator.Position == 0)
			return (void*)NULL;

		LinkedList->Iterator.CurrentNode = (LINKED_LIST_NODE*)(LinkedList->Iterator.CurrentNode->Previous);

		LinkedList->Iterator.Position--;

		return (void*)(LinkedList->Iterator.CurrentNode->Data);
	}
#endif // end of USING_ITERATOR_BACK_METHOD

#endif // end of USING_LINKED_LIST_ITERATOR

#if (USING_LINKED_LIST_GET_SIZE_IN_BYTES_METHOD == 1)
	// Returns the size in bytes of the entire Linked List, including the head
	UINT32 LinkedListGetSizeInBytes(LINKED_LIST *LinkedList, UINT32 DataSizeInBytes)
	{
		UINT32 Size;

		#if (LINKED_LIST_SAFE_MODE == 1)
			if(LinkedListIsNull(LinkedList))
				return (UINT32)0;
		#endif // end of LINKED_LIST_SAFE_MODE

		Size = (UINT32)sizeof(LINKED_LIST);

		if(DataSizeInBytes)
			Size += (UINT32)(LinkedList->Size * (DataSizeInBytes * sizeof(LINKED_LIST_NODE)));
		else
			Size += (UINT32)(LinkedList->Size * sizeof(LINKED_LIST_NODE));

		return (UINT32)Size;
	}
#endif // end of USING_LINKED_LIST_GET_SIZE_IN_BYTES_METHOD

#if (USING_LINKED_LIST_GET_LIBRARY_VERSION == 1)

	const BYTE LinkedListLibraryVersion[] = {"1.05\0"};

	const BYTE *LinkedListGetLibraryVersion(void)
	{
		return (const BYTE*)LinkedListLibraryVersion;
	}
#endif // end of USING_LINKED_LIST_GET_LIBRARY_VERSION
