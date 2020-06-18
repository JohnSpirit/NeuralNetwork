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

template<typename T>
Diag<T>::Diag() :Matrix<T>()
{
}

template<typename T>
Diag<T>::Diag(T * arrptr, int n)
{
	if (n < 1)return;
	this->ReSize(n, n);
	for (int i = 0; i < n; i++)
		this->_matptr[i][i] = arrptr[i];
}

template<typename T>
Diag<T>::Diag(Vector<T>& v)
{
	this->ReSize(v.GetLength(), v.GetLength());
	for (int i = 0; i < this->_m; i++)
	{
		this->_matptr[i][i] = v[i];
	}
}

template<typename T>
Diag<T>::Diag(Matrix<T>& mat):Matrix<T>(mat)
{

}

template<typename T>
Diag<T>::~Diag()
{
}

template<typename T>
Vector<T> Diag<T>::ToVector()
{
	return Vector<T>();
}