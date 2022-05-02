#include<iostream>
#include <cstdio>
#include <malloc.h>
#include <cassert>
#include <bitset>
using namespace std;

typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned long long u_ll;

u_long const type_size = 8 * sizeof(u_long);

class bitmap {
public:
    //构造函数和析构函数
    bitmap();
    bitmap(u_int length);
    bitmap(u_int length, u_ll init);            //构造函数，length是比特长度，init是初始赋值
   
    //辅助函数
    //移位使用，从begin 到 end 左移
    void copy2left(u_int des, u_int beg, u_int end);
    void copy2right(u_int des, u_int beg, u_int end);
    void fill(u_int beg, u_int end, bool val);//将beg到end的值置为val （0或者1）


    //功能函数
    void set(u_long index);                   //设置index位为1
    void set(u_long index, bool tag);         //设置index位为tag（tag可能是true可能是false）
    void reset();                             //设置全部位为0
    void reset(u_long index);                 //设置index位为0
    bool test(u_long index)const;             //判断index位为0或者1，0的时候返回false
    u_long to_u_long()const;                  //将bitmap转换为u_long的数据
    bitset<32>& to_32bitset()const;           //将bitmap转换为bitset
    

    //运算符重载
    bitmap& operator&=(const bitmap& b);      //两个比特串相与，赋给左值
    bitmap& operator^=(const bitmap& b);      //两个比特串异或，赋给左值
    bitmap& operator=(const bitmap& b);       //比特串赋值
    bitmap& operator <<= (u_int pos);         //比特串左移
    bitmap& operator >>= (u_int pos);         //比特串右移
    bitmap operator&(const bitmap& b)const;   //比特串相与
    bitmap operator^(const bitmap& b)const;   //比特串异或
    bool operator==(const bitmap& b)const;    //判断两个比特串是否相等
    bool operator!=(const bitmap& b)const;    //判断两个比特串不相等
    bitmap operator<<(u_int pos) const;       //比特串左移
    bitmap operator>>(u_int pos) const;       //比特串右移
    bool operator[](u_long index)const;       //取下标运算符

    friend ostream& operator << (ostream& os, const bitmap& b);

private:
    u_int buf_size; 
    u_long* buf;
};

//构造函数
bitmap::bitmap()
{
    buf_size = type_size;
    //开辟空间
    buf = new u_long[buf_size / type_size];
    if (buf == NULL) {
        return;
    }
    memset(buf, 0, buf_size/8);
}
bitmap::bitmap(u_int length) {
    u_int temp = 0;
    if ((temp = length % type_size) != 0) {
        //不是type_size的整数倍的时候就补齐
        buf_size = length + type_size - temp;
    }
    else {
        buf_size = length;
    }
    //开辟空间
    buf = new u_long[buf_size / type_size];
    if (buf == NULL) {
        return;
    }
    memset(buf, 0, buf_size/8);
}

bitmap::bitmap(u_int length, u_ll init) {
    u_int temp = 0;
    if ((temp = length % type_size) != 0) {
        //不是type_size的整数倍的时候就补齐
        buf_size = length + type_size - temp;
    }
    else {
        buf_size = length;
    }
    //开辟空间
    buf = new u_long[buf_size / type_size];
    if (buf == NULL) {
        return;
    }
    memset(buf, 0, buf_size / 8);
    for (u_int i = 0; i < length; i++) {
        if ((init >> i) & 1) {
            set(i);
        }
    }
}


//从begin到end的向左赋值到从des开始的位置
void bitmap::copy2left(u_int des, u_int beg, u_int end) {
  //  assert(des >= 0 && des <= beg);
    if (des == beg) {
        return;
    }
    while (beg != end) {
        set(des++, test(beg++));
    }
}

void bitmap::copy2right(u_int des, u_int beg, u_int end) {
   // assert(des >= end && des <= buf_size);
    if (end == buf_size) {
        return ;
    }
    while (end != beg) {
        set(--des, test(--end));
    }

}

//设置从beg到end的值为val（true or false）
void bitmap::fill(u_int beg, u_int end, bool val) {
    //assert(beg >= 0 && end <= buf_size);
    for (u_int i = beg; i <= end; i++) {
        set(i, val);
    }
}

//设置第index位为1
void bitmap::set(u_long index) {
    if (index > buf_size) {
        return;
    }
    //找到这是哪一个
    u_long* location = buf + index / type_size;
    //找到这是这个type中的第几bit
    u_int sub_index = type_size - 1 - index % type_size;
    *location = *location | 0x1 << sub_index;
}

//运算后，设置index位为tag
void bitmap::set(u_long index, bool tag) {
    if (tag) {
        set(index);
    }
    else {
        reset(index);
    }
}

//将所有的位都设置为0
void bitmap::reset() {
    memset(this->buf, 0, buf_size/8);
}

//设置第index位为0
void bitmap::reset(u_long index) {
    if (index > buf_size) {
        return;
    }
    u_long* location = buf + index / type_size;
    u_int sub_index = type_size - 1 - index % type_size;
    *location = *location & ~(0x1 << sub_index);
}


//判断这个bit是0或者1
bool bitmap::test(const u_long index)const {
    if (index >= buf_size) {
        return false;
    }
    u_long data = *(buf + index / type_size);
    u_int number = type_size - 1 - index % type_size;
    if ((data >> number) & 0x1) {
        return true;
    }else {
        return false; 
    }
}

//转换为u_long
u_long bitmap::to_u_long() const{
    u_long res = 0;
    for (u_int i = 0; i < buf_size; i++) {
        if (test(i)) {
            res += 1 << i;
        }
    }
    return res;
}


//将bitmap转换为bitset
bitset<32>& bitmap::to_32bitset()const {
    bitset<32> res;
    res.reset();
    for (u_int i = 0; i < 32; i++) {
        if (test(i)) {
            res.set(i);
        }
    }
    return res;
}



//操作符[]重载
bool bitmap::operator[](u_long index)const {
    if (test(index)) {
        return true;
    }
    else {
        return false;
    }
}

//操作符=重载
bitmap& bitmap::operator=(const bitmap& b) {
    if (this == &b) {
        return *this;
    }
    memcpy(buf, b.buf, buf_size/8);
    return *this;
}

//操作符=重载
bitmap& bitmap::operator&=(const bitmap& b) {
    u_int b_length = b.buf_size;
    for (u_int i = 0; i < b_length; i++) {
        if (test(i) && b.test(i)) {
            set(i);
        }
        else {
            reset(i);
        }
    }
    return *this;
}

//操作符&重载
bitmap bitmap::operator&(const bitmap& b)const {
    u_int b_length = buf_size;
    bitmap res(buf_size);
    res = *this;
    return res &= b;
}

//操作符^=重载
bitmap& bitmap::operator^=(const bitmap& b) {
    u_int b_length = b.buf_size;
    bitmap res(b.buf_size);
    for (u_int i = 0; i < b_length; i++) {
        //异或运算
        if (test(i) != b.test(i)) {
            set(i);
        }
        else {
            reset(i);
        }
    }
    return *this;
}

//操作符^重载
bitmap bitmap::operator^(const bitmap& b)const {
    u_int b_length = b.buf_size;
    bitmap res(b.buf_size);
    res = *this;
    return res ^= b;
}

//操作符==重载
bool bitmap::operator==(const bitmap& b)const {
    u_int b_length = b.buf_size;
    for (u_long i = 0; i < b_length; i++) {
        if (test(i) != b.test(i)) {
            return false;
        }
    }
    return true;
}

//操作符!=重载
bool bitmap::operator!=(const bitmap& b)const {
    u_int b_length = b.buf_size;
    for (u_int i = 0; i < b_length; i++) {
        if (test(i) == b.test(i)) {
            return false;
        }
    }
    return true;
}

//左移
bitmap& bitmap::operator <<= (u_int pos) {
    if (!pos) { //不移位
        return *this;
    }
    if (pos >= buf_size) {
        reset();
        return *this;
    }
    else {
        u_int end = buf_size - pos;
        copy2right(buf_size,0, end);
        fill(0, pos, false);
    }
    return *this;
}

//重载右移运算符
bitmap& bitmap::operator >>= (u_int pos) {
    if (!pos) {
        return *this;
    }
    if (pos >= buf_size) {
        memset(buf, 0, buf_size/8);
        return *this;
    }
    else {
        copy2left(0, pos, buf_size);
        fill(buf_size - pos, buf_size, false);
        return *this;
    }
}

//重载左移
bitmap bitmap::operator<<(u_int pos) const {
    bitmap res(buf_size);
    memcpy(res.buf, buf, buf_size/8);
    res <<= pos;
    return res;
}

//重载右移
bitmap bitmap::operator>>(u_int pos) const {
    bitmap res(buf_size);
    memcpy(res.buf, buf, buf_size/8);
    res >>= pos;
    return res;
}


//输出符号重载
ostream& operator<<(ostream& out, const bitmap& b) {
    u_int b_length = b.buf_size;
    for (u_int i = b_length; i > 0; ) {
        out << b.test(--i);
    }
    return out;
}