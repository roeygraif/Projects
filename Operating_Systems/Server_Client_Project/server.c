#include "segel.h"
#include "request.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
void getargs(int *port, int argc, char *argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
}


void* thread_job(void* args){
    int connfd;
    t_stats* stats = (t_stats*)malloc(sizeof(t_stats));
    T_args* ta = (T_args*)args;
    stats->dynamic_req = 0;
    stats->static_req = 0;
    stats->jobs_finished = 0;
    stats->id = ta->id;
    pthread_mutex_unlock(ta->tm);
    Node* con_node;
    while(1){
        //lock
        pthread_mutex_lock(ta->m);
        while(isEmpty(ta->waiting_requests)){
            pthread_cond_wait(ta->empty_queue, ta->m);
        }
        con_node = dequeue(ta->waiting_requests);
        gettimeofday(&(con_node->handled_time), NULL);
        connfd = con_node->data;
        enqueue(ta->running_requests, con_node->data, con_node->arrival_time);

        pthread_mutex_unlock(ta->m);
        //unlock

        requestHandle(con_node, stats);
        Close(connfd);

        pthread_mutex_lock(ta->m);
        dequeue2(ta->running_requests, con_node->data);
        free(con_node);
        if(isEmpty(ta->running_requests) && isEmpty(ta->waiting_requests)){
            pthread_cond_signal(ta->flush);
        }
        pthread_cond_signal(ta->full_queue);
        pthread_mutex_unlock(ta->m);
    }
    free(stats);
}





int main(int argc, char *argv[])
{
    if(argc < 3){
        printf("Not Enough args...");
        exit(0);
    }
    int arg_threads = atoi(argv[2]);
    int arg_queue_size = atoi(argv[3]);
    int arg_max_queue_size = 0;
    if(argc >= 6){
        arg_max_queue_size = atoi(argv[5]);
    }

    if(arg_threads < 1){
        exit(0);
    }
    if(arg_queue_size < 1){
        exit(0);
    }


    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    struct timeval arrival_time;
    Queue* running_requests = initQueue(arg_queue_size);
    Queue* waiting_requests = initQueue(arg_queue_size);
    pthread_cond_t full_queue, empty_queue, flush;
    pthread_mutex_t m, tm;
    pthread_t t_array[arg_threads];
    pthread_cond_init(&full_queue, NULL);
    pthread_cond_init(&empty_queue, NULL);
    pthread_cond_init(&flush, NULL);
    pthread_mutex_init(&m, NULL);
    pthread_mutex_init(&tm, NULL);
    getargs(&port, argc, argv);

    // 
    // HW3: Create some threads...
    //

    T_args args;
    args.m = &m;
    args.waiting_requests = waiting_requests;
    args.running_requests = running_requests;
    args.empty_queue = &empty_queue;
    args.full_queue = &full_queue;
    args.flush = &flush;
    args.tm = &tm;
    for (int i = 0; i < arg_threads; i++)
    {
        pthread_mutex_lock(args.tm);
        args.id = i;
        pthread_create(&(t_array[i]), NULL, thread_job, (void*)&args);
    }


    listenfd = Open_listenfd(port);

    enum CASE_NUM{
        block, dt, dh, bf, dynamic, random_c
    };
    enum CASE_NUM case_num;
    if(!strcmp(argv[4], "block")){
        case_num = block;
    }
    if(!strcmp(argv[4], "dt")){
        case_num = dt;
    }
    if(!strcmp(argv[4], "dh")){
        case_num = dh;
    }
    if(!strcmp(argv[4], "bf")){
        case_num = bf;
    }
    if(!strcmp(argv[4], "dynamic")){
        case_num = dynamic;
    }
    if(!strcmp(argv[4], "random")){
        case_num = random_c;
    }
    int tmp_fd = 0;


    while (1) {


        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
        //
        // HW3: In general, don't handle the request in the main thread.
        // Save the relevant info in a buffer and have one of the worker threads
        // do the work.
        //
        gettimeofday(&arrival_time, NULL);
        int flag = 0;
        pthread_mutex_lock(&m);

        while(!freeSpace(waiting_requests, running_requests)){
            switch(case_num)
            {
                case block:
                    pthread_cond_wait(&full_queue, &m);
                    break;
                case dt:
                    Close(connfd);
                    flag = 1;
                    break;
                case dh:
                    if(isEmpty(waiting_requests)){
                        Close(connfd);
                        flag = 1;
                        break;
                    }
                    tmp_fd = dequeue(waiting_requests)->data;
                    Close(tmp_fd);
                    break;
                case bf:
                    pthread_cond_wait(&flush, &m);
                    flag = 1;
                    break;
                case dynamic:
                    if(arg_max_queue_size > waiting_requests->max_size){
                        incSize(waiting_requests);
                        incSize(running_requests);
                    }
                    Close(connfd);
                    flag = 1;
                    break;
                case random_c:     ///BONUS
                    if(isEmpty(waiting_requests)){
                        Close(connfd);
                        flag = 1;
                        break;
                    }
                    dropHalf(waiting_requests);
                    break;
            }
            if(flag){
                break;
            }
        }
        if(flag){
            pthread_mutex_unlock(&m);
            continue;
        }
        enqueue(waiting_requests, connfd, arrival_time);
        pthread_cond_signal(&empty_queue);
        pthread_mutex_unlock(&m);


        //////requestHandle(connfd);
        //////Close(connfd);
    }

}


    


 
