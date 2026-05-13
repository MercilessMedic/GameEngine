#pragma once
#include <SDL.h>

class Timer
{
public:

	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();
	float getDeltaSeconds();

	bool isStarted();
	bool isPaused();

private:
	Uint32 startTicks;
	Uint32 pausedTicks;
	Uint32 lastFrameTicks;
	float dt;
	bool paused;
	bool started;
};


