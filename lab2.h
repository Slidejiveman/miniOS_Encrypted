#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define PRODUCER 0
#define CONSUMER 1
#define BUFFER_SIZE 10
#define ROUNDS 3         // used to terminate execution

// type definition for thread specific data
// this is not needed in this example, but
// included for practice.
typedef struct _thread_data_t {
    int tid;
    double stuff;
} thread_data_t;

// function prototypes: alert compiler of
// their existence
void* produce(void*);
void* consume(void*);

// global variables: shared memory between producer
// and consumer. This version uses mutex locks.
int stack[BUFFER_SIZE];
int counter = 0, rear = 0, front = 0, i;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;

// methods: Code Section
// Main establishes the threads. It is designed so that the producer
// fills the stack before allowing the consumer to consume them all.
// This is, essentially, batch processing.
int init_procon() 
{
    pthread_t tPr, tCo;                      // thread identifier
    int pid = PRODUCER, cid = CONSUMER, rc;  // IDs, return code
    thread_data_t tPr_data, tCo_data;        // thread data
    tPr_data.tid = pid;                      // store id
    tCo_data.tid = cid;                      // "      "
    
    return EXIT_SUCCESS;
}

// This function produces an integer value in the global
// stack buffer that can later be read by a consumer.
void *produce(void *arg)
{
    do { 
        pthread_mutex_lock(&mutex2);
        if (counter < BUFFER_SIZE - 1)
        {
            stack[rear] = counter; // produces an int
            printf("Produced int with counter: = %d\n", counter);
            rear = (rear + 1) % BUFFER_SIZE;
            counter++;
        }
        pthread_mutex_unlock(&mutex2);
        sleep(1);
    } while(true); 
    printf("Successfully exiting produce function...\n");
    return EXIT_SUCCESS; 
}

// This function consumes a value from the global stack
// AFTER it has been produced by the producer.
void *consume(void *arg)
{
    int consumed = 0;

    do {
        pthread_mutex_lock(&mutex2);
        if (counter > 0)
        {
            consumed = stack[front];
            printf("Consumed int: = %d\n", consumed);
            front = (front + 1) % BUFFER_SIZE;
            counter--;        
        }
        pthread_mutex_unlock(&mutex2);
        sleep(1);
    } while(true); 
     printf("Successfully exiting consume function...\n");
     return EXIT_SUCCESS;
}
