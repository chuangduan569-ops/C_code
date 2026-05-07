#ifndef __COMPLEX_H
#define __COMPLEX_H

class Complex
{
    public :    
        Complex(float real = 0.0, float img = 0.0);//全缺省的构造函数
    private :
        float m_real;//复数的实部
        float m_img;//复数的虚部
};


#endif