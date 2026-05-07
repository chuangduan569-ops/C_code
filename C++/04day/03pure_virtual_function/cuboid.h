#ifndef __CUBOID_H
#define __CUBOID_H

//为了继承Rectangle类,包含一下头文件
#include "rectangle.h"

//[1] 定义Cuboid类,以public得方式继承Rectangle类
class Cuboid : public Rectangle
{
    public :
        //Cuboid类得构造函数,使用初始化列表得方式进行初始化
        Cuboid(int len, int width, int height) : Rectangle(len, width), m_height(height){}
        int area();//面积
        int volume();//体积

    protected :
        //成员变量---->高度
        int m_height;
};

#endif