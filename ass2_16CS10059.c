#include "myl.h"
#define MAX_INT_SIZE 20

int printStr(char *s)
{
    int bytes=0;
    while(s[bytes]!='\0')bytes++;
    __asm__ __volatile__ (
        "movl $1, %%eax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall \n\t"
        :
        :"S"(s), "d"(bytes)
    );
    return bytes;
}


int printInt(int n)
{
    char buff[MAX_INT_SIZE],zero='0';
    int i=0,j=0,bytes,k;
    if(n<0){n=-n;buff[i++]='-';}
    if(n==0)buff[i++]=zero;
    while(n!=0){
        buff[i++]= (char)(n%10 + zero);
        n=n/10;
    }
    if(buff[0]=='-')j=1;
    k=i-1;
    bytes=i;
    while(j<k){
        char tmp;
        tmp=buff[j];
        buff[j]=buff[k];
        buff[k]=tmp;
        j++;k--;
    }
  __asm__ __volatile__ (
        "movl $1, %%eax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall \n\t"
        :
        :"S"(buff), "d"(bytes)
    );
	return bytes;
}

int readInt (int* eP) {
    *eP=OK;
    char buff[MAX_INT_SIZE];

    __asm__ __volatile__ (
        "movl $0, %%eax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall \n\t"
        :
        :"S"(buff), "d"(MAX_INT_SIZE)       
        );

    int len=0,num=0,i;
    while(buff[len] != '\n' && buff[len] != '.')len++;
    buff[len]='\0';
    buff[len]='\0';

    i = 0 + (buff[0]=='-' || buff[0]=='+') ;
    if((buff[0] == '-' || buff[0] == '+') && buff[1] == '\0'){
        num = 0;
        *eP=ERR;
    }

    for(;i<len;){
        if(buff[i] >= '0' && buff[i] <= '9'){
        num=num*10+(buff[i++]-'0');
        }
        else{
        num = 0;
        *eP=ERR;
        }
    }

    if(buff[0] == '-')
        num=-num;

    return num;
}
