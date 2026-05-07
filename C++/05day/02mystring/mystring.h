#ifndef __MYSTRING_H
#define __MYSTRING_H

#include <iostream>

using namespace std;//打开命名空间

class Mystring
{
    public :
        //仿照C++官方手册中声明多个构造函数
        Mystring();//默认构造函数
        Mystring(const char *p);//普通构造函数
        Mystring(const Mystring &s);//拷贝构造函数[将来涉及到动态内存开辟,需要深拷贝]
        //把输入输出运算符进行重载,声明为Mystring类的友元函数
        friend ostream &operator<<(ostream &out, const Mystring &s);
        friend istream &operator>>(istream &in, Mystring &s);
        //下标运算符重载
        char &operator[](int index);
    private :
        //成员变量的定义
        char *m_str;//指向字符串的起始地址
        int m_len;//记录字符串的长度
};

#endif