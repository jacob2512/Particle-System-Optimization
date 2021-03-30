/*
 *  Timer.h
 */

#pragma once

#include <windows.h>

class timer
{
public:
	static void initTimer();

	timer()
	{
		this->reset();
	}

	void tic();
	void toc();
	void reset();
	float timeInSeconds();

private:	
	LARGE_INTEGER getTimer();

	static float  SecondsPerCycle;
	LARGE_INTEGER ticTime;
	LARGE_INTEGER tocTime;
	LARGE_INTEGER deltaTime;
	float		  timeSeconds;

};