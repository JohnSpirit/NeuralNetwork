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
	Vector<T> Sigmoid(bool ignore_end = true);
	Vector<T> SigmoidDerive();
	Vector<T> Transpose(bool append = true, T value = 1);
	//Vector<T> Transpose()const;
	T Sum();

	Diag<T> ToDiag();
	T operator[](int m)const;
	T& operator[](int m);//set value
	Vector<T>& operator=(const Vector<T> v);
};

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
			this->_matptr[i][0] = arrptr[i];
		if (append)this->_matptr[n][0] = append_value;
	}
	else
	{
		this->ReSize(1, append ? n + 1 : n);
		for (int i = 0; i < n; i++)
			this->_matptr[0][i] = arrptr[i];
		if (append)this->_matptr[0][n] = append_value;
	}
}

template<typename T>
Vector<T>::Vector(const Diag<T>& v)
{
}

template<typename T>
Vector<T>::Vector(const Matrix<T>& v) : Matrix<T>(v)
{
}

template<typename T>
Vector<T>::~Vector()
{
}


template<typename T>
Diag<T> Vector<T>::ToDiag()
{
	Diag<T> d(*this);
	return d;
}

template<typename T>
T Vector<T>::operator[](int m)const
{
	if (this->_m > 1 && m < this->_m)return this->_matptr[m][0];
	else if (this->_n > 1 && m < this->_n)return this->_matptr[0][m];
	else
	{
		cerr << "OutOfRangeError!!" << endl;
		exit(EXIT_FAILURE);
	}
}

template<typename T>
T & Vector<T>::operator[](int m)
{
	if (this->_m > 1 && m < this->_m)return this->_matptr[m][0];
	else if (this->_n > 1 && m < this->_n)return this->_matptr[0][m];
	else
	{
		cerr << "OutOfRangeError!!" << endl;
		exit(EXIT_FAILURE);
	}
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> v)
{
	if (this->_m != v._m || this->_n != v._n)
	{
		for (int i = 0; i < this->_m; i++)delete[] this->_matptr[i];
		delete[] this->_matptr;
		this->_m = v._m;
		this->_n = v._n;
		this->_matptr = new T*[v._m];
		for (int i = 0; i < v._m; i++)
		{
			this->_matptr[i] = new T[this->_n];
			for (int j = 0; j < v._n; j++)this->_matptr[i][j] = v._matptr[i][j];
		}
	}
	else
	{
		for (int i = 0; i < v._m; i++)
			for (int j = 0; j < v._n; j++)this->_matptr[i][j] = v._matptr[i][j];
	}

	return *this;

}

template<typename T>
inline int Vector<T>::GetLength()
{
	return this->_m >= this->_n ? this->_m : this->_n;
}

template<typename T>
Vector<T> Vector<T>::Sigmoid(bool ignore_end)
{
	Vector<T> v(this->_m, this->_n);
	if (this->_m > 1)
		for (int i = 0; i < (ignore_end ? this->_m - 1 : this->_m); i++)
			v._matptr[i][0] = 1 / (1 - exp(-this->_matptr[i][0]));
	else if (this->_n > 1)
		for (int i = 0; i < (ignore_end ? this->_n - 1 : this->_n); i++)
			v._matptr[0][i] = 1 / (1 - exp(-this->_matptr[0][i]));
	return v;
}

template<typename T>
Vector<T> Vector<T>::SigmoidDerive()
{
	Vector<T> mat(this->_m, this->_n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._matptr[i][j] = -exp(-this->_matptr[i][j]) / pow(1 - exp(-this->_matptr[i][j]), 2);
	return mat;
}

template<typename T>
Vector<T> Vector<T>::Transpose(bool append, T value)
{
	Vector<T> v(*this);
	if (append)
	{
		this->ReSize(this->_n+1, this->_m);
		for (int i = 0; i < v._m; i++)
			for (int j = 0; j < v._n; j++)
				this->_matptr[j][i] = v._matptr[i][j];
		v._matptr[this->_n - 1][0] = value;
		return v;
	}
	else
	{
		this->ReSize(this->_n, this->_m);
		for (int i = 0; i < v._m; i++)
			for (int j = 0; j < v._n; j++)
				this->_matptr[j][i] = v._matptr[i][j];
		return v;
	}
}

//template<typename T>
//Vector<T> Vector<T>::Transpose()const
//{
//	Vector<T> v(Matrix<T>::Transpose());
//	return v;
//}

template<typename T>
T Vector<T>::Sum()
{
	T sum = 0;
	for (int i = 0; i < this->GetLength(); i++)
		sum += this->operator[](i);
	return sum;
}
