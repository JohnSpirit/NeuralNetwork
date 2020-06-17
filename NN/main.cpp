#include <iostream>
#include "Matrix.h"
#include "Vector.h"
#include "Diag.h"
#include "Network.h"
using namespace std;

#define TEST

#ifdef TEST
int main(void)//≤‚ ‘ª˘¥°¿‡
{
	
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

	return 0;
}
#endif