#ifndef _SENSORINPUT_H_
#define _SENSORINPUT_H_

/* System includes */
#include <winsock2.h>


namespace AC_SensorModels
{
	class SensorInput
	{
	public:
		SensorInput();

		bool Frame();

		char name[25];
		int sourcePort;
	};
}
#endif
