#ifndef __PERSON_H
#define __PERSON_H

//[1]定义Person类
class Person
{
    //修饰类关键字
    public :
        Person(const char *name, int age);//构造函数
        void show() const;//显示属性(由于不需要修改成员变量的值 可以声明为常函数)

    //修饰类关键字
    private :
        const char *m_name;//姓名
        int m_age;//年龄
};








#endif