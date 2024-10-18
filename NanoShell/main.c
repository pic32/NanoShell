#include <stdio.h>
#include <windows.h>

#include "./NanoShell/NanoShell.h"
#include "./NanoShell/NanoDirectory.h"

BYTE gDataRead = 0;

void ShellPowerDownCallback(void)
{
	// this is called at power down
}

DWORD WINAPI ThreadFunc(void* data) 
{
	BYTE LocalData;

	// Do stuff.  This will be the first function called on the new thread.
	// When this function returns, the thread goes away.  See MSDN for more details.
	while (1)
	{
		LocalData = getch();

		// 224 comes in when it is a special character
		if (LocalData == 0xE0)
		{
			while (gDataRead != 0)
				Sleep(50);

			gDataRead = ESCAPE_ASCII_VALUE;

			LocalData = getch();

			switch (LocalData)
			{
				case 0x48:
				{
					// this is the up arrow
					LocalData = 0x5B;

					while (gDataRead != 0)
						Sleep(50);

					gDataRead = LocalData;

					while (gDataRead != 0)
						Sleep(50);

					LocalData = UP_ARROW_ASCII_VALUE;

					break;
				}

				case 0x4B:
				{
					// this is the left arrow
					LocalData = 0x5B;

					while (gDataRead != 0)
						Sleep(50);

					gDataRead = LocalData;

					while (gDataRead != 0)
						Sleep(50);

					LocalData = LEFT_ARROW_ASCII_VALUE;

					break;
				}

				case 0x4D:
				{
					// this is the right arrow
					LocalData = 0x5B;

					while (gDataRead != 0)
						Sleep(50);

					gDataRead = LocalData;

					while (gDataRead != 0)
						Sleep(50);

					LocalData = RIGHT_ARROW_ASCII_VALUE;

					break;
				}

				case 0x50:
				{
					// this is the down arrow
					LocalData = 0x5B;

					while (gDataRead != 0)
						Sleep(50);

					gDataRead = LocalData;

					while (gDataRead != 0)
						Sleep(50);

					LocalData = DOWN_ARROW_ASCII_VALUE;

					break;
				}
			}
		}

		while (gDataRead != 0)
			Sleep(50);
			
		gDataRead = LocalData;
	}

	return 0;
}

// this is the callback that the shell calls to send data out
UINT32 StreamWriteDataHALCallback(BYTE* DataBuffer, UINT32 DataBuffersSizeInBytes)
{
	printf("%s", DataBuffer);

	return DataBuffersSizeInBytes;
}

// this is the callback that the shell calls to read data in
UINT32 StreamReaderDataHALCallback(BYTE* DataBuffer, UINT32 DataBuffersSizeInBytes)
{
	if (gDataRead != 0)
	{
		*DataBuffer = gDataRead;

		gDataRead = 0;

		if (*DataBuffer++ == '\r')
		{
			*DataBuffer = '\n';

			return 2;
		}

		return 1;
	}
	else
	{
		return 0;
	}
}

HANDLE Thread;
char gFolder[] = { "folder" };

char gFile1Name[] = { "readme" };
char gFile2Name[] = { "file" };
char gFile3Name[] = { "exe" };

char gFolderDescription[] = { "folder description" };

char gFile1Help[] = { "readme Help" };
char gFile3Help[] = { "exec help" };

char gFile1Desc[] = { "readme description" };
char gFile2Desc[] = { "file description" };

DIRECTORY gDir;
SHELL_FILE gFile1, gFile2, gFile3;

BYTE gDemoText[] = {"Nano Shell:\r\nThis is an example file in the Nano Shell!"};
BYTE gDemoTextBuffer[512] = {"\0"};
char *gFileExecuteMessage = SHELL_FONT_COLOR_BRIGHT_MAGENTA "file execute called!\r\n";

// this is the read handler for the readme file
SHELL_RESULT DemoRead(GENERIC_BUFFER* Buffer)
{
	if (GenericBufferWrite(Buffer, (UINT32)strlen(gDemoText), gDemoText) != (UINT32)strlen(gDemoText))
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	return SHELL_SUCCESS;
}

// this is the read handler for the file "file"
SHELL_RESULT DemoRead2(GENERIC_BUFFER* OutputStream)
{
	if (GenericBufferWrite(OutputStream, (UINT32)strlen(gDemoTextBuffer), gDemoTextBuffer) != (UINT32)strlen(gDemoTextBuffer))
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	return SHELL_SUCCESS;
}

// this is the write handler for the file "file"
SHELL_RESULT DemoWrite(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER *OutputStream)
{
	if (NumberOfArgs != 1)
		return SHELL_INVALID_PARAMETERS;

	// write the text to the array
	strcpy(gDemoTextBuffer, Args[0]);

	return SHELL_SUCCESS;
}

// this is the execute handler for the file "exe"
SHELL_RESULT DemoExecute(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER *OutputStream)
{
	// execute an action
	if (GenericBufferWrite(OutputStream, (UINT32)strlen(gFileExecuteMessage), gFileExecuteMessage) != (UINT32)strlen(gFileExecuteMessage))
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	return SHELL_SUCCESS;
}

int main(void)
{
	// start up the thread that'll read in the data input
	Thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

	// initialize the shell
	if (NanoShellInit() != SHELL_SUCCESS)
	{
		printf("NanoShellInit Failed!\r\n");

		exit(0);
	}

	// create the directories and files
	if (CreateNanoDirectory("", &gDir, gFolder, gFolderDescription) != SHELL_SUCCESS)
	{
		printf("CreateNanoDirectory Failed!\r\n");

		exit(0);
	}

	if (CreateNanoFile("", &gFile1, gFile1Name, DemoRead, NULL, NULL, gFile1Desc, gFile1Help) != SHELL_SUCCESS)
	{
		printf("CreateNanoFile Failed!\r\n");

		exit(0);
	}

	if (CreateNanoFile("/folder", &gFile2, gFile2Name, DemoRead2, DemoWrite, NULL, gFile2Desc, NULL) != SHELL_SUCCESS)
	{
		printf("CreateNanoFile Failed!\r\n");

		exit(0);
	}

	if (CreateNanoFile("/folder", &gFile3, gFile3Name, NULL, NULL, DemoExecute, NULL, gFile3Help) != SHELL_SUCCESS)
	{
		printf("CreateNanoFile Failed!\r\n");

		exit(0);
	}

	// startup the main loop
	while (1)
		NanoShellTasks();

	return 0;
}