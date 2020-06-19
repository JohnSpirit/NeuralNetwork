#pragma once
//2020.6 Copyright@JSY-2020.
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
using namespace std;

template<typename T>
class MatrixView;
template<typename T>
class Vector;
template<typename T>
class Diag;

constexpr int LEFT = 1;
constexpr int RIGHT = 5;
constexpr int UP = 2;
constexpr int DOWN = 3;
constexpr int COL = 1;
constexpr int ROW = 2;
constexpr int ALL = 0;

double random_numbers[];

template<typename T>
class Matrix
{
public:
	Matrix();
	Matrix(const Matrix<T>& mat);
	Matrix(int m, int n);
	virtual ~Matrix();
	
	Matrix<T>& ReadFromFile(fstream& input,bool close=true);
	void SaveToFile(fstream& output, bool close = true);
	Matrix<T>& SetValueByArray(T* arrptr = nullptr, int RowOrCol = COL, int toset = 0);
	Matrix<T>& Randomize();

	void ReSize(int m, int n);
	Vector<int> GetSize()const;
	Matrix<T> Inv()const;
	Matrix<T> Transpose()const;
	//Matrix<T>& Transpose();
	virtual Matrix<T>& Concat(const Matrix<T> mat, int dir = RIGHT, bool inplace = true)final;//不能被重写
	Matrix<T> Concat(const Matrix<T> mat, int dir = RIGHT)const;
	Matrix<T> Slice(int m1 = ALL, int n1 = ALL, int m2 = ALL, int n2 = ALL)const;
	MatrixView<T> SliceView(int m1 = ALL, int n1 = ALL, int m2 = ALL, int n2 = ALL)const;

	Matrix<T> Sigmoid()const;
	Matrix<T> SigmoidDerive()const;
	Matrix<T> Multi(const Matrix<T>& mat)const;//元素简单相乘

	Matrix<T>& operator=(const Matrix<T>& mat);
	Vector<T> operator[](int m)const;
	Matrix<T> operator()(int m, int n)const;//get value
	T& operator()(int m, int n);//set value
	Matrix<T> operator+(const Matrix<T> mat)const;
	Matrix<T> operator-()const;
	Matrix<T> operator-(const Matrix<T> mat)const;
	Matrix<T> operator*(const Matrix<T>& mat)const;
	Matrix<T> operator*(double value)const;
	Matrix<T> operator/(const Matrix<T>& mat)const;

	template<typename ToType>
	Matrix<ToType> TypeCast()const;

	friend ostream& operator<<(ostream& o, const Matrix<T>& mat)
	{
		cout << mat._m << "*" << mat._n << endl;
		cout << "{" << endl;
		for (int i = 0; i < mat._m; i++)
		{
			cout << "\t{";
			for (int j = 0; j < mat._n; j++)cout << "  " << mat._matptr[i][j] << "\t";
			cout << "}" << endl;
		}
		cout << "}" << endl;
		return o;
	}

	inline T** _getMatptr()const;

protected:
	int _m;
	int _n;
	T** _matptr;
};


template<typename T>
Matrix<T>::Matrix() :_m(0), _n(0), _matptr(nullptr)
{

}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mat)
{
	this->_m = mat._m;
	this->_n = mat._n;
	this->_matptr = new T*[mat._m];

	for (int i = 0; i < this->_m; i++)
	{
		this->_matptr[i] = new T[this->_n];
		for (int j = 0; j < this->_n; j++)this->_matptr[i][j] = mat._matptr[i][j];
	}
}

template<typename T>
Matrix<T>::Matrix(int m, int n) :_m(m), _n(n)
{
	this->_matptr = new T*[m];
	for (int i = 0; i < m; i++)
	{
		this->_matptr[i] = new T[n];
		for (int j = 0; j < n; j++)this->_matptr[i][j] = 0;
	}
}

template<typename T>
Matrix<T>::~Matrix()
{
	for (int i = 0; i < this->_m; i++) { delete[] this->_matptr[i]; }
	delete[] this->_matptr;
}

template<typename T>
Matrix<T>& Matrix<T>::ReadFromFile(fstream & input, bool close)
{
	for (int i = 0; i < this->_m; i++)
		input.read((char*)this->_matptr[i], this->_n * sizeof(T));
	if (close)input.close();
	return *this;
}

template<typename T>
void Matrix<T>::SaveToFile(fstream & output, bool close)
{
	for (int i = 0; i < this->_m; i++)
		output.write((char*)this->_matptr[i], this->_n * sizeof(T));
	if (close)output.close();
}

template<typename T>
Matrix<T>& Matrix<T>::SetValueByArray(T * arrptr, int RowOrCol, int toset)
{
	if (arrptr == nullptr)return *this;
	if (RowOrCol == ROW)
	{
		if (toset >= this->_m)
		{
			cerr << "OutOfRangeError!" << endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < this->_m; i++)this->_matptr[toset][i] = arrptr[i];
	}
	else if (RowOrCol == COL)
	{
		if (toset >= this->_n)
		{
			cerr << "OutOfRangeError!" << endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < this->_m; i++)this->_matptr[i][toset] = arrptr[i];
	}
	else if (RowOrCol == ALL)
	{
		for (int i = 0; i < this->_m; i++)
			for (int j = 0; j < this->_n; j++)
				this->_matptr[i][j] = arrptr[this->_n*i + j];
	}
	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::Randomize()
{
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			this->_matptr[i][j] = random_numbers[rand() % 4096]/10000000;
	return *this;
}

template<typename T>
void Matrix<T>::ReSize(int new_m, int new_n)
{
	if (new_m == this->_m&&new_n == this->_n)return;
	for (int i = 0; i < this->_m; i++)delete[] this->_matptr[i];
	delete[] this->_matptr;
	this->_m = new_m;
	this->_n = new_n;
	this->_matptr = new T*[this->_m];
	for (int i = 0; i < this->_m; i++)
	{
		this->_matptr[i] = new T[this->_n];
		for (int j = 0; j < this->_n; j++)this->_matptr[i][j] = 0;
	}
}

template<typename T>
Vector<int> Matrix<T>::GetSize()const
{
	Vector<int> v(1, 2);
	v(0, 0) = this->_m;
	v(0, 1) = this->_n;
	return v;
}

template<typename T>
Matrix<T> Matrix<T>::Inv() const
{
	return Matrix<T>();
}

template<typename T>
Matrix<T> Matrix<T>::Transpose() const
{
	Matrix<T> mat(this->_n, this->_m);
	if (this->_n == 0)return mat;
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._matptr[j][i] = this->_matptr[i][j];
	return mat;
}

//template<typename T>
//Matrix<T>& Matrix<T>::Transpose(bool inplace)
//{
//	Matrix<T> mat(*this);
//	if (_m == _n)
//		for (int i = 0; i < _m; i++)
//			for (int j = 0; j < _n; j++)
//				this->_matptr[i][j] = mat._matptr[j][i];
//	else
//	{
//		//重新分配内存
//		this->ReSize(this->_n, this->_m);
//		for (int i = 0; i < _n; i++)
//			for (int j = 0; j < _m; j++)
//				this->_matptr[i][j] = mat._matptr[j][i];
//	}
//	return *this;
//}

template<typename T>
Matrix<T>& Matrix<T>::Concat(const Matrix<T> mat, int dir, bool inplace)
{
	Matrix<T> oldmat(*this);
	switch (dir)
	{
	case LEFT:

		break;
	case RIGHT:
		if (mat._n == this->_n)
		{
			this->ReSize(this->_m + mat._m, this->_n);
			for (int i = 0; i < oldmat._m; i++)
				for (int j = 0; j < oldmat._n; j++)
					this->_matptr[i][j] = oldmat._matptr[i][j];
			for (int i = oldmat._m; i < mat._m; i++)
				for (int j = 0; j < oldmat._n; j++)
					this->_matptr[i][j] = mat._matptr[i - oldmat._m][j];
		}
		break;
	case UP:

		break;
	case DOWN:

		break;
	default:
		break;
	}
	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::Concat(const Matrix<T> mat, int dir) const
{
	Matrix<T> new_mat;
	switch (dir)
	{
	case LEFT:

		break;
	case RIGHT:
		if (mat._n == this->_n)
		{
			new_mat.ReSize(this->_m + mat._m, this->_n);
			for (int i = 0; i < this->_m; i++)
				for (int j = 0; j < this->_n; j++)
					new_mat._matptr[i][j] = this->_matptr[i][j];
			for (int i = this->_m; i < this->_m + mat._m; i++)
				for (int j = 0; j < mat._n; j++)
					new_mat._matptr[i][j] = mat._matptr[i - this->_m][j];
		}
		break;
	case UP:

		break;
	case DOWN:

		break;
	default:
		break;
	}
	return new_mat;
}

template<typename T>
Matrix<T> Matrix<T>::Slice(int m1, int n1, int m2, int n2)const
{
	if (m1 >= 0 && n1 >= 0 && m2 < this->_m&&n2 < this->_n&&m1 <= m2 && n1 <= n2)
	{
		Matrix<T> mat(m2 - m1 + 1, n2 - n1 + 1);
		for (int i = m1; i <= m2; i++)
			for (int j = n1; j <= n2; j++)
				mat._matptr[i - m1][j - n1] = this->_matptr[i][j];
		return mat;
	}
	else
	{
		cerr << "OutOfRangeError!" << endl;
		exit(EXIT_FAILURE);
	}
}

template<typename T>
MatrixView<T> Matrix<T>::SliceView(int m1, int n1, int m2, int n2) const
{
	return MatrixView<T>();
}

template<typename T>
Matrix<T> Matrix<T>::Sigmoid()const
{
	Matrix<T> mat(this->_m, this->_n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._matptr[i][j] = 1 / (1 + exp(-this->_matptr[i][j]));
	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::SigmoidDerive()const
{
	Matrix<T> mat(this->_m, this->_n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._matptr[i][j] = exp(-this->_matptr[i][j]) / pow(1 + exp(-this->_matptr[i][j]), 2);
	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::Multi(const Matrix<T>& mat)const
{
	Matrix<T> m(*this);
	if (mat._m == this->_m&&mat._n == this->_n)
	{
		for (int i = 0; i < this->_m; i++)
			for (int j = 0; j < this->_n; j++)
				m._matptr[i][j] *= mat._matptr[i][j];
		return m;
	}
	else
	{
		cerr << "DifferentSizeError!" << endl;
		exit(EXIT_FAILURE);
	}
}


template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& mat)
{
	if (this->_m != mat._m || this->_n != mat._n)
	{
		for (int i = 0; i < this->_m; i++)delete[] this->_matptr[i];
		delete[] this->_matptr;
		this->_m = mat._m;
		this->_n = mat._n;
		this->_matptr = new T*[this->_m];
		for (int i = 0; i < this->_m; i++)
		{
			this->_matptr[i] = new T[this->_n];
			for (int j = 0; j < this->_n; j++)this->_matptr[i][j] = mat._matptr[i][j];
		}
	}
	else
	{
		for (int i = 0; i < this->_m; i++)
			for (int j = 0; j < this->_n; j++)this->_matptr[i][j] = mat._matptr[i][j];
	}

	return *this;
}

template<typename T>
Vector<T> Matrix<T>::operator[](int m)const
{
	if (m < 0 || m >= this->_m)
	{
		cerr << "OutOfRangeError!" << endl;
		exit(EXIT_FAILURE);
	}
	Vector<T> mat(1, this->_n);
	for (int i = 0; i < this->_n; i++)mat._matptr[0][i] = this->_matptr[m][i];
	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator()(int m, int n)const
{
	if (m < 0 || n < 0 || m >= this->_m || n >= this->_n)
	{
		cerr << "OutOfRangeError!" << endl;
		exit(EXIT_FAILURE);
	}
	Matrix<T> mat(1, 1);
	mat._matptr[0][0] = this->_matptr[m][n];
	return mat;
}

template<typename T>
T& Matrix<T>::operator()(int m, int n)
{
	if (m < 0 || n < 0 || m >= this->_m || n >= this->_n)
	{
		cerr << "OutOfRangeError!" << endl;
		exit(EXIT_FAILURE);
	}
	else return this->_matptr[m][n];
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> mat) const
{
	if (this->_m != mat._m || this->_n != mat._n)
	{
		cerr << "DimError!" << endl;
		exit(EXIT_FAILURE);
	}

	Matrix<T> ret(_m, _n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			ret._matptr[i][j] = this->_matptr[i][j] + mat._matptr[i][j];

	return ret;
}

template<typename T>
Matrix<T> Matrix<T>::operator-() const
{
	Matrix<T> mat(_m, _n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._matptr[i][j] = -this->_matptr[i][j];
	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> mat) const
{
	return mat.operator-() + *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mat) const
{
	if (this->_n == mat._m)
	{
		Matrix<T> m(this->_m, mat._n);

		for (int i = 0; i < this->_m; i++)
		{
			for (int j = 0; j < mat._n; j++)
			{
				T count = 0;
				for (int k = 0; k < this->_n; k++)
				{
					count += this->_matptr[i][k] * mat._matptr[k][j];
				}
				m._matptr[i][j] = count;
			}
		}
		return m;
	}
	else {
		cerr << "An!=Bm,multiply function forbidden!" << endl;
		exit(EXIT_FAILURE);
	}
}

template<typename T>
Matrix<T> Matrix<T>::operator*(double value) const
{
	Matrix<T> mat(*this);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._matptr[i][j] *= value;
	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const Matrix<T>& mat) const
{
	return Matrix<T>();
}

template<typename T>
inline T ** Matrix<T>::_getMatptr()const
{
	return this->_matptr;
}

template<typename T>
template<typename ToType>
Matrix<ToType> Matrix<T>::TypeCast()const
{
	Matrix<ToType> mat(this->_m, this->_n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._getMatptr()[i][j] = static_cast<ToType>(this->_matptr[i][j]);
	return mat;
}