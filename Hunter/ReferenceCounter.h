#ifndef REFERENCE_COUNTER_H
#define REFERENCE_COUNTER_H


class ReferenceCounter
{
public :
	ReferenceCounter();
	ReferenceCounter(const ReferenceCounter &other);

	virtual ~ReferenceCounter();

	ReferenceCounter &operator= (const ReferenceCounter &other);

	virtual int32 AddRef();
	virtual int32 Release();

	int32 GetCount();

private :

	int32 _referenceCount;
};


#endif