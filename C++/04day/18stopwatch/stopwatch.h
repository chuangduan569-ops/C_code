#ifndef __STOPWATCH_
#define __STOPWATCH_

#include <iostream>

using namespace std;

//[1]定义计秒表的额类
class Stopwatch
{
    public :
        Stopwatch() : m_sec(0), m_min(0) { }//构造函数(使用初始化列表的方式进行初始化)
        void run();//成员函数
        //自增运算符
        //后置自增
        Stopwatch operator++(int n);
        //输出运算符的重载
        friend ostream&operator<<(ostream &out, const Stopwatch &s);

    private :
        //定义成员变量
        int m_sec;//秒数
        int m_min;//分钟
};


#endif