#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Телефонная книга с неболее чем 10-значными номерами и только именем (без фамилии)
 * 1. Находит номер по имени
 * 2. Печатает все контакты
 * 3. Добавляет контакт в книгу
 * 4. Удаляет контакт из книги
 * 5. Выходит из программы
 */

int N = 0; //инициализация кол-ва записей в книге

typedef struct book{
    char name[20];
    int number;
} book;

void find_number(book*);

void print_all_contacts(book*);

void add_contact(book*);

void delete_contact(book*);


int main() {

    //book bk[N];

    book *first_contact = NULL;

    //memset(bk,0,N* sizeof(book));
    int choise;

    while (1) {
        printf("Choose a number:\n1.Find a number (by name)\n2.Print the whole list\n3.Add a contact\n4.Delete a contact\n5.Exit\n");
        scanf("%d",&choise);
        switch (choise) {
            case 1:
                find_number(first_contact);
                break;
            case 2:
                printf("All contacts below:\n");
                print_all_contacts(first_contact);
                break;
            case 3:
                N++;
                first_contact = realloc(first_contact,N* sizeof(book));
                add_contact(&first_contact[N-1]);
                break;
            case 4:
                delete_contact(first_contact);
                N--;
                first_contact = realloc(first_contact,N* sizeof(book));
                break;
            case 5:
                printf("Good day!!!\n");
                exit(0);
        }
    }
    free(first_contact);

    return 0;
}

void find_number(book *node){
    char name[20];
    memset(name,0,sizeof(name));
    printf("Enter a name to be found\n");
    scanf("%19s",name);


    for (int i=0; i<N; i++){
        if (!strcmp(name,node[i].name)){
            printf("_____________________\n");
            printf("Name: %s  Number: %d\n",node[i].name, node[i].number);
            printf("_____________________\n");
            return;
        }
    }
    printf("_____________________\n");
    printf("No contact found\n");
    printf("_____________________\n");
}

void print_all_contacts(book *node) {
    if (N > 0) {
        for (int i = 0; i < N; i++) {
            printf("_____________________\n");
            printf("Name: %s  Number: %d\n", node[i].name, node[i].number);
            printf("_____________________\n");
        }
    }
    else {
        printf("_____________________\n");
        printf("List is empty\n");
        printf("_____________________\n");
    }
}

void add_contact(book *node){

    printf("Please write a name to be written\n");
    scanf("%19s",node->name);
    printf("Please write a number to be written\n");
    scanf("%d",&node->number);

}

void delete_contact(book *node){
    printf("Enter a name to delete\n");
    char delete_name[20];
    memset(delete_name,0,sizeof(delete_name));
    scanf("%s",delete_name);
    for (int i=0; i<N; i++){
        if (!strcmp(delete_name,node[i].name)){
            strcpy(node[i].name, node[N-1].name);   //swap позиции где найдено соответствие с последней
            node[i].number=node[N-1].number;
            printf("Card phone deleted\n\n");
            return;
        }
    }
    printf("Name is not found\n\n");
}