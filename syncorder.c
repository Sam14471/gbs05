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
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static long AnDerReihe = 0;

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

int write_buffer(long thread, char *buffer, int len)
{
	pthread_mutex_lock(&mutex);
	while(thread != AnDerReihe)
	{
		pthread_cond_wait(&cond, &mutex);
		pthread_cond_signal(&cond);
	}
	int readLength = len;
	int numberOfReads = 0;
	int fileDescriptor = 0;
	char *filename =(char *) malloc(5*sizeof(char));
	sprintf(filename, "%i.txt",((int) thread));
	fileDescriptor = open(filename, 0);
	char *prefix = (char *) malloc(9*sizeof(char));
	while(readLength == 64)
	{
		readLength = read(fileDescriptor, buffer, 64);
		sprintf(prefix, "[%02d] %03d\t", ((int) thread), numberOfReads);
		write(1, prefix, 9);
		write(1, buffer, readLength);
		write(1, "\n", 1);
		numberOfReads++;
	}
	AnDerReihe++;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	return 1;
}

void *AusgabeO(void *x)
{
	char *fileInput =(char *) malloc(64*sizeof(char));
	write_buffer( (* ((long *) x)), fileInput, 64);
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
	if(n>10){n = 10;}

	/*Thread erstellen*/
	list_t *threadl = (list_t *) malloc(sizeof(list_t));
	threadl = list_init();
	pthread_t *thread = (pthread_t *) malloc(sizeof(pthread_t));
	int valuePointer[]={0,1,2,3,4,5,6,7,8,9};
	long valuePointerl[]={0,1,2,3,4,5,6,7,8,9};
	if(l==1)
	{
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
		pthread_create(thread, NULL, AusgabeO, valuePointerl+i);
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
		if(f==0 && l== 0 && o==1){sleep(1);}
	}

	fflush(stdout);
	free(thread);
	free(threadl);
	//free(fileInput);
	//free(prefix);
	//free(fileName);
	return 0;
}
