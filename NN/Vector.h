#pragma once
#include "Matrix.h"
template<typename T>
class Vector :
	public Matrix<T>
{
public:
	Vector();
	Vector(int m ,int n);
	Vector(const Vector<T>& v);
	Vector(T* arrptr,int n,bool transpose=false,bool append=true,T append_value=1);
	Vector(Diag<T>& v);
	Vector(Matrix<T>& v);
	~Vector();

	int GetLength();

	Diag<T> ToDiag();
	T operator[](int m);
};

template<typename T>
int Vector<T>::GetLength()
{
	return this->m >= this->n ? this->m : this->n;
}