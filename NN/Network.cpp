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
	cout << "开始训练" << endl;
	double avr_e = 0.0;//全部输入数据的平均误差
	//对于每一组输入数据，进行前向计算，返回原始误差。
	for (int count = 0;; count++)
	{
		cout << count << "\t";
		for (int i = 0; i < _input->GetSize()[0]; i++)
		{
			avr_e += ForwardCalc(i);
			BackPpg();
		}
		avr_e /= _input->GetSize()[0];
		cout << "error=" << avr_e << endl;
		if (avr_e < _error_limit)break;
	}
}

double Network::ForwardCalc(int group_num)
{
	//首先将该组输入数据赋值给第一层输入列向量。
	_output_vector[0] = (*_input)[group_num].Transpose(true,1);//输出向量要在末尾append=1
	for (int layer = 0; layer < _layers - 1; layer++)
	{
		_input_vector[layer] = _weight[layer] * _output_vector[layer];
		_output_vector[layer + 1] = _input_vector[layer].Sigmoid(true);
	}
	//计算输出层误差
	Vector<double> error(_exp_output[group_num].Transpose() -
		_output_vector[_layers - 1].Slice(0, 0, _output_vector[_layers - 1].GetLength() - 2, 0));
	//计算最后一层的delta值
	_delta[_layers - 2] = _input_vector[_layers - 2].SigmoidDerive().ToDiag()*
		error;
	
	return Vector<double>(error.Multi(error)*0.5).Sum();
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
				0, 0, _weight[i].GetSize()[0] - 1, _weight[i].GetSize()[1] - 2
			).Transpose()*
			_delta[i + 1];
	}
	//生成各层deltaweight矩阵，并将deltaweight矩阵与weight矩阵相加以实现权重更新
	for (int i = _layers - 2; i >= 0; i--)
	{
		_deltaweight[i] = _delta[i] * (_output_vector[i].Transpose())*(-_alpha);
		_weight[i] = _weight[i] + _deltaweight[i];
	}

}
