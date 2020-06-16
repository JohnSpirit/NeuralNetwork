#include "Vector.h"


template<typename T>
Vector<T>::Vector() :Matrix<T>()
{

}

template<typename T>
Vector<T>::Vector(int m, int n) :Matrix<T>(m, n)
{
}

template<typename T>
Vector<T>::Vector(const Vector<T>& v) : Matrix<T>(v)
{

}

template<typename T>
Vector<T>::Vector(T * arrptr, int n, bool transpose, bool append, T append_value) :Matrix<T>()
{
	if (transpose)
	{
		this->ReSize(append ? n + 1 : n, 1);
		for (int i = 0; i < n; i++)
			this->arrptr[i][0] = arrptr[i];
		if (append)this->arrptr[n][0] = append_value;
	}
	else
	{
		this->ReSize(1, append ? n + 1 : n);
		for (int i = 0; i < n; i++)
			this->arrptr[0][i] = arrptr[i];
		if (append)this->arrptr[0][n] = append_value;
	}
}

template<typename T>
Vector<T>::Vector(Diag<T>& v)
{
}

template<typename T>
Vector<T>::Vector(Matrix<T>& v)
{
}

template<typename T>
Vector<T>::~Vector()
{
}



template<typename T>
Diag<T> Vector<T>::ToDiag()
{
	return Diag<T>();
}

template<typename T>
T Vector<T>::operator[](int m)
{
	if (this->m > 1 && m < this->m)return this->matptr[m][0];
	else if (this->n > 1 && m < this->n)return this->matptr[0][m];
	else
	{
		cerr << "OutOfRangeError!!" << endl;
		exit(EXIT_FAILURE);
	}
}
