#pragma once
//2020.6 Copyright@JSY-2020.
#include <iostream>
#include <cmath>
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

template<typename T>
class Matrix
{
public:
	Matrix();
	Matrix(const Matrix<T>& mat);
	Matrix(int m, int n);
	virtual ~Matrix();

	void SetValueByArray(T* arrptr=nullptr, int RowOrCol=COL,int toset=0);

	void ReSize(int m, int n);
	Vector<int> GetSize()const;
	Matrix<T> Inv()const;
	Matrix<T> Transpose()const;
	Matrix<T>& Transpose();
	virtual Matrix<T>& Concat(const Matrix<T> mat, int dir = RIGHT,bool inplace=true)final;//不能被重写
	Matrix<T> Concat(const Matrix<T> mat, int dir = RIGHT)const;
	Matrix<T> Slice(int m1=ALL, int n1=ALL, int m2=ALL, int n2=ALL)const;
	MatrixView<T> SliceView(int m1 = ALL, int n1 = ALL, int m2 = ALL, int n2 = ALL)const;

	Matrix<T>& Sigmoid();
	Matrix<T>& SigmoidDerive();
	Matrix<T>& Multi(double value);

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
	Matrix<ToType> TypeCast();

	friend ostream& operator<<(ostream& o, const Matrix<T>& mat)
	{
		cout << mat._m << "*" << mat._n << endl;
		cout << "{" << endl;
		for (int i = 0; i < mat._m; i++)
		{
			cout << "\t{";
			for (int j = 0; j < mat._n; j++)cout << "\t" << mat._matptr[i][j] << "\t";
			cout << "}" << endl;
		}
		cout << "}" << endl;
		return o;
	}

	inline T** _getMatptr();

protected:
	int _m;
	int _n;
	T** _matptr;
};
