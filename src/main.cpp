
/* system includes */
#include <stdio.h>
#include <ctime>

/* local project includes */
#include "ac_dataconcentratorconfig.h"
#include "framework.h"

using namespace AC_DataConcentrator;

int main()
{
	/* Local variables */
	bool running				= true;	/* termination flag */
	unsigned int sensorCount	= 0;	/* number of sensors */


	/* Print version information to screen */
	printf("AC Data Concentrator - Version %d.%d\n", AC_Data_Concentrator_VERSION_MAJOR, AC_Data_Concentrator_VERSION_MINOR);

	/* Create Framework */
	Framework* framework = new Framework();

	/* Initialise the framework */
	framework->Initialise();

	while (running == true)
	{
		/* Run framework frame */
		running = framework->Frame();
	}
	float currentTime = 0.0f;

	/* enter infinite loop */
	while (true)
	{
		if (clock() % 2000 == 0)
		{
			//printf("foobar\n");
		}
	}

	// TEMP allow console to remain open in IDE
	getchar();
}