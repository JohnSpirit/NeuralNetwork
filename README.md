# NeuralNetwork

这是一个全连接型神经网络训练/测试程序，用来实现mnist库的手写数字识别。


### 节点的设置

在nodecfg数组中依次输入：

+ 输入数据个数
+ 所有中间层的节点数
+ 输出层节点数

```C++
int main(void)
{
    int nodecfg[] = {
        28 * 28,28 * 28,10
    };
    Vector<int> nodeconfig(nodecfg, 3, false, false);
    ...
}
```

### 数据的导入

分别在以下四个字符串内输入训练和测试数据集（图片和标签）的路径（默认相对路径，不用更改）

```C++
int main(void)
{
    ...
    uint8_t* inputdat = read_images("minist\\train-images.idx3-ubyte");
    uint8_t* labels = read_labels("minist\\train-labels.idx1-ubyte");
    ...
    uint8_t* test_inputdat = read_images("minist\\t10k-images.idx3-ubyte");
    uint8_t* test_labels = read_labels("minist\\t10k-labels.idx1-ubyte");
    ...
}
```


### 训练与测试

首先生成一个Network类实例，输入误差限与学习率：
```C++
Network n(
        inputmat.TypeCast<double>().operator*(0.00390625),
        outputmat,
        nodeconfig,
        0.00001,//误差限
        0.1f//学习率
        );
```

进行训练，则调用Train方法：
```C++
n.Train();
```

根据10000个数字的测试数据集进行测试，需调用Test方法：
```C++
n.Test(test_inputmat.TypeCast<double>().operator*(0.00390625), test_outputmat);
```

如果要导入权重数据，则需调用ReadWeightFromFile方法：
```C++
n.ReadWeightFromFile("minist\\weight.dat");
```

根据图片进行数字识别，需调用TestImage方法：
```C++
n.TestImage();
```
此时程序会每隔一段时间（默认2秒）对`minist\\test_image.bmp`所对应的图像进行识别，注意该图像必须为黑白/灰度图像，28*28像素。

### 部分实现细节介绍

待补充（2020.6.20）

