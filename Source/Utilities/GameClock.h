#pragma once
#include <Precompiled.h>

class GameClock
{
public:
    GameClock();
    const DoubleInteger& GetStartTime()    const   { return StartTime;   }
    const DoubleInteger& GetCurrentClockTime()  const   { return CurrentTime; }
    const DoubleInteger& GetLastTime()     const   { return LastTime;    }
    const double& GetElapsedTime()  const   { return ElapsedTime; }
    double GetFrequency() const { return Frequency; }
    

    void Reset();
    double Update();
    
    void Pause() { SetPaused(true); }
    void Resume() { SetPaused(false); }
    void SetPaused(bool isPaused) { Paused = isPaused; }
private:

    DoubleInteger StartTime = 0;
    DoubleInteger CurrentTime = 0;
    DoubleInteger LastTime = 0;
    double ElapsedTime = 0;
    double Frequency;
    bool Paused = false;
};