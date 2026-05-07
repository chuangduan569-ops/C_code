#ifndef __COMPLEX_H
#define __COMPLEX_H

class Complex
{
    public :    
        Complex(float real = 0.0, float img = 0.0);//全缺省的构造函数
        /*
        我们需要重载operato+()
        operator+()作为Complex类的成员函数存在
        返回值 : Complex类的对象
        参数 :: const Complex &c  其他Complex类的对象引用
        */
       Complex operator+(const Complex &c);
       void show() const;//展示成员属性
    private :
        float m_real;//复数的实部
        float m_img;//复数的虚部
};


#endif