#include "Matrix.h"

template<typename T>
Matrix<T>::Matrix() :m(0), n(0), matptr(nullptr)
{

}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mat)
{
	this->m = mat.m;
	this->n = mat.n;
	this->matptr = new T*[mat.m];

	for (int i = 0; i < mat.m; i++)
	{
		this->matptr[i] = new T[n];
		for (int j = 0; j < mat.n; j++)this->matptr[i][j] = mat.matptr[i][j];
	}
}

template<typename T>
Matrix<T>::Matrix(int m, int n) :m(m), n(n)
{
	this->matptr = new T*[m];
	for (int i = 0; i < m; i++)
	{
		this->matptr[i] = new T[n];
		for (int j = 0; j < n; j++)this->matptr[i][j] = 0;
	}
}

template<typename T>
Matrix<T>::~Matrix()
{
	for (int i = 0; i < m; i++) { delete[] this->matptr[i]; }
	delete[] this->matptr;
}

template<typename T>
void Matrix<T>::SetValueByArray(T * arrptr, int RowOrCol, int toset)
{
	if (arrptr == nullptr)return;
	if (RowOrCol == ROW)
	{
		if (toset >= m)
		{
			cerr << "OutOfRangeError!" << endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < m; i++)this->matptr[toset][i] = arrptr[i];
	}
	else if (RowOrCol == COL)
	{
		if (toset >= n)
		{
			cerr << "OutOfRangeError!" << endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < m; i++)this->matptr[i][toset] = arrptr[i];
	}
	else if (RowOrCol == ALL)
	{
		for (int i = 0; i < m; i++)
			for (int j = 0; j < m; j++)
				this->matptr[i][toset] = arrptr[m*i + j];
	}
}

template<typename T>
void Matrix<T>::ReSize(int new_m, int new_n)
{
	if (m == this->m&&n == this->n)return;
	for (int i = 0; i < this->m; i++)delete[] this->matptr[i];
	delete[] this->matptr;
	this->matptr = new T*[new_m];
	for (int i = 0; i < new_m; i++)
	{
		this->matptr[i] = new T[new_n];
		for (int j = 0; j < new_n; j++)this->matptr[i][j] = 0;
	}
}

template<typename T>
Vector<int> Matrix<T>::GetSize()const
{
	Vector<int> v(1,2);
	v(0, 0) = this->m;
	v(0, 1) = this->n;
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
	Matrix<T> mat(n, m);
	if (n == 0)return mat;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
			mat.matptr[j][i] = this->matptr[i][j];
	return mat;
}

template<typename T>
Matrix<T>& Matrix<T>::Transpose()
{
	Matrix<T> mat(*this);
	if (m == n)
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				mat.matptr[j][i] = this->matptr[i][j];
	else
	{
		//重新分配内存
		this->ReSize(this->n,this->m);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				this->matptr[i][j] = mat.matptr[j][i];
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
		if (mat.n == this->n)
		{	
			this->ReSize(this->m + mat.m, this->n);
			for (int i = 0; i < oldmat.m; i++)
				for (int j = 0; j < oldmat.n; j++)
					this->matptr[i][j] = oldmat.matptr[i][j];
			for (int i = oldmat.m; i < mat.m; i++)
				for (int j = 0; j < oldmat.n; j++)
					this->matptr[i][j] = mat.matptr[i - oldmat.m][j];
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
		if (mat.n == this->n)
		{
			new_mat.ReSize(this->m + mat.m, this->n);
			for (int i = 0; i < this->m; i++)
				for (int j = 0; j < this->n; j++)
					new_mat.matptr[i][j] = this->matptr[i][j];
			for (int i = this->m; i < this->m+mat.m; i++)
				for (int j = 0; j < mat.n; j++)
					new_mat.matptr[i][j] = mat.matptr[i - this->m][j];
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
	if (m1 >= 0 && n1 >= 0 && m2 < this->m&&n2 < this->n&&m1 <= m2 && n1 <= n2)
	{
		Matrix<T> mat(m2 - m1 + 1, n2 - n1 + 1);
		for (int i = m1; i <= m2; i++)
			for (int j = n1; j <= n2; j++)
				mat.matptr[i - m1][j - n1] = this->matptr[i][j];
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
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			this->matptr[i][j] = 1 / (1 - exp(-this->matptr[i][j]));
	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::SigmoidDerive()
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			this->matptr[i][j] = -exp(-this->matptr[i][j]) / pow(1 - exp(-this->matptr[i][j]), 2);
	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& mat)
{
	if (this->m != mat.m || this->n != mat.n)
	{
		for (int i = 0; i < this->m; i++)delete[] this->matptr[i];
		delete[] this->matptr;
		this->m = mat.m;
		this->n = mat.n;
		this->matptr = new T*[mat.m];
		for (int i = 0; i < mat.m; i++)
		{
			this->matptr[i] = new T[n];
			for (int j = 0; j < mat.n; j++)this->matptr[i][j] = mat.matptr[i][j];
		}
	}
	else
	{
		for (int i = 0; i < mat.m; i++)
			for (int j = 0; j < mat.n; j++)this->matptr[i][j] = mat.matptr[i][j];
	}

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator[](int m)const
{
	if (m < 0 || m >= this->m)
	{
		cerr << "OutOfRangeError!" << endl;
		exit(EXIT_FAILURE);
	}
	Matrix<T> mat(1, this->n);
	for (int i = 0; i < this->n; i++)mat.matptr[0][i] = this->matptr[m][i];
	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator()(int m, int n)const
{
	if (m < 0 || n < 0 || m >= this->m || n >= this->n)
	{
		cerr << "OutOfRangeError!" << endl;
		exit(EXIT_FAILURE);
	}
	Matrix<T> mat(1, 1);
	mat.matptr[0][0] = this->matptr[m][n];
	return mat;
}

template<typename T>
T& Matrix<T>::operator()(int m, int n)
{
	if (m < 0 || n < 0 || m >= this->m || n >= this->n)
	{
		cerr << "OutOfRangeError!" << endl;
		exit(EXIT_FAILURE);
	}
	else return this->matptr[m][n];
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> mat) const
{
	if (this->m != mat.m || this->n != mat.n)
	{
		cerr << "DimError!" << endl;
		exit(EXIT_FAILURE);
	}

	Matrix<T> mat(m, n);
	for (int i = 0; i < this->m; i++)
		for (int j = 0; j < mat.n; j++)
			mat.matptr[i][j] = this->matptr[i][j] + mat.matptr[i][j];

	return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator-() const
{
	Matrix<T> mat(m, n);
	for (int i = 0; i < this->m; i++)
		for (int j = 0; j < mat.n; j++)
			mat.matptr[i][j] = -this->matptr[i][j];
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
	if (this->n == mat.m)
	{
		Matrix<T> m(this->m, mat.n);

		for (int i = 0; i < this->m; i++)
		{
			for (int j = 0; j < mat.n; j++)
			{
				count = 0;
				for (int k = 0; k < this->n; k++)
				{
					count += this->matptr[i][k] * mat.matptr[k][j];
				}
				m.matptr[i][j] = count;
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
Matrix<T> Matrix<T>::operator/(const Matrix<T>& mat) const
{
	return Matrix<T>();
}

template<typename T>
template<typename ToType>
Matrix<ToType> Matrix<T>::TypeCast()
{
	Matrix<ToType> mat;
	for (int i = 0; i < this->m; i++)
		for (int j = 0; j < this->n; j++)
			mat.matptr[i][j] = static_cast<ToType>(this->matptr[i][j]);
	return mat;
}