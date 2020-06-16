#include "Network.h"

Network::Network()
{
}


Network::~Network()
{
	delete _node_config;
	delete _input;
	delete _exp_output;
	delete[] _input_vector;
	delete[] _output_vector;
	delete[] _weight;
	delete[] _derive;
	delete[] _delta;
	delete[] _deltaweight;
}

void Network::Train()
{
	double avr_e = 0.0;//全部输入数据的平均误差
	//对于每一组输入数据，进行前向计算，返回原始误差。
	for (int i = 0; i < _input->GetSize()[0]; i++)
	{
		avr_e += ForwardCalc(i);
		BackPpg(i);
	}
}

double Network::ForwardCalc(int group_num)
{
	//首先将该组输入数据赋值给第一层输入列向量。
	_output_vector[0] = (*_input)[group_num].Transpose();//输出向量要在末尾append=1
	for (int layer = 0; layer < _layers-1; layer++)
	{
		_input_vector[layer] = _weight[layer] * _output_vector[layer];
		_output_vector[layer+1] = _input_vector[layer].Sigmoid(true);
	}
	//计算最后一层的delta值
	_delta[_layers - 1] = _exp_output[group_num].Transpose() - _output_vector[_layers - 1];
}

void Network::BackPpg(int group_num)
{
	//逆序计算delta值，生成f'x矩阵，最后生成deltaweight矩阵

}
