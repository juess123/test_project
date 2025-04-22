#include <stdio.h>
int MyStrcmp (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;

  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
	return c1 - c2;
    }
  while (c1 == c2);

  return c1 - c2;
}
int main()
{
  char* p1="hello";
  char* p2="hello";
  int a=MyStrcmp(p1,p2);
  printf("%d\n",a);
  return 0;
}
//没啥好说的的  依次比较两个字符 25-4-22
