#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "headers.h"


void print_menu();

int main() {

    void *lib_d;
    lib_d = dlopen("./libdyn.so", RTLD_LAZY);
    if(!lib_d){
        fprintf(stderr, "dlopen() error: %s\n", dlerror());
        return 1;
    }

    float n1=0, n2=0;
    float (*func_ptr)(float, float);

    int choice;
    float result=0;

    while(1){
        print_menu();
        scanf("%d",&choice);

        switch (choice) {
            case 1:
                printf("Please choose 1st and 2nd number\n");
                scanf("%f %f",&n1, &n2);
                func_ptr=(float (*)(float, float)) dlsym(lib_d, "addition");
                result=func_ptr(n1,n2);

                break;
            case 2:
                printf("Please choose 1st and 2nd number\n");
                scanf("%f %f",&n1, &n2);
                func_ptr=(float (*)(float, float)) dlsym(lib_d, "subtraction");
                result=func_ptr(n1,n2);
                break;
            case 3:
                printf("Please choose 1st and 2nd number\n");
                scanf("%f %f",&n1, &n2);
                func_ptr=(float (*)(float, float)) dlsym(lib_d, "multiplexing");
                result=func_ptr(n1,n2);
                break;
            case 4:
                printf("Please choose 1st and 2nd number\n");
                scanf("%f %f",&n1, &n2);
                func_ptr=(float (*)(float, float)) dlsym(lib_d, "dividing");
                result=func_ptr(n1,n2);
                break;
            default:
                exit(0);
        }
        printf("%.2f\n",result);

    }
    dlclose(lib_d);

    return 0;
}


void print_menu(){
    printf("_____\nMENU:\n1. Addition\n2. Subtraction\n3. Multiplexing\n4. Dividing\n5. Exit\n_____\n");
}



