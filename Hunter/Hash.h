#ifndef HASH_H
#define HASH_H

class HashMurmur2A
{
public:
	void Begin(uint32 seed = 0);

	void Add(const void* data, int len);

	template<typename T>
	void Add(T value);

	uint32 End();

private:
	void AddAligned(const void* data, int len);

	///
	void AddUnaligned(const void* data, int len);

	///
	static void ReadUnaligned(const void* data, uint32 &out);

	///
	void MixTail(const uint8 *&data, int &len);

	uint32 _hash;
	uint32 _tail;
	uint32 _count;
	uint32 _size;
};


template<typename T>
inline void HashMurmur2A::Add(T value)
{
	Add(&_value, sizeof(T));
}

#endif
