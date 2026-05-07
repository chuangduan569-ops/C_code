#ifndef __ARR_H
#define __ARR_H

/*
[1]定义一个可变长数组的类
*/
class VLA
{
    public :
        VLA(int len = 0);//如果不传参时使用的构造函数[全缺省]
        VLA(const VLA &v);//拷贝构造函数[需要深拷贝]
        ~VLA();//析构函数
        //能够体现构造函数可以重载,但是析构函数不能重载

        int at(int i);//返回下标为 i 的元素
        int setValue(int i, int value);//给下标为 i 的元素设置 value 值
        void show();//遍历可变长数组
        void printThis();//打印this指针
    private :
        int *m_a;//保存可变长数组的起始地址
        int m_len;//保存可变长数组的长度[容量]
};

#endif