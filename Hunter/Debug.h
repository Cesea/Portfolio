#ifndef DEBUG_H
#define DEBUG_H

#include "SingletonBase.h"
#include "BitFlags.h"

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef assert
#undef assert
#endif

#ifdef VERIFY
#undef VERIFY
#endif

#ifdef verify
#undef verify
#endif

#ifdef TRACE
#undef trace
#endif

#ifdef _DEBUG_OUTPUT
#define debug_print Debug.output

#define debug_assert(x, comment) {\
	static bool _ignoreAssert = false; \
	if(!_ignoreAssert && !(x)) \
	{ \
		ErrorResult _errorResult = NotifyAssertion(#x, #comment, __FILE__, __LINE__); \
		if(_errorResult == ER_IGNORE) \
		{ \
			_ignoreAssert = true;\
		}\
		else if(_errorResult == ER_BREAKPOINT) \
		{ \
			_asm{int 3}; \
		}\
	}\
	}

#define debug_error(x) {\
	static bool _ignoreError = false; \

}


#else
#endif


#define ASSERT(x) debug_assert(x, "")
#define assert(x) debug_assert(x, "")
#define error(x) debug_error(x, "")
#define TRACE(x) debug_print

enum ErrorResult
{
	ER_IGNORE,
	ER_CONTINUE,
	ER_BREAKPOINT,
	ER_ABORT
};

extern ErrorResult NotifyError(uint32 errorCode, const char *fileName, int32 lineNumber);
extern ErrorResult NotifyAssertion(const char *condition, const char *description, const char *fileName, int32 lineNumber);

extern ErrorResult DisplayError(const char *errorTitle, 
	const char *errorText, 
	const char *errorDescription, 
	const char *fileName, 
	int32 lineNumber);

class DebugMessageChannel;

class DebugMessageHandler : public SingletonBase<DebugMessageHandler>
{
public :
	enum
	{
		MaxOutputStringSize = 2048
	};

	enum DebugChannels
	{
		SystemChannelBit = 0,
		MaxChannels = 32
	};

	enum MessageFlag
	{
		DefautlMessageFlag = FLAG(0),
	};

	DebugMessageHandler();
	~DebugMessageHandler();

	bool32 SetChannel(int32 index, DebugMessageChannel *pChannel);
	void ClearChannel(int32 index);
	bool32 OpenChannel(int32 index);
	void CloseChannel(int32 index);

	void SetChannelMessageFilter(int32 index, Uint32Flags flags);
	void AddChannelMessageFilter(int32 index, Uint32Flags flags);
	void RemoveChannelMessageFilter(int32 index, Uint32Flags flags);

	void Output(const char *text, ...);
	void Output(uint32 messageFlags, const char *text, ...);
	void ProcessMessage(uint32 messageFlags, const char *text, va_list argList);

	bool32 IsChannelSet(int32 index) const;
	bool32 IsChannelOpen(int32 index) const;
	Uint32Flags ChannelFilter(int32 index) const;

private :

	// Private Data...
	DebugMessageChannel*		_channels[MaxChannels];
	Uint32Flags					_openChannels;
};


//. Accessors ...................................................
inline bool32 DebugMessageHandler::IsChannelSet(int32 index)const
{
	debug_assert(index >= 0 && index < MaxChannels, "invalid channel index");
	return _channels[index] != 0;
}

inline bool32 DebugMessageHandler::IsChannelOpen(int32 index)const
{
	debug_assert(index >= 0 && index < MaxChannels, "invalid channel index");
	return TEST_BIT(_openChannels, index);
}


#endif