# bitset

 A simple bitset implemented in c++.

## motivation:

​	For the programmer's operation requirements on the bit, the C++ library function <bitset.h> has given a more comprehensive and complete function implementation.

​	But in the process of actual use, I found that the efficiency of calling <bitset.h> library functions is not optimistic.

​	So it took me some time myself to achieve this high efficiency goal.

## Implementation

​	In actual operation, I use unsigned long type as the basic type, 4 bytes to carry 32 bits.

​	It basically implements the basic operations of the <bitset.h> library and the overloading of operators.

```c++
void set(u_long index);                  
void set(u_long index, bool tag);     
void reset();                          
void reset(u_long index);               
bool test(u_long index)const; 
```

```c++
bitmap& operator&=(const bitmap& b);      
bitmap& operator^=(const bitmap& b);    
bitmap& operator=(const bitmap& b);       
bitmap& operator <<= (u_int pos);         
bitmap& operator >>= (u_int pos);       
bitmap operator&(const bitmap& b)const;   
bitmap operator^(const bitmap& b)const;   
bool operator==(const bitmap& b)const;   
bool operator!=(const bitmap& b)const;   
bitmap operator<<(u_int pos) const;       
bitmap operator>>(u_int pos) const;       
bool operator[](u_long index)const;      

friend ostream& operator << (ostream& os, const bitmap& b);
```

## issue

​	However, in the process of running, there will always be a situation where a breakpoint is triggered at the destructor call. I'm guessing it's because of an out-of-bounds situation when allocating space or modifying values, but I haven't found a bug for a long time. Therefore, in the code I omitted the destructor to make the code work properly.

​	If anyone can solve this problem, a **PR** is also welcome.

## Efficiency

​	In the value operation, XOR operation, AND operation, SET operation than the C++ STL library function has a significant performance improvement.

​	The performance of 500,000 AND operations, XOR operations, and SET operations is as follows.

![img](D:\LearningFiles\GitHubRepos\Repos\bitset\3.png)

​	But there is still a shortcoming in the shift operation, the efficiency is lower than the STL standard library. I guess this is because of the different implementation methods. Each shift will take more time to traverse all the positions. If someone has a better implementation method, **PR** is also welcome.

![img](D:\LearningFiles\GitHubRepos\Repos\bitset\1.png)

![img](D:\LearningFiles\GitHubRepos\Repos\bitset\2.png)

