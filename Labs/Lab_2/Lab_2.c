#include <pthread.h>
#include <stdio.h>

#include <stdlib.h>

int sum; /*globally used among the threads*/

void *runner(void *param); /* thread calls this fxn*/

int main(int argc, char *argv[]){
    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_init(&attr); /* get the default attributes */
    pthread_create(&tid,&attr,runner,NULL); /* create the thread */
    pthread_join(tid,NULL); /* wait for the thread to exit */
    printf("sum = %d\n",sum);
    return 0;
}

void *runner(void *param){
    int i, upper = atoi(param);
    sum = 0;
    for(i=1; i<=upper; i++){
        sum += i;
    }
    pthread_exit(0);
}
