#include "Network.h"
#include <cstring>

Network::Network()
{
}


Network::~Network()
{
}

template<typename T1, typename T2>
Network::Network(
	Matrix<T1>& input,
	Matrix<T2>& exp_output,
	Vector<int>& node_config,
	double error_limit,
	float alpha):_error_limit(error_limit),_alpha(alpha)
{
	//首先进行类型转换，将输入输出数据集全部转换成double形式。
	if (typeid(&input) != typeid(this->_input))
	{
		Matrix<double> input_mat = input.TypeCast<double>();
		this->_input = new Matrix<double>(input_mat);
	}
	else
		this->_input = new Matrix<double>(input);

	if (typeid(&exp_output) != typeid(this->_exp_output))
	{
		Matrix<double> input_mat = input.TypeCast<double>();
		this->_input = new Matrix<double>(input_mat);
	}
	else
		this->_exp_output = new Matrix<double>(exp_output);
	
	//读取网络信息并建立网络
	/*
		节点信息包括各层节点个数（第0层节点数为输入数据个数，第1层为输入层）。
		然后程序会根据这些信息自动分配内存并初始化。
	*/
	this->_node_config = new Vector<int>(node_config);
	
	this->_layers = _node_config->GetLength();//获取层数（包含输入数据-第0层）
	
	_input_vector = new Vector<double>[_layers];
	
	_output_vector = new Vector<double>[_layers-1];
	for (int i = 0; i < _layers; i++)
	{
		_input_vector[i].ReSize((*_node_config)[i], 1);
	}
	for (int i = 0; i < _layers-1; i++)
	{
		_output_vector[i].ReSize((*_node_config)[i], 1);
	}

}