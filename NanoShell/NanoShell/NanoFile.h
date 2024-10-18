#ifndef SHELL_FILE_H
	#define SHELL_FILE_H

#include "NanoShellObjects.h"
#include "../GenericBuffer.h"

SHELL_FILE* NanoFileNameExists(DIRECTORY* Directory, char* FileNameToFind);

SHELL_FILE* NanoShellGetWorkingFile(char* Buffer, DIRECTORY* RootDir, DIRECTORY* CurrentDir);

SHELL_RESULT CreateNanoFile(char* ParentDirectory, SHELL_FILE* NewFileToInitialize,

	char *FileName, SHELL_RESULT(*ReadFileData)(GENERIC_BUFFER *

		#if (EXTENDED_CAT_SUPPORT == 1)
				, READ_OPTIONS ReadOptions
		#endif // end of #if (EXTENDED_CAT_SUPPORT == 1)

		),


	SHELL_RESULT(*WriteFileData)(char*[], UINT32, GENERIC_BUFFER *

		#if (EXTENDED_ECHO_SUPPORT == 1)
				, BOOL Append
		#endif // end of #if (EXTENDED_ECHO_SUPPORT == 1)

		),


	SHELL_RESULT(*ExecuteFile)(char*[], UINT32, GENERIC_BUFFER *)

	#if (USE_FILE_DESCRIPTION == 1)
		, char* FileDescription
	#endif // end of #if (USE_FILE_DESCRIPTION == 1)

	#if (USE_FILE_HELP == 1)
		, char* FileHelp
	#endif // end of #if (USE_FILE_HELP == 1)

);

SHELL_RESULT NanoShellProcessOutgoingData(char* Data, GENERIC_BUFFER* Buffer, UINT32 NumberOfBytesToProcess, UINT32 TransferSizeInBytes, SHELL_RESULT(*WriteTasks)(GENERIC_BUFFER* Buffer));

SHELL_RESULT NanoShellWriteTasks(GENERIC_BUFFER* GenericBuffer);

#endif // end of #ifndef SHELL_FILE_H
