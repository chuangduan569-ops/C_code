#ifndef __COMPLEX_H
#define __COMPLEX_H

#include <iostream>
using namespace std;


class Complex
{
    public :
        Complex(float real = 0.0, float img = 0.0);//全缺省的构造函数
        /*我们需要重载operator+()
        operator+()作为Complex类的成员函数存在
        返回值 : Complex类的对象
        参数 : const Complex &c 其他Complex类对象的引用
        */
        Complex operator+(const Complex &c);
        Complex operator*(const Complex &c);          // 成员乘法（新增）
        //重载运算符 ==
        bool operator==(const Complex &c);
        void show() const;//展示成员属性
        //将类外部的函数声明为Complex类的友元函数
        friend Complex operator-(const Complex &c1, const Complex &c2);
         // 友元除法（新增）
        friend Complex operator/(const Complex &c1, const Complex &c2);
        friend ostream &operator<<(ostream &out, const Complex &c);
        friend istream &operator>>(istream &in, Complex &c);
    private :
        float m_real;//复数的实部
        float m_img;//复数的虚部
};


#endif