// CTimer.cpp

#include "CTimer.h"


//********** CClock Functions **********

// Initialize the private data
CClock* CClock::mcpInstance								= 0;
unsigned long CClock::mulSavedTime						= timeGetTime();
float CClock::mfScaleFactor								= 1.0f;	// Initialize to Normal speed
unsigned long CClock::mulScaleFactorDuration			= 0;
unsigned long CClock::mulScaleFactorDurationStartTime	= 0;

// Constructor - Does nothing
CClock::CClock()
{}

// Destructor - Does nothing
CClock::~CClock()
{}

// Returns a pointer to the class's Instance
CClock* CClock::ReturnClassInstance()
{
	// If class has been Instanced already
	if (mcpInstance != 0)
	{
		// Return pointer to class Instance
		return mcpInstance;
	}
	
	// Else class has not been Instanced yet so Instance it and return it
	mcpInstance = new CClock();
	return mcpInstance;
}

// Returns the actual amount of Time Elapsed since the last Reset, ignoring the Scale Factor, and Resets the Clock
unsigned long CClock::ReturnActualElapsedTimeAndResetClock()
{
	// Get old Saved Time
	unsigned long ulOldSavedTime = ReturnSavedTime();

	// Reset the Clock
	ResetClock();

	// Return how much time had elapsed since last Reset
	return (ReturnCurrentTime() - ulOldSavedTime);
}

// Returns the actual amount of Time Elapsed since the last Reset, ignoring the Scale Factor, without resetting the Clock
unsigned long CClock::ReturnActualElapsedTimeWithoutResettingClock()
{
	// Return how much time had elapsed since last Reset
	return (ReturnCurrentTime() - ReturnSavedTime());
}

// Returns the Time Elapsed * Scale Factor, and Resets the Clock
unsigned long CClock::ReturnElapsedTimeAndResetClock()
{
	// Get old Saved Time
	unsigned long ulOldSavedTime = ReturnSavedTime();

	// Reset the Clock
	ResetClock();

	// Return how much time had elapsed between old Saved Time and new one
	return ((unsigned long)((ReturnCurrentTime() - ulOldSavedTime) * mfScaleFactor));
}

// Returns the Time Elapsed * Scale Factor, without Resetting the Clock
unsigned long CClock::ReturnElapsedTimeWithoutResettingClock()
{
	// Return how much time had elapsed since last Reset
	return ((unsigned long)((ReturnCurrentTime() - ReturnSavedTime()) * mfScaleFactor)); 
}


// Set Scale Factor for Clocks Modified Elapsed Time
// NOTE: ScaleFactor = 0.5 returns half of elapsed time, ScaleFactor = 2 returns 2 times elapsed time
//		 ScaleFactor = 1 returns actual amount of elapsed time
void CClock::SetScaleFactor(float fScaleFactor, unsigned long ulDuration)
{
	mfScaleFactor = fScaleFactor;

	// If a Duration was specified for this Scale effect
	if (ulDuration != 0)
	{
		// Record the Current Time the new Scale Factor took affect
		mulScaleFactorDurationStartTime = ReturnCurrentTime();
		
		// Record the Duration the new Scale Factor should last for
		mulScaleFactorDuration = ulDuration;
	}
}

// Return Scale Factor for Clocks Modified Elapsed Time
// NOTE: ScaleFactor = 0.5 returns half of elapsed time, ScaleFactor = 2 returns 2 times elapsed time
//		 ScaleFactor = 1 returns actual amount of elapsed time
float CClock::ReturnScaleFactor() const
{
	return mfScaleFactor;
}

// Returns the Saved Time
unsigned long CClock::ReturnSavedTime()
{
	// Restores the Clocks Scale Factor if it should no longer be used
	RestoreScaleFactorIfExpired();

	return mulSavedTime;
}

// Returns the Current system Time in milliseconds (1000ms = 1s)
unsigned long CClock::ReturnCurrentTime()
{
	// Restores the Clocks Scale Factor if it should no longer be used
	RestoreScaleFactorIfExpired();

	return timeGetTime();
}

// Resets the Clock
void CClock::ResetClock()
{
	// Get the new Saved Time
	mulSavedTime = ReturnCurrentTime();
}

// Restores the Scale Factor to 1.0 if it should no longer be used
void CClock::RestoreScaleFactorIfExpired()
{
	// Check to see if a Scale Duration Timer is running
	if (mulScaleFactorDurationStartTime != 0)
	{
		// If this Timer has expired
		if ((timeGetTime() - mulScaleFactorDurationStartTime) > mulScaleFactorDuration)
		{
			// Restore the original Scale Factor
			mfScaleFactor = 1.0f;

			// Reset Scale Duration Timer Start Time
			mulScaleFactorDurationStartTime = 0;
		}
	}
}




//********** CTimer Functions **********

// Constructor
CTimer::CTimer(bool bEffectedByClockScaleFactor)
{
	// Initialize private data
	mcpClock = CClock::ReturnClassInstance();
	RestartTimerUsingCurrentTime();	// Sets mulStartTime
	mbEffectedByClockScaleFactor = bEffectedByClockScaleFactor;
	mfTimerScaleFactor = 1.0f;
	mulTimerScaleDuration = 0;
	mulTimerScaleDurationStartTime = 0;
}

// Resets the Start Time to the Current Time
void CTimer::RestartTimerUsingCurrentTime()
{
	mulStartTime = ReturnCurrentTime();
}

// Resets the Start Time to the Clocks Saved Time
void CTimer::RestartTimerUsingSavedTime()
{
	mulStartTime = ReturnSavedTime();
}

// Returns True if ulTime has passed (Current Time - Start Time), False if not
// NOTE: Restarts Timer if True
bool CTimer::XTimeHasPassedUsingCurrentTime(unsigned long ulTimeInMilliseconds)
{
	if (ReturnTimePassedSinceLastCheckUsingCurrentTime() < ulTimeInMilliseconds)
	{
		// Return that ulTimeInMilliseconds has not yet elapsed
		return false;
	}

	RestartTimerUsingCurrentTime();	// Restart the Timer

	// Return that ulTimeInMilliseconds has elapsed
	return true;
}

// Returns True if ulTime has passed (Clocks Saved Time - Start Time), False if not
// NOTE: Restarts Timer if True
bool CTimer::XTimeHasPassedUsingSavedTime(unsigned long ulTimeInMilliseconds)
{
unsigned long test = 0;
	if (test = ReturnTimePassedSinceLastCheckUsingSavedTime() < ulTimeInMilliseconds)
	{
		// Return that ulTimeInMilliseconds has not yet elapsed
		return false;
	}

	RestartTimerUsingSavedTime();	// Restart the Timer

	// Return that ulTimeInMilliseconds has elapsed
	return true;
}

// Returns True if ulTime has passed (Current Time - Start Time), False if not
// NOTE: Does not restart Timer
bool CTimer::XTimeHasPassedUsingCurrentTimeWithoutRestartingTimer(unsigned long ulTimeInMilliseconds)
{
	if (ReturnTimePassedSinceLastCheckUsingCurrentTime() < ulTimeInMilliseconds)
	{
		return false;	// Return that ulTimeInMilliseconds has not yet elapsed
	}

	return true;		// Return that ulTimeInMilliseconds has elapsed
}

// Returns True if ulTime has passed (Saved Time - Start Time), False if not
// NOTE: Does not restart Timer
bool CTimer::XTimeHasPassedUsingSavedTimeWithoutRestartingTimer(unsigned long ulTimeInMilliseconds)
{
	if (ReturnTimePassedSinceLastCheckUsingSavedTime() < ulTimeInMilliseconds)
	{
		return false;	// Return that ulTimeInMilliseconds has not yet elapsed
	}

	return true;		// Return that ulTimeInMilliseconds has elapsed
}

// Returns how much time has passed since last check using the Current Time
unsigned long CTimer::ReturnTimePassedSinceLastCheckUsingCurrentTime()
{
	// If this Timer is succeptable to the Clock Scale Factor
	if (mbEffectedByClockScaleFactor == true)
	{
		// Return the Time Passed * the Clock Scale Factor
		return ((unsigned long)(((ReturnCurrentTime() - ReturnStartTime()) * mcpClock->ReturnScaleFactor()) * mfTimerScaleFactor));
	}else
	{
		// Return the Time Passed
		return ((unsigned long)((ReturnCurrentTime() - ReturnStartTime()) * mfTimerScaleFactor));
	}
}

// Returns how much time has passed since last check using the Clocks Saved Time
unsigned long CTimer::ReturnTimePassedSinceLastCheckUsingSavedTime()
{
	// If this Timer is succeptable to the Clock Scale Factor
	if (mbEffectedByClockScaleFactor == true)
	{
		// Return the Time Passed * the Clock Scale Factor
		return ((unsigned long)(((ReturnSavedTime() - ReturnStartTime()) * mcpClock->ReturnScaleFactor()) * mfTimerScaleFactor));
	}else
	{
		// Return the Time Passed
		return ((unsigned long)((ReturnSavedTime() - ReturnStartTime()) * mfTimerScaleFactor));
	}
}

// Returns how much time has passed since last check using the Current Time
// NOTE: Also restarts the Timer
unsigned long CTimer::ReturnTimePassedSinceLastCheckUsingCurrentTimeAndRestartTimer()
{
	unsigned long ulTimePassed = 0;

	// Get the Time Passed
	ulTimePassed = ReturnTimePassedSinceLastCheckUsingCurrentTime();

	// Restart the Timer
	RestartTimerUsingCurrentTime();

	// Return how much Time has Passed since last check
	return ulTimePassed;
}

// Returns how much time has passed since last check using the Clocks Saved Time
// NOTE: Also restarts the Timer
unsigned long CTimer::ReturnTimePassedSinceLastCheckUsingSavedTimeAndRestartTimer()
{
	unsigned long ulTimePassed = 0;

	// Get the Time Passed
	ulTimePassed = ReturnTimePassedSinceLastCheckUsingSavedTime();

	// Restart the Timer
	RestartTimerUsingSavedTime();

	// Return how much Time has Passed since last check
	return ulTimePassed;
}

// Set if this Timer is effected by the Clocks Scale Factor or not
void CTimer::TimerIsAffectedByClockScaleFactor(bool bTrueOrFalse)
{
	mbEffectedByClockScaleFactor = bTrueOrFalse;
}

// Return if this Timer is effected by the Clocks Scale Factor or not
bool CTimer::ReturnIfTimerIsAffectedByClockScaleFactor() const
{
	return mbEffectedByClockScaleFactor;
}

// Set this Timers individual Scale Factor
// NOTE: ScaleFactor = 0.5 returns half of elapsed time, ScaleFactor = 2 returns 2 times elapsed time
//		 ScaleFactor = 1.0 returns actual amount of elapsed time, ScaleFactor = 0 pauses Timer
// NOTE: ulDuration = 0 means to use the given Scale Factor until explicitly changed again later
// NOTE: After ulDuration expires, the Scale Factor will be set back to 1.0 automatically
void CTimer::SetTimerScaleFactor(float fScaleFactor, unsigned long ulDuration)
{
	// Set the new Scale Factor
	mfTimerScaleFactor = fScaleFactor;

	// If a Duration was specified for this Scale effect
	if (ulDuration != 0)
	{
		// Record the Current Time the new Scale Factor took affect
		mulTimerScaleDurationStartTime = ReturnCurrentTime();
		
		// Record the Duration the new Scale Factor should last for
		mulTimerScaleDuration = ulDuration;
	}
}

// Return this Timers individual Scale Factor
float CTimer::ReturnTimerScaleFactor() const
{
	return mfTimerScaleFactor;
}

// Returns the time the Timer was started
unsigned long CTimer::ReturnStartTime() const
{
	return mulStartTime;
}

// Returns the Clocks Saved Time
unsigned long CTimer::ReturnSavedTime()
{
	// Restores the Timers Scale Factor if it should no longer be used
	RestoreTimerScaleFactorIfExpired();

	return mcpClock->ReturnSavedTime();
}

// Returns the Current Time
unsigned long CTimer::ReturnCurrentTime()
{
	// Restores the Timers Scale Factor if it should no longer be used
	RestoreTimerScaleFactorIfExpired();

	return mcpClock->ReturnCurrentTime();
}

// Restores the Scale Factor to 1.0 if it should no longer be used
void CTimer::RestoreTimerScaleFactorIfExpired()
{
	// Check to see if a Scale Duration Timer is running
	if (mulTimerScaleDurationStartTime != 0)
	{
		// If this Timer has expired
		if ((timeGetTime() - mulTimerScaleDurationStartTime) > mulTimerScaleDuration)
		{
			// Restore the original Scale Factor
			mfTimerScaleFactor = 1.0f;

			// Reset Scale Duration Timer Start Time
			mulTimerScaleDurationStartTime = 0;
		}
	}
}
