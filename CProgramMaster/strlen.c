#include <stdio.h>
int MyStrlen(const char* str)
{
    const char *p1,*p3;
    long int* p2;
    long int himagic=0x808080808080;
    long int lomagic=0x010101010101;
    long int data;
    for(p1=str;((long)p1 & 7)!=0;++p1)
    {
        if(*p1==0)
        {
            return p1-str;
        }
    }
    p2=(long int*)p1;
    while(1)
    {
        data=*p2++;
        if(((data-lomagic) & ~data & himagic)!=0)break;
    }
    p3=(const char*)(p2-1);
    for(int i=0;i<8;++i)
    {
        if(p3[i]==0)return p3-str+i;
    }
    return -1;
}
int main()
{
    const char* a="cd /home/scout/Documents/C_Pram/algorithm/strlen";
    int c=MyStrlen(a);
    printf("%d",c);
    return 0;
}
//glibc库函数 处理的思想 就是 内存地址对齐 然后多字节比较。25-4-22
