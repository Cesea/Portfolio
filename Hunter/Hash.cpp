#include "stdafx.h"
#include "Hash.h"

#define MURMUR_M (0x5bd1e995)
#define MURMUR_R (24)
#define mmix(_h, _k) { _k *= MURMUR_M; _k ^= _k >> MURMUR_R; _k *= MURMUR_M; _h *= MURMUR_M; _h ^= _k; }

void HashMurmur2A::Begin(uint32 seed)
{
	_hash = seed;
	_tail = 0;
	_count = 0;
	_size = 0;
}
 
void HashMurmur2A::Add(const void * data, int len)
{
	if (!IsAligned(data, len))
	{
		AddAligned(data, len);
	}
}

uint32 HashMurmur2A::End()
{
	mmix(_hash, _tail);
	mmix(_hash, _size);

	_hash ^= _hash >> 13;
	_hash *= MURMUR_M;
	_hash ^= _hash >> 15;

	return _hash;
}

void HashMurmur2A::AddAligned(const void * data, int len)
{
	const uint8* idata = (const uint8_t*)data;
	_size += len;

	MixTail(idata, len);

	while (len >= 4)
	{
		uint32 kk = *(uint32 *)idata;

		mmix(_hash, kk);

		idata += 4;
		len -= 4;
	}

	MixTail(idata, len);
}

void HashMurmur2A::AddUnaligned(const void * data, int len)
{
	const uint8_t* idata = (const uint8 *)data;
	_size += len;

	MixTail(idata, len);

	while (len >= 4)
	{
		uint32_t kk;
		ReadUnaligned(idata, kk);

		mmix(_hash, kk);

		idata += 4;
		len -= 4;
	}

	MixTail(idata, len);
}

void HashMurmur2A::ReadUnaligned(const void * data, uint32 & out)
{
	const uint8 *idata = (const uint8*)data;
		out = 0 | idata[0] | idata[1] << 8 | idata[2] << 16 | idata[3] << 24 ;
}

void HashMurmur2A::MixTail(const uint8 *& data, int & len)
{
	while (len && ((len < 4) || _count))
	{
		_tail |= (*data++) << (_count * 8);

		_count++;
		len--;

		if (_count == 4)
		{
			mmix(_hash, _tail);
			_tail = 0;
			_count = 0;
		}
	}
}
