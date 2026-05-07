#ifndef ARR_H
#define ARR_H

class VLA
{
public:
	VLA(int len = 0);   // 构造函数 (带默认参数)
	VLA(const VLA &v);  // 拷贝构造函数 (深拷贝)
	~VLA();             // 析构函数 (析构函数不能重载)

	int at(int i);                   // 返回下标为 i 的元素
	int setValue(int i, int value);  // 给下标为 i 的元素赋 value 值
	void show();                     // 遍历可变长数组
	void printThis();                // 打印 this 指针
	/*
	operator[]()函数返回值应该是下标为 i 的成员的引用
	如果将来需要做v1[1] = 100;我们需要返回的是下标为 i 的成员的引用
	如果返回的是下标为 i 的成员的值, 将来做 v1[1] = 100的操作会报错
	*/
	int &operator[](int i);
	//常函数版本
	const int &operator[](int i) const;
	//前面的const表示函数的返回值是一个常引用,后面的表示是一个常函数

	// 增删改查
	int insert(int value);
	int del(int i);
	int update(int i, int value);
	int select(int i);

private:
	int *m_a;   // 保存可变长数组的起始地址
	int m_len;  // 保存可变长数组的长度
};

#endif