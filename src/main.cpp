
/* system includes */
#include <stdio.h>
#include <ctime>

/* local project includes */
#include "ac_dataconcentratorconfig.h"
#include "framework.h"

using namespace AC_DataConcentrator;

char dataLocation[255];

unsigned int ProcessArguments(int argc, char *argv[]);

void DisplayVersionInfo();

void DisplayUsage();

void main(int argc, char *argv[])
{
	/* Local variables */
	bool running				= true;	/* termination flag */
	unsigned int sensorCount	= 0;	/* number of sensors */
	unsigned int result			= 0;	/* process arguments result */

	/* Process arguments */
	result = ProcessArguments(argc, argv);
	if (result < 0)
	{
		/* Failure to process args, display program usage */
		DisplayUsage();
		return;
	}
	else if (result > 0)
	{
		/* No error, but args configuration means we terminate the application */
		return;
	}

	/* Create Framework */
	Framework* framework = new Framework();

	/* Initialise the framework */
	framework->Initialise(dataLocation);

	while (running == true)
	{
		/* Run framework frame */
		running = framework->Frame();
	}
}

unsigned int ProcessArguments(int argc, char *argv[])
{
	/* Flow variables */
	int i = 0;

	/* We should have at least 1 arg. First arg is always applcation invoke path */
	if (argc == 1)
	{
		printf("At least 1 argument must to specified\n");
		return -1;
	}

	/* Process Arguments */
	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-v") == 0)
		{
			DisplayVersionInfo();
			return 1;
		}
		else if (strcmp(argv[i], "-h") == 0)
		{
			DisplayUsage();
			return 1;
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			/* Make sure there is enought args to continue */
			if (i + 1 == argc)
			{
				printf("Unable to process '-d' directory\n");
				return -1;
			}

			/* Copy arg to directory variable */
			strcpy_s(dataLocation, argv[i + 1]);
			i++;
		}

	}

	return 0;
}

void DisplayVersionInfo()
{
	/* Print version information to screen */
	printf("AC Data Concentrator - Version %d.%d\n", AC_Data_Concentrator_VERSION_MAJOR, AC_Data_Concentrator_VERSION_MINOR);
}

void DisplayUsage()
{
	printf("Usage:\n");
	printf("\t-h\t\t:Show usage page\n");
	printf("\t-v\t\t:Show version info\n");
	printf("\t-d <dir>\t:Set data directory\n");
}