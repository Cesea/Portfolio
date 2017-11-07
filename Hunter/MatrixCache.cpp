#include "stdafx.h"
#include "MatrixCache.h"

MatrixCache::MatrixCache()
{
}

void MatrixCache::AddMatrix(const Matrix & matrix)
{
	_matrices[_currentCount++] = matrix;
}

const Matrix & MatrixCache::GetAt(uint32 i)
{
	Assert(i < _currentCount);
	return _matrices[i];
}

void MatrixCache::Clear()
{
	_currentCount = 0;
}
