#ifndef HELP_COMMAND_H
	#define HELP_COMMAND_H

#include "../NanoShell.h"

#if (EXTENDED_HELP_SUPPORT == 1)
	#define HELP_HELP_TEXT \
	\
	HELP_COMMAND_NAME ": " HELP_COMMAND_NAME " [-d] [file]" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "Display information about built in commands or files." SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "This command will output the help text of the specified file." SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "If no file is specified, the global help text is generated." SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "Options:" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB SHELL_TAB "-d        output short description for each topic" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "Examples:" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB SHELL_TAB HELP_COMMAND_NAME SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB SHELL_TAB HELP_COMMAND_NAME " /a-folder/file-to-get-help-about" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB SHELL_TAB HELP_COMMAND_NAME " -d /a-folder/file-to-get-help-about another-help-file" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE
#else
	#define HELP_HELP_TEXT \
	\
	HELP_COMMAND_NAME ": " HELP_COMMAND_NAME " [file]" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "Display information about built in commands or files." SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "This command will output the help text of the specified file." SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "If no file is specified, the global help text is generated." SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB "Examples:" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB SHELL_TAB HELP_COMMAND_NAME SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_TAB SHELL_TAB HELP_COMMAND_NAME " /a-folder/file-to-get-help-about" SHELL_DEFAULT_END_OF_LINE_SEQUENCE \
	SHELL_DEFAULT_END_OF_LINE_SEQUENCE
#endif // end of #if (EXTENDED_HELP_SUPPORT == 1)

SHELL_RESULT helpCommandExecuteMethod(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER* Buffer);

#endif // end of HELP_COMMAND_H