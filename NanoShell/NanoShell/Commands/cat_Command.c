#include <string.h>
#include <ctype.h>

#include "../NanoFile.h"
#include "../../GenericBuffer.h"
#include "cat_Command.h"

extern DIRECTORY *gCurrentWorkingNanoDirectory;
extern DIRECTORY *gRootNanoDirectory;

#if (EXTENDED_CAT_SUPPORT == 1)
	static BOOL ProcessOptions(char* OptionsString, READ_OPTIONS* Options)
	{
		if (*OptionsString++ != '-')
			return FALSE;

		while (*OptionsString)
		{
			switch (*OptionsString++)
			{
				case 'E':
				{
					Options->Bits.ShowLineEnds = 1;

					break;
				}

				case 'n':
				{
					Options->Bits.NumberAllLines = 1;

					break;
				}

				case 's':
				{
					Options->Bits.SupressRepeativeEmptyLines = 1;

					break;
				}

				case 'T':
				{
					Options->Bits.ShowTabs = 1;

					break;
				}

				case 'v':
				{
					Options->Bits.ShowControlCharacters = 1;

					break;
				}

				default:
				{
					return FALSE;
				}
			}
		}

		return TRUE;
	}
#endif // end of #if (EXTENDED_CAT_SUPPORT == 1)

SHELL_RESULT catCommandExecuteMethod(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER* Buffer)
{
	#if (EXTENDED_CAT_SUPPORT == 1)
		SHELL_FILE* File;
		SHELL_RESULT Result;
		READ_OPTIONS ReadOptions;
		UINT32 ArgsProcessed;

		if (NumberOfArgs == 0)
			return SHELL_INVALID_PARAMETERS;

		// clear it out first
		ReadOptions.Value = 0;

		// zero this out at the start
		ArgsProcessed = 0;

		// get any potential options
		if ((Result = ProcessOptions(Args[0], &ReadOptions)) == TRUE)
		{
			// if we only have one set of arguments, that's an issue
			if (NumberOfArgs == 1)
				return SHELL_INVALID_PARAMETERS;

			ArgsProcessed++;
		}

		do
		{
			// find the file
			File = NanoShellGetWorkingFile(Args[ArgsProcessed], gRootNanoDirectory, gCurrentWorkingNanoDirectory);

			// did we find it?
			if (File == NULL)
				return SHELL_FILE_NOT_FOUND;

			if (File->ReadFileData == NULL)
				return SHELL_FILE_NOT_READABLE;

			Result = File->ReadFileData(Buffer, ReadOptions);

			if (Result != SHELL_SUCCESS)
				return Result;

			if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
				return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
		}
		while (++ArgsProcessed != NumberOfArgs);
	#else
		SHELL_FILE *File;
		SHELL_RESULT Result;

		if (NumberOfArgs != 1)
			return SHELL_INVALID_PARAMETERS;

		// find the file
		File = NanoShellGetWorkingFile(Args[0], gRootNanoDirectory, gCurrentWorkingNanoDirectory);

		// did we find it?
		if(File == NULL)
			return SHELL_FILE_NOT_FOUND;

		if (File->ReadFileData == NULL)
			return SHELL_FILE_NOT_READABLE;

		Result = File->ReadFileData(Buffer);

		if (Result != SHELL_SUCCESS)
			return Result;

		if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
	#endif // end of #if (EXTENDED_CAT_SUPPORT == 1)

	return SHELL_SUCCESS;
}