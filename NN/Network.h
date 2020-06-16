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
