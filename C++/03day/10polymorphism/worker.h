#ifndef __WORKER_H
#define __WORKER_H


#include "person.h"//为了导入Person类,包含person.h头文件

//[1] Worker类以public方式继承Person类
class Worker : public Person
{
    public :
        Worker(const char *name, int age, float salary);//构造函数 
        void show() const;
    private :
        float m_salary;//薪水

};


#endif