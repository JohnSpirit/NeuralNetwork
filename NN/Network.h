#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "Diag.h"
#include "MatrixView.h"
class Network
{
public:

	Network();
	template<typename T1, typename T2>
	Network(Matrix<T1>& input, Matrix<T2>& exp_output, Vector<int>& node_config, double error_limit, float alpha);
	~Network();

	void Train();//训练
	double ForwardCalc(int group_num);//前向计算
	void BackPpg(int group_num);//反向传播

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

template<typename T1, typename T2>
Network::Network(
	Matrix<T1>& input,
	Matrix<T2>& exp_output,
	Vector<int>& node_config,
	double error_limit,
	float alpha) :_error_limit(error_limit), _alpha(alpha)
{
	//首先进行类型转换，将输入输出数据集全部转换成double形式。
	if (typeid(&input) != typeid(this->_input))
	{
		Matrix<double> input_mat = input.TypeCast<double>();
		this->_input = new Matrix<double>(input_mat);
	}
	else
		this->_input = new Matrix<double>(reinterpret_cast<Matrix<double>&>(input));

	if (typeid(&exp_output) != typeid(this->_exp_output))
	{
		Matrix<double> input_mat = input.TypeCast<double>();
		this->_input = new Matrix<double>(input_mat);
	}
	else
		this->_exp_output = new Matrix<double>(reinterpret_cast<Matrix<double>&>(exp_output));

	//读取网络信息并建立网络
	/*
		节点信息包括各层节点个数（第0层节点数为输入数据个数，第1层为输入层）。
		然后程序会根据这些信息自动分配内存并初始化。
	*/
	this->_node_config = new Vector<int>(node_config);//复制节点信息
	this->_layers = _node_config->GetLength();//获取层数（包含输入数据-第0层输出）

	//给各层输入输出列向量分配内存
	_input_vector = new Vector<double>[_layers - 1];
	_output_vector = new Vector<double>[_layers];
	for (int i = 0; i < _layers - 1; i++)_input_vector[i].ReSize((*_node_config)[i], 1);
	for (int i = 0; i < _layers; i++)_output_vector[i].ReSize((*_node_config)[i] + 1, 1);

	//给各层权重矩阵分配内存
	_weight = new Matrix<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
	{
		_weight[i].ReSize((*_node_config)[i] + 1, (*_node_config)[i + 1]);
	}

	//以下为反向传播中需要反复使用的矩阵，故也预先分配好内存。
	//diag(f'x)矩阵：
	_derive = new Diag<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_derive[i].ReSize((*_node_config)[i + 1], (*_node_config)[i + 1]);
	//delta矩阵
	_delta = new Vector<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_delta[i].ReSize((*_node_config)[i + 1], 1);
	//deltaweight矩阵
	_deltaweight = new Matrix<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_deltaweight[i].ReSize((*_node_config)[i] + 1, (*_node_config)[i + 1]);
}
