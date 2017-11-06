#ifndef CODE_TIMER_H
#define CODE_TIMER_H


#define BEGIN_PROFILE(name) static CodeTimer _ct_##name(#name); \
	_ct_##name.BeginSection();

#define END_PROFILE(name) _ct_##name.EndSection();

#define PROFILE_SCOPE(name) static CodeTimer _ct_##name(#name); \
	FunctionTimer _ft_name(&_ct_##name);

class CodeTimer
{
public :
	static CodeTimer RootTimer;

	enum Constants
	{
		MaxNameLength = 32
	};

	CodeTimer(const char *name);
	~CodeTimer() {}

	void BeginSession();
	void EndSession();
	void Reset();
	void Output(u32Flags messageFlags);

	void OutputAllTimers(u32Flags messageFlags);
	void ResetAllTimers();

	// Accessors...
	float	AverageTime()const;
	float	TotalTime()const;
	uint32	TotalCalls()const;

	float	MaximumTimeSample()const;
	float	MinimumTimeSample()const;

	const char* Name()const;
private :

	// Private Data...
	CodeTimer* _pNextProfile;
	CodeTimer* _pLastProfile;

	float	_totalTime;
	uint32	_totalCalls;

	float	_maximumTimeSample;
	float	_minimumTimeSample;

	char	_name[MaxNameLength];
	float	_startTime;

	static CodeTimer* _pPreviousTimer;
};


class FunctionTimer
{
public:
	FunctionTimer(CodeTimer* timer)
		:_pInternalTimerLink(timer)
	{
		debug_assert(_pInternalTimerLink, "A timer link must be provided");
		_pInternalTimerLink->BeginSession();
	}

	~FunctionTimer()
	{
		_pInternalTimerLink->EndSession();
	}

private:
	CodeTimer* _pInternalTimerLink;
};


#endif