#include "Diag.h"


template<typename T>
Diag<T>::Diag():Matrix<T>()
{
}

template<typename T>
Diag<T>::Diag(T * arrptr, int n)
{
	if (n < 1)return;
	this->ReSize(n, n);
	for (int i = 0; i < n; i++)
		this->matptr[i][i] = arrptr[i];
}

template<typename T>
Diag<T>::Diag(Vector<T>& v)
{
}

template<typename T>
Diag<T>::Diag(Matrix<T>& mat)
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
