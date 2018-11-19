#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "list.h"
#include <pthread.h>

void *threadWork(void *test) {
    int *k = (int *) test;
    //printf("Thread gestartet\n");
    for (int i = 1; i <= *k; ++i) {
        printf("%10u\t%d\t%i\n", (unsigned int) pthread_self(), getpid(), i);
        sleep(1);
        // pro Sekunde eine Zahl ausgeben (1-k)
    }
    return 0;
}

int main(int argc, char **argv) {
    // Standardinitialisierung
    int k = 10;
    int n = 1;
    int r = 0;
    
    // Initialisierung durch Parameter
    char i = 0;
    while ((i = getopt(argc, argv, "k:n:r")) != -1) {
        switch(i) {
            case 'k':
                k = atoi(optarg);
                break;
            case 'n':
                n = atoi(optarg);
                break;
            case 'r':
                r = 1;
                break;
        }
    }
    // Falls r angegeben 
    if (r == 1) {
        k = (k/2) + rand() % k ;
    }

    // Ausgabe k,n,r
    // printf("k:%i, n:%i, r:%i\n", k, n, r);

    // aktuelle Uhrzeit ausgeben
    time_t *timep = (time_t *) malloc(sizeof(time_t));
    *timep = time(NULL);
    printf("Start: %s", ctime(timep));
    free(timep);

    // Threads erzeugen
    list_t *threads = (list_t *) malloc(sizeof(list_t));
    threads = list_init();
    pthread_t *thread = (pthread_t *) malloc(sizeof(pthread_t));
    void *arg = malloc(sizeof(int));
    for (int i = 1;i < n+1; ++i) {
        arg = &k;
        pthread_create(thread,NULL,threadWork,arg);
        list_insert(threads, thread);  
    }
    
    //auf Threads warten
    for (int i = 0; i < n; ++i) {
        // auf liste zugreifen 
        pthread_t current = *threads->first->thread;
        pthread_join(current,NULL);
        list_remove(threads,threads->first);
    }
    free(threads);
    //free(thread);
    //free(arg);
    
    //aktuelle Uhrzeit ausgeben
    timep = (time_t *) malloc(sizeof(time_t));
    *timep = time(NULL);
    printf("Ende: %s", ctime(timep));
    free(timep);
    return 0;
}