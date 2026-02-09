#include <pthread.h>
#include <stdio.h>

#include <stdlib.h>

#define NUM_THREADS 6

pthread_t threads[NUM_THREADS];


int amount;
// void* runner(void * param);
void* deposit(void * param);
void* withdraw(void * param);

int main (int argc, char* argv[]){
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, deposit, argv[0]);
    pthread_create(&tid, &attr, deposit, argv[0]);
    pthread_create(&tid, &attr, deposit, argv[0]);

    pthread_create(&tid, &attr, withdraw, argv[1]);
    pthread_create(&tid, &attr, withdraw, argv[1]);
    pthread_create(&tid, &attr, withdraw, argv[1]);

    int i = 0;

    for (i = 0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Final amount = %d\n", amount);

}

void * deposit (void * param){
    int i, upper = atoi(param);

    for (i = 1; i<=upper; i++){
        amount +=1;
    }
    printf("Deposit amount = %d\n", upper);
    pthread_exit(0);
}

void * withdraw (void * param){
    int i, upper = atoi(param);

    for (i = 1; i<=upper; i++){
        amount -=1;
    }
    printf("Withdraw amount = %d\n", upper);

    pthread_exit(0);
}

// void * runner (void * param){
//     int i, upper = atoi(param);
//     amount = 0;

//     for (i = 1; i<=upper; i++){
//         amount +=i;
//     }

//     pthread_exit(0);
// }
