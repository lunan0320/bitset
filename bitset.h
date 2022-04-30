#include<iostream>
#include <cstdio>
#include <malloc.h>
#include <cassert>

using namespace std;

typedef unsigned long u_long;
typedef unsigned int u_int;

u_long const type_size = 8 * sizeof(u_long);

class bitmap {
public:
    //���캯������������
    bitmap();
    bitmap(u_int length);
   
    //��������
    //��λʹ�ã���begin �� end ����
    void copy2left(u_int des, u_int beg, u_int end);
    void copy2right(u_int des, u_int beg, u_int end);
    void fill(u_int beg, u_int end, bool val);//��beg��end��ֵ��Ϊval ��0����1��


    //���ܺ���
    void set(u_long index);                   //����indexλΪ1
    void set(u_long index, bool tag);         //����indexλΪtag��tag������true������false��
    void reset();                             //����ȫ��λΪ0
    void reset(u_long index);                 //����indexλΪ0
    bool test(u_long index)const;             //�ж�indexλΪ0����1��0��ʱ�򷵻�false


    //���������
    bitmap& operator&=(const bitmap& b);      //�������ش����룬������ֵ
    bitmap& operator^=(const bitmap& b);      //�������ش���򣬸�����ֵ
    bitmap& operator=(const bitmap& b);       //���ش���ֵ
    bitmap& operator <<= (u_int pos);         //���ش�����
    bitmap& operator >>= (u_int pos);         //���ش�����
    bitmap operator&(const bitmap& b)const;   //���ش�����
    bitmap operator^(const bitmap& b)const;   //���ش����
    bool operator==(const bitmap& b)const;    //�ж��������ش��Ƿ����
    bool operator!=(const bitmap& b)const;    //�ж��������ش������
    bitmap operator<<(u_int pos) const;       //���ش�����
    bitmap operator>>(u_int pos) const;       //���ش�����
    bool operator[](u_long index)const;       //ȡ�±������

    friend ostream& operator << (ostream& os, const bitmap& b);

private:
    u_int buf_size; 
    u_long* buf;
};

//���캯��
bitmap::bitmap()
{
    buf_size = type_size;
    //���ٿռ�
    buf = new u_long[buf_size / type_size];
    if (buf == NULL) {
        return;
    }
    memset(buf, 0, buf_size/8);
}
bitmap::bitmap(u_int length) {
    u_int temp = 0;
    if ((temp = length % type_size) != 0) {
        //����type_size����������ʱ��Ͳ���
        buf_size = length + type_size - temp;
    }
    else {
        buf_size = length;
    }
    //���ٿռ�
    buf = new u_long[buf_size / type_size];
    if (buf == NULL) {
        return;
    }
    memset(buf, 0, buf_size/8);
}


//��begin��end������ֵ����des��ʼ��λ��
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

//���ô�beg��end��ֵΪval��true or false��
void bitmap::fill(u_int beg, u_int end, bool val) {
    //assert(beg >= 0 && end <= buf_size);
    for (u_int i = beg; i <= end; i++) {
        set(i, val);
    }
}

//���õ�indexλΪ1
void bitmap::set(u_long index) {
    if (index > buf_size) {
        return;
    }
    //�ҵ�������һ��
    u_long* location = buf + index / type_size;
    //�ҵ��������type�еĵڼ�bit
    u_int sub_index = type_size - 1 - index % type_size;
    *location = *location | 0x1 << sub_index;
}

//���������indexλΪtag
void bitmap::set(u_long index, bool tag) {
    if (tag) {
        set(index);
    }
    else {
        reset(index);
    }
}

//�����е�λ������Ϊ0
void bitmap::reset() {
    memset(this->buf, 0, buf_size/8);
}

//���õ�indexλΪ0
void bitmap::reset(u_long index) {
    if (index > buf_size) {
        return;
    }
    u_long* location = buf + index / type_size;
    u_int sub_index = type_size - 1 - index % type_size;
    *location = *location & ~(0x1 << sub_index);
}

//�ж����bit��0����1
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

//������[]����
bool bitmap::operator[](u_long index)const {
    if (test(index)) {
        return true;
    }
    else {
        return false;
    }
}

//������=����
bitmap& bitmap::operator=(const bitmap& b) {
    if (this == &b) {
        return *this;
    }
    memcpy(buf, b.buf, buf_size/8);
    return *this;
}

//������=����
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

//������&����
bitmap bitmap::operator&(const bitmap& b)const {
    u_int b_length = buf_size;
    bitmap res(buf_size);
    res = *this;
    return res &= b;
}

//������^=����
bitmap& bitmap::operator^=(const bitmap& b) {
    u_int b_length = b.buf_size;
    bitmap res(b.buf_size);
    for (u_int i = 0; i < b_length; i++) {
        //�������
        if (test(i) != b.test(i)) {
            set(i);
        }
        else {
            reset(i);
        }
    }
    return *this;
}

//������^����
bitmap bitmap::operator^(const bitmap& b)const {
    u_int b_length = b.buf_size;
    bitmap res(b.buf_size);
    res = *this;
    return res ^= b;
}

//������==����
bool bitmap::operator==(const bitmap& b)const {
    u_int b_length = b.buf_size;
    for (u_long i = 0; i < b_length; i++) {
        if (test(i) != b.test(i)) {
            return false;
        }
    }
    return true;
}

//������!=����
bool bitmap::operator!=(const bitmap& b)const {
    u_int b_length = b.buf_size;
    for (u_int i = 0; i < b_length; i++) {
        if (test(i) == b.test(i)) {
            return false;
        }
    }
    return true;
}

//����
bitmap& bitmap::operator <<= (u_int pos) {
    if (!pos) { //����λ
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

//�������������
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

//��������
bitmap bitmap::operator<<(u_int pos) const {
    bitmap res(buf_size);
    memcpy(res.buf, buf, buf_size/8);
    res <<= pos;
    return res;
}

//��������
bitmap bitmap::operator>>(u_int pos) const {
    bitmap res(buf_size);
    memcpy(res.buf, buf, buf_size/8);
    res >>= pos;
    return res;
}


//�����������
ostream& operator<<(ostream& out, const bitmap& b) {
    u_int b_length = b.buf_size;
    for (u_int i = b_length; i > 0; ) {
        out << b.test(--i);
    }
    return out;
}