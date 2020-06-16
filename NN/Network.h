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

	void Train();//ѵ��
	double ForwardCalc(int group_num);//ǰ�����
	void BackPpg(int group_num);//���򴫲�

private:
	/*---- �������� ----*/
	double _error_limit = 0.1;//�����
	float _alpha = 0.1f;//ѧϰ��
	int _layers = 0;
	Vector<int>* _node_config = nullptr;
	Matrix<double>* _input = nullptr;//�������ݼ�
	Matrix<double>* _exp_output = nullptr;//��ʦ���ݼ�

	/*---- �Զ��������� ----*/
	Vector<double>* _input_vector = nullptr;//��������������
	Vector<double>* _output_vector = nullptr;//�������������
	Matrix<double>* _weight = nullptr;//����Ȩ�ؾ���

	Diag<double>* _derive = nullptr;//�����ԽǾ������������Ȩ�ؾ���
	Vector<double>* _delta = nullptr;//�������������������Ȩ�ؾ���
	Matrix<double>* _deltaweight = nullptr;//�������Ȩ�ؾ���
};

template<typename T1, typename T2>
Network::Network(
	Matrix<T1>& input,
	Matrix<T2>& exp_output,
	Vector<int>& node_config,
	double error_limit,
	float alpha) :_error_limit(error_limit), _alpha(alpha)
{
	//���Ƚ�������ת����������������ݼ�ȫ��ת����double��ʽ��
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

	//��ȡ������Ϣ����������
	/*
		�ڵ���Ϣ��������ڵ��������0��ڵ���Ϊ�������ݸ�������1��Ϊ����㣩��
		Ȼ�����������Щ��Ϣ�Զ������ڴ沢��ʼ����
	*/
	this->_node_config = new Vector<int>(node_config);//���ƽڵ���Ϣ
	this->_layers = _node_config->GetLength();//��ȡ������������������-��0�������

	//������������������������ڴ�
	_input_vector = new Vector<double>[_layers - 1];
	_output_vector = new Vector<double>[_layers];
	for (int i = 0; i < _layers - 1; i++)_input_vector[i].ReSize((*_node_config)[i], 1);
	for (int i = 0; i < _layers; i++)_output_vector[i].ReSize((*_node_config)[i] + 1, 1);

	//������Ȩ�ؾ�������ڴ�
	_weight = new Matrix<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
	{
		_weight[i].ReSize((*_node_config)[i] + 1, (*_node_config)[i + 1]);
	}

	//����Ϊ���򴫲�����Ҫ����ʹ�õľ��󣬹�ҲԤ�ȷ�����ڴ档
	//diag(f'x)����
	_derive = new Diag<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_derive[i].ReSize((*_node_config)[i + 1], (*_node_config)[i + 1]);
	//delta����
	_delta = new Vector<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_delta[i].ReSize((*_node_config)[i + 1], 1);
	//deltaweight����
	_deltaweight = new Matrix<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_deltaweight[i].ReSize((*_node_config)[i] + 1, (*_node_config)[i + 1]);
}
