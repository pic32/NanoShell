#include <string.h>
#include <ctype.h>

#include "NanoShell.h"
#include "NanoShellConfig.h"
#include "../GenericBuffer.h"
#include "NanoDirectory.h"
#include "NanoFile.h"

// commands only
#if (USE_CAT_COMMAND == 1)
	#include "./Commands/cat_Command.h"
#endif // end of #if (USE_CAT_COMMAND == 1)

#if (USE_CD_COMMAND == 1)
	#include "./Commands/cd_Command.h"
#endif // end of #if (USE_CD_COMMAND == 1)

#if (USE_CLEAR_COMMAND == 1)
	#include "./Commands/clear_Command.h"
#endif // end of #if (USE_CLEAR_COMMAND == 1)

#if (USE_ECHO_COMMAND == 1)
	#include "./Commands/echo_Command.h"
#endif // end of #if (USE_ECHO_COMMAND == 1)

#if (USE_HELP_COMMAND == 1)
	#include "./Commands/help_Command.h"
#endif // end of #if (USE_HELP_COMMAND == 1)

#if (USE_LS_COMMAND == 1)
	#include "./Commands/ls_Command.h"
#endif // end of #if (USE_LS_COMMAND == 1)

#if (USE_PWD_COMMAND == 1)
	#include "./Commands/pwd_Command.h"
#endif // end of #if (USE_PWD_COMMAND == 1)

#if (USE_SHUTDOWN_COMMAND == 1)
	#include "./Commands/shutdown_Command.h"
#endif // end of #if (USE_SHUTDOWN_COMMAND == 1)

DIRECTORY *gCurrentWorkingNanoDirectory;
DIRECTORY *gRootNanoDirectory;

#if (SHELL_USE_CONSOLE_ECHO == RUNTIME_CONFIGURABLE)
	BOOL gConsoleEcho;
#endif // end of #if (SHELL_USE_CONSOLE_ECHO == RUNTIME_CONFIGURABLE)

// these are the standard stream inputs and outputs
GENERIC_BUFFER gOutputStream, gInputStream;

// these are the array buffers the gOutputStream and gInputStream use
BYTE gOutputStreamBuffer[SIZE_OF_OUTPUT_STREAM_BUFFER_IN_BYTES];
BYTE gInputStreamBuffer[SIZE_OF_INPUT_STREAM_BUFFER_IN_BYTES];

// this is used to keep track of the escape sequences
UINT32 gEscapeSequence;

#if (USE_SHELL_COMMAND_HISTORY == 1)
	LINKED_LIST gHistoryList; // this holds data of type char *
	UINT32 gHistoryIndex; // this is the index inside the linked list to read for hte next history
#endif // end of #if (USE_SHELL_COMMAND_HISTORY == 1)

UINT32 StreamWriteDataHALCallback(BYTE* DataBuffer, UINT32 DataBuffersSizeInBytes);
UINT32 StreamReaderDataHALCallback(BYTE* DataBuffer, UINT32 DataBuffersSizeInBytes);

const char *gNanoShellError[] = {
	"success",
	"file not found",
	"insufficient arguments for file",
	"invalid parameter",
	"linked list library error",
	"generic buffer library error",

	#if (USE_SHELL_COMMAND_HISTORY == 1)
		"history buffer error",
	#endif // end of #if (USE_SHELL_COMMAND_HISTORY == 1)

	"invalid number of bytes transferred",
	"input generic buffer overflow",
	"output generic buffer overflow",
	"invalid directory name",
	"directory name already exists",
	"file name already exists",
	"argument overflow",
	"invalid character found",
	"", // empty entry for a user supplied message
	"no help file present",
	"file not executable",
	"file not writable",
	"file not readable"
};

COMMAND_INFO gCommandList[] = 
{
	#if (USE_CAT_COMMAND == 1)
		{CAT_COMMAND_NAME, catCommandExecuteMethod, "Reads the contents of a file", CAT_HELP_TEXT},
	#endif // end of #if (USE_CAT_COMMAND == 1)

	#if (USE_CD_COMMAND == 1)
		{CD_COMMAND_NAME, cdCommandExecuteMethod, "Changes the current directory", CD_HELP_TEXT},
	#endif // end of #if (USE_CD_COMMAND == 1)

	#if (USE_CLEAR_COMMAND == 1)
		{CLEAR_COMMAND_NAME, clearCommandExecuteMethod, "Clears the screen", CLEAR_HELP_TEXT},
	#endif // end of #if (USE_CLEAR_COMMAND == 1)

	#if (USE_ECHO_COMMAND == 1)
		{ECHO_COMMAND_NAME, echoCommandExecuteMethod, "Writes data to a file", ECHO_HELP_TEXT},
	#endif // end of #if (USE_ECHO_COMMAND == 1)

	#if (USE_HELP_COMMAND == 1)
		{HELP_COMMAND_NAME, helpCommandExecuteMethod, "Gets detailed help information on files", HELP_HELP_TEXT},
	#endif // end of #if (USE_HELP_COMMAND == 1)

	#if (USE_LS_COMMAND == 1)
		{LS_COMMAND_NAME, lsCommandExecuteMethod, "List the contents of a directory", LS_HELP_TEXT},
	#endif // end of #if (USE_LS_COMMAND == 1)

	#if (USE_PWD_COMMAND == 1)
		{PWD_COMMAND_NAME, pwdCommandExecuteMethod, "Outputs the current full directory", PWD_HELP_TEXT},
	#endif // end of #if (USE_PWD_COMMAND == 1)

	#if (USE_SHUTDOWN_COMMAND == 1)
		{SHUTDOWN_COMMAND_NAME, shutdownCommandExecuteMethod, "Bring the system down", SHUTDOWN_HELP_TEXT},
	#endif // end of #if (USE_SHUTDOWN_COMMAND == 1)

	{NULL, NULL, NULL, NULL}
};

static SHELL_RESULT OutputPrompt(char *CurrentDirectory)
{
	#if(SHELL_USE_PROMPT == 1)
		#ifdef SHELL_PROMPT_LEADING_SEQUENCE
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_PROMPT_LEADING_SEQUENCE), SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_PROMPT_LEADING_SEQUENCE) != (UINT32)strlen(SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_PROMPT_LEADING_SEQUENCE))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_PROMPT_LEADING_SEQUENCE

		#ifdef SHELL_USERNAME
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_USERNAME_TEXT_COLOR SHELL_USERNAME), SHELL_USERNAME_TEXT_COLOR SHELL_USERNAME) != (UINT32)strlen(SHELL_USERNAME_TEXT_COLOR SHELL_USERNAME))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_USERNAME

		#ifdef SHELL_USERNAME_DIRECTORY_SEPERATION_SEQUENCE
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_USERNAME_DIRECTORY_SEPERATION_SEQUENCE), SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_USERNAME_DIRECTORY_SEPERATION_SEQUENCE) != (UINT32)strlen(SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_USERNAME_DIRECTORY_SEPERATION_SEQUENCE))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_USERNAME_DIRECTORY_SEPERATION_SEQUENCE

		#if (USE_SHELL_COLOR == 1)
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_DIRECTORY_TEXT_COLOR), SHELL_DIRECTORY_TEXT_COLOR) != (UINT32)strlen(SHELL_DIRECTORY_TEXT_COLOR))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of #if (USE_SHELL_COLOR == 1)

		if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(CurrentDirectory), (const BYTE*)CurrentDirectory) != (UINT32)strlen(CurrentDirectory))
			return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

		#ifdef SHELL_PROMPT_ENDING_SEQUENCE
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_PROMPT_ENDING_SEQUENCE), SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_PROMPT_ENDING_SEQUENCE) != (UINT32)strlen(SHELL_PROMPT_BRACKETS_TEXT_COLOR SHELL_PROMPT_ENDING_SEQUENCE))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_PROMPT_ENDING_SEQUENCE

		#ifdef SHELL_ATTENTION_CHARACTER
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_ATTENTION_CHARACTER_TEXT_COLOR SHELL_ATTENTION_CHARACTER), SHELL_ATTENTION_CHARACTER_TEXT_COLOR SHELL_ATTENTION_CHARACTER) != (UINT32)strlen(SHELL_ATTENTION_CHARACTER_TEXT_COLOR SHELL_ATTENTION_CHARACTER))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_ATTENTION_CHARACTER

		#if (USE_SHELL_COLOR == 1)
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_DEFAULT_TEXT_COLOR), SHELL_DEFAULT_TEXT_COLOR) != (UINT32)strlen(SHELL_DEFAULT_TEXT_COLOR))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of #if (USE_SHELL_COLOR == 1)

		if (GenericBufferWrite(&gOutputStream, 1, (const BYTE*)" ") != 1)
			return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
	#endif // end of #if(SHELL_USE_PROMPT == 1)

	return SHELL_SUCCESS;
}

#if (EXTENDED_CAT_SUPPORT == 1)
	SHELL_RESULT ProcessFileReadWithReadOptions(READ_OPTIONS Options, GENERIC_BUFFER* Buffer, char* DataRead, UINT32 NumberOfBytesRead)
	{
		char LineNumberBuffer[6];
		UINT32 LineNumber = 0;
		UINT32 NewLinePrior = 0;
		BOOL PrintOutLineNumber = FALSE;

		if (Buffer == NULL)
			return SHELL_INVALID_PARAMETERS;

		if (DataRead == NULL)
			return SHELL_INVALID_PARAMETERS;

		if (NumberOfBytesRead == 0)
			return SHELL_SUCCESS;

		// do they even have any options on?
		if (Options.Value == 0)
		{
			// they don't, just write the data out
			return NanoShellProcessOutgoingData(DataRead, Buffer, NumberOfBytesRead, SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, NanoShellWriteTasks);
		}

		// we know they have at least 1 option on.
		if (Options.Bits.NumberAllLines == TRUE)
		{
			// initialize the line buffer with all spaces and null terminate it
			LineNumber = 1;

			Shell_sprintf(LineNumberBuffer, "% 4i ", LineNumber);

			if (GenericBufferWrite(Buffer, strlen(LineNumberBuffer), LineNumberBuffer) != strlen(LineNumberBuffer))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		}

		while (NumberOfBytesRead)
		{
			// we have enough data for a new line sequence, does it exist?
			if (NumberOfBytesRead >= SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
			{
				// find it if it exists
				if (strncmp(DataRead, SHELL_DEFAULT_END_OF_LINE_SEQUENCE, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES) == 0)
				{
					// it does exist, so now what are our options?
					NumberOfBytesRead -= SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES;

					// increment our line number too
					LineNumber++;

					// do they want to suppress the new lines?
					if (NewLinePrior == SHELL_NEW_LINE_SUPPRESS_THRESHOLD && Options.Bits.SupressRepeativeEmptyLines == 1)
					{
						// increment our pointer
						DataRead += SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES;
					}
					else
					{
						// increment this so when it gets to 2, we don't display if the option
						// is set
						NewLinePrior++;

						// do they want line ends?
						if (Options.Bits.ShowLineEnds == TRUE)
						{
							// output $ before the new line
							if (GenericBufferWrite(Buffer, 1, "$") != 1)
								return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
						}

						// output the new line sequence
						if (GenericBufferWrite(Buffer, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, DataRead) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
							return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

						// increment our pointer
						DataRead += SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES;

						// do we number lines?
						if (Options.Bits.NumberAllLines == TRUE && NewLinePrior == SHELL_NEW_LINE_SUPPRESS_THRESHOLD && Options.Bits.SupressRepeativeEmptyLines == 1)
						{
							PrintOutLineNumber = TRUE;
						}
						else
						{
							PrintOutLineNumber = FALSE;

							// update the line number and output the value
							Shell_sprintf(LineNumberBuffer, "% 4i ", LineNumber);

							if (GenericBufferWrite(Buffer, strlen(LineNumberBuffer), LineNumberBuffer) != strlen(LineNumberBuffer))
								return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
						}
					}

					// don't execute anything else in the loop
					continue;
				}
			}

			NewLinePrior = 0;

			if(PrintOutLineNumber == TRUE)
			{
				PrintOutLineNumber = FALSE;

				// update the line number and output the value
				Shell_sprintf(LineNumberBuffer, "% 4i ", LineNumber);

				if (GenericBufferWrite(Buffer, strlen(LineNumberBuffer), LineNumberBuffer) != strlen(LineNumberBuffer))
					return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
			}

			// do they want to show tabs?
			if (Options.Bits.ShowTabs == 1)
			{
				// they do, do we have a tab
				if (*DataRead == '\t')
				{
					if (GenericBufferWrite(Buffer, 2, "^I") != 2)
						return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

					// update our size and pointer
					NumberOfBytesRead -= 1;
					DataRead++;

					// do not execute anymore of the loop
					continue;
				}
			}

			// if the character is print, print it out
			if (isprint(*DataRead) == FALSE && isspace(*DataRead) == FALSE)
			{
				// its not print

				// do they have the option set for printing non printable characters?
				if (Options.Bits.ShowControlCharacters == 1)
				{
					// do they have the option set
					if (*DataRead < 128)
					{
						// we are under 128, use ^@
						char TempBuffer[2];

						TempBuffer[0] = '^';
						TempBuffer[1] = '@' + *DataRead;

						// now write the data
						if (GenericBufferWrite(Buffer, 2, TempBuffer) != 2)
							return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
					}
					else
					{
						// we are at or over 128, use M-^@
						char TempBuffer[4];

						TempBuffer[0] = 'M';
						TempBuffer[1] = '-';
						TempBuffer[2] = '^';
						TempBuffer[3] = '@' + (*DataRead) - 128;

						// now write the data
						if (GenericBufferWrite(Buffer, 4, TempBuffer) != 4)
							return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
					}
				}
			}
			else
			{
				// write the data out
				if (GenericBufferWrite(Buffer, 1, DataRead) != 1)
					return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
			}

			// update our size and pointer
			NumberOfBytesRead -= 1;
			DataRead++;
		}

		return SHELL_SUCCESS;
	}
#endif // end of #if (EXTENDED_CAT_SUPPORT == 1)

SHELL_RESULT NanoShellInit(void)
{
	// initialize our input and output streams
	if (CreateGenericBuffer(&gOutputStream, SIZE_OF_OUTPUT_STREAM_BUFFER_IN_BYTES, gOutputStreamBuffer) == NULL)
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	if (CreateGenericBuffer(&gInputStream, SIZE_OF_INPUT_STREAM_BUFFER_IN_BYTES, gInputStreamBuffer) == NULL)
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	gEscapeSequence = 0;

	#if (USE_SHELL_COMMAND_HISTORY == 1)
		if (CreateLinkedList(&gHistoryList, NULL, NanoShellFreeMethod) == NULL)
			return SHELL_LINKED_LIST_LIBRARY_FAILED;

		gHistoryIndex = 0;
	#endif // end of #if (USE_SHELL_COMMAND_HISTORY == 1)

	// now initialize our console echo
	#if (SHELL_USE_CONSOLE_ECHO == RUNTIME_CONFIGURABLE)
		gConsoleEcho = DEFAULT_CONSOLE_ECHO_VALUE;
	#endif // end of #if (SHELL_USE_CONSOLE_ECHO == RUNTIME_CONFIGURABLE)

	// now output the starting string, then the prompt
	#if (SHELL_USE_PROJECT_NAME == 1)
		#ifdef SHELL_PROJECT_NAME
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_PROJECT_NAME_TEXT_COLOR SHELL_PROJECT_NAME), SHELL_PROJECT_NAME_TEXT_COLOR SHELL_PROJECT_NAME) != (UINT32)strlen(SHELL_PROJECT_NAME_TEXT_COLOR SHELL_PROJECT_NAME))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

			if (GenericBufferWrite(&gOutputStream, 1, " ") != 1)
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_PROJECT_NAME

		#if (USE_SHELL_COLOR == 1)
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_DEFAULT_TEXT_COLOR), SHELL_DEFAULT_TEXT_COLOR) != (UINT32)strlen(SHELL_DEFAULT_TEXT_COLOR))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of #if (USE_SHELL_COLOR == 1)

		#ifdef SHELL_MAJOR_VERSION
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_MAJOR_VERSION), SHELL_MAJOR_VERSION) != (UINT32)strlen(SHELL_MAJOR_VERSION))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

			if (GenericBufferWrite(&gOutputStream, 1, (const BYTE*)".") != 1)
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_MAJOR_VERSION

		#ifdef SHELL_MINOR_VERSION
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_MINOR_VERSION), SHELL_MINOR_VERSION) != (UINT32)strlen(SHELL_MINOR_VERSION))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

			if (GenericBufferWrite(&gOutputStream, 1, (const BYTE*)".") != 1)
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_MINOR_VERSION

		#ifdef SHELL_TEST_VERSION
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_TEST_VERSION), SHELL_TEST_VERSION) != (UINT32)strlen(SHELL_TEST_VERSION))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		#endif // end of SHELL_TEST_VERSION

		if (GenericBufferWrite(&gOutputStream, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
			return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
	#endif // end of #if (SHELL_USE_PROJECT_NAME == 1)

	// now initialize the current directory
	gCurrentWorkingNanoDirectory = gRootNanoDirectory = GenerateRootNanoDirectory();

	// now output the prompt
	return OutputPrompt(gCurrentWorkingNanoDirectory->DirectoryName);
}

SHELL_RESULT NanoShellWriteTasks(GENERIC_BUFFER *GenericBuffer)
{
	if (GenericBufferGetSize(GenericBuffer) != 0)
	{
		BYTE CharacterBuffer[SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES + 1];
		UINT32 BytesToTransfer, OriginalBytesToTransfer;

		OriginalBytesToTransfer = GenericBufferPeek(GenericBuffer, 0, SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, CharacterBuffer, SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, FALSE);

		// now null it out for the method that is about to send it
		CharacterBuffer[OriginalBytesToTransfer] = 0;

		// now get the data from the user if ther is any
		if ((BytesToTransfer = StreamWriteDataHALCallback(CharacterBuffer, OriginalBytesToTransfer)) != 0)
		{
			// did they write a valid amount?
			if (BytesToTransfer > SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES || BytesToTransfer > OriginalBytesToTransfer)
				return SHELL_INVALID_NUMBER_OF_BYTES_TRANSFERRED;

			// somr or all was written, now read what thye wrote out of the buffer since it was a peek prior
			GenericBufferRead(GenericBuffer, BytesToTransfer, CharacterBuffer, SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES, FALSE);
		}
	}

	return SHELL_SUCCESS;
}

static char* ParseArgument(char** Buffer)
{
	char* StartOfString;
	BOOL SkipSpaces = FALSE;

	// go through and skip white space at the beginning
	while (isspace(**Buffer) != FALSE)
		(*Buffer)++;

	// now we're pointing to something
	// is it null?
	if (**Buffer == 0)
		return NULL;

	// point to the beginning
	StartOfString = *Buffer;

	// so we have a valid string
	// start moving across it, and don't ignore ""
	if (**Buffer == '"')
	{
		// iterate beyond the "
		StartOfString++;
		(*Buffer)++;

		// keep going until we find " or nothing
		while (**Buffer != '"')
		{
			// if we found null in quotes, thats a problem
			if (**Buffer == 0)
				return NULL;

			(*Buffer)++;
		}

		// so now we're pointing to "
		**Buffer = 0;

		// iterate beyond the " to the next portion of the string
		(*Buffer)++;
	}
	else
	{
		if (**Buffer == '\'')
		{
			// iterate beyond the '
			StartOfString++;
			(*Buffer)++;

			// keep going until we find ' or nothing
			while (**Buffer != '\'')
			{
				// if we found null in quotes, thats a problem
				if (**Buffer == 0)
					return NULL;

				// can't have quotes in this mode
				if (**Buffer == '\"')
					return NULL;

				// cant have backslash in this mode
				if (**Buffer == '\\')
					return NULL;

				(*Buffer)++;
			}

			// so now we're pointing to '
			**Buffer = 0;

			// iterate beyond the ' to the next portion of the string
			(*Buffer)++;
		}
		else
		{
			// go through and skip all non white space
			while (isspace(**Buffer) == FALSE && **Buffer != 0)
				(*Buffer)++;

			// if it wasn't the end of the string, null it out and advance
			if (**Buffer != 0)
			{
				// so now we're pointing to some space or NULL
				**Buffer = 0;

				(*Buffer)++;
			}
		}
	}

	return StartOfString;
}

static SHELL_RESULT NanoShellProcessCommand(char* Buffer)
{
	DIRECTORY *WorkingDirectory;
	SHELL_FILE *WorkingFile;
	BOOL Outcome;
	UINT32 argc;
	UINT32 WorkingIndex;

	// we add 2 for the potential location and command name
	// this way the command/file can get a full SHELL_WORKING_ARGUMENTS_ARRAY_SIZE_IN_ELEMENTS
	char *argv[SHELL_WORKING_ARGUMENTS_FULL_ARRAY_SIZE_IN_ELEMENTS];

	// get the first arguments
	for (argc = 0; argc < SHELL_WORKING_ARGUMENTS_FULL_ARRAY_SIZE_IN_ELEMENTS; argc++)
	{
		// get the next argument from the user
		argv[argc] = ParseArgument(&Buffer);

		// we're done if it is NULL
		if (argv[argc] == NULL)
			break;
	}

	// check for overflow
	if (argc == SHELL_WORKING_ARGUMENTS_FULL_ARRAY_SIZE_IN_ELEMENTS)
		if (ParseArgument(&Buffer) != NULL)
			return SHELL_ARGUMENT_OVERFLOW;

	// did we get any arguemnts?
	if (argc == 0)
		return SHELL_SUCCESS;

	WorkingIndex = 0;

	// now we have to assume that the first thing the user wrote is a directory, lets get the directory
	WorkingDirectory = FollowNanoDirectory(argv[WorkingIndex], gRootNanoDirectory, gCurrentWorkingNanoDirectory, &Outcome);

	if (WorkingDirectory == NULL)
	{
		return SHELL_INVALID_CHARACTER_FOUND;
	}

	// did we find a directory
	if (Outcome == TRUE)
	{
		// we did, now look for a file at index 1
		WorkingIndex = 1;
	}

	// now do a search for local files first, then global
	WorkingFile = NanoShellGetWorkingFile(argv[WorkingIndex], gRootNanoDirectory, gCurrentWorkingNanoDirectory);

	if (WorkingFile == NULL)
	{
		// we need to see if it is a global file
		UINT32 i;

		WorkingIndex = 0;

		for (i = 0; i < (sizeof(gCommandList) / sizeof(COMMAND_INFO)) - 1; i++)
		{
			// compare it to a file
			if (strcmp(gCommandList[i].CommandName, argv[WorkingIndex]) == 0)
			{
				// we found a winner
				return gCommandList[i].ExecuteFile(&argv[++WorkingIndex], argc - WorkingIndex, &gOutputStream);
			}
		}
	}
	else
	{
		if (WorkingFile->ExecuteFile == NULL)
			return SHELL_FILE_NOT_EXECUTABLE;

		return WorkingFile->ExecuteFile(&argv[++WorkingIndex], argc - WorkingIndex, &gOutputStream);
	}

	return SHELL_FILE_NOT_FOUND;
}

#if (USE_SHELL_COMMAND_HISTORY == 1)
	static BOOL AddLineToHistory(char* Line)
	{
		char* HistoryLine;

		// if it is an empty string, just leave
		// don't add it to the history
		if (strlen(Line) == 0)
			return FALSE;

		// clear out our history
		gHistoryIndex = 0;

		// if we're at max size, delete the oldest node
		if (LinkedListGetSize(&gHistoryList) == NUMBER_OF_ENTRIES_IN_HISTORY)
			if (LinkedListDeleteNode(&gHistoryList, 1) != TRUE)
				return FALSE;

		// get the space
		HistoryLine = NanoShellMalloc(strlen(Line) + 1);

		// did we get the space?
		if (HistoryLine == NULL)
			return FALSE;

		// now add the string to the linked list

		// copy the data in
		strcpy(HistoryLine, Line);

		return LinkedListAdd(&gHistoryList, HistoryLine);
	}

	SHELL_RESULT UpdateHistory(BOOL Increment)
	{
		UINT32 i;
		char* DataFromHistoryBuffer;

		// Increment our index
		if (Increment == TRUE)
		{
			if (gHistoryIndex == 0)
			{
				gHistoryIndex = LinkedListGetSize(&gHistoryList);
			}
			else
			{
				gHistoryIndex--;

				if (gHistoryIndex == 0)
					gHistoryIndex = 1;
			}
		}
		else
		{
			// if we're at the size, disable us
			if (gHistoryIndex == LinkedListGetSize(&gHistoryList))
			{
				gHistoryIndex = 0;
			}
			else
			{
				if (gHistoryIndex != 0)
					gHistoryIndex++;
			}
		}

		// shift over to the beginning of the prompt
		for (i = 0; i < GenericBufferGetSize(&gInputStream); i++)
		{
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_MOVE_CURSOR_LEFT_COMMAND), SHELL_MOVE_CURSOR_LEFT_COMMAND) != (UINT32)strlen(SHELL_MOVE_CURSOR_LEFT_COMMAND))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		}

		// now clear out the line
		if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_CLEAR_REMAINING_LINE_COMMAND), SHELL_CLEAR_REMAINING_LINE_COMMAND) != (UINT32)strlen(SHELL_CLEAR_REMAINING_LINE_COMMAND))
			return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

		// now read the data from the list
		DataFromHistoryBuffer = LinkedListGetData(&gHistoryList, gHistoryIndex);

		// did we get the data?
		if (DataFromHistoryBuffer == NULL)
			return SHELL_LINKED_LIST_LIBRARY_FAILED;

		// always clear this out
		if (GenericBufferFlush(&gInputStream) == FALSE)
			return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

		if (DataFromHistoryBuffer != NULL)
		{
			if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(DataFromHistoryBuffer), DataFromHistoryBuffer) != (UINT32)strlen(DataFromHistoryBuffer))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

			if(GenericBufferWrite(&gInputStream, (UINT32)strlen(DataFromHistoryBuffer), DataFromHistoryBuffer) != (UINT32)strlen(DataFromHistoryBuffer))
				return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
		}

		return SHELL_SUCCESS;
	}
#endif // end of #if (USE_SHELL_COMMAND_HISTORY == 1)

SHELL_RESULT NanoShellProcessOutgoingData(char* Data, GENERIC_BUFFER* Buffer, UINT32 NumberOfBytesToProcess, UINT32 TransferSizeInBytes, SHELL_RESULT(*WriteTasks)(GENERIC_BUFFER *Buffer))
{
	SHELL_RESULT Result;
	UINT32 DataWritten, NumberOfBytesToWrite;

	// check that they want to send a valid number of bytes
	if (NumberOfBytesToProcess == 0)
		return SHELL_INVALID_PARAMETERS;

	// perform the write tasks
	Result = WriteTasks(Buffer);

	// did it work ok?
	if (Result != SHELL_SUCCESS)
		return Result;

	while (NumberOfBytesToProcess != 0)
	{
		// get the next chunk of data to send
		if (TransferSizeInBytes < (UINT32)NumberOfBytesToProcess)
			NumberOfBytesToWrite = TransferSizeInBytes;
		else
			NumberOfBytesToWrite = (UINT32)NumberOfBytesToProcess;

		// update the number of bytes left to transfer
		NumberOfBytesToProcess -= NumberOfBytesToWrite;

		// now get some data
		DataWritten = GenericBufferWrite(Buffer, NumberOfBytesToWrite, Data);

		// advance the pointer
		Data += DataWritten;

		// perform the write tasks
		Result = WriteTasks(Buffer);

		// did it work ok?
		if (Result != SHELL_SUCCESS)
			return Result;
	}

	return SHELL_SUCCESS;
}

static SHELL_RESULT NanoShellProcessIncomingBuffer(char *IncomingData, UINT32 NumberOfBytes)
{
	while (NumberOfBytes != 0)
	{
		switch(gEscapeSequence)
		{
			case 0:
			{
				switch (*IncomingData)
				{
					case BACKSPACE_ASCII_VALUE:
					{
						if (GenericBufferRemoveLastByteWritten(&gInputStream, NULL) == TRUE)
						{
							// if echo is on, output the data
							#if (SHELL_USE_CONSOLE_ECHO == RUNTIME_CONFIGURABLE)
								if (gConsoleEcho == TRUE)
								{
									if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_MOVE_CURSOR_LEFT_COMMAND), SHELL_MOVE_CURSOR_LEFT_COMMAND) != (UINT32)strlen(SHELL_MOVE_CURSOR_LEFT_COMMAND))
										return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

									if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_CLEAR_REMAINING_LINE_COMMAND), SHELL_CLEAR_REMAINING_LINE_COMMAND) != (UINT32)strlen(SHELL_CLEAR_REMAINING_LINE_COMMAND))
										return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
								}
							#else
								#if (SHELL_USE_CONSOLE_ECHO == ALWAYS_ON)
									// always output the data in this configuration
									if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_DELETE_COMMAND), SHELL_DELETE_COMMAND) != (UINT32)strlen(SHELL_DELETE_COMMAND))
										return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

									if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_CLEAR_REMAINING_LINE_COMMAND), SHELL_CLEAR_REMAINING_LINE_COMMAND) != (UINT32)strlen(SHELL_CLEAR_REMAINING_LINE_COMMAND))
										return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
								#endif // end of #if (SHELL_USE_CONSOLE_ECHO == ALWAYS_ON)
							#endif // end of #if (SHELL_USE_CONSOLE_ECHO == RUNTIME_CONFIGURABLE)
						}

						break;
					}

					case ESCAPE_ASCII_VALUE:
					{
						gEscapeSequence = 1;

						break;
					}

					default:
					{
						// we have data we can write to the input stream

						// write the data to the input stream
						if (GenericBufferWrite(&gInputStream, 1, IncomingData) != 1)
							return SHELL_INPUT_GENERIC_BUFFER_OVERFLOW;

						// if echo is on, output the data
						#if (SHELL_USE_CONSOLE_ECHO == RUNTIME_CONFIGURABLE)
							if (gConsoleEcho == TRUE)
							{
								if (GenericBufferWrite(&gOutputStream, 1, IncomingData) != 1)
									return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
							}
						#else
							#if (SHELL_USE_CONSOLE_ECHO == ALWAYS_ON)
								// always output the data in this configuration
								if (GenericBufferWrite(&gOutputStream, 1, IncomingData) != 1)
									return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
							#endif // end of #if (SHELL_USE_CONSOLE_ECHO == ALWAYS_ON)
						#endif // end of #if (SHELL_USE_CONSOLE_ECHO == RUNTIME_CONFIGURABLE)

						break;
					}
				}

				break;
			}

			case 1:
			{
				// next sequence in the escape potentially
				if (*IncomingData == 0x5B || *IncomingData == 0x4F)
					gEscapeSequence++;
				else
					gEscapeSequence = 0;

				break;
			}

			case 2:
			{
				gEscapeSequence = 0;

				switch (*IncomingData)
				{
					#if (USE_SHELL_COMMAND_HISTORY == 1)
						case UP_ARROW_ASCII_VALUE:
						{
							return UpdateHistory(TRUE);
						}

						case DOWN_ARROW_ASCII_VALUE:
						{
							return UpdateHistory(FALSE);
						}

						case RIGHT_ARROW_ASCII_VALUE:
						{
							break;
						}

						case LEFT_ARROW_ASCII_VALUE:
						{
							break;
						}
					#endif // end of #if (USE_SHELL_COMMAND_HISTORY == 1)
				}

				break;
			}
		}

		// increment our pointer
		IncomingData++;

		// decrease our size
		NumberOfBytes--;
	}

	return SHELL_SUCCESS;
}

static SHELL_RESULT NanoShellReadTasks(void)
{
	BYTE CharacterBuffer[SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES + 1];
	UINT32 BytesToTransfer;

	// read whatever is available
	BytesToTransfer = StreamReaderDataHALCallback(CharacterBuffer, SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES);

	if (BytesToTransfer != 0)
	{
		UINT32 NumberOfBytesToRead;

		if (BytesToTransfer > SHELL_HAL_MAX_TRANSFER_SIZE_IN_BYTES)
			return SHELL_INVALID_NUMBER_OF_BYTES_TRANSFERRED;

		// now write all the incoming data to our buffer
		NanoShellProcessIncomingBuffer(CharacterBuffer, BytesToTransfer);
	
		// did we get an end of line sequence?
		if (GenericBufferContainsSequence(&gInputStream, SHELL_DEFAULT_END_OF_LINE_SEQUENCE, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, &NumberOfBytesToRead) == TRUE)
		{
			SHELL_RESULT Result;

			char TempBuffer[SIZE_OF_SHELL_STACK_BUFFER_IN_BYTES + 1];

			// add on the size of the sequence we are reading
			NumberOfBytesToRead += SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES;

			// read the data out and process the command
			if (GenericBufferRead(&gInputStream, NumberOfBytesToRead, TempBuffer, sizeof(TempBuffer), FALSE) == NumberOfBytesToRead)
			{
				TempBuffer[NumberOfBytesToRead - SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES] = 0;

				#if (USE_SHELL_COMMAND_HISTORY == 1)
					if (AddLineToHistory(TempBuffer) == FALSE)
						return SHELL_HISTORY_BUFFER_FAILURE;
				#endif // end of #if (USE_SHELL_COMMAND_HISTORY == 1)

				// now go ahead and process the command
				Result = NanoShellProcessCommand(TempBuffer);
			}
			else
			{
				Result = SHELL_GENERIC_BUFFER_LIBRARY_FAILED;
			}

			if (Result != SHELL_SUCCESS)
			{
				// to string the error and output it

				if (Result != SHELL_OPERATION_FAILED_OUTPUT_USER_BUFFER)
				{
					#if (USE_SHELL_COLOR == 1)
						if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_ERROR_COLOR), SHELL_ERROR_COLOR) != (UINT32)strlen(SHELL_ERROR_COLOR))
							return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
					#endif // end of #if (USE_SHELL_COLOR == 1)

					if(GenericBufferWrite(&gOutputStream, (UINT32)strlen(gNanoShellError[Result]), gNanoShellError[Result]) != (UINT32)strlen(gNanoShellError[Result]))
						return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;

					#if (USE_SHELL_COLOR == 1 && SHELL_USE_PROMPT == 0)
						if (GenericBufferWrite(&gOutputStream, (UINT32)strlen(SHELL_DEFAULT_TEXT_COLOR), SHELL_DEFAULT_TEXT_COLOR) != (UINT32)strlen(SHELL_DEFAULT_TEXT_COLOR))
							return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
					#endif // end of #if (USE_SHELL_COLOR == 1 && SHELL_USE_PROMPT == 0)

					if (GenericBufferWrite(&gOutputStream, SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES, SHELL_DEFAULT_END_OF_LINE_SEQUENCE) != SHELL_END_OF_LINE_SEQUENCE_SIZE_IN_BYTES)
						return SHELL_OUTPUT_GENERIC_BUFFER_OVERFLOW;
				}
			}

			// now output the prompt last
			return OutputPrompt(gCurrentWorkingNanoDirectory->DirectoryName);
		}
	}

	return SHELL_SUCCESS;
}

SHELL_RESULT NanoShellTasks(void)
{
	SHELL_RESULT WriteResult = NanoShellWriteTasks(&gOutputStream);
	SHELL_RESULT ReadResult = NanoShellReadTasks();

	return SHELL_SUCCESS;
}
