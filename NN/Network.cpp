#include <Windows.h>
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

void Network::ReadWeightFromFile(const char * filename)
{
	fstream weight_is(filename, ios::in | ios::binary);
	for (int i = 0; i < _layers - 1; i++)
		_weight[i].ReadFromFile(weight_is, false);
	weight_is.close();
}

void Network::SaveWeightToFile(const char * filename)
{
	fstream weight_os(filename, ios::out | ios::binary | ios::trunc);
	for (int i = 0; i < _layers - 1; i++)
		_weight[i].SaveToFile(weight_os, false);
	weight_os.close();
}

void Network::Train()
{
	cout << "��ʼѵ��" << endl;
	double avr_e = 0.0, e = 0.0;//ȫ���������ݵ�ƽ�����
	//����ÿһ���������ݣ�����ǰ����㣬����ԭʼ��
	for (int count = 0;; count++)
	{
		avr_e = 0.0;
		cout << "*******************��" << count << "��ѭ��*******************" << endl;
		for (int i = 0; i < _input->GetSize()[0]; i++)
		{
			e = ForwardCalc(i);
			avr_e += e;
			BackPpg();
			if (!(i % 100))cout << "��" << i << "������" << "\t" << e << "\n";
		}
		avr_e /= _input->GetSize()[0];
		cout << "error=" << avr_e << endl;
		SaveWeightToFile("D:\\program files\\C++\\NN\\minist\\weight.dat");
		if (abs(avr_e) < _error_limit) {
			ShowResult();
			break;
		}
	}
}

double Network::ForwardCalc(int group_num)
{
	//���Ƚ������������ݸ�ֵ����һ��������������
	_output_vector[0] = (*_input)[group_num].Transpose(true, 1);//�������Ҫ��ĩβappend=1
	for (int layer = 0; layer < _layers - 1; layer++)
	{
		_input_vector[layer] = _weight[layer] * _output_vector[layer];
		_output_vector[layer + 1] = _input_vector[layer].Sigmoid().Append();
	}
	//������������
	Vector<double> error((*_exp_output)[group_num].Transpose() -
		_output_vector[_layers - 1].Slice(0, 0, _output_vector[_layers - 1].GetLength() - 2, 0));
	//�������һ���deltaֵ
	_delta[_layers - 2] = _input_vector[_layers - 2].SigmoidDerive().ToDiag()*
		error;

	return Vector<double>(error.Multi(error)*0.5).Sum();
}

void Network::BackPpg()
{
	//��������f'x����
	for (int i = _layers - 2; i >= 0; i--)
	{
		_derive[i] = _input_vector[i].SigmoidDerive().ToDiag();
	}

	//�������deltaֵ��������deltaweight����
	for (int i = _layers - 3; i >= 0; i--)
	{
		_delta[i] = _derive[i] *
			_weight[i + 1].Slice(
				0, 0, _weight[i + 1].GetSize()[0] - 1, _weight[i + 1].GetSize()[1] - 2
			).Transpose()*
			_delta[i + 1];
	}
	//���ɸ���deltaweight���󣬲���deltaweight������weight���������ʵ��Ȩ�ظ���
	for (int i = _layers - 2; i >= 0; i--)
	{
		_deltaweight[i] = _delta[i] * (_output_vector[i].Transpose())*(_alpha);
		_weight[i] = _weight[i] + _deltaweight[i];
	}
}

void Network::ShowResult(bool predict)
{
	double avr_e = 0.0;
	for (int i = 0; i < _input->GetSize()[0]; i++)
	{
		double e = ForwardCalc(i);
		cout << "�������Ӧƽ�������=" << e << endl;
		avr_e += e;
		cout << "����\n" << (*_input)[i] << endl;
		cout << "�������\n" << (*_exp_output)[i] << endl;
		cout << "ʵ�����\n" << _output_vector[_layers - 1].Slice(0, 0, _output_vector[_layers - 1].GetLength() - 2, 0).Transpose() << endl;
	}
	cout << "ƽ��ƽ�������=" << avr_e / _input->GetSize()[0] << endl;
}

void Network::TestImage()
{
	uint8_t* imagedat = new uint8_t[28 * 28];
	Matrix<uint8_t> imagemat(1, 28 * 28);
	Matrix<double> imagematd(1, 28 * 28);
	double e = 0.0;
	int predict_num = -1;
	system("cls");
	for (int count = 0;; count++)
	{
		system("python \"minist\\convert.py\"");
		fstream input("minist\\test_image", ios::in | ios::binary);
		input.seekg(16, ios::cur);
		input.read((char*)imagedat, 28 * 28);
		input.close();

		imagemat = imagemat.SetValueByArray(imagedat, ALL);
		imagematd = imagemat.TypeCast<double>()*0.00390625;

		this->_input = &imagematd;
		//���Ƚ������������ݸ�ֵ����һ��������������
		_output_vector[0] = (*_input)[0].Transpose(true, 1);//�������Ҫ��ĩβappend=1
		for (int layer = 0; layer < _layers - 1; layer++)
		{
			_input_vector[layer] = _weight[layer] * _output_vector[layer];
			_output_vector[layer + 1] = _input_vector[layer].Sigmoid().Append();
		}
		predict_num = Vector<double>(
			_output_vector[_layers - 1].
			Slice(0, 0, _output_vector[_layers - 1].
				GetLength() - 2, 0)
			).Argmax();
		cout << count << "\t" << predict_num << "\n";
		Sleep(2000);
	}
	delete[] imagedat;
}
