#include <string.h>

#include "clear_Command.h"
#include "../../GenericBuffer.h"

SHELL_RESULT clearCommandExecuteMethod(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER* Buffer)
{
	if (NumberOfArgs != 0)
		return SHELL_INVALID_PARAMETERS;
	
	// this clears the screen
	if (GenericBufferWrite(Buffer, (UINT32)strlen(SHELL_CLEAR_SCREEN_COMMAND), SHELL_CLEAR_SCREEN_COMMAND) != (UINT32)strlen(SHELL_CLEAR_SCREEN_COMMAND))
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	// this puts the cursor in the upper left hand corner of the screen
	if (GenericBufferWrite(Buffer, (UINT32)strlen(SHELL_HOME_CURSOR_COMMAND), SHELL_HOME_CURSOR_COMMAND) != (UINT32)strlen(SHELL_HOME_CURSOR_COMMAND))
		return SHELL_GENERIC_BUFFER_LIBRARY_FAILED;

	return SHELL_SUCCESS;
}
