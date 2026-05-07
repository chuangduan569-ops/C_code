#ifndef __TEST_H
#define __TEST_H

#include <iostream>

using namespace std;

//定义Base类
class Base
{
    public : 
        //定义构造函数,使用初始化列表的方式进行初始化
        Base(int base) : m_base(base){}
    protected :
        int m_base;//定义成员变量
};


//定义A类,以public的方式继承Base类
//也就相当于A类中可以使用两个成员变量,一个是m_a,一个是m_base
class A : virtual public Base
{
    public : 
        //定义构造函数,使用初始化列表的方式进行初始化
        A(int a, int base) : m_a(a), Base(base)
        {
        }
    protected :
        int m_a;//定义成员变量
};

//定义B类,以public的方式继承Base类
//也就相当于A类中可以使用两个成员变量,一个是m_a,一个是m_base
class B : virtual public Base
{
    public : 
        //定义构造函数,使用初始化列表的方式进行初始化
        B(int b, int base) : m_b(b), Base(base)
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
        C(int a, int b, int c, int base) : A(a, base + 1), B(b, base - 1), m_c(c), Base(base)
        {
        }
        //由于A类继承了Base类,B类继承了Base类,而且修改了A类和B类的构造函数
        //所以需要多传一个参数
        //注意 : 因为Base类并不是C类的直接基类,所以不能直接调用Base类的构造函数
        void show() const//定义成员函数,展示成员变量
        {
            cout << m_a << "," << m_b << "," << m_c << endl;
            cout << "A::m_base " << A::m_base << endl;
            cout << "B::m base " << B::m_base << endl;

    // 打印两个 Base 子对象的地址
    // 方法  通过 this 指针转换
    const Base* baseA = static_cast<const A*>(this);  // A子对象中的Base部分
    const Base* baseB = static_cast<const B*>(this);  // B子对象中的Base部分
    cout << "Address of Base from A: " << baseA << endl;
    cout << "Address of Base from B: " << baseB << endl;
        }
    private :
        int m_c;//定义成员变量
};  


#endif