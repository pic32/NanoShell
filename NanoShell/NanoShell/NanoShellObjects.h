#ifndef SHELL_OBJECTS_H
	#define SHELL_OBJECTS_H

#include "../LinkedList.h"
#include "../GenericBuffer.h"
#include "NanoShellReturnTypes.h"
#include "NanoShellConfig.h"

typedef struct
{
	LINKED_LIST Directories; // this is of type DIRECTORY
	LINKED_LIST Files; // this is of type SHELL_FILE
	void* ParentDirectory; // this is of type DIRECTORY
	char* DirectoryName;

	#if (USE_DIRECTORY_DESCRIPTION == 1)
		char* DirectoryDescription;
	#endif // end of #if (USE_DIRECTORY_DESCRIPTION == 1)
}DIRECTORY;

typedef union
{
	BYTE Value;

	struct
	{
		unsigned int ShowLineEnds : 1;					// -E
		unsigned int NumberAllLines : 1;				// -n
		unsigned int SupressRepeativeEmptyLines : 1;	// -s
		unsigned int ShowTabs : 1;						// -T
		unsigned int ShowControlCharacters : 1;			// -v
		unsigned int Reserved : 3;
	}Bits;
}READ_OPTIONS;

typedef struct
{

	SHELL_RESULT(*ReadFileData)(GENERIC_BUFFER* OutputBuffer

		#if (EXTENDED_CAT_SUPPORT == 1)
			, READ_OPTIONS ReadOptions
		#endif // end of #if (EXTENDED_CAT_SUPPORT == 1)

		);

	SHELL_RESULT(*WriteFileData)(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER *OutputBuffer

	
		#if (EXTENDED_ECHO_SUPPORT == 1)
			, BOOL AppendMode
		#endif // end of #if (EXTENDED_ECHO_SUPPORT == 1)

		);
	
	SHELL_RESULT(*ExecuteFile)(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER *OutputBuffer);

	char* FileName;

	#if (USE_FILE_DESCRIPTION == 1)
		char* FileDescription;
	#endif // end of #if (USE_FILE_DESCRIPTION == 1)

	#if (USE_FILE_HELP == 1)
		char* FileHelp;
	#endif // end of #if (USE_FILE_HELP == 1)
}SHELL_FILE;



#endif // end of #ifndef SHELL_OBJECTS_H
