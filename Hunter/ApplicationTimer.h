#ifndef APPLICATION_TIMER_H
#define APPLICATION_TIMER_H

#include "SingletonBase.h"

class ApplicationTimer : public SingletonBase<ApplicationTimer>
{
public :
	ApplicationTimer();
	~ApplicationTimer();

	void Initialize(int32 targetMS);

	void Tick();

	inline float GetDeltaTime() { return _currentDeltaSecond; }
	inline int32 GetDeltaMS() { return _currentDeltaMS; }

	inline float GetTargetTime() { return _targetFramePerMS / 1000.0f; }


private :
	LARGE_INTEGER _prevCounter;;
	LARGE_INTEGER _currentCounter;
	LARGE_INTEGER _startCounter;

	float _timeScale;
	LARGE_INTEGER _frequency;

	int32 _currentDeltaMS;
	float _currentDeltaSecond;
	int32 _targetFramePerMS;

};

#define APPTIMER (ApplicationTimer::GetInstance())


#endif