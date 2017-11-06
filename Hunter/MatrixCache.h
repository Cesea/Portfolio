#ifndef MATRIX_CACHE_H
#define MATRIX_CACHE_H

constexpr uint32 MAX_MATRIX_CACHE = 1000;

class MatrixCache
{
public :
	MatrixCache();
	void AddMatrix(const Matrix &matrix);

	const Matrix &GetAt(uint32 i);
	void Clear();

private :
	uint32 _currentCount;
	Matrix _matrices[MAX_MATRIX_CACHE];

};


#endif