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
	double avr_e = 0.0;//ȫ���������ݵ�ƽ�����
	//����ÿһ���������ݣ�����ǰ����㣬����ԭʼ��
	for (int i = 0; i < _input->GetSize()[0]; i++)
	{
		avr_e += ForwardCalc(i);
		BackPpg(i);
	}
}

double Network::ForwardCalc(int group_num)
{
	//���Ƚ������������ݸ�ֵ����һ��������������
	_output_vector[0] = (*_input)[group_num].Transpose();//�������Ҫ��ĩβappend=1
	for (int layer = 0; layer < _layers-1; layer++)
	{
		_input_vector[layer] = _weight[layer] * _output_vector[layer];
		_output_vector[layer+1] = _input_vector[layer].Sigmoid(true);
	}
	//�������һ���deltaֵ
	_delta[_layers - 1] = _exp_output[group_num].Transpose() - _output_vector[_layers - 1];
}

void Network::BackPpg(int group_num)
{
	//�������deltaֵ������f'x�����������deltaweight����

}
