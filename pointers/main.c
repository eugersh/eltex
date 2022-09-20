#include <stdio.h>

int main() {
    /*1-ое задание*/
    printf("Первое задание\n");

    int n = 0x00CCBBAA;
    printf("%X\n",n);
    char *pointer = (char*)&n;
    for (int i=0; i<sizeof(n);i++){
        printf("%d byte is %X\n",i+1,*pointer&0xff);
        if (2==i) *pointer=*pointer+2;
        if (3==i) *pointer=0xFA;
        pointer++;
    }
    printf("%X\n",n);



    /* 2-ое задание */
    printf("\n\nВторое задание\n");
    char a = 'A';
    char b = 'B';
    char *ptr1;
    char **ptr2;

    ptr1=&a;

    printf("Variable 'char a' has VALUE '%c' with ADDRESS %p\n",a, &a);
    printf("*ptr1 pointer dereferencing '%c' with ADDRESS %p\n",*ptr1, &ptr1);

    ptr2=&ptr1;

    printf("*ptr2 pointer dereferencing '%p' (address of 'char a' through value ptr1) with ADDRESS %p\n",*ptr2, &ptr2);
    printf("**ptr2 pointer dereferencing '%c' (value of 'char a')\n",**ptr2);
    printf("\n");

    /* Changing value of ptr1 by changing ptr2*/

    *ptr2=&b;

    printf("Variable 'char b' has VALUE '%c' with ADDRESS %p\n",b, &b);
    printf("*ptr1 pointer dereferencing '%c' with ADDRESS %p\n",*ptr1, &ptr1);
    printf("*ptr2 pointer dereferencing '%p' (address of 'char b' through value ptr1) with ADDRESS %p\n",*ptr2, &ptr2);




    return 0;
}
