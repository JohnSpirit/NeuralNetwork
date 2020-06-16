#pragma once
#include "Matrix.h"
template<typename T>
class Diag :
	public Matrix<T>
{
public:
	Diag();
	Diag(T* arrptr, int n);
	Diag(Vector<T>& v);
	Diag(Matrix<T>& mat);

	~Diag();

	Vector<T> ToVector();
};

