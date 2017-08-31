#include "timer.h"

Timer::Timer()
{

}

bool Timer::Initialise()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	if (_frequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	_ticksPerMs = (float)(_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);

	return true;
}

void Timer::Frame()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - _startTime);

	_frameTime = timeDifference / _ticksPerMs;

	//_startTime = currentTime;

	return;
}

float Timer::GetTime()
{
	return _frameTime;
}