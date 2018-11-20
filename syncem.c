#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <asm/ldt.h>
#include <linux/unistd.h>
#include "list.h"

/*Ausgabe der Threads*/
void *Ausgabe(void *x)
{
	int fileDescriptor=0;
	fileDescriptor=open(x+".txt");
	char *fileInput =(char *) malloc(64*sizeof(char));
	char *prefix = (char *) malloc(9* sizeof(char));
	int numberOfReads=0;
	while(read(fileDescriptor,fileInput,64)==64){
		sprintf(prefix,"[%02d] %03d\t", x, numberOfReads);
		write(stdout,prefix,9);
		write(stdout,fileInput,64);
		write(stdout,"\n",1);
		numberOfReads++;
	}
	sprintf(prefix,"[%02d] %03d\t", x, numberOfReads);
	write(stdout,prefix,9);
	write(stdout,fileInput,64);
	write(stdout,"\n",1);

	/*for(int i=1;i<=*(int*)x;i++)
	{
		printf("%10u\t%d\t%i\n", (unsigned int) pthread_self(), getpid(), i);
		sleep(1);
	}*/
	return NULL;
}

int main(int argc, char **argv)
{
	/*Initialisierung*/
	//int k = 10;
	int n = 1;
	//int r  = 0;

	//char *kp = "k";
	char *np = "n";
	//char *rp = "r";

	/*Übergabewerte übernehmen*/
	for(int j=1;j<argc;j++)
	{
		//if(*(argv[j]+1) == *kp){k = atoi(argv[j+1]);}
		if(*(argv[j]+1) == *np){n = atoi(argv[j+1]);}
		//if(*(argv[j]+1) == *rp){r = 1;}
	}

	/*Zeit ausgeben*/
	time_t *timep = (time_t *) malloc(sizeof(time_t));
	*timep = time(NULL);
	printf("Start: %s", ctime(timep));
	free(timep);

	/*Threadwerte für -r festlegen*/
	//if(r==1)k = ((random()%(k+1))+(int)k/2);

	/*Thread erstellen*/
	//pthread_t threads[n];
	list_t *threadl = (list_t *) malloc(sizeof(list_t));
	threadl = list_init();
	pthread_t *thread = (pthread_t *) malloc(sizeof(pthread_t));
	if()
	for(int i = 0; i<n; i++)
	{
		pthread_create(thread, NULL, Ausgabe, &i);
		list_insert(threadl, thread, 0, 0, 0, 0);
	}

	/*Auf Threads warten*/
	for(int i = 0; i<n; i++)
	{
		//pthread_join(threads[i], NULL);
		pthread_t current = *threadl->first->thread;
		pthread_join(current,NULL);
		list_remove(threadl, threadl->first);
	}

	/*Zeit ausgeben*/
	time_t *timep2 = (time_t *) malloc(sizeof(time_t));
	*timep2 = time(NULL);
	printf("Ende: %s", ctime(timep2));
	free(timep2);

	free(thread);
	free(threadl);
	return 0;
}
