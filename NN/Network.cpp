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
	for (int count = 0;; count++)
	{
		for (int i = 0; i < _input->GetSize()[0]; i++)
		{
			avr_e += ForwardCalc(i);
			BackPpg();
		}
		avr_e /= _input->GetSize()[0];
		if (avr_e < _error_limit)break;
	}
}

double Network::ForwardCalc(int group_num)
{
	//首先将该组输入数据赋值给第一层输入列向量。
	_output_vector[0] = (*_input)[group_num].Transpose();//输出向量要在末尾append=1
	for (int layer = 0; layer < _layers - 1; layer++)
	{
		_input_vector[layer] = _weight[layer] * _output_vector[layer];
		_output_vector[layer + 1] = _input_vector[layer].Sigmoid(true);
	}
	//计算最后一层的delta值
	_delta[_layers - 2] = _input_vector[_layers - 2].SigmoidDerive().ToDiag()*
		(_exp_output[group_num].Transpose() - _output_vector[_layers - 1]);
	return 0;
}

void Network::BackPpg()
{
	//首先生成f'x矩阵
	for (int i = _layers - 2; i >= 0; i--)
	{
		_derive[i] = _input_vector[i].SigmoidDerive().ToDiag();
	}

	//逆序计算delta值，并生成deltaweight矩阵
	for (int i = _layers - 3; i >= 0; i--)
	{
		_delta[i] = _derive[i] *
			_weight[i].Slice(
				0, 0, _weight[i].GetSize()[0], _weight[i].GetSize()[1] - 1
			).Transpose()*
			_delta[i + 1];
	}
	//生成各层deltaweight矩阵
	for (int i = _layers - 2; i >= 0; i--)
	{
		_deltaweight[i] = _delta[i] * (_output_vector[i].Transpose()).Multi(-_alpha);
	}
}
