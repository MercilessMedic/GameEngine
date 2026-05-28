#include <SDL.h>
#include "Timer.h"

Timer::Timer():
	startTicks(0), pausedTicks(0), lastFrameTicks(0), dt(0.0f),
	paused(false), started(false)
{}

//Starts or Restarts the timer
void Timer::start()
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks64();
	pausedTicks = 0;
}

void Timer::stop()
{
	started = false;
	paused = false;
	startTicks = 0;
	pausedTicks = 0;
}

void Timer::pause()
{
	if(started && !paused)
	{
		paused = true;
		pausedTicks = SDL_GetTicks64() - startTicks;
		startTicks = 0;
	}
}

void Timer::unpause()
{
	if (started && paused)
	{
		paused = false;
		startTicks = SDL_GetTicks64() - pausedTicks;
		pausedTicks = 0;
	}
}

Uint32 Timer::getTicks()
{
	Uint32 time = 0;
	if (started)
	{
		if (paused)
		{
			time = pausedTicks;
		}
		else
		{
			time = SDL_GetTicks64() - startTicks;
		}
	}
	return time;
}

float Timer::getDeltaSeconds()
{
	Uint64 currentTicks = SDL_GetTicks64();
	Uint64 diff = currentTicks - lastFrameTicks;
	dt = float(diff) / 1000.0f;
	lastFrameTicks = currentTicks;

	return dt;
}

bool Timer::isStarted()
{
	return started;
}

bool Timer::isPaused()
{
	return paused;
}