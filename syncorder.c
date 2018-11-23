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
#include <sys/stat.h>
#include <fcntl.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*Ausgabe der Threads*/
void *Ausgabe(void *x)
{
	int fileDescriptor=0;
	char *filename =(char *)malloc(5*sizeof(char));
	sprintf(filename,"%i.txt",(*(int *) x));
	fileDescriptor=open(filename,0);
	char *fileInput =(char *) malloc(64*sizeof(char));
	char *prefix = (char *) malloc(9* sizeof(char));
	int numberOfReads=0;
	int readLength=64;
	while(readLength==64){
		readLength=read(fileDescriptor,fileInput,64);
		sprintf(prefix,"[%02d] %03d\t",(*(int *) x), numberOfReads);
		write(1,prefix,9);
		write(1,fileInput,readLength);
		write(1,"\n",1);
		numberOfReads++;

	}
close(fileDescriptor);

	free(filename);
	free(fileInput);
	free(prefix);
	return NULL;
}

void *AusgabeZusammen(void *x)
{
	int fileDescriptor=0;
	char *filename =(char *)malloc(5*sizeof(char));
	sprintf(filename,"%i.txt",(*(int *) x));
	fileDescriptor=open(filename,0);
	char *fileInput =(char *) malloc(64*sizeof(char));
	char *prefix = (char *) malloc(9* sizeof(char));
	int numberOfReads=0;
	int readLength=64;
	while(readLength==64){
	pthread_mutex_lock(&mutex);
		readLength=read(fileDescriptor,fileInput,64);
		sprintf(prefix,"[%02d] %03d\t",(*(int *) x), numberOfReads);
		write(1,prefix,9);
		write(1,fileInput,readLength);
		write(1,"\n",1);
		numberOfReads++;
	pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *AusgabeMutex(void *x)
{
	int fileDescriptor=0;
	char *filename =(char *)malloc(5*sizeof(char));
	sprintf(filename,"%i.txt",(*(int *) x));
	fileDescriptor=open(filename,0);
	char *fileInput =(char *) malloc(64*sizeof(char));
	char *prefix = (char *) malloc(9* sizeof(char));
	int numberOfReads=0;
	int readLength=64;
	pthread_mutex_lock(&mutex);
	while(readLength==64){
		readLength=read(fileDescriptor,fileInput,64);
		sprintf(prefix,"[%02d] %03d\t",(*(int *) x), numberOfReads);
		write(1,prefix,9);
		write(1,fileInput,readLength);
		write(1,"\n",1);
		numberOfReads++;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void *AusgabeO(void *x)			//TODO: Umsetzung mit write_buffer();
{
	pthread_mutex_lock(&mutex);
	int fileDescriptor=0;
	char *filename =(char *)malloc(5*sizeof(char));
	sprintf(filename,"%i.txt",(*(int *) x));
	fileDescriptor=open(filename,0);
	char *fileInput =(char *) malloc(64*sizeof(char));
	char *prefix = (char *) malloc(9* sizeof(char));
	int numberOfReads=0;
	int readLength=64;
	while(readLength==64){
		readLength=read(fileDescriptor,fileInput,64);
		sprintf(prefix,"[%02d] %03d\t",(*(int *) x), numberOfReads);
		write(1,prefix,9);
		write(1,fileInput,readLength);
		write(1,"\n",1);
		numberOfReads++;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}


int main(int argc, char **argv)
{
	/*Initialisierung*/
	int n = 1;
	int l = 0;
	int f = 0;
	int o = 0;

	char *np = "n";
	char *lp = "l";
	char *fp = "f";
	char *op = "o";

	/*Übergabewerte übernehmen*/
	for(int j=1;j<argc;j++)
	{
		if(*(argv[j]+1) == *np){n = atoi(argv[j+1]);}
		if(*(argv[j]+1) == *lp){l=1;}
		if(*(argv[j]+1) == *fp){f=1;}
		if(*(argv[j]+1) == *op){o=1;}
	}

	/*Thread erstellen*/
	list_t *threadl = (list_t *) malloc(sizeof(list_t));
	threadl = list_init();
	pthread_t *thread = (pthread_t *) malloc(sizeof(pthread_t));
	int valuePointer[]={0,1,2,3,4,5,6,7,8,9};
	if(l==1){

	for(int i = 0; i<n; i++)
	{
		pthread_create(thread, NULL, AusgabeZusammen, valuePointer+i);
		list_insert(threadl, thread, 0, 0, 0, 0);
	}
	}
	else if(f==1){
	for(int i = 0; i<n; i++)
	{
		pthread_create(thread, NULL, AusgabeMutex, valuePointer+i);
		list_insert(threadl, thread, 0, 0, 0, 0);
	}
	}
	else if(o==1)
	{
	for(int i = 0; i<n; i++)
	{
		pthread_create(thread, NULL, AusgabeO, valuePointer+i);
		list_insert(threadl, thread, 0, 0, 0, 0);
	}
	}
	else{
	for(int i = 0; i<n; i++)
	{
		pthread_create(thread, NULL, Ausgabe, valuePointer+i);
		list_insert(threadl, thread, 0, 0, 0, 0);
	}
	}
	/*Auf Threads warten*/
	for(int i = 0; i<n; i++)
	{

		pthread_t current = *threadl->first->thread;
		pthread_join(current,NULL);
		list_remove(threadl, threadl->first);
		sleep(1);
	}

	fflush(stdout);
	free(thread);
	free(threadl);
	return 0;
}
