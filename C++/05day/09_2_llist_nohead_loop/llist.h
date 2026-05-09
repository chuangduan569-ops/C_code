#ifndef __LLIST_H
#define __LLIST_H

#include <iostream>

using namespace std;//打开命名空间

//需要加模板头(T是数据的类型[通用型])
template <typename T>
class ListNode//构建节点的类
{
    public :
        //构造函数
        //C++中的nullptr其实就是C语言中的NULL
        ListNode(const T &data) : _data(data), _prev(nullptr), _next(nullptr) { }
        //由于_prev指针和_next指针指向的都是对象,不需要开辟存储空间
        //所以节点的类不需要析构函数
        //打印节点数据的成员函数
        void printNode() const { cout << _data; }
    //private :
        /*
        因为插入数据的时候List类需要访问ListNode类的成员,所以需要使用public修饰
        或者在ListNode类中提供两个方法,获取前驱节点对象的地址和后继节点对象的地址
        */
    public :
        //成员变量
        ListNode<T> *_prev;//指向前驱节点对象的指针
        ListNode<T> *_next;//指向后继节点对象的指针
        T _data;//数据域
};

/*
除了节点的类,还需要链表的类
链表里面都是由节点组合而成的,整个链表只需要知道第一个节点的地址就可以管理整个链表了
*/
///如果怕自己创建的链表类和C++的STL模板库的链表类重名,可以限定在自己的命名空间中
namespace ML
{
    template <typename T>
    class List
    {
        public :
            //构造函数
            List() : _head(nullptr) { }
            //析构函数  (将来再写 先写个空)
            ~List();
            //头插法
            int headInsert(const T &d);
            //尾插法
            int tailInsert(const T &d);
            //遍历
            void display() const;
            //查找 : 返回首个匹配要查找的数据的节点对象的地址;如果没有找到返回nullprt
            ListNode<T> *find(const T &find_data);
            //删除
            int remove(const T &find_data);
            //拿出
            int fetch(const T &find_data, T &out_data);
        private :
            //成员变量
            //指向第一个数据节点对象的指针
            ListNode<T> *_head;
    };
}

using namespace ML;//打开命名空间

//需要加模板头(头插)
template <typename T>
int List<T>::headInsert(const T &d)
{
    ListNode<T> *newnode = new ListNode<T>(d);//创建新的节点对象(并且数据存储成功)

    //把新创建的节点插入到头指针指向的节点之前
    if(_head == nullptr)//判断头指针是否指向空(证明现在插入的节点是第一个节点)
    {
        newnode->_prev = newnode;//前驱指针指向自己
        newnode->_next = newnode;//后继指针指向自己
    }
    else//如果链表中已有节点(证明新创建的节点要插入到第一个节点的位置)
    {
        newnode->_next = _head;
        newnode->_prev = _head->_prev;
        newnode->_prev->_next = newnode;
        newnode->_next->_prev = newnode;
    }
    //无论是if里面的操作还是else里面的操作,整个链表都需要_head来访问,更新_head
    _head = newnode;//更新_head(让_head指针指向第一个节点对象)
    return 0;
}

//需要加模板头(尾插)
template <typename T>
int List<T>::tailInsert(const T &d)
{
    ListNode<T> *newnode = new ListNode<T>(d);//创建新的节点对象(并且数据存储成功)

    //把新创建的节点插入到链表最后一个节点对象的下一个位置
    if(_head == nullptr)//判断头指针是否指向空(证明现在插入的节点是第一个节点)
    {
        newnode->_prev = newnode;//前驱指针指向自己
        newnode->_next = newnode;//后继指针指向自己
        _head = newnode;//更新_head(让_head指针指向第一个节点对象)
    }
    else//如果链表中已有节点(证明新创建的节点要插入到最后一个的位置)
    {
        newnode->_next = _head;
        newnode->_prev = _head->_prev;
        newnode->_prev->_next = newnode;
        newnode->_next->_prev = newnode;
    }
    return 0;
}

//需要模板头(遍历)
template <typename T>
void List<T>::display() const
{
    ListNode<T> *cur;//cur指针指向链表每一个节点数据
    for (cur = _head; cur->_next != _head; cur = cur->_next)//循环遍历
    {
        cur->printNode();//打印当前节点对象的数据
        cout << " ";
    }
    cur->printNode();//打印链表中最后一个节点的数据
    cout << endl;
}

//需要模板头(析构函数)
template <typename T>
List<T>::~List()
{
    ListNode<T> *cur = _head;//cur指针指向链表每一个节点数据
    //cout << "---------------------------" << endl;
    while(cur->_next != _head)//循环遍历
    {
    	display();//遍历链表(展示释放过程)
        _head = cur->_next;//_head指针指向当前节点(要释放节点对象)
        //display();//遍历链表(展示释放过程)
        cur->_next->_prev = cur->_prev;
        cur->_prev->_next = cur->_next;
        delete cur;
        //display();//遍历链表(展示释放过程)
        cur = _head;
    }
    display();//遍历链表(展示释放过程)
    delete cur;
    //display();//遍历链表(展示释放过程)
    _head = cur = nullptr;
    cout << "---------------------------" << endl;
}

//需要加模板头(查找)
template <typename T>
ListNode<T> *List<T>::find(const T &find_data)
{
    ListNode<T> *cur = _head;//cur指针指向找到的节点对象
    if (_head == nullptr)//判断是否空链表
    {
        return nullptr;//由于空链表,结束函数,返回nullptr
    }
    do
    {
        if (cur->_data == find_data)
            return cur;//返回cur指针指向的节点对象
        cur = cur->_next;//向下一个移动
    } while (cur != _head);
    return nullptr;//没有找到要查找的额节点对象,返回nullptr
}

//需要加模板头(删除)
template <typename T>
int List<T>::remove(const T &find_data)
{
    ListNode<T> *cur = find(find_data);//调用类内部的成员函数查找要删除的节点

    if (cur == nullptr)//判断是否没有找到要删除的节点
        return -1;//没有找到要删除的节点
    if(cur->_next == cur)//判断链表中是否只有一个节点(并且是要删除的节点)
    {
        delete cur;//释放要删除的节点对象的动态存储空间
        _head = cur = nullptr;//避免出现野指针
    }
    else//链表中存在着多个节点
    {
        cur->_next->_prev = cur->_prev;
        cur->_prev->_next = cur->_next;
        if (cur == _head)//判断要删除的节点是否是第一个节点对象
            _head = cur->_next;//让_head指针指向新的位置
        delete cur;//释放要删除的节点对象的动态存储空间
    }
    return 0;
}

//需要加模板头(取出)
template <typename T>
int List<T>::fetch(const T &find_data, T &out_data)
{
    ListNode<T> *cur = find(find_data);//调用类内部的成员函数查找要取出的节点

    if (cur == nullptr)//判断是否没有找到要取出的节点
        return -1;//没有找到要取出的节点

    out_data = cur->_data;
    if(cur->_next == cur)//判断链表中是否只有一个节点(并且是要取出的节点)
    {
        delete cur;//释放要取出的节点对象的动态存储空间
        _head = cur = nullptr;//避免出现野指针
    }
    else//链表中存在着多个节点
    {
        cur->_next->_prev = cur->_prev;
        cur->_prev->_next = cur->_next;
        if (cur == _head)//判断要取出的节点是否是第一个节点对象
            _head = cur->_next;//让_head指针指向新的位置
        delete cur;//释放要取出的节点对象的动态存储空间
    }
    return 0;
}


#endif






