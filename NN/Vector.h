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
	Vector<T> Sigmoid();
	Vector<T> SigmoidDerive();
	Vector<T> Transpose(bool append = false, T value = 1);
	Vector<T> Append(T value = 1);
	T Sum();
	T Max();
	int Argmax();

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
	return Diag<T>(*this);
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
inline T & Vector<T>::operator[](int m)
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
Vector<T> Vector<T>::Sigmoid()
{
	Vector<T> v(this->_m, this->_n);
	if (this->_m > 1)
		for (int i = 0; i < this->_m; i++)
			v._matptr[i][0] = 1 / (1 + exp(-this->_matptr[i][0]));
	else if (this->_n > 1)
		for (int i = 0; i < this->_n; i++)
			v._matptr[0][i] = 1 / (1 + exp(-this->_matptr[0][i]));
	return v;
}

template<typename T>
Vector<T> Vector<T>::SigmoidDerive()
{
	Vector<T> mat(this->_m, this->_n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._matptr[i][j] = exp(-this->_matptr[i][j]) / pow(1 + exp(-this->_matptr[i][j]), 2);
	return mat;
}

template<typename T>
Vector<T> Vector<T>::Transpose(bool append, T value)
{
	Vector<T> v(append ? this->_n + 1 : this->_n, this->_m);
	if (append)
	{
		for (int i = 0; i < this->_m; i++)
			for (int j = 0; j < this->_n; j++)
				v._matptr[j][i] = this->_matptr[i][j];
		v._matptr[this->_n][0] = value;
		return v;
	}
	else
	{
		for (int i = 0; i < this->_m; i++)
			for (int j = 0; j < this->_n; j++)
				v._matptr[j][i] = this->_matptr[i][j];
		return v;
	}
}

template<typename T>
Vector<T> Vector<T>::Append(T value)
{
	if (this->_m > 1)
	{
		Vector<T> v(this->_m + 1, 1);
		for (int i = 0; i < this->_m; i++)v._matptr[i][0] = this->_matptr[i][0];
		v._matptr[this->_m][0] = value;
		return v;
	}
	else
	{
		Vector<T> v(1, this->_n + 1);
		for (int i = 0; i < this->_n; i++)v._matptr[0][i] = this->_matptr[0][i];
		v._matptr[0][this->_n] = value;
		return v;
	}
}

template<typename T>
T Vector<T>::Sum()
{
	T sum = 0;
	for (int i = 0; i < this->GetLength(); i++)
		sum += this->operator[](i);
	return sum;
}

template<typename T>
T Vector<T>::Max()
{
	T max = this->operator[](0);
	for (int i = 1; i < this->GetLength(); i++)
		if (this->operator[](i) > max)max = this->operator[](i);
	return max;
}

template<typename T>
int Vector<T>::Argmax()
{
	T max = this->operator[](0);
	int index = 0;
	for (int i = 1; i < this->GetLength(); i++)
		if (this->operator[](i) > max)
		{
			max = this->operator[](i);
			index = i;
		}
	return index;
}
