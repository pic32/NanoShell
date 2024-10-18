#include <string.h>
#include <ctype.h>

#include "NanoShellConfig.h"
#include "NanoFile.h"
#include "NanoDirectory.h"

// this is our root directory
extern DIRECTORY *gCurrentWorkingNanoDirectory;
extern DIRECTORY *gRootNanoDirectory;

static BOOL FileCharacterIsValid(char Character)
{
	// we can't have spaces
	if (isgraph((int)Character) == 0)
		return FALSE;

	if (Character == '\\' || Character == '/' || Character == ' ')
		return FALSE;

	return TRUE;
}

static BOOL FileNameIsValid(char* Buffer)
{
	if (*Buffer == 0)
		return FALSE;

	while (1)
	{
		if (*Buffer == 0)
			return TRUE;

		if (FileCharacterIsValid(*Buffer++) == FALSE)
			return FALSE;
	}
}

SHELL_FILE* NanoFileNameExists(DIRECTORY* Directory, char* FileNameToFind)
{
	UINT32 i, Size;

	if (Directory == NULL || FileNameToFind == NULL)
		return NULL;

	Size = LinkedListGetSize(&Directory->Files);

	for (i = 1; i <= Size; i++)
	{
		SHELL_FILE* FileNodeToCompare = LinkedListGetData(&Directory->Files, i);

		// compare the names
		if (strcmp(FileNodeToCompare->FileName, FileNameToFind) == 0)
			return FileNodeToCompare;
	}

	return NULL;
}

SHELL_FILE *NanoShellGetWorkingFile(char *Buffer, DIRECTORY *RootDir, DIRECTORY *CurrentDir)
{
	DIRECTORY *WorkingDirectory;
	char TempBuffer[SIZE_OF_SHELL_STACK_BUFFER_IN_BYTES + 1];

	if (Buffer == NULL)
		return NULL;

	// now are we relative or absolute for the path?
	if (*Buffer == '/' || *Buffer == '\\')
	{
		// they must have passed in a valid directory
		if (RootDir == NULL)
			return NULL;

		// we are absolute
		WorkingDirectory = RootDir;

		// iterate beyond the slashes
		Buffer++;
	}
	else
	{
		// did they pass in a valid directory?
		if (CurrentDir == NULL)
			return NULL;

		// we are relative
		WorkingDirectory = CurrentDir;
	}

	// now follow the directory
	while (1)
	{
		// get the next portion of the directory
		// it'll look like folder1/folder2/textfile
		// and we will return folder1\0 in TempBuffer
		// and Buffer will be pointing to folder2/textfile 
		if ((Buffer = GetNextNanoDirectorySection(Buffer, TempBuffer, sizeof(TempBuffer))) == NULL)
			return NULL;

		// did we get our last string?
		if (*Buffer == 0)
		{
			// we did, TempBuffer should hold a file name
			return NanoFileNameExists(WorkingDirectory, TempBuffer);
		}

		// find the specified directory
		if (TempBuffer[0] == '.')
		{
			// this is a relative directory
			if (TempBuffer[1] == '.')
			{
				// this is going up
				WorkingDirectory = (DIRECTORY*)WorkingDirectory->ParentDirectory;

				// is it even valid?
				if (WorkingDirectory == NULL)
				{
					WorkingDirectory = RootDir;
				}
			}
			else
			{
				// this is the current directory
				// do nothing
			}
		}
		else
		{
			// does the directory exist?
			DIRECTORY* Directory = NanoDirectoryNameExists(WorkingDirectory, TempBuffer);

			if (Directory == NULL)
			{
				// so the directory doesn't exist
				return NULL;
			}

			// the directory does exist, iterate to it
			WorkingDirectory = Directory;
		}
	}
}

SHELL_RESULT CreateNanoFile(char* ParentDirectory, SHELL_FILE* NewFileToInitialize, 

	char *FileName, SHELL_RESULT(*ReadFileData)(GENERIC_BUFFER *

		#if (EXTENDED_CAT_SUPPORT == 1)
			, READ_OPTIONS ReadOptions
		#endif // end of #if (EXTENDED_CAT_SUPPORT == 1)
		
		), 


	SHELL_RESULT(*WriteFileData)(char* [], UINT32 , GENERIC_BUFFER *
		
		#if (EXTENDED_ECHO_SUPPORT == 1)
			, BOOL Append
		#endif // end of #if (EXTENDED_ECHO_SUPPORT == 1)

		), 


	SHELL_RESULT(*ExecuteFile)(char* [], UINT32 , GENERIC_BUFFER *)

	#if (USE_FILE_DESCRIPTION == 1)
		, char* FileDescription
	#endif // end of #if (USE_FILE_DESCRIPTION == 1)

	#if (USE_FILE_HELP == 1)
		, char* FileHelp
	#endif // end of #if (USE_FILE_HELP == 1)

)
{
	DIRECTORY* ParentDirectoryNode;
	BOOL Outcome;

	// check all incoming parameters
	if (ParentDirectory == NULL)
		return SHELL_INVALID_PARAMETERS;

	if (NewFileToInitialize == NULL)
		return SHELL_INVALID_PARAMETERS;

	if (FileName == NULL)
		return SHELL_INVALID_PARAMETERS;

	// now, is the name valid?
	if (FileNameIsValid(FileName) == FALSE)
		return SHELL_INVALID_DIRECTORY_NAME;

	// so we know the characters in the name are valid
	// does the name already exist?
	ParentDirectoryNode = FollowNanoDirectory(ParentDirectory, gRootNanoDirectory, gCurrentWorkingNanoDirectory, &Outcome);

	// was it valid?
	if (Outcome == FALSE)
		return SHELL_INVALID_DIRECTORY_NAME;

	// does it already exist in the parent directory as a directory or file?
	if (NanoDirectoryNameExists(ParentDirectoryNode, FileName) != NULL)
		return SHELL_DIRECTORY_NAME_ALREADY_EXISTS;

	// does the file already exist?
	if (NanoFileNameExists(ParentDirectoryNode, FileName) != NULL)
		return SHELL_FILE_NAME_ALREADY_EXISTS;

	// set the name
	NewFileToInitialize->FileName = FileName;

	// it is a valid directory, add it
	if (LinkedListAddLast(&ParentDirectoryNode->Files, NewFileToInitialize) == FALSE)
		return SHELL_LINKED_LIST_LIBRARY_FAILED;

	// it was valid, initialize it


	// add the description if present
	#if (USE_FILE_DESCRIPTION == 1)
		NewFileToInitialize->FileDescription = FileDescription;
	#endif // end of #if (USE_FILE_DESCRIPTION == 1)

		// add the description if present
	#if (USE_FILE_HELP == 1)
		NewFileToInitialize->FileHelp = FileHelp;
	#endif // end of #if (USE_FILE_HELP == 1)

	NewFileToInitialize->ReadFileData = ReadFileData;
	NewFileToInitialize->WriteFileData = WriteFileData;
	NewFileToInitialize->ExecuteFile = ExecuteFile;

	return SHELL_SUCCESS;
}