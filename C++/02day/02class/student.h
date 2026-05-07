#ifndef __STUDENT_H
#define __STUDENT_H

class Student
{
    //[2]把成员变量的访问设置为private
    private :
    	//[3]定义成员变量->类的属性
    	int m_age;
    	const char *m_name;
    //[4]把成员函数的访问设置为public
    public :
    	//[4]定义成员函数->类的行为]
		Student(void);//默认构造函数(没有参数的构造函数)
    	void show();
    	void setName(const char *name);
    	void setAge(int age);
};
#endif