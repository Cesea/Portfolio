#ifndef MATRIX_CACHE_H
#define MATRIX_CACHE_H

constexpr uint32 MAX_MATRIX_CACHE = 256;

class MatrixCache
{
public :
	MatrixCache();
	uint32 AddMatrix(const Matrix &matrix);

	const Matrix &GetAt(uint32 i) const;
	void Clear();

private :
	uint32 _currentCount;
	Matrix *_matrices;

};


#endif