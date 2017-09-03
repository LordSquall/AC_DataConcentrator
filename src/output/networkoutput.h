#ifndef _NETWORKOUTPUT_H_
#define _NETWORKOUTPUT_H_

/* System includes */
#include <winsock2.h>
#include <atomic>

/* Typedefs */
typedef char Byte;

namespace AC_DataConcentrator
{
	struct NetworkStructureSensorBlock
	{
		char name[25];
		unsigned int size;
		unsigned int offset;

		std::atomic<float>* boundValue;
	};

	class NetworkOutputStructure
	{
	public:
		NetworkOutputStructure();
		NetworkOutputStructure(unsigned int size);

		void AllocateBuffer();
		void BuildBuffer();

		NetworkStructureSensorBlock* blocks;
		unsigned int blockCount;

		unsigned int totalSize;
	
		Byte* msgBuffer;

	};

	class NetworkOutput
	{
	public:
		NetworkOutput();

		bool UpdateValue(Byte* buffer);

		char name[25];
		char destinationIPAddress[16];
		int destinationPort;

		NetworkOutputStructure outputStructure;

		int socket;
		struct sockaddr_in socketAddr;
		struct sockaddr_in socketOtherAddr;
		Byte buf[255];
	};
}
#endif
