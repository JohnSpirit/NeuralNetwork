#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "Diag.h"
uint8_t* read_images(const char* filename);
class Network
{
public:

	Network();
	template<typename T1, typename T2>
	Network(const Matrix<T1>& input, const Matrix<T2>& exp_output,const Vector<int>& node_config, double error_limit, float alpha);
	~Network();

	void ReadWeightFromFile(const char* filename);//���ļ���ȡȨ����Ϣ
	void SaveWeightToFile(const char* filename);//����Ȩ����Ϣ���ļ�

	void Train();//ѵ��
	double ForwardCalc(int group_num);//ǰ�����
	void BackPpg();//���򴫲�
	void ShowResult(bool predict=true);//��ʾ���н��
	template<typename T1, typename T2>
	void Test(const Matrix<T1>& test_input, const Matrix<T2>& test_exp_output);//����
	void TestImage();//����ͼƬ

private:
	/*---- �������� ----*/
	double _error_limit = 0.1;//�����
	float _alpha = 0.1f;//ѧϰ��
	int _layers = 0;
	Vector<int>* _node_config = nullptr;
	Matrix<double>* _input = nullptr;//ѵ�����ݼ�
	Matrix<double>* _exp_output = nullptr;//ѵ����ʦ���ݼ�

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
	const Matrix<T1>& input,
	const Matrix<T2>& exp_output,
	const Vector<int>& node_config,
	double error_limit,
	float alpha) :_error_limit(error_limit), _alpha(alpha)
{
	cout << "������������:" << typeid(input).name() << endl
		<< "�ڵ���Ϣ(������������ڵ���):" << node_config << endl
		<< "ѧϰ��" << alpha << endl
		<< "�����" << error_limit << endl;

	//���Ƚ�������ת����������������ݼ�ȫ��ת����double��ʽ��
	Matrix<double> input_mat = input.TypeCast<double>();
	Matrix<double> output_mat = exp_output.TypeCast<double>();
	this->_input = new Matrix<double>(input_mat);
	this->_exp_output = new Matrix<double>(output_mat);

	//��ȡ������Ϣ����������
	//�ڵ���Ϣ��������ڵ����,Ȼ�����������Щ��Ϣ�Զ������ڴ沢��ʼ����
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
		_weight[i].ReSize((*_node_config)[i + 1], (*_node_config)[i] + 1);
		cout << "��" << i << "������С=" << (*_node_config)[i + 1] << "*" << (*_node_config)[i] + 1 << endl;
		_weight[i].Randomize();//�����
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
		_deltaweight[i].ReSize((*_node_config)[i + 1], (*_node_config)[i] + 1);
}

template<typename T1, typename T2>
void Network::Test(const Matrix<T1>& test_input, const Matrix<T2>& test_exp_output)
{
	//����ת��
	Matrix<double> test_input_mat = test_input.TypeCast<double>();
	Matrix<double> test_output_mat = test_exp_output.TypeCast<double>();
	//����ԭ��ѵ�����ݣ�����ָ���滻Ϊ�������ݼ�
	Matrix<double>* save_train_input = this->_input;
	Matrix<double>* save_train_output = this->_exp_output;
	_input = &test_input_mat;
	_exp_output = &test_output_mat;

	cout << "��ʼ����" << endl;
	double avr_e = 0.0, e = 0.0;
	int predict_num = -1, exp_num = -1, same_counter = 0;
	for (int i = 0; i < _input->GetSize()[0]; i++)
	{
		e = ForwardCalc(i);
		avr_e += e;
		//���Ԥ��ֵ������ֵ
		predict_num = Vector<double>(
			_output_vector[_layers - 1].
			Slice(0, 0, _output_vector[_layers - 1].
				GetLength() - 2, 0)
			).Argmax();
		exp_num = (*_exp_output)[i].Argmax();
		cout << i << "\tpredict: " << predict_num
			<< "\texpect: " << exp_num << endl;
		if (exp_num == predict_num)same_counter++;
	}
	avr_e /= _input->GetSize()[0];
	cout.precision(7);
	cout << "error=" << avr_e << endl;
	cout.precision(4);
	cout << "accuracy=" << double(same_counter) / _input->GetSize()[0] * 100 << "%" << endl;
	_input = save_train_input;
	_exp_output = save_train_output;
}
