#ifndef __COORDINATE_H
#define __COORDINATE_H

#include <iostream>

using namespace std;//打开命名空间

//加模板头
template<typename T1, typename T2>
class Coordinate
{
    public :
        //构造函数
        Coordinate(T1 x, T2 y);
        //成员函数
        void show() const;
    private :
        //成员变量的定义
        T1 _x;//_x是T1类型的
        T2 _y;//_y是T2类型的
};

//[1]模板类的成员函数在实现的时候必须要加模板头
//[2]作用域解析符号(::)前面不能直接写类名,需要加T1和T2的模板类型
template <typename T1, typename T2>
Coordinate<T1, T2>::Coordinate(T1 x, T2 y) : _x(x), _y(y)///使用初始化列表的方式进行初始化
{

}

template <typename T1, typename T2>
void Coordinate<T1, T2>::show() const
{
    cout << "(" << _x << ", " << _y << ")" << endl;
}



#endif
