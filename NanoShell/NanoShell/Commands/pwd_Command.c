#include <string.h>

#include "../NanoDirectory.h"
#include "pwd_Command.h"

extern DIRECTORY *gCurrentWorkingNanoDirectory;
extern DIRECTORY *gRootNanoDirectory;

SHELL_RESULT pwdCommandExecuteMethod(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER* Buffer)
{
	UINT32 DirectoryLevels, CurrentLevel;
	DIRECTORY *Directory;

	if (NumberOfArgs != 0)
		return SHELL_INVALID_PARAMETERS;

	Directory = gCurrentWorkingNanoDirectory;

	DirectoryLevels = 0;

	// see how deep we are
	while (Directory != gRootNanoDirectory)
	{
		Directory = (DIRECTORY*)Directory->ParentDirectory;

		DirectoryLevels++;
	}

	// are we in root?
	if (DirectoryLevels == 0)
	{
		if (GenericBufferWrite(Buffer, 1, "/") != 1)
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
	}
	else
	{
		// now iterate through and print out the directories from the parents
		while (DirectoryLevels != 0)
		{
			DirectoryLevels--;
			Directory = gCurrentWorkingNanoDirectory;

			if (GenericBufferWrite(Buffer, 1, "/") != 1)
				return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

			for (CurrentLevel = 0; CurrentLevel < DirectoryLevels; CurrentLevel++)
			{
				Directory = (DIRECTORY*)Directory->ParentDirectory;
			}

			if (GenericBufferWrite(Buffer, (UINT32)strlen(Directory->DirectoryName), Directory->DirectoryName) != (UINT32)strlen(Directory->DirectoryName))
				return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
		}
	}

	if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	return SHELL_SUCCESS;
}