#include <string.h>

#include "../NanoShellConfig.h"
#include "../NanoDirectory.h"
#include "ls_Command.h"

extern DIRECTORY *gCurrentWorkingNanoDirectory;
extern DIRECTORY *gRootNanoDirectory;

static SHELL_RESULT OutputDirectoryInfo(char *DirectoryName, GENERIC_BUFFER* Buffer

#if (USE_DIRECTORY_DESCRIPTION == 1)
	, char *DirectoryDescription
#endif // end of #ifdef #if (USE_DIRECTORY_DESCRIPTION == 1)
)
{
	#if (USE_FILE_HELP == 1)
		char FileAttributes[6];
	#else
		char FileAttributes[5];
	#endif // end of #if (USE_FILE_HELP == 1)

	// clear it out first
	memset(FileAttributes, 0, sizeof(FileAttributes));

	FileAttributes[0] = 'd';
	FileAttributes[1] = '-';
	FileAttributes[2] = '-';
	FileAttributes[3] = '-';

	#if (USE_FILE_HELP == 1)
		FileAttributes[4] = '-';
		FileAttributes[5] = ' ';
	#else
		FileAttributes[4] = ' ';
	#endif // end of #if (USE_FILE_HELP == 1)

	if (GenericBufferWrite(Buffer, sizeof(FileAttributes), FileAttributes) != sizeof(FileAttributes))
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	if (strlen(DirectoryName) > SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY)
	{
		// display a poriton
		if (GenericBufferWrite(Buffer, SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY, DirectoryName) != SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY)
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

		if (GenericBufferWrite(Buffer, 3, "...") != 3)
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
	}
	else
	{
		// display them all
		if (GenericBufferWrite(Buffer, (UINT32)strlen(DirectoryName), DirectoryName) != (UINT32)strlen(DirectoryName))
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
	}

	#if (USE_DIRECTORY_DESCRIPTION == 1)
		if (DirectoryDescription != NULL)
		{
			if (strlen(DirectoryName) > SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY)
			{
				// this is the max space, add one space for the description seperation
				if (GenericBufferWrite(Buffer, 1, " ") != 1)
					return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
			}
			else
			{
				UINT32 i;

				for (i = 0; i < SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY - (UINT32)strlen(DirectoryName) + 3; i++)
					if (GenericBufferWrite(Buffer, 1, " ") != 1)
						return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
			}

			if (GenericBufferWrite(Buffer, 2, " -") != 2)
				return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

			if (GenericBufferWrite(Buffer, (UINT32)strlen(DirectoryDescription), DirectoryDescription) != (UINT32)strlen(DirectoryDescription))
				return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
		}
	#endif // end of #if (USE_DIRECTORY_DESCRIPTION == 1)

	if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	return SHELL_SUCCESS;
}

static SHELL_RESULT OutputFileInfo(SHELL_FILE *File, GENERIC_BUFFER* Buffer)
{
	#if (USE_FILE_HELP == 1)
		char FileAttributes[6];
	#else
		char FileAttributes[5];
	#endif // end of #if (USE_FILE_HELP == 1)

	// clear it out first
	memset(FileAttributes, 0, sizeof(FileAttributes));

	FileAttributes[0] = 'v';

	if (File->ReadFileData != NULL)
		FileAttributes[1] = 'r';
	else
		FileAttributes[1] = '-';

	if (File->WriteFileData != NULL)
		FileAttributes[2] = 'w';
	else
		FileAttributes[2] = '-';

	if (File->ExecuteFile != NULL)
		FileAttributes[3] = 'x';
	else
		FileAttributes[3] = '-';

	#if (USE_FILE_HELP == 1)
		if (File->ReadFileData != NULL)
			FileAttributes[4] = 'h';
		else
			FileAttributes[4] = '-';

		FileAttributes[5] = ' ';
	#else
		FileAttributes[4] = ' ';
	#endif // end of #if (USE_FILE_HELP == 1)

	if (GenericBufferWrite(Buffer, sizeof(FileAttributes), FileAttributes) != sizeof(FileAttributes))
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	if (strlen(File->FileName) > SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY)
	{
		// display a poriton
		if (GenericBufferWrite(Buffer, SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY, File->FileName) != SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY)
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

		if (GenericBufferWrite(Buffer, 3, "...") != 3)
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
	}
	else
	{
		// display them all
		if (GenericBufferWrite(Buffer, (UINT32)strlen(File->FileName), File->FileName) != (UINT32)strlen(File->FileName))
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
	}

	#if (USE_FILE_DESCRIPTION == 1)
		if (File->FileDescription != NULL)
		{
			if (strlen(File->FileName) > SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY)
			{
				// this is the max space, add one space for the description seperation
				if (GenericBufferWrite(Buffer, 1, " ") != 1)
					return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
			}
			else
			{
				UINT32 i;

				for (i = 0; i < SHELL_NUMBER_OF_FILE_CHARACTERS_TO_DISPLAY - (UINT32)strlen(File->FileName) + 3; i++)
					if (GenericBufferWrite(Buffer, 1, " ") != 1)
						return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
			}

			if (GenericBufferWrite(Buffer, 2, " -") != 2)
				return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

			if (GenericBufferWrite(Buffer, (UINT32)strlen(File->FileDescription), File->FileDescription) != (UINT32)strlen(File->FileDescription))
				return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
		}
	#endif // end of #if (USE_FILE_DESCRIPTION == 1)

	if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	return SHELL_SUCCESS;
}

static SHELL_RESULT OutputDirectoryDirectories(DIRECTORY *LocalWorkingDirectory, GENERIC_BUFFER *Buffer)
{
	DIRECTORY *Directory;
	UINT32 Size, i;
	SHELL_RESULT Result;

	// now output all the directories present
	Size = NanoDirectoryGetNumberOfDirectories(LocalWorkingDirectory);

	for (i = 1; i <= Size; i++)
	{
		Directory = NanoDirectoryGetDirectory(LocalWorkingDirectory, i);

		Result = OutputDirectoryInfo(Directory->DirectoryName, Buffer

		#if (USE_DIRECTORY_DESCRIPTION == 1)
			, Directory->DirectoryDescription
		#endif // end of #ifdef USE_DIRECTORY_DESCRIPTION

		);

		if (Result != SHELL_SUCCESS)
			return Result;
	}

	return SHELL_SUCCESS;
}

static SHELL_RESULT OutputDirectoryFiles(DIRECTORY *LocalWorkingDirectory, GENERIC_BUFFER *Buffer)
{
	UINT32 Size, i;
	SHELL_FILE *File;
	SHELL_RESULT Result;

	// now output all the file present
	Size = NanoDirectoryGetNumberOfFiles(LocalWorkingDirectory);

	for (i = 1; i <= Size; i++)
	{
		File = NanoDirectoryGetFile(LocalWorkingDirectory, i);

		Result = OutputFileInfo(File, Buffer);

		if (Result != SHELL_SUCCESS)
			return Result;
	}

	return SHELL_SUCCESS;
}

static SHELL_RESULT OutputRelativeDirectories(GENERIC_BUFFER *Buffer)
{
	SHELL_RESULT Result;

	// output the relative directories since we're not in root
	Result = OutputDirectoryInfo(".", Buffer

		#if (USE_DIRECTORY_DESCRIPTION == 1)
			, NULL
		#endif // end of #if (USE_DIRECTORY_DESCRIPTION == 1)

	);

	if (Result != SHELL_SUCCESS)
		return Result;

	Result = OutputDirectoryInfo("..", Buffer

		#if (USE_DIRECTORY_DESCRIPTION == 1)
			, NULL
		#endif // end of #if (USE_DIRECTORY_DESCRIPTION == 1)

	);

	if (Result != SHELL_SUCCESS)
		return Result;

	return SHELL_SUCCESS;
}

SHELL_RESULT lsCommandExecuteMethod(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER* Buffer)
{
	#if (EXTENDED_LS_SUPPORT == 1)
		DIRECTORY *LocalWorkingDirectory;
		UINT32 ArgIndex;
		SHELL_RESULT Result;
		BOOL Outcome;

		ArgIndex = 0;

		do
		{
			// get our next working directory
			LocalWorkingDirectory = FollowNanoDirectory(Args[ArgIndex++], gRootNanoDirectory, gCurrentWorkingNanoDirectory, &Outcome);

			// is it valid?
			if (LocalWorkingDirectory == NULL || Outcome == FALSE)
			{
				if(NumberOfArgs != 0)
					return SHELL_INVALID_PARAMETERS;

				// we are just 1, and need to output the current directory
				LocalWorkingDirectory = gCurrentWorkingNanoDirectory;
			}

			// first output the current and upper directory shorthand symbols "." and ".." if we're not in root
			if (LocalWorkingDirectory != gRootNanoDirectory)
			{
				Result = OutputRelativeDirectories(Buffer);

				if (Result != SHELL_SUCCESS)
					return Result;
			}

			// output the directories
			Result = OutputDirectoryDirectories(LocalWorkingDirectory, Buffer);

			if (Result != SHELL_SUCCESS)
				return Result;

			// output the files
			Result = OutputDirectoryFiles(LocalWorkingDirectory, Buffer);

			if (Result != SHELL_SUCCESS)
				return Result;
		}
		while (ArgIndex < NumberOfArgs);
	#else
		DIRECTORY *LocalWorkingDirectory;
		SHELL_RESULT Result;
		BOOL Outcome;

		// we only take 1 arg
		if (NumberOfArgs > 1)
			return SHELL_INVALID_PARAMETERS;

		// get our next working directory
		LocalWorkingDirectory = FollowNanoDirectory(Args[0], gRootNanoDirectory, gCurrentWorkingNanoDirectory, &Outcome);

		// is it valid?
		if (LocalWorkingDirectory == NULL || Outcome == FALSE)
		{
			if (NumberOfArgs != 0)
				return SHELL_INVALID_PARAMETERS;

			// we are just 1, and need to output the current directory
			LocalWorkingDirectory = gCurrentWorkingNanoDirectory;
		}

		// first output the current and upper directory shorthand symbols "." and ".." if we're not in root
		if (LocalWorkingDirectory != gRootNanoDirectory)
		{
			Result = OutputRelativeDirectories(Buffer);

			if (Result != SHELL_SUCCESS)
				return Result;
		}

		// output the directories
		Result = OutputDirectoryDirectories(LocalWorkingDirectory, Buffer);

		if (Result != SHELL_SUCCESS)
			return Result;

		// output the files
		Result = OutputDirectoryFiles(LocalWorkingDirectory, Buffer);

		if (Result != SHELL_SUCCESS)
			return Result;
	#endif // end of #if (EXTENDED_LS_SUPPORT == 1)

	return SHELL_SUCCESS;
}