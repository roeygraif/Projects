#ifndef __REQUEST_H__

/**********************************************
               *  OUR CODE *
 **********************************************/

typedef struct node {
    int data;
    struct timeval arrival_time;
    struct timeval handled_time;
    struct node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int max_size;
    int size;
} Queue;

typedef struct {
    pthread_mutex_t* m;
    Queue* waiting_requests;
    Queue* running_requests;
    pthread_cond_t* full_queue;
    pthread_cond_t* empty_queue;
    pthread_cond_t* flush;
    int id;
    pthread_mutex_t* tm;
} T_args;

typedef struct{
    int dynamic_req;
    int static_req;
    int jobs_finished;
    int id;
}t_stats;

Queue* initQueue(int max_size);
int isEmpty(Queue* queue);
int isFull(Queue* queue);
int enqueue(Queue* queue, int x, struct timeval arr);
Node* dequeue(Queue* queue);
int freeSpace(Queue* queue1, Queue* queue2);
void dequeue2(Queue* queue, int x);
int dequeue3(Queue* queue, int p);
void dropHalf(Queue* queue);
void incSize(Queue* queue);


/**********************************************
               *  GIVEN CODE *
 **********************************************/


///void requestHandle(int fd);
void requestHandle(Node* con_node, t_stats* stats);

#endif
