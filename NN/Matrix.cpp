#include "Matrix.h"

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

	for (int i = 0; i < mat._m; i++)
	{
		this->_matptr[i] = new T[_n];
		for (int j = 0; j < mat._n; j++)this->_matptr[i][j] = mat._matptr[i][j];
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
	for (int i = 0; i < _m; i++) { delete[] this->_matptr[i]; }
	delete[] this->_matptr;
}

template<typename T>
void Matrix<T>::SetValueByArray(T * arrptr, int RowOrCol, int toset)
{
	if (arrptr == nullptr)return;
	if (RowOrCol == ROW)
	{
		if (toset >= _m)
		{
			cerr << "OutOfRangeError!" << endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < _m; i++)this->_matptr[toset][i] = arrptr[i];
	}
	else if (RowOrCol == COL)
	{
		if (toset >= _n)
		{
			cerr << "OutOfRangeError!" << endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < _m; i++)this->_matptr[i][toset] = arrptr[i];
	}
	else if (RowOrCol == ALL)
	{
		for (int i = 0; i < _m; i++)
			for (int j = 0; j < _n; j++)
				this->_matptr[i][j] = arrptr[_m*i + j];
	}
}

template<typename T>
void Matrix<T>::ReSize(int new_m, int new_n)
{
	if (_m == this->_m&&_n == this->_n)return;
	for (int i = 0; i < this->_m; i++)delete[] this->_matptr[i];
	delete[] this->_matptr;
	this->_matptr = new T*[new_m];
	for (int i = 0; i < new_m; i++)
	{
		this->_matptr[i] = new T[new_n];
		for (int j = 0; j < new_n; j++)this->_matptr[i][j] = 0;
	}
}

template<typename T>
Vector<int> Matrix<T>::GetSize()const
{
	Vector<int> v(1,2);
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
	Matrix<T> mat(_n, _m);
	if (_n == 0)return mat;
	for (int i = 0; i < _m; i++)
		for (int j = 0; j < _m; j++)
			mat._matptr[j][i] = this->_matptr[i][j];
	return mat;
}

template<typename T>
Matrix<T>& Matrix<T>::Transpose()
{
	Matrix<T> mat(*this);
	if (_m == _n)
		for (int i = 0; i < _m; i++)
			for (int j = 0; j < _n; j++)
				mat._matptr[j][i] = this->_matptr[i][j];
	else
	{
		//重新分配内存
		this->ReSize(this->_n,this->_m);
		for (int i = 0; i < _n; i++)
			for (int j = 0; j < _m; j++)
				this->_matptr[i][j] = mat._matptr[j][i];
	}
	return *this;
}

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
			for (int i = this->_m; i < this->_m+mat._m; i++)
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
Matrix<T>& Matrix<T>::Sigmoid()
{
	for (int i = 0; i < _m; i++)
		for (int j = 0; j < _n; j++)
			this->_matptr[i][j] = 1 / (1 - exp(-this->_matptr[i][j]));
	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::SigmoidDerive()
{
	for (int i = 0; i < _m; i++)
		for (int j = 0; j < _n; j++)
			this->_matptr[i][j] = -exp(-this->_matptr[i][j]) / pow(1 - exp(-this->_matptr[i][j]), 2);
	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::Multi(double value)
{
	for (int i = 0; i < _m; i++)
		for (int j = 0; j < _n; j++)
			this->_matptr[i][j] *= value;
	return *this;
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
		this->_matptr = new T*[mat._m];
		for (int i = 0; i < mat._m; i++)
		{
			this->_matptr[i] = new T[_n];
			for (int j = 0; j < mat._n; j++)this->_matptr[i][j] = mat._matptr[i][j];
		}
	}
	else
	{
		for (int i = 0; i < mat._m; i++)
			for (int j = 0; j < mat._n; j++)this->_matptr[i][j] = mat._matptr[i][j];
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

	Matrix<T> mat(_m, _n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < mat._n; j++)
			mat._matptr[i][j] = this->_matptr[i][j] + mat._matptr[i][j];

	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator-() const
{
	Matrix<T> mat(_m, _n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < mat._n; j++)
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
				count = 0;
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
	Matrix<T> mat(this->_m,this->_n);
	for (int i = 0; i < _m; i++)
		for (int j = 0; j < _n; j++)
			mat._matptr[i][j] *= value;
	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const Matrix<T>& mat) const
{
	return Matrix<T>();
}

template<typename T>
inline T ** Matrix<T>::_getMatptr()
{
	return this->_matptr;
}

template<typename T>
template<typename ToType>
Matrix<ToType> Matrix<T>::TypeCast()
{
	Matrix<ToType> mat(this->_m,this->_n);
	for (int i = 0; i < this->_m; i++)
		for (int j = 0; j < this->_n; j++)
			mat._getMatptr()[i][j] = static_cast<ToType>(this->_matptr[i][j]);
	return mat;
}