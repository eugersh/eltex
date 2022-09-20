#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define CONSUMERS 3
#define SHOPS 5

int shops[SHOPS];

typedef struct consumption{
    int index;
    int consumption;
}consumption;

pthread_mutex_t shop_lock[SHOPS];


void *attend_shop(void *varg){
    consumption *con_f = (consumption *)varg;
    int bought_products;

    while (con_f->consumption>0) {
        for (int i = 0; i < SHOPS; i++) {
            if(con_f->consumption <= 0) {
                break;
            }
            if(pthread_mutex_trylock(&shop_lock[i])) continue;
            /* locked */
            if(shops[i]<=0){
                printf("Dear consumer #%d, shop #%d is empty, please check next\n",\
                (con_f->index)+1,i+1);
                /* 1sec to check shop is empty or not instead of whole 2sec-shopping */
                sleep(1);
                /* if shop is empty, go to next and unlock current shop */
                pthread_mutex_unlock(&shop_lock[i]);
                continue;
            }
            bought_products = (rand() % (550 - 450 + 1) + 450);
            /* condition to not overbuy */
            if (bought_products > shops[i]) bought_products = shops[i];
            if (bought_products > con_f->consumption) bought_products = con_f->consumption;
            con_f->consumption -= bought_products;
            shops[i] -= bought_products;
            sleep(2);
            pthread_mutex_unlock(&shop_lock[i]);
            /* unlocked */


            printf("Consumer #%d bought %d products and still need %d products. Shop #%d has %d products\n", \
            (con_f->index)+1, bought_products, con_f->consumption, i+1, shops[i]);
        }
    }
    printf("Consumer #%d finished\n",(con_f->index)+1);
    free(con_f);
}


void *loader(){

    while(1) {
        for (int i = 0; i < SHOPS; i++) {
            if(pthread_mutex_trylock(&shop_lock[i])) continue;
            shops[i]+=500;                 //load products into shop
            sleep(1);
            pthread_mutex_unlock(&shop_lock[i]);
            printf("Delivered to shop #%d\tNow it has %d products\n", i+1, shops[i]);

        }
    }
}


int main() {
    pthread_t pt[CONSUMERS];
    pthread_t pt_loader;
    consumption *con;

    /* Values initialization of consuming need and starting products */
    for (int i = 0; i < SHOPS; i++) {
        shops[i] = (rand() % (1050 - 950 + 1) + 950);
        printf("Start value of products in shop #%d : %d\n",i+1, shops[i]);
        pthread_mutex_init(&shop_lock[i], 0);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        con = malloc(sizeof(consumption));
        con->index=i;
        con->consumption = (rand() % (6000 - 4000 + 1) + 4000);
        pthread_create(&pt[i], NULL, attend_shop, (void *) con);
    }

    /* initialization end */

    pthread_create(&pt_loader, NULL, loader, NULL);

    /* join/destroy part */
    for (int i = 0; i < CONSUMERS; i++) {
        pthread_join(pt[i], NULL);
    }
    for (int i=0;i<SHOPS;i++){
        pthread_mutex_destroy(&shop_lock[i]);
    }

    printf("\nConsumers satisfied\n");
    exit(EXIT_SUCCESS);
}

