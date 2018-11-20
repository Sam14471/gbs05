#include <pthread.h>
#include <time.h>

struct list_elem {
    struct list_elem *next;	// Zeiger auf das naechste Element
    pthread_t *thread; // Zeiger auf einen Thread
    int number;
    int prio;
    int start;
    int lauf;
    int done;
};

typedef struct list {
	struct list_elem *first;// erstes Element in der Liste
	struct list_elem *last;	// letztes Element in der Liste
} list_t;

/* function prototypes */
list_t           *list_init ();
struct list_elem *list_insert (list_t *list, pthread_t *thread, int number, int prio, int start, int lauf);
struct list_elem *list_append (list_t *list, pthread_t *thread, int number, int prio, int start, int lauf);
int              list_remove (list_t *list, struct list_elem *elem);
void             list_finit (list_t *list);
void             list_print (list_t *list, void (*print_elem) (pthread_t *thread));
struct list_elem *list_find (list_t *list, pthread_t *thread, int (*cmp_elem) (pthread_t *, pthread_t *));
int              list_empty (list_t *list);
void             list_add (list_t *list, struct list_elem *elem);




