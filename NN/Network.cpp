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
	//���Ƚ�������ת����������������ݼ�ȫ��ת����double��ʽ��
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
	
	//��ȡ������Ϣ����������
	/*
		�ڵ���Ϣ��������ڵ��������0��ڵ���Ϊ�������ݸ�������1��Ϊ����㣩��
		Ȼ�����������Щ��Ϣ�Զ������ڴ沢��ʼ����
	*/
	this->_node_config = new Vector<int>(node_config);
	
	this->_layers = _node_config->GetLength();//��ȡ������������������-��0�㣩
	
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