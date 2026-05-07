#ifndef __LINE_H
#define __LINE_H

//[1]定义线类
class Line
{
    public :
        //线类构造函数,使用初始化列表的方式进行初始化
        Line(int len) : m_len(len){}
        //纯虚函数
        virtual int area() = 0;//面积
        virtual int volume() = 0;//体积

    protected :
        //成员变量---->线的长度
        int m_len;
};

#endif