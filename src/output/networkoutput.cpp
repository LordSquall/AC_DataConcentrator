#include "networkoutput.h"

namespace AC_DataConcentrator
{
	NetworkOutputStructure::NetworkOutputStructure()
	{
		blocks = NULL;
		blockCount = 0;
		msgBuffer = 0;
		totalSize = 0;
	}


	NetworkOutputStructure::NetworkOutputStructure(unsigned int count)
	{
		blocks = new NetworkStructureSensorBlock[count];
		blockCount = count;
		msgBuffer = 0;
		totalSize = 0;
	}

	void NetworkOutputStructure::AllocateBuffer()
	{
		msgBuffer = new Byte[totalSize];
	}

	void NetworkOutputStructure::BuildBuffer()
	{
		/* Flow variables */
		unsigned int i = 0;
		float value;

		for (i = 0; i < blockCount; i++)
		{
			value = blocks[i].boundValue->load();
			printf("Value: %f\n", value);
			memcpy(&msgBuffer[blocks[i].offset], &value, blocks[i].size);
		}
	}

	NetworkOutput::NetworkOutput()
	{

	}

	bool NetworkOutput::UpdateValue(Byte* buffer)
	{
		return true;
	}
}

