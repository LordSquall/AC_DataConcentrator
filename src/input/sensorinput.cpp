#include "sensorinput.h"

namespace AC_DataConcentrator
{
	SensorInput::SensorInput()
	{
		currentValue.store(0.0f);
	}

	bool SensorInput::UpdateValue(Byte* buffer)
	{
		float value;
		memcpy(&value, buffer, sizeof(currentValue));
		currentValue.store(value);

		return true;
	}
}

