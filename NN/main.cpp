#include <iostream>
#include "Matrix.h"
#include "Vector.h"
#include "Diag.h"
#include "Network.h"
using namespace std;

//#define TEST

#ifdef TEST
int main(void)//≤‚ ‘ª˘¥°¿‡
{
	int arr1[] = {
		4,0,-1,6,
		-1,2,5,3,
		3,7,1,-2
	};
	int arr2[] = {
		5,-1,
		2,0,
		-4,7,
		1,3
	};
	Matrix<int> mat1(3, 4);
	Matrix<int> mat2(4, 2);
	mat1.SetValueByArray(arr1, ALL);
	mat2.SetValueByArray(arr2, ALL);
	//cout << mat1 << endl << mat2 << endl;
	//cout << "Origin" << mat1 * mat2 << endl;
	//cout<< "Transposed" << (mat1 * mat2).Transpose() << endl;
	//cout << mat1.Sigmoid();
	//cout<<mat1.TypeCast<double>().operator*(1.12);

	Vector<int> v(mat1[1]);
	Diag<int> d(v);
	cout << d << endl;
	getchar();
	return 0;
}
#else
int main(void)
{
	int inputdat[70] = {
		1,1,1,1,1,1,0,
		0,1,1,0,0,0,0,
		1,1,0,1,1,0,1,
		1,1,1,1,0,0,1,
		0,1,1,0,0,1,1,
		1,0,1,1,0,1,1,
		1,0,1,1,1,1,1,
		1,1,1,0,0,0,0,
		1,1,1,1,1,1,1,
		1,1,1,1,0,1,1
	};
	int outputdat[100] = {
		1,0,0,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,0,0,
		0,0,1,0,0,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,1,0,0,0,0,
		0,0,0,0,0,0,1,0,0,0,
		0,0,0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,0,0,1,0,
		0,0,0,0,0,0,0,0,0,1
	};
	int nodecfg[] = {
		7,7,10,10
	};


	Matrix<int> inputmat(10, 7), outputmat(10, 10);
	Vector<int> nodeconfig(nodecfg, 4, false, false);
	inputmat.SetValueByArray(inputdat, ALL);
	outputmat.SetValueByArray(outputdat, ALL);

	Network n(inputmat, outputmat, nodeconfig, 0.01, 0.1f);
	n.Train();
	return 0;
}
#endif