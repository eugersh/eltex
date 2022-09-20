#include <stdio.h>
#define N 5

// Вывести квадрратную матрицу по заданному N
void first_task(){
    printf("Первое задание:\n");
    int array1[N][N];

    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            array1[i][j]=((i*N)+(j))+1;
            printf("%2d ",array1[i][j]);
        }
        printf("\n");
    }
}

//Вывести заданный массив размером N в обратном порядке.
void second_task(){
    printf("\n\nВторое задание:\n");
    int array2[N];
    printf("Ввод  -> ");
    for(int i=1; i<=N; i++){
        array2[i]=i;
        printf("%d ",array2[i]);
    }
    printf("\nВывод -> ");
    for(int i=N; i>0; i--){
        printf("%d ",array2[i]);
    }
}


//Заполнть верхний треугольник матрицы 1, а нижний 0.
void third_task(){
    printf("\n\nТретье задание:\n");
    int array3[N][N]={0};
    for (int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if (j>=N-(i+1)) {
                array3[i][j]=1;
            }
            printf("%d ",array3[i][j]);
        }
        printf("\n");
    }
}


//Заполнть матриицу числами от 1 до N^2 улиткой.
fourth_task(){
    printf("\n\nЧетвертое задание:\n");
    int array4[N][N]={0};
    int i=1,j,k;
    if (N%2==1) array4[N/2][N/2]=N*N;
    for (k=1;k<=N/2;k++){
        for (j=k-1;j<N-k+1;j++) array4[k-1][j]=i, i++;
        for (j=k;j<N-k+1;j++) array4[j][N-k]=i, i++;
        for (j=N-k-1;j>=k-1;--j) array4[N-k][j]=i, i++;
        for (j=N-k-1;j>=k;j--) array4[j][k-1]=i, i++;
    }

    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
        {
            printf("%2d ",array4[i][j]);
            if(j==N-1) printf("\n");
        }
}

int main() {

    first_task();
    second_task();
    third_task();
    fourth_task();

    return 0;
}
