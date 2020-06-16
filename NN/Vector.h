#pragma once
#include "Matrix.h"
template<typename T>
class Vector :
	public Matrix<T>
{
public:
	Vector();
	Vector(int m, int n);
	Vector(const Vector<T>& v);
	Vector(T* arrptr, int n, bool transpose = false, bool append = true, T append_value = 1);
	Vector(const Diag<T>& v);
	Vector(const Matrix<T>& v);
	~Vector();

	inline int GetLength();
	Vector<T>& Sigmoid(bool ignore_end=true);
	Vector<T> Transpose(bool append = true, T value = 1);

	T Sum();

	Diag<T> ToDiag();
	T operator[](int m);
	Vector<T>& operator=(const Vector<T> v);
};