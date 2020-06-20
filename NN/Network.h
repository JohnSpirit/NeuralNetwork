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

	void ReadWeightFromFile(const char* filename);//从文件读取权重信息
	void SaveWeightToFile(const char* filename);//保存权重信息至文件

	void Train();//训练
	double ForwardCalc(int group_num);//前向计算
	void BackPpg();//反向传播
	void ShowResult(bool predict=true);//显示运行结果
	template<typename T1, typename T2>
	void Test(const Matrix<T1>& test_input, const Matrix<T2>& test_exp_output);//测试
	void TestImage();//测试图片

private:
	/*---- 输入数据 ----*/
	double _error_limit = 0.1;//误差限
	float _alpha = 0.1f;//学习率
	int _layers = 0;
	Vector<int>* _node_config = nullptr;
	Matrix<double>* _input = nullptr;//训练数据集
	Matrix<double>* _exp_output = nullptr;//训练教师数据集

	/*---- 自动生成数据 ----*/
	Vector<double>* _input_vector = nullptr;//各层输入列向量
	Vector<double>* _output_vector = nullptr;//各层输出列向量
	Matrix<double>* _weight = nullptr;//各层权重矩阵

	Diag<double>* _derive = nullptr;//导数对角矩阵（用于求更新权重矩阵）
	Vector<double>* _delta = nullptr;//δ列向量（用于求更新权重矩阵）
	Matrix<double>* _deltaweight = nullptr;//各层更新权重矩阵
};

template<typename T1, typename T2>
Network::Network(
	const Matrix<T1>& input,
	const Matrix<T2>& exp_output,
	const Vector<int>& node_config,
	double error_limit,
	float alpha) :_error_limit(error_limit), _alpha(alpha)
{
	cout << "输入数据类型:" << typeid(input).name() << endl
		<< "节点信息(输入个数与各层节点数):" << node_config << endl
		<< "学习率" << alpha << endl
		<< "误差限" << error_limit << endl;

	//首先进行类型转换，将输入输出数据集全部转换成double形式。
	Matrix<double> input_mat = input.TypeCast<double>();
	Matrix<double> output_mat = exp_output.TypeCast<double>();
	this->_input = new Matrix<double>(input_mat);
	this->_exp_output = new Matrix<double>(output_mat);

	//读取网络信息并建立网络
	//节点信息包括各层节点个数,然后程序会根据这些信息自动分配内存并初始化。
	this->_node_config = new Vector<int>(node_config);//复制节点信息
	this->_layers = _node_config->GetLength();//获取层数（包含输入数据-第0层输出）

	//给各层输入输出列向量分配内存
	_input_vector = new Vector<double>[_layers - 1];
	_output_vector = new Vector<double>[_layers];
	for (int i = 0; i < _layers - 1; i++)_input_vector[i].ReSize((*_node_config)[i], 1);
	for (int i = 0; i < _layers; i++)_output_vector[i].ReSize((*_node_config)[i] + 1, 1);

	//给各层权重矩阵分配内存
	_weight = new Matrix<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
	{
		_weight[i].ReSize((*_node_config)[i + 1], (*_node_config)[i] + 1);
		cout << "第" << i << "层矩阵大小=" << (*_node_config)[i + 1] << "*" << (*_node_config)[i] + 1 << endl;
		_weight[i].Randomize();//随机化
	}

	//以下为反向传播中需要反复使用的矩阵，故也预先分配好内存。
	//diag(f'x)矩阵：
	_derive = new Diag<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_derive[i].ReSize((*_node_config)[i + 1], (*_node_config)[i + 1]);
	//delta矩阵
	_delta = new Vector<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_delta[i].ReSize((*_node_config)[i + 1], 1);
	//deltaweight矩阵
	_deltaweight = new Matrix<double>[_layers - 1];
	for (int i = 0; i < _layers - 1; i++)
		_deltaweight[i].ReSize((*_node_config)[i + 1], (*_node_config)[i] + 1);
}

template<typename T1, typename T2>
void Network::Test(const Matrix<T1>& test_input, const Matrix<T2>& test_exp_output)
{
	//类型转换
	Matrix<double> test_input_mat = test_input.TypeCast<double>();
	Matrix<double> test_output_mat = test_exp_output.TypeCast<double>();
	//保存原来训练数据，并将指针替换为测试数据集
	Matrix<double>* save_train_input = this->_input;
	Matrix<double>* save_train_output = this->_exp_output;
	_input = &test_input_mat;
	_exp_output = &test_output_mat;

	cout << "开始测试" << endl;
	double avr_e = 0.0, e = 0.0;
	int predict_num = -1, exp_num = -1, same_counter = 0;
	for (int i = 0; i < _input->GetSize()[0]; i++)
	{
		e = ForwardCalc(i);
		avr_e += e;
		//输出预测值和期望值
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
