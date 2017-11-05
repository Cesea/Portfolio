#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

template <typename T>
class CommandBucket
{
	typedef T Key;
public :
	CommandBucket(uint32 numCommand, const Matrix &viewMatrix, const Matrix &projectionMatrix);

	template <typename C>
	C *AddCommand(Key key);

	template <typename C>
	C *AllocateCommand();

	void Sort();

	void Submit();

private :
	Key *_keys;
	void **_ppDatas;

	Matrix _viewMatrix;
	Matrix _projectionMatrix;
};


template<typename T>
inline CommandBucket<T>::CommandBucket(uint32 numCommand, const Matrix & viewMatrix, const Matrix & projectionMatrix)
{
}

#endif
