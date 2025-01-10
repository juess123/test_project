//针对64位linux操作系统，long只有在linux上是8字节
#include <stdio.h>
int My_strlen(const char* str)
{
    const char *p,*p3;
    long* p2;
    long himagic=0x808080808080;
    long lomagic=0x010101010101;
    long data;
    for(p=str;((long)p & 7)!=0;++p)
    {
        if(*p==0)
        {
            return p-str;
        }
    }
    p2=(long*)p;
    while(1)
    {
        data=*p2++;
        if(((data-lomagic) & ~data & himagic)!=0)break;
    }
    p3=(const char*)(p2-1);
    for(int i=0;i<8;i++)
    {
        if(p3[i]==0)return p3-str+i;
    }
    return -1;
}
int main()
{
    const char* a="hahah";
    int c=My_strlen(a);
    printf("%d",c);
    return 0;
}
