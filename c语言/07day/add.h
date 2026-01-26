#ifndef __ADD_H//如果没有定义过__ADD_H这个宏
#define __ADD_h//那就定义__ADD_H这个宏

int add(int,int);//在函数声明时可以省略形参的名字,但是不可以省略形参类型

#endif//结束  #ifndef 的预处理
//这三行预处理的内容是为了防止重定义
//避免头文件被重复包含

