#include <string.h>
#include <ctype.h>

#include "NanoShellReturnTypes.h"
#include "NanoDirectory.h"
#include "NanoFile.h"
#include "NanoShellConfig.h"

// this is our root directory
static DIRECTORY gRootNanoDirectory;
static char gRootDirectoryName;

static BOOL DirectoryCharacterIsValid(char Character)
{
	// we can't have spaces
	if (isgraph((int)Character) == 0)
		return FALSE;

	if (Character == '\\' || Character == '/' || Character == ' ')
		return FALSE;

	return TRUE;
}

static BOOL DirectoryNameIsValid(char* Buffer)
{
	if (*Buffer == 0)
		return FALSE;

	while (1)
	{
		if (*Buffer == 0)
			return TRUE;

		if (DirectoryCharacterIsValid(*Buffer++) == FALSE)
			return FALSE;
	}
}

UINT32 NanoDirectoryGetNumberOfDirectories(DIRECTORY* Directory)
{
	if (Directory == NULL)
		return 0;

	return LinkedListGetSize(&Directory->Directories);
}

UINT32 NanoDirectoryGetNumberOfFiles(DIRECTORY* Directory)
{
	if (Directory == NULL)
		return 0;

	return LinkedListGetSize(&Directory->Files);
}

DIRECTORY* NanoDirectoryGetDirectory(DIRECTORY* Directory, UINT32 DirectoryIndex)
{
	if (Directory == NULL)
		return NULL;

	return (DIRECTORY*)LinkedListGetData(&Directory->Directories, DirectoryIndex);
}

SHELL_FILE *NanoDirectoryGetFile(DIRECTORY *Directory, UINT32 FileIndex)
{
	if (Directory == NULL)
		return NULL;

	return (SHELL_FILE*)LinkedListGetData(&Directory->Files, FileIndex);
}

DIRECTORY* NanoDirectoryNameExists(DIRECTORY* Directory, char* DirectoryNameToFind)
{
	UINT32 i, Size;

	if (Directory == NULL || DirectoryNameToFind == NULL)
		return NULL;

	Size = LinkedListGetSize(&Directory->Directories);

	for (i = 1; i <= Size; i++)
	{
		DIRECTORY* DirectoryNodeToCompare = LinkedListGetData(&Directory->Directories, i);

		// compare the names
		if (strcmp(DirectoryNodeToCompare->DirectoryName, DirectoryNameToFind) == 0)
			return DirectoryNodeToCompare;
	}

	return NULL;
}

DIRECTORY *GenerateRootNanoDirectory(void)
{
	gRootDirectoryName = 0;

	gRootNanoDirectory.ParentDirectory = NULL;
	gRootNanoDirectory.DirectoryName = &gRootDirectoryName;

	#if (USE_DIRECTORY_DESCRIPTION == 1)
		gRootNanoDirectory.DirectoryDescription = NULL;
	#endif // end of #ifdef USE_DIRECTORY_DESCRIPTION == 1

	CreateLinkedList(&gRootNanoDirectory.Directories, NULL, NULL);
	CreateLinkedList(&gRootNanoDirectory.Files, NULL, NULL);

	return &gRootNanoDirectory;
}

SHELL_RESULT CreateNanoDirectory(char *ParentDirectory, DIRECTORY* NewDirectoryToInitialize, char* DirectoryName

	#if (USE_DIRECTORY_DESCRIPTION == 1)
		, char* DirectoryDescription
	#endif // end of #if (USE_DIRECTORY_DESCRIPTION == 1)

)
{
	DIRECTORY* ParentDirectoryNode;
	BOOL Outcome;

	// check all incoming parameters
	if (ParentDirectory == NULL)
		return SHELL_INVALID_PARAMETERS;

	if (NewDirectoryToInitialize == NULL)
		return SHELL_INVALID_PARAMETERS;

	if (DirectoryName == NULL)
		return SHELL_INVALID_PARAMETERS;

	// now, is the name valid?
	if (DirectoryNameIsValid(DirectoryName) == FALSE)
		return SHELL_INVALID_DIRECTORY_NAME;

	// so we know the characters in the name are valid
	// does the name already exist?
	ParentDirectoryNode = FollowNanoDirectory(ParentDirectory, &gRootNanoDirectory, NULL, &Outcome);

	// was it valid?
	if (Outcome == FALSE)
		return SHELL_INVALID_DIRECTORY_NAME;

	// does it already exist in the parent directory as a directory or file?
	if (NanoDirectoryNameExists(ParentDirectoryNode, DirectoryName) != NULL)
		return SHELL_DIRECTORY_NAME_ALREADY_EXISTS;

	// does the file already exist?
	if (NanoFileNameExists(ParentDirectoryNode, DirectoryName) != NULL)
		return SHELL_FILE_NAME_ALREADY_EXISTS;

	// set the name
	NewDirectoryToInitialize->DirectoryName = DirectoryName;

	// it is a valid directory, add it
	if (LinkedListAddLast(&ParentDirectoryNode->Directories, NewDirectoryToInitialize) == FALSE)
		return SHELL_LINKED_LIST_LIBRARY_FAILED;

	// it was valid, initialize it
	NewDirectoryToInitialize->ParentDirectory = (void*)ParentDirectoryNode;

	// add the description if present
	#if (USE_DIRECTORY_DESCRIPTION == 1)
		NewDirectoryToInitialize->DirectoryDescription = DirectoryDescription;
	#endif // end of #if (USE_DIRECTORY_DESCRIPTION == 1)

	// initialize the linked lists
	CreateLinkedList(&NewDirectoryToInitialize->Directories, NULL, NULL);
	CreateLinkedList(&NewDirectoryToInitialize->Files, NULL, NULL);

	return SHELL_SUCCESS;
}

char *GetNextNanoDirectorySection(char *Buffer, char *DirectoryHolder, UINT32 DirectoryHolderSizeInBytes)
{
	UINT32 i;

	// clear out the stack buffer
	memset(DirectoryHolder, 0, DirectoryHolderSizeInBytes);

	i = 0;

	// are they specifying a relative directory?
	if (*Buffer == '.')
	{
		// they are, copy in the .
		while (*Buffer == '.')
		{
			// copy it over
			DirectoryHolder[i++] = *Buffer;

			// iterate to the next
			Buffer++;
		}

		// how did we end?
		if (*Buffer == '/' || *Buffer == '\\')
		{
			Buffer++;

			return Buffer;
		}

		// null is ok
		if (*Buffer == 0)
			return Buffer;
	}
	else
	{
		// now start copying
		// copy over up to the first slash, or null
		while (DirectoryCharacterIsValid(*Buffer) == TRUE)
		{
			DirectoryHolder[i++] = *Buffer;

			Buffer++;

			// are we getting overflow?
			if (i == DirectoryHolderSizeInBytes - 1)
				return NULL;
		}
	}

	switch (*Buffer)
	{
		case '/':
		case '\\':
		{
			// iterate to the next directory
			Buffer++;

			return Buffer;
		}

		case '.':
		{
			return Buffer;
		}

		case 0:
		{
			// we are at the end
			return Buffer;
		}

		default:
		{
			// invalid character
			return NULL;
		}
	}
}

// this function allows you to bottom out over and over on root.
// meaning if you were at root and you did ../../../../ that is fine, it just stays at root
DIRECTORY* FollowNanoDirectory(char* Buffer, DIRECTORY* RootDir, DIRECTORY* CurrentDir, BOOL *Outcome)
{
	DIRECTORY* WorkingDirectory;
	char TempBuffer[SIZE_OF_SHELL_STACK_BUFFER_IN_BYTES + 1];

	if (Buffer == NULL)
		return NULL;

	if (Outcome == NULL)
		return NULL;

	*Outcome = FALSE;

	if (strlen(Buffer) == 0)
	{
		// this is the root
		*Outcome = TRUE;

		return RootDir;
	}

	#if (USE_SHELL_HOME_DIRECTORY == 1)
		// are they supplying the home directory?
		if (*Buffer == SHELL_HOME_DIRECTORY_CHARACTER && Buffer[1] == 0)
		{
			Buffer = SHELL_HOME_DIRECTORY;
		}
	#endif // end of #if (USE_SHELL_HOME_DIRECTORY == 1)

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

		// did we get a zero sized string?
		if (TempBuffer[0] == 0)
		{
			// we had a valid outcome
			*Outcome = TRUE;

			// return our current directory that we found
			return WorkingDirectory;
		}

		// find the specified directory
		if (TempBuffer[0] == '.')
		{
			// this is a relative directory
			if (TempBuffer[1] == '.')
			{
				// this is going up
				WorkingDirectory = (DIRECTORY*)WorkingDirectory->ParentDirectory;

				// is it root?
				if (WorkingDirectory == NULL)
				{
					WorkingDirectory =  RootDir;
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
				// so the directory doesn't exist, but this could
				// still be a file.  So we will set outcome to false (already)
				// and we will return the directory to signal this
				return WorkingDirectory;
			}

			// the directory does exist, iterate to it
			WorkingDirectory = Directory;
		}
	}
}
