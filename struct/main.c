#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 5 //Кол-во записей в книге

/* Телефонная книга с неболее чем 10-ти значными номерами и только именем (без фамилии)
 * 1. Находит номер по имени
 * 2. Печатает все контакты
 * 3. Добавляет контакт в книгу
 * 4. Удаляет контакт из книги
 * 5. Выходит из программы
 */

typedef struct book{
    char name[20];
    int number;
} book;

void find_number(book *node){
    char name[20];
    memset(name,0,sizeof(name));
    printf("Enter a name to be found\n");
    scanf("%19s",name);
    int exist_contacts=0;

    for (int i=0; i<N; i++){
        if (!strcmp(name,node[i].name)){
            exist_contacts++;
            printf("_____________________\n");
            printf("Name: %s  Number: %d\n",node[i].name, node[i].number);
            printf("_____________________\n");
        }
    }
    if (!exist_contacts) printf("No contact found\n\n");
}

void print_all_contacts(book *node){
    int exist_contacts=0;

    for (int i=0; i<N; i++){
        if (0!=node[i].number){
            exist_contacts++;
            printf("_____________________\n");
            printf("Name: %s  Number: %d\n",node[i].name, node[i].number);
            printf("_____________________\n");
        }
    }
    if (!exist_contacts) printf("List is empty\n");
    printf("Allowed to add %d more contacts\n\n",N-exist_contacts);
}

void add_contact(book *node){

    for (int i=0; i<N; i++){
        if (0 == node[i].number) {
            printf("Please write a name to be written\n");
            scanf("%19s",node[i].name);
            printf("Please write a number to be written\n");
            scanf("%d",&node[i].number);
            return;
        }
    }
    printf("no memory for adding...\n\n");
}

void delete_contact(book *node){
    printf("Enter a name to delete\n");
    char delete_name[20];
    memset(delete_name,0,sizeof(delete_name));
    scanf("%s",delete_name);
    for (int i=0; i<N; i++){
        if (!strcmp(delete_name,node[i].name)){
            node[i].number=0;
            strcpy(node[i].name,"");
            printf("Card phone deleted\n\n");
            return;
        }
    }
    printf("Name is not found\n\n");
}




int main() {

    book bk[N];
    memset(bk,0,N* sizeof(book));
    int choise;

    while (1) {
        printf("Choose a number:\n1.Find a number (by name)\n2.Print the whole list\n3.Add a contact\n4.Delete a contact\n5.Exit\n");
        scanf("%d",&choise);
        switch (choise) {
            case 1:
                find_number(&bk[0]);
                break;
            case 2:
                printf("All contacts below:\n");
                print_all_contacts(&bk[0]);
                break;
            case 3:
                add_contact(&bk[0]);
                break;
            case 4:
                delete_contact(&bk[0]);
                break;
            case 5:
                printf("Good day!!!\n");
                exit(0);
        }
    }

  return 0;
}
