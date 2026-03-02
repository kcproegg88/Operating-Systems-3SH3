/*
Compile:
    gcc -pthread A2.c -o A2

Run:
    ./A2 <number_of_students>
*/

/* Contributions:

Krishna Chauhan: 50% (TA thread, main function, and overall structure)
Fiona Douglas: 50% (Student thread and synchronization logic)

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_CHAIRS 3
#define MAX_STUDY_TIME 5
#define MAX_TA_TIME 5
#define default_num_students 3

int waiting_students = 0;
pthread_mutex_t chair_mut;
sem_t students_sem; // Ta waiting for Students
sem_t ta_sem; // Student waiting for Ta

// You need two since the Student needs to wait for the TA to finish helping before they can leave the chair and free it up for another student

void random_sleep(int max_seconds) {
    sleep(rand() % max_seconds + 1);
}

void* ta(void* param){
    while(1){
        sem_wait(&students_sem); // Ta waits for a student to arrive
        
        pthread_mutex_lock(&chair_mut);
        waiting_students--; // Ta starts helping a student, so one less student is waiting
        printf("TA starts helping a student. Students waiting: %d\n", waiting_students);
        pthread_mutex_unlock(&chair_mut);

        random_sleep(MAX_TA_TIME);
        printf("TA finished helping a student.\n");
        sem_post(&ta_sem); // Notify waiting students that TA is done helping
    }
}

void* student(void* param){
    int id = *((int*)param);
    while(1){
        printf("Student %d is programming.\n", id);
        random_sleep(MAX_STUDY_TIME);

        printf("Student %d is done programming. Checks if chair is available.\n", id);
        // Other stuff may happen between checking and reacting.

        pthread_mutex_lock(&chair_mut);
        if (waiting_students < NUM_CHAIRS){
            waiting_students++;
            printf("Student %d is waiting. Students waiting: %d\n", id, waiting_students);
            pthread_mutex_unlock(&chair_mut);
            sem_post(&students_sem); // Notify TA that a student is waiting
            // Here the student waits for their turn and also the time spent with the TA
            sem_wait(&ta_sem); // Wait for TA to finish helping
            printf("Student %d received help and leaves.\n", id);

        } else {
            printf("Student %d leaves because no chairs are available.\n", id);
            pthread_mutex_unlock(&chair_mut);
        }

    }
}

int main(int argc, char* argv[]){
    int number_of_students = default_num_students;
    if (argc != 2){
        printf("Usage: %s <number_of_students>, using default student value %d\n", argv[0], default_num_students);
    } else {
        number_of_students = atoi(argv[1]);
    }

    srand(time(NULL));
    pthread_t ta_thread;
    pthread_t student_threads[number_of_students];

    sem_init(&students_sem, 0, 0);
    sem_init(&ta_sem, 0, 0);
    pthread_mutex_init(&chair_mut, NULL);

    // Create all threads first
    pthread_create(&ta_thread, NULL, ta, NULL);

    int student_ids[number_of_students];
    for (int i = 0; i < number_of_students; i++){
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, student, &student_ids[i]);
    }

    // Join threads
    pthread_join(ta_thread, NULL);
    for (int i = 0; i < number_of_students; i++){
        pthread_join(student_threads[i], NULL);
    }

    // We don't think these will be reached tbh
    pthread_mutex_destroy(&chair_mut);
    sem_destroy(&students_sem);
    sem_destroy(&ta_sem);
}

