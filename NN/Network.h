#pragma once
#include "Matrix.h"
class Network
{
public:

	Network();
	template<typename T1, typename T2>
	Network(Matrix<T1>& input, Matrix<T2>& exp_output, Vector<int>& node_config, double error_limit, float alpha);
	~Network();

private:
	/*---- 输入数据 ----*/
	double _error_limit = 0.1;//误差限
	float _alpha = 0.1f;//学习率
	int _layers = 0;
	Vector<int>* _node_config = nullptr;
	Matrix<double>* _input = nullptr;//输入数据集
	Matrix<double>* _exp_output = nullptr;//教师数据集

	/*---- 自动生成数据 ----*/
	Vector<double>* _input_vector = nullptr;//各层输入列向量
	Vector<double>* _output_vector = nullptr;//各层输出列向量
	Matrix<double>* _weight = nullptr;//各层权重矩阵

	Diag<double>* _derive = nullptr;//导数对角矩阵（用于求更新权重矩阵）
	Vector<double>* _delta = nullptr;//δ列向量（用于求更新权重矩阵）
	Matrix<double>* _deltaweight = nullptr;//各层更新权重矩阵
};
