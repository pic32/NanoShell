#ifndef DIRECTORY_H
	#define DIRECTORY_H

#include "NanoShellObjects.h"

DIRECTORY* GenerateRootNanoDirectory(void);

SHELL_RESULT CreateNanoDirectory(char* ParentDirectory, DIRECTORY* NewDirectoryToInitialize, char *DirectoryName
	
	#if (USE_DIRECTORY_DESCRIPTION == 1)
		, char* DirectoryDescription
	#endif // end of #if (USE_DIRECTORY_DESCRIPTION == 1)

);

DIRECTORY *NanoDirectoryNameExists(DIRECTORY* Directory, char* DirectoryNameToFind);

DIRECTORY *FollowNanoDirectory(char *Buffer, DIRECTORY *RootDir, DIRECTORY *CurrentDir, BOOL *Outcome);

char *GetNextNanoDirectorySection(char *Buffer, char *DirectoryHolder, UINT32 DirectoryHolderSizeInBytes);

UINT32 NanoDirectoryGetNumberOfDirectories(DIRECTORY* Directory);

UINT32 NanoDirectoryGetNumberOfFiles(DIRECTORY* Directory);

DIRECTORY* NanoDirectoryGetDirectory(DIRECTORY* Directory, UINT32 DirectoryIndex);

SHELL_FILE* NanoDirectoryGetFile(DIRECTORY* Directory, UINT32 FileIndex);

#endif // end of #ifndef DIRECTORY_H
