#ifndef APPLICATION_TIMER_H
#define APPLICATION_TIMER_H

class ApplicationTimer
{
public :
	ApplicationTimer();
	~ApplicationTimer();

	void Initialize(int32 targetMS);

	void Tick();

	float GetDeltaTime();


private :
	LARGE_INTEGER _prevCounter;;
	LARGE_INTEGER _currentCounter;
	LARGE_INTEGER _startCounter;

	LARGE_INTEGER _frequency;

	int32 _currentDeltaMS;
	int32 _targetFramePerMS;
};


#endif