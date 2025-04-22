#include <stdio.h>
void *MyMemeset(void* dstpp,int c,size_t len)
{
    long int dstp=(long int)dstpp;
    if(len>=8)
    {
        size_t xlen;
        unsigned long int cccc;
        cccc=(unsigned char)c;
        cccc|=cccc<<8;
        cccc|=cccc<<16;
        cccc|=cccc<<32;
        while(dstp%8 != 0)
        {
            ((unsigned char*)dstp)[0]=c;
            dstp+=1;
            len-=1;
        }
        xlen=len/64;
        while(xlen>0)
        {
            ((long int*)dstp)[0]=cccc;
            ((long int*)dstp)[1]=cccc;
            ((long int*)dstp)[2]=cccc;
            ((long int*)dstp)[3]=cccc;
            ((long int*)dstp)[4]=cccc;
            ((long int*)dstp)[5]=cccc;
            ((long int*)dstp)[6]=cccc;
            ((long int*)dstp)[7]=cccc;
            dstp+=64;
            xlen-=1;
        }
        xlen=len/8;
        while(xlen>0)
        {
            ((long int*)dstp)[0]=cccc;
            dstp+=8;
            xlen-=1;
        }
    }
    len%=8;
    while(len)
    {
        ((unsigned char*)dstp)[0]=c;
        dstp+=1;
        len-=1;
    }
    return dstpp;
}
int main()
{
    char p[512]={0};
    MyMemeset(p,'A',181);
    printf("%s\n",p);
    return 0;
}
//模仿glibc的memset函数来写的函数
//核心思想是地址转整数 然后对齐地址 然后大块赋值 然后 8字节赋值 然后单字节赋值 
//相比于一个一个字节赋值  速度上会快很多25-4-22
