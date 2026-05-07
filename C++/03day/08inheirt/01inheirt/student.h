#ifndef __STUDENT_H
#define __STUDENT_H

#include "person.h"//由于需要继承Person类,先包含一个头文件

//[1] Student类以public的方式继承Person类
class Student : public Person
{
    public : 
        //Student是Person类的派生类,拥有Person类的成员变量和成员函数
        //由于Person类中的m_name 和m_age都是private修饰,派生类中不可见
        //可以通过调用基类的构造函数进行初始化
        Student(const char *name, int age, float score);//构造函数
        void print() const;//展示属性
    private :
        float m_score;//成绩
};




#endif