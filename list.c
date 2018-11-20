#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

list_t *list_init () {
    list_t *listPointer = (list_t *) malloc(sizeof(list_t));
    if (listPointer != NULL) {
        return listPointer;
    } else {
        return NULL;//Fehlerbehandlung???
    }
}

struct list_elem *list_insert (list_t *list, pthread_t *thread, int number, int prio, int start, int lauf) {

    struct list_elem *newElement = (struct list_elem *) malloc(sizeof( struct list_elem));
    if (newElement != NULL) {
        newElement->thread = thread;
        newElement->next = list->first;
        newElement->number = number;
        newElement->prio = prio;
        newElement->start = start;
        newElement->lauf = lauf;
        newElement->done = 0;

        if(list->last==NULL){
	        list->last=newElement;
	    }
        list->first = newElement;
        return newElement;
    } 
    else {
        return NULL;//Fehlerbehandlung???
    }
}

void list_add (list_t *list, struct list_elem *elem) {
    elem->next = list->first;
    if(list->last==NULL){
	    list->last=elem;
	}
    list->first = elem;
    return;
}

struct list_elem *list_append (list_t *list, pthread_t *thread, int number, int prio, int start, int lauf) {

    struct list_elem *newElement = (struct list_elem *) malloc(sizeof(struct list_elem));
    if (newElement != NULL) {
        newElement->thread = thread;
        newElement->number = number;
        newElement->prio = prio;
        newElement->start = start;
        newElement->lauf = lauf;
        newElement->done = 0;

        if(list->last!=NULL){
           list->last->next = newElement;
        }
        list->last = newElement;
        if(list->first==NULL){
	  list->first=newElement;
	}
        return newElement;

    } else {
        return NULL;
    }
}

int list_remove (list_t *list,struct list_elem *elem){
    if(list->first==elem){
        list->first=list->first->next;
        return 0;
    }
    struct list_elem *currentElement=list->first;
    while(currentElement!=NULL){
        if(currentElement->next==elem){
            currentElement->next=currentElement->next->next;
            return 0;
        }
        else{
            currentElement=currentElement->next;
        }
    }
    return -1;
}

void list_finit(list_t *list){
    while(list->first!=NULL){
        list->first=list->first->next;
    }
    free(list);
}

struct list_elem *list_find (list_t *list, pthread_t *thread, int (*cmp_elem) (pthread_t *, pthread_t *)) {

    struct list_elem *current = list->first;
    while (current != NULL) {
        if (cmp_elem(current->thread, thread) == 0) {
            return current;
        } else {
            if (current->next != NULL) {
                current = current->next;
            } else {
                //data not found
                return NULL;
            }
        }
    }
    return NULL;
}

void list_print (list_t *list, void (*print_elem) (pthread_t *)) {
    struct list_elem *current = list->first;
    int position=1;
    while (current != NULL) {
        printf("%d:",position);
        print_elem(current->thread);
        current=current->next;
        position++;
	printf("\n");
    }
}

int list_empty (list_t *list) {
    if (list->first == NULL) {
        return 1;
    } 
    return 0;
}

/*
void print_string (char *data){
    printf(data);
}


int main (int argc , char *argv [ ] , char *envp [ ] ) {

 list_t *li ;
 if((li=list_init()) == NULL) {
  perror( " Cannot allocate memory " ) ;
  exit(-1);
 }
 if(list_append(li,"erstes") == NULL ||
  list_append(li,"zweites")== NULL ||
  list_append(li,"drittes") == NULL) {
  perror(" Cannot allocate memory " ) ;
  exit(-1);
 }
 list_print (li, print_string );
 exit (0);
}

**/
