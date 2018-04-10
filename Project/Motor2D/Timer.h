#ifndef __TIMER_H__
#define __TIMER_H__

#include "p2Defs.h"

class Timer
{
public:

	Timer();

	void Start();
	uint32 Read()const;
	float ReadSec()const;

private:

	uint32 started_at;

};

#endif