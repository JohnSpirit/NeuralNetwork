//2020.6.9
#include <cmath>
#include <iostream>
#include "Matrix.h"
#include "Matrix.cpp"//模板类不支持分离编译
#include "Vector.h"
#include "Vector.cpp"
#include "Diag.h"
#include "Diag.cpp"
#include "Network.h"
using namespace std;

const float ALPHA = 0.5;

inline int f(float A)
{
	return A >= 0 ? 1 : 0;
}

inline float loss(float y, float eo)
{
	return eo - y;
}

float matmul(int *input, float *weight)
{
	float sum = 0;
	for (int i = 0; i < 2; i++)
	{
		sum += input[i] * weight[i];
	}
	return sum;
}

int main()
{

	Matrix<int> m(3, 4);
	cout<<m;
	m(2, 2) = 4;
	m(1, 2) = 6;
	cout << m;
	cout << m[1];
	cout << m.GetSize().GetLength()<<endl;
	
	Vector<int>* c=new Vector<int>();
	cout<<c->GetLength();

	getchar();
	return 0;
}