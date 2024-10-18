#ifndef SHELL_H
	#define SHELL_H

#include "../GenericBuffer.h"
#include "NanoShellReturnTypes.h"
#include "NanoFile.h"

#define SHELL_WORKING_ARGUMENTS_FULL_ARRAY_SIZE_IN_ELEMENTS		(SHELL_WORKING_ARGUMENTS_ARRAY_SIZE_IN_ELEMENTS + 2)

#define SHELL_FONT_STYLE_RESET							"\x1B[0m"
#define SHELL_FONT_STYLE_BOLD							"\x1B[1m"
#define SHELL_FONT_STYLE_DISABLED						"\x1B[2m"
#define SHELL_FONT_STYLE_ITALIC							"\x1B[3m"
#define SHELL_FONT_STYLE_UNDERSCORE						"\x1B[4m"

#define SHELL_FONT_COLOR_RED							"\x1B[31m"
#define SHELL_FONT_COLOR_GREEN							"\x1B[32m"
#define SHELL_FONT_COLOR_YELLOW							"\x1B[33m"
#define SHELL_FONT_COLOR_BLUE							"\x1B[34m"
#define SHELL_FONT_COLOR_MAGENTA						"\x1B[35m"
#define SHELL_FONT_COLOR_CYAN							"\x1B[36m"
#define SHELL_FONT_COLOR_WHITE							"\x1B[37m"

#define SHELL_FONT_COLOR_BRIGHT_RED						"\x1B[91m"
#define SHELL_FONT_COLOR_BRIGHT_GREEN					"\x1B[92m"
#define SHELL_FONT_COLOR_BRIGHT_YELLOW					"\x1B[93m"
#define SHELL_FONT_COLOR_BRIGHT_BLUE					"\x1B[94m"
#define SHELL_FONT_COLOR_BRIGHT_MAGENTA					"\x1B[95m"
#define SHELL_FONT_COLOR_BRIGHT_CYAN					"\x1B[96m"
#define SHELL_FONT_COLOR_BRIGHT_WHITE					"\x1B[97m"

#define SHELL_DELETE_COMMAND							"\x1B[1D"
#define SHELL_MOVE_CURSOR_LEFT_COMMAND					"\x1B[1D"
#define SHELL_CLEAR_SCREEN_COMMAND						"\x1B[2J"
#define SHELL_HOME_CURSOR_COMMAND						"\x1B[0;0f"
#define SHELL_CLEAR_REMAINING_LINE_COMMAND				"\x1B[0K"

#define SHELL_RELATIVE_DIRECTORY_SEQUENCE				("." SHELL_DEFAULT_END_OF_LINE_SEQUENCE ".." SHELL_DEFAULT_END_OF_LINE_SEQUENCE)

#define CAT_COMMAND_NAME								"cat"
#define CD_COMMAND_NAME									"cd"
#define CLEAR_COMMAND_NAME								"clear"
#define ECHO_COMMAND_NAME								"echo"
#define HELP_COMMAND_NAME								"help"
#define HEXDUMP_COMMAND_NAME							"hexdump"
#define LS_COMMAND_NAME									"ls"
#define PWD_COMMAND_NAME								"pwd"
#define SHUTDOWN_COMMAND_NAME							"shutdown"

// this is the ascii value for the backspace character
#define BACKSPACE_ASCII_VALUE							0x08
#define ESCAPE_ASCII_VALUE								0x1B
#define UP_ARROW_ASCII_VALUE							41
#define DOWN_ARROW_ASCII_VALUE							42
#define RIGHT_ARROW_ASCII_VALUE							43
#define LEFT_ARROW_ASCII_VALUE							44

typedef struct
{
	char* CommandName;
	SHELL_RESULT(*ExecuteFile)(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER *Buffer);
	char* Description;

	#if (USE_COMMAND_HELP == 1)
		char* Help;
	#endif // #if (USE_COMMAND_HELP == 1)
}COMMAND_INFO;

SHELL_RESULT NanoShellInit(void);

#if (EXTENDED_CAT_SUPPORT == 1)
	SHELL_RESULT ProcessFileReadWithReadOptions(READ_OPTIONS Options, GENERIC_BUFFER* Buffer, char* DataRead, UINT32 NumberOfBytesRead);
#endif // end of #if (EXTENDED_CAT_SUPPORT == 1)

// call this continuously
SHELL_RESULT NanoShellTasks(void);

#endif // end of #ifndef SHELL_H
