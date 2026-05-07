#ifndef __TEST_H
#define __TEST_H

#include <iostream>

using namespace std;

//定义A类
class A
{
    public : 
        A(int a) : m_a(a)//定义构造函数,使用初始化列表的方式进行初始化
        {
        }
    protected :
        int m_a;//定义成员变量
};

//定义B类
class B
{
    public : 
        B(int b) : m_b(b)
        {
        }
    protected :
        int m_b;//定义成员变量
};

//定义C类
//以public方式继承A类和B类
//也就是说C类拥有两个基类
class C : public A, public B
{
    public :
    //定义构造函数,使用初始化列表的方式进行初始化
        C(int a, int b, int c) : A(a), B(b), m_c(c)
        {
        }
        void show() const//定义成员函数,展示成员变量
        {
            cout << m_a << "," << m_b << "," << m_c << endl;
        }
    private :
        int m_c;//定义成员变量
};  


#endif