#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  long diff,size = 8;
  char *buf1;
  char *buf2;

  buf1 = (char *)malloc(size);
  buf2 = (char *)malloc(size);
  if(buf1 == NULL || buf2 == NULL)
  {
	perror("malloc");
	exit(-1);
  }

  diff =  (long)buf2 -  (long)buf1;
  printf("buf1 = %p & buf2 = %p & diff %d\n",buf1,buf2,diff);
  memset(buf2,'2',size);
  printf("BEFORE: buf2 = %s\n",buf2);
  memset(buf1,'1',diff+3);  /* We overwrite 3 chars */
  printf("AFTER:  buf2 = %s\n",buf2);

  return 0;
}