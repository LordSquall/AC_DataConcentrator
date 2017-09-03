#ifndef _SENSORINPUT_H_
#define _SENSORINPUT_H_

/* System includes */
#include <winsock2.h>
#include <atomic>

/* Typedefs */
typedef char Byte;

namespace AC_DataConcentrator
{
	class SensorInput
	{
	public:
		SensorInput();

		bool UpdateValue(Byte* buffer);

		std::atomic<float> currentValue;

		char name[25];
		int sourcePort;

		int socket;
		struct sockaddr_in socketAddr;
		struct sockaddr_in socketOtherAddr;
		Byte buf[255];
	};
}
#endif
