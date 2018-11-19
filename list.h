#include <pthread.h>

struct list_elem {
    struct list_elem *next;	// Zeiger auf das naechste Element
    pthread_t *thread; // Zeiger auf einen Thread
};

typedef struct list {
	struct list_elem *first;// erstes Element in der Liste
	struct list_elem *last;	// letztes Element in der Liste
} list_t;

/* function prototypes */
list_t           *list_init ();
struct list_elem *list_insert (list_t *list, pthread_t *thread);
struct list_elem *list_append (list_t *list, pthread_t *thread);
int              list_remove (list_t *list, struct list_elem *elem);
void             list_finit (list_t *list);
void             list_print (list_t *list, void (*print_elem) (pthread_t *thread));
struct list_elem *list_find (list_t *list, pthread_t *thread, int (*cmp_elem) (pthread_t *, pthread_t *));




