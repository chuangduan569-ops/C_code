#ifndef __VLA_H
#define __VLA_H

#include <iostream>
#include <cstring>

using namespace std;//打开命名空间

//加模板头
template <typename T>
class VLA
{   
    public :
        //全缺省版本的构造函数
        VLA(int len = 0);
        //显示定义拷贝构造函数[深拷贝]
        VLA(const VLA<T> &v);
        /*注意 : 需要显示定义析构函数
        因为编译器默认生成的析构函数只是销毁对象,而不会释放开辟的动态存储空间*/
        ~VLA();
        /*赋值运算符的重载
        其实以后只要涉及到类中存在指针以及动态开辟存储空间,
        那么拷贝构造函数以及赋值运算符的重载都需要手动实现,因为需要深拷贝*/
        VLA<T> &operator=(const VLA<T> &v);
        //下标运算符的重载
        T &operator[](int i);
        //返回当前对象的成员个数
        int length() const {return m_len;}
        //给当前对象的最后一个成员位置再添加一个数据
        void push_back(const T &value);
    private :
        //成员变量
        T *m_a;//指向开辟空间的起始地址
        //(在创建VLA类的时候使用的是通用类型,只有在实例化对象时才确定是什么类型)
        int m_len;//可变长数组的成员个数
};

//需要加模板头(全缺省版本的构造函数)
template <typename T>
VLA<T>::VLA(int len)
{   
    m_len = len;//把形参len的值存储到当前对象的m_len成员变量中
    if(m_len > 0)//判断m_len是否合法
        m_a = new T[m_len];//动态开辟m_len个成员的空间
    else
        m_a = NULL;//不开辟空间(设置为空指针)
}

//需要加模板头(拷贝构造函数[深拷贝])
template <typename T>
VLA<T>::VLA(const VLA<T> &v)
{   
    //深拷贝
    m_len = v.m_len;//把v对象的成员个数存储到当前对象的成员个数中
    m_a = new T[m_len];//动态开辟m_len个成员的空间
    memcpy(m_a, v.m_a, sizeof(T) * m_len);//把v对象的数据拷贝到当前对象中
}

//需要加模板头(析构函数)
template <typename T>
VLA<T>::~VLA()
{   
    delete[] m_a;//释放当前对象的动态存储空间
    m_a = NULL;//避免产生野指针,设置为空指针
    m_len = 0;//清空可变长数组的成员个数
}

//需要加模板头(赋值运算符的重载--->类似于拷贝构造函数)
template <typename T>
VLA<T> &VLA<T>::operator=(const VLA<T> &v)
{
    if(m_len < v.m_len)//判断当前对象的存储空间是否少于v对象的存储空间
    {
        if(m_len > 0)//判断当前对象的存储空间是否大于0
            delete[] m_a;//释放掉当前对象的存储空间
        m_len = v.m_len;//把v对象的成员个数存储到当前对象的成员个数中
        m_a = new T[m_len];//动态开辟m_len个成员的空间
    }
    memcpy(m_a, v.m_a, sizeof(T) * m_len);//把v对象的数据拷贝到当前对象中

    return *this;//返回当前对象
}

//需要加模板头(下标运算符的重载)
template <typename T>
T &VLA<T>::operator[](int i)
{
    return m_a[i];//返回下标为i的成员的引用
}

//需要加模板头(扩容)
template <typename T>
void VLA<T>::push_back(const T &value)
{
    T *tmp;//备份指针


    tmp = m_a;//把当前对象动态存储空间的额首地址赋值给备份指针
    m_a = new T(m_len + 1);//开辟新的动态存储空间(原有的动态存储空间 + 1)
    memcpy(m_a, tmp, sizeof(T) * m_len);//把原数据拷贝到新的动态存储空间
    m_a[m_len] = value;//把要插入的新数据存放到新的动态存储空间
    m_len++;//更新当前对象的成员个数
    delete[] tmp;//把原空间释放掉
    tmp = NULL;//避免出现野指针
}

#endif

