#include <stdio.h>
#include <string.h>
char* MyStrcpy(char *dest, const char *src)
{
  return memcpy (dest, src, strlen (src) + 1);
}
int main()
{
  char a[20]="hello,worlddasas";
  char b[15]={0};
  //char c[10]="value";
  MyStrcpy(b,a);
  //printf("%p,%p,%p,%p,%p,%p\n",a,b,c,&a[19],&b[1],&c[9]);
  printf("%s\n%s\n",b,a);
}
//在使用复制字符的函数的时候 会出现复制字符溢出的风险
//在关闭 栈保护 ，位置无关可执行文件（PIE）后 -fno-stack-protector和-no-pie后,可以观察到变量在内存的具体分布情况
