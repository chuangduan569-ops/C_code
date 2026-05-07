#ifndef __STUDENT_H
#define __STUDENT_H

//前置声明
class Address;


//[1]抽象类
class Student
{
    //[2]把成员变量的访问设置为private
    private :
        //[3]定义成员变量->类的属性
        int m_age;
        const char *m_name;
        //静态的成员变量,统计有多少个学生对象
        //无论是哪种构造函数调用了,都要进行计数变量的自增
        static int sum;
    //[4]把成员函数的访问设置为public
    public :
        //[4]定义成员函数->类的行
        //Student(void);//默认构造函数(没有参数的构造函数)
        Student(int age = 19, const char *name = "侯佳兴")//默认构造函数(全缺省参数)
        {
            m_age = age;
            m_name = name;
            sum++;
        }
        //初始化列表的方式
        Student(const char *name, int age) : m_name(name), m_age(age)
        {
            sum++;
        }
        //拷贝构造函数
        Student(const Student &s)
        {
            m_name = s.m_name;
            m_age = s.m_age;
            sum++;
        }
        void show();
        void setName(const char *name);
        void setAge(int age);
        int getAge() const;
        const char *getName() const;
        //前面的const修饰的是返回值,后面的const表示对于成员变量的值只读

        //可以使用静态的成员函数来访问静态的成员变量
        static int getSum();

        //普通函数 并非类的成员函数
        friend void printObj(const Student *p);

        //访问学生对象的地址(因为这个函数是Student类的成员函数,并不是Address类的成员函数,所以需要传参 传的是类的地址或者引用)
        //由于Address类在后面定义的 所以需要在开头进行前置声明
        //而且因为只是加了前置声明,类的定义不完整,所以不能在这里展开实现函数功能
        void showAddress(Address &addr);

};

//地址类
class Address
{
    public :
        //构造函数(使用初始化列表的方式进行初始化)
        Address(const char *p, const char *c, const char *r) : m_provinces(p), m_city(c), m_region(r){};
        //将Student类的成员函数showAddress声明为Address的友元函数
        //friend void Student::showAddress(Address &addr);
        //将Student类z作为Adress类的友元类
        friend class Student;
    
    
        private :
        //成员变量
        const char *m_provinces;//省份
        const char *m_city;//城市
        const char *m_region;//区域
};



#endif


