#include <string.h>

#include "help_Command.h"
#include "../NanoShellConfig.h"

extern COMMAND_INFO gCommandList[];
extern DIRECTORY* gCurrentWorkingNanoDirectory;
extern DIRECTORY* gRootNanoDirectory;

SHELL_RESULT helpCommandExecuteMethod(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER* Buffer)
{
	SHELL_FILE* File;

	// if there are zero arguments just print out all the global 
	// files that are available
	switch (NumberOfArgs)
	{
		#if (USE_COMMAND_DESCRIPTIONS == 1)
			case 0:
			{
				char SpaceBuffer[HELP_COMMAND_NAME_SIZE_IN_BYTES];
				UINT32 i = 0;

				do
				{
					// clear out the buffer with all spaces
					memset(SpaceBuffer, ' ', HELP_COMMAND_NAME_SIZE_IN_BYTES - 2);

					// put a - for the description
					SpaceBuffer[HELP_COMMAND_NAME_SIZE_IN_BYTES - 2] = '-';

					// now null terminate
					SpaceBuffer[HELP_COMMAND_NAME_SIZE_IN_BYTES - 1] = 0;

					// now copy over the command name into the buffer
					memcpy(SpaceBuffer, gCommandList[i].CommandName, strlen(gCommandList[i].CommandName));

					// write the command name with spaces to the buffer
					if (GenericBufferWrite(Buffer, (UINT32)strlen(SpaceBuffer), SpaceBuffer) != (UINT32)strlen(SpaceBuffer))
						return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

					// now output the description
					if (GenericBufferWrite(Buffer, (UINT32)strlen(gCommandList[i].Description), gCommandList[i].Description) != (UINT32)strlen(gCommandList[i].Description))
						return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

					if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
						return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
				} 
				while (gCommandList[++i].CommandName != NULL);

				return SHELL_SUCCESS;
			}
		#endif // end of #if (USE_COMMAND_DESCRIPTIONS == 1)

		#if (EXTENDED_HELP_SUPPORT != 1)
			case 1:
			{
				// look up a file with the name
				File = NanoShellGetWorkingFile((char*)Args[0], gRootNanoDirectory, gCurrentWorkingNanoDirectory);

				// did we find it?
				if (File != NULL)
				{
					if (File->FileHelp != NULL)
					{
						SHELL_RESULT Result;

						Result = NanoShellProcessOutgoingData(File->FileHelp, Buffer, (UINT32)strlen(File->FileHelp), SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, NanoShellWriteTasks);

						if (Result != SHELL_SUCCESS)
							return Result;

						if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
							return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

						return SHELL_SUCCESS;
					}
					else
					{
						return SHELL_NO_HELP_FILE_PRESENT;
					}
				}

				// just output the help
				// we need to see if it is a global file
				#if (USE_COMMAND_HELP == 1)
					{
						UINT32 i;

						for (i = 0; gCommandList[i].CommandName != NULL; i++)
						{
							// compare it to a file
							if (strcmp(gCommandList[i].CommandName, (char*)Args[0]) == 0)
							{
								// we found a winner
								if (gCommandList[i].Help != NULL)
									return NanoShellProcessOutgoingData(gCommandList[i].Help, Buffer, (UINT32)strlen(gCommandList[i].Help), SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, NanoShellWriteTasks);
								else
									return SHELL_NO_HELP_FILE_PRESENT;
							}
						}
					}
				#endif // end of USE_COMMAND_HELP

				return SHELL_FILE_NOT_FOUND;
			}
		#endif // end of #if (EXTENDED_HELP_SUPPORT != 1)

		default:
		{
			#if (EXTENDED_HELP_SUPPORT == 1)
				SHELL_RESULT Result;
				UINT32 ArgsProcessed = 0;
				BOOL FileFound = FALSE;
				BOOL DataOutput = FALSE;

				#if (USE_FILE_DESCRIPTION == 1)
					BOOL DescriptionOnly = FALSE;
				#endif // end of #if (USE_FILE_DESCRIPTION == 1)

				// get the options first
				if (strcmp(Args[0], "-d") == 0)
				{
					DescriptionOnly = TRUE;

					ArgsProcessed++;
				}

				do
				{
					// look up a file with the name
					File = NanoShellGetWorkingFile((char*)Args[ArgsProcessed], gRootNanoDirectory, gCurrentWorkingNanoDirectory);

					// did we find it?
					if (File != NULL)
					{
						#if (USE_FILE_DESCRIPTION == 1)
							if (DescriptionOnly == TRUE)
							{
								if (File->FileDescription == NULL)
									return SHELL_FILE_NO_DESCRIPTION;

								if (GenericBufferWrite(Buffer, (UINT32)strlen(File->FileName), File->FileName) != (UINT32)strlen(File->FileName))
									return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

								if (GenericBufferWrite(Buffer, (UINT32)strlen(": "), ": ") != (UINT32)2)
									return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

								Result = NanoShellProcessOutgoingData(File->FileDescription, Buffer, strlen(File->FileDescription), SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, NanoShellWriteTasks);

								// write out a new line
								if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
									return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

								BOOL DataOutput = FALSE;
								FileFound = TRUE;

								continue;
							}
						#endif // end of #if (USE_FILE_DESCRIPTION == 1)

						if (File->FileHelp != NULL)
						{
							Result = NanoShellProcessOutgoingData(File->FileHelp, Buffer, strlen(File->FileHelp), SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, NanoShellWriteTasks);
						}
						else
						{
							Result = SHELL_FILE_NOT_FOUND;
						}

						if (Result != SHELL_SUCCESS)
							return Result;

						DataOutput = TRUE;
						FileFound = TRUE;
					}

					// just output the help
					// we need to see if it is a global file
					#if (USE_COMMAND_HELP == 1)
						{
							UINT32 i;

							for (i = 0; gCommandList[i].CommandName != NULL; i++)
							{
								// compare it to a file
								if (strcmp(gCommandList[i].CommandName, (char*)Args[ArgsProcessed]) == 0)
								{
									// we found a winner
									if (gCommandList[i].Help != NULL)
									{
										#if (USE_FILE_DESCRIPTION == 1)
											if (DescriptionOnly == TRUE)
											{
												if (File->FileDescription == NULL)
													return SHELL_FILE_NO_DESCRIPTION;

												if (GenericBufferWrite(Buffer, (UINT32)strlen(gCommandList[i].CommandName), gCommandList[i].CommandName) != (UINT32)strlen(gCommandList[i].CommandName))
													return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

												if (GenericBufferWrite(Buffer, (UINT32)strlen(": "), ": ") != (UINT32)2)
													return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

												Result = NanoShellProcessOutgoingData(gCommandList[i].Description, Buffer, strlen(gCommandList[i].Description), SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, NanoShellWriteTasks);

												// write out a new line
												if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
													return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

												DataOutput = TRUE;
												FileFound = TRUE;

												break;
											}
											else
										#endif // end of #if (USE_FILE_DESCRIPTION == 1)
											{
												Result = NanoShellProcessOutgoingData(gCommandList[i].Help, Buffer, strlen(gCommandList[i].Help), SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, NanoShellWriteTasks);

												if (Result != SHELL_SUCCESS)
													return Result;

												DataOutput = TRUE;
												FileFound = TRUE;

												break;
											}
									}
								}
							}
						}
					#endif // end of USE_COMMAND_HELP

					if (DataOutput == FALSE)
						return SHELL_FILE_NOT_FOUND;

					DataOutput = FALSE;
				} 
				while (++ArgsProcessed != NumberOfArgs);

				if (FileFound == FALSE)
					return SHELL_FILE_NOT_FOUND;

				return SHELL_SUCCESS;
			#else
				return SHELL_FILE_NOT_FOUND;
			#endif // end of #if (EXTENDED_HELP_SUPPORT == 1)
		}
	}
}
