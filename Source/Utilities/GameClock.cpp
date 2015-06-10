#include <Precompiled.h>
#include "GameClock.h"

GameClock::GameClock()
{
    LARGE_INTEGER frequency;

    ErrorIf(QueryPerformanceFrequency(&frequency) == false, "QueryPerformanceFrequency() failed!");

    Frequency = (double)frequency.QuadPart;
    
    Reset();
}

void GameClock::Reset()
{
    LARGE_INTEGER startTime;
    QueryPerformanceCounter(&startTime);
    StartTime = startTime.QuadPart;

    CurrentTime = StartTime;
    LastTime = CurrentTime;
    ElapsedTime = 0;
}

double GameClock::Update()
{
    if (Paused)
    {
        return 0;
    }
    LARGE_INTEGER startTime;
    QueryPerformanceCounter(&startTime);
    StartTime = startTime.QuadPart;
    double elapsedTime = (StartTime - CurrentTime) / Frequency;
    double dt = elapsedTime - ElapsedTime;
    ElapsedTime = elapsedTime;
    return dt;
}