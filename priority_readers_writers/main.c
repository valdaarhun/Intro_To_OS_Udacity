#include <pthread.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned long long shared_var;
int x;

// thread-related vars
int num_readers;
pthread_t writers[5];
pthread_t readers[5];

// synchronization-related vars
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_w = PTHREAD_COND_INITIALIZER;

void *write_to_var(void *args){
    unsigned long long val = *(pthread_t *)args;

    // write x times
    for(int i = 0 ; i < x ; i++){
        int t = (rand() % 3) + 1;
        sleep(t);

        pthread_mutex_lock(&mutex);
        while(num_readers > 0){
            pthread_cond_wait(&cond_w, &mutex);
        }
        shared_var = val;
        printf("shared_var written = %llu\tnum of readers = %d\n", shared_var, num_readers);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

void *read_from_var(void *args){
    int t = (rand() % 3) + 1;
    sleep(t);

    pthread_mutex_lock(&mutex);
    num_readers++;
    pthread_mutex_unlock(&mutex);

    for(int i = 0 ; i < x ; i++){
        int t = (rand() % 3) + 1;
        sleep(t);

        pthread_mutex_lock(&mutex);
        printf("shared_var read = %llu\tnum of readers = %d\n", shared_var, num_readers);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    num_readers--;
    if (num_readers == 0)
        pthread_cond_broadcast(&cond_w);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int main(){
    // Get value of x
    printf("Input the value of x: ");
    fflush(stdout);
//    scanf(" %d", &x);
    x = 5;

    // Set random seed
    srand(0);

    // Create writer threads
    for(int i = 0 ; i < 5 ; i++){
        errno = 0;
        if (pthread_create(&writers[i], NULL, write_to_var, writers + i) != 0){
            fprintf(stderr, strerror(errno));
        }
    }

    // Create reader threads
    for(int i = 0 ; i < 5 ; i++){
        errno = 0;
        if (pthread_create(&readers[i], NULL, read_from_var, NULL) != 0){
            fprintf(stderr, strerror(errno));
        }
    }

    // join threads;
    for(int i = 0 ; i < 5 ; i++){
        errno = 0;
        if (pthread_join(writers[i], NULL)){
            fprintf(stderr, strerror(errno));
        }
        errno = 0;
        if (pthread_join(readers[i], NULL)){
            fprintf(stderr, strerror(errno));
        }
    }

    pthread_cond_destroy(&cond_w);
    pthread_mutex_destroy(&mutex);
}
