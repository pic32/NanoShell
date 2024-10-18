#include "../NanoShellConfig.h"

#if (EXTENDED_CD_SUPPORT == 1)
	#include <string.h>
#endif // end of #if (EXTENDED_CD_SUPPORT == 1)

#include "cd_Command.h"
#include "../NanoDirectory.h"

extern DIRECTORY *gCurrentWorkingNanoDirectory;
extern DIRECTORY *gRootNanoDirectory;

#if (EXTENDED_CD_SUPPORT == 1)
	DIRECTORY *gPriorDirectory = NULL;
#endif // end of #if (EXTENDED_CD_SUPPORT == 1)

SHELL_RESULT cdCommandExecuteMethod(char* Args[], UINT32 NumberOfArgs, GENERIC_BUFFER* Buffer)
{
	#if (EXTENDED_CD_SUPPORT == 1)
		BOOL Outcome = TRUE;
		DIRECTORY *Directory = NULL;
	#else
		BOOL Outcome;
		DIRECTORY *Directory;
	#endif // end of #if (EXTENDED_CD_SUPPORT == 1)

	if (NumberOfArgs == 0)
		return SHELL_INSUFFICIENT_ARGUMENTS_FOR_FILE;

	#if (EXTENDED_CD_SUPPORT == 1)
		if (strlen(Args[0]) == 1)
		{
			// look for -

			// is it back to previous?
			if (*Args[0] == '-')
			{
				// it is

				// if this is the first one, just make it look like we were at the root
				if (gPriorDirectory == NULL)
					gPriorDirectory = gRootNanoDirectory;

				Directory = gPriorDirectory;
			}
		}

		// if we didn't get assigned, assume the directory is something else and valid
		if(Directory == NULL)
			Directory = FollowNanoDirectory(Args[0], gRootNanoDirectory, gCurrentWorkingNanoDirectory, &Outcome);
	#else
		Directory = FollowNanoDirectory(Args[0], gRootNanoDirectory, gCurrentWorkingNanoDirectory, &Outcome);
	#endif // end of #if (EXTENDED_CD_SUPPORT == 1)

	if (Directory == NULL || Outcome == FALSE)
		return SHELL_INVALID_DIRECTORY_NAME;

	#if (EXTENDED_CD_SUPPORT == 1)
	if (gPriorDirectory == NULL)
		gPriorDirectory = gRootNanoDirectory;
	else
		gPriorDirectory = gCurrentWorkingNanoDirectory;
	#endif // end of #if (EXTENDED_CD_SUPPORT == 1)

	gCurrentWorkingNanoDirectory = Directory;

	return SHELL_SUCCESS;
}