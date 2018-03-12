#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define NUM_NODES 30
#define MAX 2147483647

/* type definitions */
// these nodes will be strung together in a doubly-linked queue
typedef struct _node {
    int ptid;             // process/thread ID
    int prio;             // priority. Lower # is higher priority
    char desc[100];        // description of process/thread
    struct _node *next;    // pointer to next node in list
    struct _node *prev;    // pointer to previous node in list
} node;

/* function prototypes */
void init_queue();
void init_node();
void enqueue();
bool is_empty();
void silent_traverse();
void requeue();
void enqueue_dispatched();
void *schedule();
void *interrupt();
void *traverse();

/* global variables */
node *CURRENT  = NULL;
node *HEAD     = NULL;
node *TAIL     = NULL;
node *MIN_PRIO = NULL;
pthread_mutex_t mutex3=PTHREAD_MUTEX_INITIALIZER;

/* methods */

// initializes data and creates threads
int init_scheduler() 
{
    srand(time(NULL));
    // initialize doubly-linked queue
    init_queue();
    
    return EXIT_SUCCESS;
}

// Creates a doubly-linked queue with initialized nodes
void init_queue()
{
    for (i = 0; i < NUM_NODES; ++i)
    {
        node *new_node = (node*) malloc(sizeof(node));
        if (new_node == NULL) return;
        init_node(new_node, i);    
        enqueue(new_node);
    }
}

// Initialize a new node
void init_node(node *new_node, int i)
{
    new_node->ptid = i;
    new_node->prio = rand() % 70 + 31; // values 30-100
    new_node->desc[0] = 'n';
    new_node->desc[1] = '\0'; // plan to do more with desc if time allows   
}

// add new node to the end of the queue
void enqueue(node *new_node)
{
    if (!is_empty()) 
    { 
        TAIL->next = new_node;
        new_node->prev = TAIL;
    }
    else
        HEAD = new_node;
    TAIL = new_node;   
}

// helper function that adds the min prio node
// back to the end of the list
void enqueue_dispatched()
{
    TAIL->next = MIN_PRIO;
    MIN_PRIO->prev = TAIL;
    TAIL = MIN_PRIO;
}

// determines if the doubly-linked queue is empty
bool is_empty() 
{
    return HEAD == NULL;
}

// traverses the queue, returning a pointer to the
// node with the highest priority
void silent_traverse() 
{
    int min = MAX;
    CURRENT = HEAD;
    while(CURRENT != TAIL) // tail not considered
    {
        if (CURRENT->prio < min)
        {
            min = CURRENT->prio;
            MIN_PRIO = CURRENT;
        }
        CURRENT = CURRENT->next;
    }
}

// removes the scheduled node from its location in the queue
// and replaces it at the end of the queue
void requeue()
{
    node *previous = MIN_PRIO->prev;
    node *next = MIN_PRIO->next;
    previous->next = next;
    next->prev = previous;
    enqueue_dispatched();
}

/* thread methods */
void *schedule()
{
    while(true) 
    {
        // find the minimum priority with a traversal
        // print this node's information out
        // adjust the node's priority 
        // place it at the end of the queue 
        pthread_mutex_lock(&mutex3);
        silent_traverse();
        printf("SCHEDULED: Node %d with Prio %d and Desc %s\n", MIN_PRIO->ptid, MIN_PRIO->prio, MIN_PRIO->desc);
        MIN_PRIO->prio = rand() % 70 + 31; // 30-100
        requeue();
        pthread_mutex_unlock(&mutex3);       
        sleep(2);
    }
    return EXIT_SUCCESS;
}

// Randomly chooses node(s) every minute
// and changes priority.
void *interrupt()
{
    while(true)
    {
        int steps = rand() % 30;          // 0 - 29
        pthread_mutex_lock(&mutex3);
        CURRENT = HEAD;
        for(i = 0; i < steps && i < NUM_NODES; ++i)
        {
            CURRENT = CURRENT->next;
        }

        CURRENT->prio = rand() % 70 + 31; // 30 - 100

        pthread_mutex_unlock(&mutex3);
        sleep(3);
    }
    return EXIT_SUCCESS;
}

// traverses through the queue starting at the head
// printing out a list of nodes as identified by the
// node's ptid
void *traverse()
{
    while(true) 
    {
        pthread_mutex_lock(&mutex3);
        CURRENT = HEAD;
        while(CURRENT != TAIL)
        {
            printf("Current node: %d\n", CURRENT->ptid);
            CURRENT = CURRENT->next;
        }
        printf("Current node: %d\n", TAIL->ptid); // must print the tail too 
        pthread_mutex_unlock(&mutex3);
        sleep(5);
    }
    return EXIT_SUCCESS;
}
