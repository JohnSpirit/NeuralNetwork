#pragma once
#include "Matrix.h"
template<typename T>
class MatrixView :
	public Matrix<T>
{
public:
	MatrixView();
	~MatrixView();
};

template<typename T>
MatrixView<T>::MatrixView()
{
}

template<typename T>
MatrixView<T>::~MatrixView()
{
}


