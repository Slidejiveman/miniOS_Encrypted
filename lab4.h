#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define NUM_BLOCKS  3       // Initial number of mem_nodes, chunks of memory
#define BLOCK_SIZE 1024    // Maximum block size
#define DEBUG

/* type definitions */
// these mem_nodes will be strung together in a doubly-linked queue
typedef struct _mem_node 
{
    int ptid;              // mem_node ID
    int nBase;             // the base register memory offset of the mem_node
    int nStay;             // the length of time the mem_node has been allocated
    int nBlocks;           // the limit register of the memory process
    struct _mem_node *next;    // pointer to next mem_node in list
    struct _mem_node *prev;    // pointer to previous mem_node in list
} mem_node;

// used to form the memory allocated and available queues
typedef struct _doubly_linked_queue
{
    mem_node *head, *tail, *current;
    int length;
} doubly_linked_queue;

/* function prototypes */
void init_queues();
void init_mem_node();
void enqueue2();
bool is_empty2();
void requeue2();
mem_node *dequeue();
void *allocate();
void *collect();
void *traverse();
void *increment_times();
mem_node *split_mem_node();
void merge_mem_nodes();
void compact();

/* global variables */
doubly_linked_queue *AVAILABLE_MEMORY = NULL;
doubly_linked_queue *ALLOCATED_MEMORY = NULL;

/* methods */

// initializes data and creates threads
int init_memory_manager() 
{
    srand(time(NULL));
    
    // initialize doubly-linked queues
    init_queues();
    
    return EXIT_SUCCESS;
}

// Creates a doubly-linked queue with initialized mem_nodes for available memory
// allocated memory starts out empty, but this method creates both queues
void init_queues()
{
    AVAILABLE_MEMORY = (doubly_linked_queue*) malloc(sizeof(doubly_linked_queue));
    if (AVAILABLE_MEMORY == NULL) return;
    ALLOCATED_MEMORY = (doubly_linked_queue*) malloc(sizeof(doubly_linked_queue));
    if (ALLOCATED_MEMORY == NULL) return;
    for (i = 0; i < NUM_BLOCKS; ++i)
    {
        mem_node *new_mem_node = (mem_node*) malloc(sizeof(mem_node));
        if (new_mem_node == NULL) return; // check to see if heap is full (malloc failure)
        init_mem_node(new_mem_node, i);    
        enqueue2(AVAILABLE_MEMORY, new_mem_node);
#ifdef DEBUG       
        printf("New mem_node ptid: %d nBase: %d nStay: %d nBlocks: %d\n", new_mem_node->ptid, new_mem_node->nBase, new_mem_node->nStay, new_mem_node->nBlocks);
#endif        
    }
}

// Initialize a new mem_node during program startup for available memory
void init_mem_node(mem_node *new_mem_node, int i)
{
    new_mem_node->ptid = i;
    new_mem_node->nBase = i * BLOCK_SIZE - 1; // 0 -> BLOCKSIZE - 1 for each initial mem_node
    if (new_mem_node->nBase == -1) new_mem_node->nBase = 0;
    new_mem_node->nStay = 0;                  // 0 since the memory hasn't been allocated at all yet.
    new_mem_node->nBlocks = BLOCK_SIZE;   
}

// add new mem_node to the end of the passed in queue
void enqueue2(doubly_linked_queue *memory, mem_node *new_mem_node)
{
    if (!is_empty2(memory)) 
    { 
        memory->tail->next = new_mem_node;
        new_mem_node->prev = memory->tail;
    }
    else 
        memory->head = new_mem_node;
    memory->tail = new_mem_node;
    memory->length++;
#ifdef DEBUG
    printf("queue tail ptid: %d queue head ptid: %d\n", memory->tail->ptid, memory->head->ptid);
#endif
  
}

// determines if the doubly-linked queue is empty
bool is_empty2(doubly_linked_queue *memory) 
{
    return memory == NULL || memory->head == NULL;
}

// removes the memory mem_node from its location in one queue
// and replaces it at the end of the other queue
void requeue2(doubly_linked_queue *target, doubly_linked_queue *source, mem_node *a_mem_node)
{
    if (source->head == a_mem_node) 
        source->head = a_mem_node->next;
    else if (source->tail == a_mem_node)
        source->tail = a_mem_node->prev; 
    else // standard case
    {
        mem_node *previous = a_mem_node->prev;
        mem_node *next = a_mem_node->next;
        previous->next = next;
        next->prev = previous;
    }
    source->length--;
    enqueue2(target, a_mem_node);
}

// removes a mem_node from a memory queue and returns it
mem_node *dequeue(doubly_linked_queue *source, mem_node *a_mem_node)
{
    if (source->head == a_mem_node)
        source->head = a_mem_node->next;
    else if (source->tail == a_mem_node)
        source->tail = a_mem_node->prev;
    else // standard case
    {
        mem_node *previous = a_mem_node->prev;
        mem_node *next = a_mem_node->next;
        previous->next = next;
        next->prev = previous;
    }
    source->length--;
    return a_mem_node;
}

// splits a mem_node to allocate a smaller amount of memory
// thus minimizing waste.
mem_node *split_mem_node(mem_node *a_mem_node, int blocks)
{
    a_mem_node->nBlocks   = a_mem_node->nBlocks - blocks;
    mem_node *new_mem_node    = (mem_node*) malloc(sizeof(mem_node));
    new_mem_node->ptid    = ++i;                           // i is global, never reset to 0
    new_mem_node->nBase   = a_mem_node->nBlocks - blocks;      // base starts at the last index of original mem_node's
    new_mem_node->nStay   = 0;                             // 0 because only just now allocated
    new_mem_node->nBlocks = blocks;                        // blocks to allocate was passed into this function
    return new_mem_node;
}

// merge adjacent mem_nodes that have been returned to available memory:
// if a mem_node is found that doesn't have a maximum number of blocks,
// find some mem_nodes that can return their blocks to it without exceeding
// the maximum number of blocks
void merge_mem_nodes()
{
    doubly_linked_queue *temp = (doubly_linked_queue*) malloc(sizeof(doubly_linked_queue));
    mem_node *temp_mem_node;

    // find all eligible mem_nodes in available memory for deallocation
    // the ones that are found will all be in a single block of memory
    // All should be stored in the high range first.
    // Ignore the head. If it is still in the AVAILABLE_MEMORY pool, then it
    // is still large enough to be subdivided into other mem_nodes. Just give blocks
    // back to it.
    pthread_mutex_lock(&mutex);
    AVAILABLE_MEMORY->current = AVAILABLE_MEMORY->head->next;
    pthread_mutex_unlock(&mutex);
    while (AVAILABLE_MEMORY->current != AVAILABLE_MEMORY->tail)
    {
        if (AVAILABLE_MEMORY->current->nBlocks < BLOCK_SIZE)
        {
            temp_mem_node = AVAILABLE_MEMORY->current;
            pthread_mutex_lock(&mutex);
            AVAILABLE_MEMORY->current = AVAILABLE_MEMORY->current->next;
            AVAILABLE_MEMORY->head->nBlocks += temp_mem_node->nBlocks;
            requeue2(temp, AVAILABLE_MEMORY, temp_mem_node);
            pthread_mutex_unlock(&mutex);
        }
        if (AVAILABLE_MEMORY->tail->nBlocks < BLOCK_SIZE) // handle tail separately
        {
            temp_mem_node = AVAILABLE_MEMORY->tail;
            pthread_mutex_lock(&mutex);
            AVAILABLE_MEMORY->head->nBlocks += temp_mem_node->nBlocks;
            requeue2(temp, AVAILABLE_MEMORY, temp_mem_node);
            pthread_mutex_unlock(&mutex);
        }
    }
    free(temp); // free allocated temp queue 
}

// compacts memory segments after mem_nodes have been merged.
// this will change the base values of the allocated memory
// mem_nodes based on the merger of unallocated memory into a
// single available memory mem_node.
void compact() 
{
    mem_node *temp_mem_node;
    // based on the available memory's head's base and limit
    // registers, update the allocated memory's base and limit
    // registers. The updates occur tail first because memory 
    // is allocated sequentially starting at the highest memory
    // offset.
    pthread_mutex_lock(&mutex);
    ALLOCATED_MEMORY->tail->nBase = AVAILABLE_MEMORY->head->nBlocks + 1; // offset the tail first as it is a special case
    ALLOCATED_MEMORY->current = ALLOCATED_MEMORY->tail;
    pthread_mutex_unlock(&mutex);
    while (ALLOCATED_MEMORY->current != ALLOCATED_MEMORY->head)
    {
        temp_mem_node = ALLOCATED_MEMORY->current;
        pthread_mutex_lock(&mutex);
        ALLOCATED_MEMORY->current = ALLOCATED_MEMORY->current->prev;     // traversing backwards
        ALLOCATED_MEMORY->current->nBase = temp_mem_node->nBase + temp_mem_node->nBlocks + 1; // base + limit + 1
        pthread_mutex_unlock(&mutex);
    }
    
}

/* thread methods */
// select a mem_node from the available queue to allocate
// based a randomly generated number. The number should
// be compared to the nBlocks value of the mem_node. If the
// nBlocks value is not too large, use that mem_node. If it
// is too large, split the mem_node: Decrement the nBlocks
// value of the existing mem_node by the amount that will be
// allocated. Create a new mem_node with the random number
// as the nBlocks size. Enqueue this mem_node in the allocated
// memory queue.
void *allocate()
{
#ifdef DEBUG
    printf("******** IN ALLOCATION ROUTINE ******** \n\n");
#endif
    while(true) 
    {
        if (!is_empty2(AVAILABLE_MEMORY))
        {
#ifdef DEBUG
            printf("\n==== Allocating Memory ====\n");
#endif
            int blocks_to_allocate = rand() % 41 + 10; // 10 - 50 blocks
            pthread_mutex_lock(&mutex);
            AVAILABLE_MEMORY->current = AVAILABLE_MEMORY->head;
            pthread_mutex_unlock(&mutex);
            while (AVAILABLE_MEMORY->current != AVAILABLE_MEMORY->tail) // looking via "First Fit" method
            {
                if (AVAILABLE_MEMORY->current->nBlocks > blocks_to_allocate)
                {
                    // we can allocate blocks from this mem_node, but is the mem_node too large?
                    if (AVAILABLE_MEMORY->current->nBlocks > blocks_to_allocate * 2)
                    {
                        // split the mem_node by decrementing its nBlocks and creating a new mem_node
                        // with the difference. Add the new mem_node to the allocated queue
                        pthread_mutex_lock(&mutex);
                        mem_node* new_mem_node = split_mem_node(AVAILABLE_MEMORY->current, blocks_to_allocate); 
                        enqueue2(ALLOCATED_MEMORY, new_mem_node);
                        pthread_mutex_unlock(&mutex);
                    } 
                    else
                    {    // allocate the current mem_node by moving it to the allocated queue
                         pthread_mutex_lock(&mutex);
                         requeue2(ALLOCATED_MEMORY, AVAILABLE_MEMORY,AVAILABLE_MEMORY->current);
                         pthread_mutex_unlock(&mutex);
                    }
                    // exit the loop after a fit is found.
                    pthread_mutex_lock(&mutex);
                    AVAILABLE_MEMORY->current = AVAILABLE_MEMORY->tail;    
                    pthread_mutex_unlock(&mutex);
                }
                else // we cannot allocate blocks from this mem_node 
                {
                    pthread_mutex_lock(&mutex);
                    AVAILABLE_MEMORY->current = AVAILABLE_MEMORY->current->next;
                    pthread_mutex_unlock(&mutex);
                }
            }       
            sleep(1);
        }
    }
    return EXIT_SUCCESS;
}

// Garbage collects an allocated mem_node and move is back to the available queue
// Chooses whichever mem_node has the highest stay value. This should be the head.
// Bonus: try to combine adjacent freed mem_nodes back together and if one has
// nBlocks of than some value, free the memory
void *collect()
{
#ifdef DEBUG
    printf("******** IN COLLECTION ROUTINE ******** \n\n");
#endif
    while(true)
    {
        if (!is_empty2(ALLOCATED_MEMORY))
        {
#ifdef DEBUG
            printf("\n==== Garbage Collecting ====\n");
#endif
            pthread_mutex_lock(&mutex);
            ALLOCATED_MEMORY->head->nStay = 0;                                   // deallocate and clear timer
            requeue2(AVAILABLE_MEMORY, ALLOCATED_MEMORY, ALLOCATED_MEMORY->head); // the head should have highest stay
            pthread_mutex_unlock(&mutex);
            if (AVAILABLE_MEMORY->length > NUM_BLOCKS * 3)
            {
                merge_mem_nodes();                                                   // merge free mem_nodes in available memory
                compact();                                                       // recalculate memory offsets
            }
        }
        sleep(2);
    }
    return EXIT_SUCCESS;
}

// traverses through the queues starting at the head. Print out info.
void *monitor()
{
#ifdef DEBUG
    printf("******** IN TRAVERSE ROUTINE ********\n\n");
#endif
    while(true) 
    {
        if (!is_empty2(AVAILABLE_MEMORY))
        {
#ifdef DEBUG
            printf("\n==== Traversing Available Memory ====\n");
#endif
            pthread_mutex_lock(&mutex);
            AVAILABLE_MEMORY->current = AVAILABLE_MEMORY->head;
            while(AVAILABLE_MEMORY->current != AVAILABLE_MEMORY->tail)
            {
                printf("Current mem_node: %d nBase: %d nStay %d nBlocks %d\n", AVAILABLE_MEMORY->current->ptid, AVAILABLE_MEMORY->current->nBase, AVAILABLE_MEMORY->current->nStay, AVAILABLE_MEMORY->current->nBlocks);
                AVAILABLE_MEMORY->current = AVAILABLE_MEMORY->current->next;
            }
            printf("Current mem_node: %d nBase: %d nStay %d nBlocks %d\n", AVAILABLE_MEMORY->tail->ptid, AVAILABLE_MEMORY->tail->nBase, AVAILABLE_MEMORY->tail->nStay, AVAILABLE_MEMORY->tail->nBlocks); // must print the tail too 
            pthread_mutex_unlock(&mutex);
        }

        // Releasing lock before checking allocated memory because no memory may be allocated.
        // This also gives other threads the chance to go when checking to see if there is 
        // allocated memory.
        if (ALLOCATED_MEMORY != NULL && !is_empty2(ALLOCATED_MEMORY))
        {
#ifdef DEBUG
            printf("\n==== Traversing Allocated Memory ====\n");
#endif
            pthread_mutex_lock(&mutex);
            ALLOCATED_MEMORY->current = ALLOCATED_MEMORY->head;
            while(ALLOCATED_MEMORY->current != ALLOCATED_MEMORY->tail)
            {
                printf("Current mem_node: %d nBase: %d nStay: %d nBlocks: %d\n", ALLOCATED_MEMORY->current->ptid, ALLOCATED_MEMORY->current->nBase, ALLOCATED_MEMORY->current->nStay, ALLOCATED_MEMORY->current->nBlocks);
                ALLOCATED_MEMORY->current = ALLOCATED_MEMORY->current->next;
            }
            printf("Current mem_node: %d nBase: %d nStay: %d nBlocks: %d\n", ALLOCATED_MEMORY->tail->ptid, ALLOCATED_MEMORY->tail->nBase, ALLOCATED_MEMORY->tail->nStay, ALLOCATED_MEMORY->tail->nBlocks); // must print the tail too 
            pthread_mutex_unlock(&mutex);
        } 
        sleep(5);
    }
    return EXIT_SUCCESS;
}

// increments the nStay value of each mem_node in the allocated queue
void *increment_times()
{
#ifdef DEBUG
    printf("******** IN INCREMENT TIMES ROUTINE ********\n\n");
#endif
    while(true)
    {
        // if the allocated_memory isn't empty, increment all of the
        // mem_nodes' stay value
        if (!is_empty2(ALLOCATED_MEMORY))
        {
#ifdef DEBUG
            printf("\n==== Incrementing Allocated Memory Stay Values  ====\n");
#endif
            pthread_mutex_lock(&mutex);
            ALLOCATED_MEMORY->current = ALLOCATED_MEMORY->head;
            while (ALLOCATED_MEMORY->current != ALLOCATED_MEMORY->tail)
            {
                ALLOCATED_MEMORY->current->nStay++;
                ALLOCATED_MEMORY->current = ALLOCATED_MEMORY->current->next;
            }
            ALLOCATED_MEMORY->tail->nStay++; // handle the tail
            pthread_mutex_unlock(&mutex);
        }
        sleep(1);
    }
    return EXIT_SUCCESS;
}

