#ifndef __CUBE_H
#define __CUBE_H

//为了继承Cuboid类,包含一下"cuboid.h"的头文件
#include "cuboid.h"

//[1]定义Cube类,以public的方式继承Cuboid类
class Cube : public Cuboid
{
    public :
        //Cube类的构造函数,使用初始化列表的方式进行初始化
        Cube(int a) : Cuboid(a, a, a){ }
        //求面积和体积的函数,直接从Cuboid类中继承即可
};

#endif