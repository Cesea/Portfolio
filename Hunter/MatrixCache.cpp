#include "stdafx.h"
#include "MatrixCache.h"

MatrixCache::MatrixCache()
{
	_matrices = new Matrix[MAX_MATRIX_CACHE];
	_currentCount = 0;
}

uint32 MatrixCache::AddMatrix(const Matrix & matrix)
{
	uint32 result = _currentCount;
	_matrices[_currentCount++] = matrix;
	return result;
}

const Matrix & MatrixCache::GetAt(uint32 i) const
{
	Assert(i < _currentCount);
	return _matrices[i];
}

void MatrixCache::Clear()
{
	_currentCount = 0;
}
