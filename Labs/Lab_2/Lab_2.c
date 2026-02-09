#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include <stdlib.h>

#define NUM_THREADS 10

pthread_t threads[NUM_THREADS];
pthread_mutex_t lock;

sem_t upper_limit;
sem_t lower_limit;

int amount = 0;
// void* runner(void * param);
void* deposit(void * param);
void* withdraw(void * param);

int main (int argc, char* argv[]){

    pthread_attr_t attr;

    pthread_mutex_init(&lock, NULL);

    if (sem_init(&upper_limit, 0, 4) != 0){
        printf("Error in initializing empty upper semaphore \n");
    }

    if (sem_init(&lower_limit, 0, 0) != 0){
        printf("Error in initializing empty lower semaphore \n");
    }

    pthread_attr_init(&attr);
    pthread_create(&threads[3], &attr, deposit, argv[1]);
    pthread_create(&threads[4], &attr, deposit, argv[1]);
    pthread_create(&threads[5], &attr, deposit, argv[1]);

    pthread_create(&threads[0], &attr, withdraw, argv[1]);
    pthread_create(&threads[1], &attr, withdraw, argv[1]);
    pthread_create(&threads[2], &attr, withdraw, argv[1]);

    pthread_create(&threads[6], &attr, deposit, argv[1]);
    pthread_create(&threads[7], &attr, deposit, argv[1]);
    pthread_create(&threads[8], &attr, deposit, argv[1]);
    pthread_create(&threads[9], &attr, deposit, argv[1]);


    int i = 0;


    for (i = 0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Final amount = %d\n", amount);
    pthread_mutex_destroy(&lock);

}

void * deposit (void * param){
    sem_wait(&upper_limit);
    int money = atoi(param);
    printf ("Executing deposit function\n");
    pthread_mutex_lock(&lock);
    


    // printf("Current amount = %d\n", amount);
        amount += money;
        printf("Deposit amount = %d, new amount is %d\n", money, amount);
  


    
    pthread_mutex_unlock(&lock);
    sem_post(&lower_limit);
    pthread_exit(0);

}

void * withdraw (void * param){
    sem_wait(&lower_limit);
    int money = atoi(param);

    printf("Executing withdraw function\n");

    // printf("Current amount = %d\n", amount);
    pthread_mutex_lock(&lock);

        amount -= money;
        printf("Withdraw amount = %d, new amount is %d\n", money, amount);
    
    pthread_mutex_unlock(&lock);
    sem_post(&upper_limit);
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