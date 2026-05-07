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
		//Student(void);//默认构造函数(没有参数的构造函数)ASDFGHJKL
		Student(int age = 19, const char *name = "段闯")//构造函数 全缺省参数
		{

			m_age = age;
			m_name = name;
		}
		//初始化列尔表的方式
		Student(const char *name, int age) : m_name(name), m_age(age)
        {
        }
		//拷贝构造函数
		Student(const Student &s)
		{

			m_name = s.m_name;
			m_age = s.m_age;
		}


		/*
		Student(const char *name, int age)
		{
			m_age = age;
			m_name = name;
		}
		*/
    	void show();
    	void setName(const char *name);
    	void setAge(int age);
};
#endif