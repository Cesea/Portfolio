#include "stdafx.h"
#include "Debug.h"

ErrorResult NotifyError(uint32 errorCode, const char * fileName, int32 lineNumber)
{
	return ErrorResult();
}

ErrorResult NotifyAssertion(const char * condition, const char * description, const char * fileName, int32 lineNumber)
{
	return ErrorResult();
}

ErrorResult DisplayError(const char * errorTitle, const char * errorText, const char * errorDescription, const char * fileName, int32 lineNumber)
{
	return ErrorResult();
}
