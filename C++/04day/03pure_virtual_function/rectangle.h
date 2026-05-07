#ifndef __RECTANGLE_H
#define __RECTANGLE_H

//为了继承Line类,包含"line.h"
#include "line.h"

//[1]定义Rectangle类,以public得方式继承Line类
class Rectangle : public Line
{
    public :
        //Rectangle类得构造函数,使用初始化列表得方式进行初始化
        Rectangle(int len, int width) : Line(len), m_width(width){}
        //从Line类中继承的额纯虚函数volumee()没有实现,所以也不能实例化对象
        int area();//面积

    protected :
        //成员变量---->宽度
        int m_width;
};


#endif
