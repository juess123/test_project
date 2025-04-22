#include <stdio.h>
#include <string.h>
#define BYTE_COPY(dstp,srcp,nbytes)                            \
    do                                                      \
    {                                                       \
        size_t __nbytes=(nbytes);                      \
        while(__nbytes>0)                                     \
        {                                                   \
            unsigned char temp=((unsigned char*)srcp)[0];   \
            srcp+=1;                                        \
            ((unsigned char*)dstp)[0]=temp;                 \
            dstp+=1;                                        \
            __nbytes-=1;                                      \
        }                                                   \
    } while(0)                                              

  
#define WORD_COPY(dstp,srcp,nwords)                                     \
    do                                                              \
    {                                                               \
        size_t __nwords=nwords;                              \
        while(__nwords>0)                                             \
        {                                                           \
            unsigned long int temp=((unsigned long int*)srcp)[0]; \
            srcp+=8;                                                \
            ((unsigned long int*)dstp)[0]=temp;                    \
            dstp+=8;                                                \
            __nwords-=1;                                              \
        }                                                           \
    } while(0)                                                      

void* MyMemcpy(void* dstpp,const void* srcpp,size_t len)
{
    unsigned long int dstp=(long int)dstpp;
    unsigned long int srcp=(long int)srcpp;
    if(len>16)
    {
        
        BYTE_COPY(dstp,srcp,(-dstp)%8);
        len-=(-dstp)%8;
        WORD_COPY(dstp,srcp,(len/8));
        len=len%8;
    }
    BYTE_COPY(dstp,srcp,len);
    return dstpp;
}
int main()
{
    const char* p1="hellloc:Usersscoutvscodeextensionsms-vscode.cebugAdaptersbinindowsDebugLauncher.exe";

    char p2[100]={0};
    printf("%p,%p\n",p1,p2);

    char*p3=(char*)MyMemcpy(p2,p1,100);

    printf("%s\n",p2);
    return 0;
}
//模仿glibc官方的内存复制函数 来实现的，核心思想就是 尽量使用最大字节来复制数据 提高效率
//如果逐字节复制需要复制100次 如果使用内存对齐 首先对齐内容，循环最差8次 然后100/8次循环 最后补几次循环
//逐字节（1 字节）拷贝	     100次循环      	循环次数多，慢
//对齐复制 未对齐部分逐字节，主部分按 8 字节	~13 次循环	循环少，大块复制快
