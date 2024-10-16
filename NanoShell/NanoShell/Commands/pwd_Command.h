#ifndef PWD_COMMAND_H
	#define PWD_COMMAND_H

#include "../NanoShell.h"

#define PWD_HELP_TEXT \
\
PWD_COMMAND_NAME ": " PWD_COMMAND_NAME SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
SHELL_TAB "Print name of current/working directory." SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
SHELL_TAB "Example:" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
SHELL_TAB SHELL_TAB PWD_COMMAND_NAME SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
SHELL_DEFAULT_END_OF_LINE_SEQUENCE

SHELL_RESULT pwdCommandExecuteMethod(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER* Buffer);

#endif // end of PWD_COMMAND_H