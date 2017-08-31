#ifndef _TIMER_H_
#define _TIMER_H_

#include <Windows.h>

class Timer
{
public:
	Timer();

	bool Initialise();
	void Frame();
	float GetTime();

private:
	INT64 _frequency;
	float _ticksPerMs;
	INT64 _startTime;
	float _frameTime;
};

#endif
