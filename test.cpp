#pragma warning(disable:4996)  
#include <iostream>
#include <malloc.h>
#include <stdexcept>
#include <ctime>
#include <bitset>
#include "bitset.h"
#include <sys/timeb.h>

using namespace std;

typedef unsigned int u_int;

const u_int init_length = 32;
const u_int cycles = 50000;

long getSystemTime() {
    timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
};

int main() {
 /*   bitmap a(32);
    a.set(10);
    a.set(1);
    a.set(4);
    a.set(30);
    cout << a << endl;
    a = a << 5;
    cout <<a << endl;
    bitmap c(32);
    c = a >> 8;
    cout << c << endl;*/

 
     
    /* ²âÊÔ´úÂë  */
    time_t t1;
    time(&t1);
    long tb1 = getSystemTime();
    cout << "time: " << ctime(&t1) << endl;
    cout << "...  my bitset  ... " << endl;
    bitmap b1(init_length);
    for (u_int t = 0; t < cycles; t++) {
        for (u_int i = 0; i < init_length; i++) {
            //b1 <<= 1;
     
            b1.set(i, b1[20] ^ b1[25]);
            b1.set(i, b1[20] & b1[30]);
        }
    }

    time_t t2;
    time(&t2);
    long tb2 = getSystemTime();
    cout << "time: " << ctime(&t2) << endl;
    cout << "...  c++ library bitset  ... " << endl;
    bitset<init_length> b2 ;
    for (u_int t = 0; t < cycles; t++) {
        for (u_int i = 0; i < init_length; i++) {
           // b2 <<= 1;
            b2[i] = b2[20] ^ b2[25];
            b2[i] = b2[20] & b2[30];
        }
    }

    time_t t3;
    time(&t3);
    long tb3 = getSystemTime();
    cout << "time: " << ctime(&t3) << endl;
    cout << "my bitset: " << tb2 - tb1 << "ms" << endl;
    cout << "c++ bitset: " << tb3 - tb2 << "ms" << endl;

    return 0;
}