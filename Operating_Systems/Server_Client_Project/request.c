//
// request.c: Does the bulk of the work for the web server.
// 

#include "segel.h"
#include "request.h"

// requestError(      fd,    filename,        "404",    "Not found", "OS-HW3 Server could not find this file");
void requestError(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg, Node* con_node, t_stats* stats)
{
   char buf[MAXLINE], body[MAXBUF];

   // Create the body of the error message
   sprintf(body, "<html><title>OS-HW3 Error</title>");
   sprintf(body, "%s<body bgcolor=""fffff"">\r\n", body);
   sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
   sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
   sprintf(body, "%s<hr>OS-HW3 Web Server\r\n", body);

   // Write out the header information for this response
   sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
   Rio_writen(fd, buf, strlen(buf));
   printf("%s", buf);

   sprintf(buf, "Content-Type: text/html\r\n");
   Rio_writen(fd, buf, strlen(buf));
   printf("%s", buf);

   sprintf(buf, "Content-Length: %lu\r\n", strlen(body));
   Rio_writen(fd, buf, strlen(buf));
   printf("%s", buf);

   struct timeval timediff;
   timersub(&con_node->handled_time, &con_node->arrival_time, &timediff);
   sprintf(buf, "Stat-Req-Arrival:: %lu.%06lu\r\n", con_node->arrival_time.tv_sec, con_node->arrival_time.tv_usec);
   sprintf(buf, "%sStat-Req-Dispatch:: %lu.%06lu\r\n", buf, timediff.tv_sec, timediff.tv_usec);
   sprintf(buf, "%sStat-Thread-Id:: %d\r\n", buf, stats->id);
   sprintf(buf, "%sStat-Thread-Count:: %d\r\n", buf, stats->jobs_finished);
   sprintf(buf, "%sStat-Thread-Static:: %d\r\n", buf, stats->static_req);
   sprintf(buf, "%sStat-Thread-Dynamic:: %d\r\n\r\n", buf, stats->dynamic_req);
   Rio_writen(fd, buf, strlen(buf));
   printf("%s", buf);

   // Write out the content
   Rio_writen(fd, body, strlen(body));
   printf("%s", body);

}


//
// Reads and discards everything up to an empty text line
//
void requestReadhdrs(rio_t *rp)
{
   char buf[MAXLINE];

   Rio_readlineb(rp, buf, MAXLINE);
   while (strcmp(buf, "\r\n")) {
      Rio_readlineb(rp, buf, MAXLINE);
   }
   return;
}

//
// Return 1 if static, 0 if dynamic content
// Calculates filename (and cgiargs, for dynamic) from uri
//
int requestParseURI(char *uri, char *filename, char *cgiargs) 
{
   char *ptr;

   if (strstr(uri, "..")) {
      sprintf(filename, "./public/home.html");
      return 1;
   }

   if (!strstr(uri, "cgi")) {
      // static
      strcpy(cgiargs, "");
      sprintf(filename, "./public/%s", uri);
      if (uri[strlen(uri)-1] == '/') {
         strcat(filename, "home.html");
      }
      return 1;
   } else {
      // dynamic
      ptr = index(uri, '?');
      if (ptr) {
         strcpy(cgiargs, ptr+1);
         *ptr = '\0';
      } else {
         strcpy(cgiargs, "");
      }
      sprintf(filename, "./public/%s", uri);
      return 0;
   }
}

//
// Fills in the filetype given the filename
//
void requestGetFiletype(char *filename, char *filetype)
{
   if (strstr(filename, ".html")) 
      strcpy(filetype, "text/html");
   else if (strstr(filename, ".gif")) 
      strcpy(filetype, "image/gif");
   else if (strstr(filename, ".jpg")) 
      strcpy(filetype, "image/jpeg");
   else 
      strcpy(filetype, "text/plain");
}

void requestServeDynamic(int fd, char *filename, char *cgiargs, Node* con_node, t_stats* stats)
{
   (stats->dynamic_req)++;
   char buf[MAXLINE], *emptylist[] = {NULL};

   // The server does only a little bit of the header.  
   // The CGI script has to finish writing out the header.

   struct timeval timediff;
   timersub(&con_node->handled_time, &con_node->arrival_time, &timediff);
   sprintf(buf, "HTTP/1.0 200 OK\r\n");
   sprintf(buf, "%sServer: OS-HW3 Web Server\r\n", buf);
   sprintf(buf, "%sStat-Req-Arrival:: %lu.%06lu\r\n", buf, con_node->arrival_time.tv_sec, con_node->arrival_time.tv_usec);
   sprintf(buf, "%sStat-Req-Dispatch:: %lu.%06lu\r\n", buf, timediff.tv_sec, timediff.tv_usec);
   sprintf(buf, "%sStat-Thread-Id:: %d\r\n", buf, stats->id);
   sprintf(buf, "%sStat-Thread-Count:: %d\r\n", buf, stats->jobs_finished);
   sprintf(buf, "%sStat-Thread-Static:: %d\r\n", buf, stats->static_req);
   sprintf(buf, "%sStat-Thread-Dynamic:: %d\r\n", buf, stats->dynamic_req);


   Rio_writen(fd, buf, strlen(buf));

   int pid = Fork();
   if (pid == 0) {
      /* Child process */
      Setenv("QUERY_STRING", cgiargs, 1);
      /* When the CGI process writes to stdout, it will instead go to the socket */
      Dup2(fd, STDOUT_FILENO);
      Execve(filename, emptylist, environ);
   }
   WaitPid(pid, NULL, 0);
}


void requestServeStatic(int fd, char *filename, int filesize, Node* con_node, t_stats* stats)
{
   (stats->static_req)++;
   int srcfd;
   char *srcp, filetype[MAXLINE], buf[MAXBUF];

   requestGetFiletype(filename, filetype);

   srcfd = Open(filename, O_RDONLY, 0);

   // Rather than call read() to read the file into memory, 
   // which would require that we allocate a buffer, we memory-map the file
   srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
   Close(srcfd);

   // put together response
   struct timeval timediff;
   timersub(&con_node->handled_time, &con_node->arrival_time, &timediff);
   sprintf(buf, "HTTP/1.0 200 OK\r\n");
   sprintf(buf, "%sServer: OS-HW3 Web Server\r\n", buf);
   sprintf(buf, "%sContent-Length: %d\r\n", buf, filesize);
   sprintf(buf, "%sContent-Type: %s\r\n", buf, filetype);
   sprintf(buf, "%sStat-Req-Arrival:: %lu.%06lu\r\n", buf, con_node->arrival_time.tv_sec, con_node->arrival_time.tv_usec);
   sprintf(buf, "%sStat-Req-Dispatch:: %lu.%06lu\r\n", buf, timediff.tv_sec, timediff.tv_usec);
   sprintf(buf, "%sStat-Thread-Id:: %d\r\n", buf, stats->id);
   sprintf(buf, "%sStat-Thread-Count:: %d\r\n", buf, stats->jobs_finished);
   sprintf(buf, "%sStat-Thread-Static:: %d\r\n", buf, stats->static_req);
   sprintf(buf, "%sStat-Thread-Dynamic:: %d\r\n\r\n", buf, stats->dynamic_req);
   Rio_writen(fd, buf, strlen(buf));

   //  Writes out to the client socket the memory-mapped file 
   Rio_writen(fd, srcp, filesize);
   Munmap(srcp, filesize);

}

// handle a request
void requestHandle(Node* con_node, t_stats* stats)
{
   ///gettimeofday(&(con_node->handled_time), NULL);
   stats->jobs_finished++;
   int fd = con_node->data;
   int is_static;
   struct stat sbuf;
   char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
   char filename[MAXLINE], cgiargs[MAXLINE];
   rio_t rio;

   Rio_readinitb(&rio, fd);
   Rio_readlineb(&rio, buf, MAXLINE);
   sscanf(buf, "%s %s %s", method, uri, version);

   printf("%s %s %s\n", method, uri, version);

   if (strcasecmp(method, "GET")) {
      requestError(fd, method, "501", "Not Implemented", "OS-HW3 Server does not implement this method", con_node, stats);
      return;
   }
   requestReadhdrs(&rio);

   is_static = requestParseURI(uri, filename, cgiargs);
   if (stat(filename, &sbuf) < 0) {
      requestError(fd, filename, "404", "Not found", "OS-HW3 Server could not find this file", con_node, stats);
      return;
   }

   if (is_static) {
      if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
         requestError(fd, filename, "403", "Forbidden", "OS-HW3 Server could not read this file", con_node, stats);
         return;
      }
      requestServeStatic(fd, filename, sbuf.st_size, con_node, stats);
   } else {
      if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
         requestError(fd, filename, "403", "Forbidden", "OS-HW3 Server could not run this CGI program", con_node, stats);
         return;
      }
      requestServeDynamic(fd, filename, cgiargs, con_node, stats);
   }
}




/**********************************************
               *  OUR CODE *
 **********************************************/


Queue* initQueue(int max_size){
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->max_size = max_size;
    queue->size = 0;
    return queue;
}

int isEmpty(Queue* queue){
    if(queue->size == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int isFull(Queue* queue){
    if(queue->size == queue->max_size){
        return 1;
    }
    else{
        return 0;
    }
}

int enqueue(Queue* queue, int x, struct timeval arr){
    if (isFull(queue)){
        return -1;
    }
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = x;
    new_node->arrival_time = arr;
    new_node->handled_time = arr;
    new_node->next = NULL;
    if(isEmpty(queue)){
        queue->head = new_node;
        queue->tail = new_node;
        (queue->size)++;
        return 0;
    }
    queue->tail->next = new_node;
    queue->tail = new_node;
    (queue->size)++;
    return 0;
}

Node* dequeue(Queue* queue){
    Node* ret = NULL;
    if(isEmpty(queue)){
        return ret;
    }
    ret = queue->head;
    if(queue->size == 1){
        queue->head = NULL;
        queue->tail = NULL;
        (queue->size)--;
        return ret;
    }
    else{
        queue->head = queue->head->next;
    }
    (queue->size)--;
    return ret;
}

int freeSpace(Queue* queue1, Queue* queue2){
    if(queue1->size + queue2->size < queue1->max_size){
        return 1;
    }
    else{
        return 0;
    }
}

void dequeue2(Queue* queue, int x){
    if(isEmpty(queue)){
        return;
    }
    Node* last = NULL;
    Node* iterator = queue->head;
    while(iterator){
        if(iterator->data == x){
            if(last){
                last->next = iterator->next;
            }
            else{
                queue->head = iterator->next;
            }
            if(queue->tail == iterator){
                queue->tail = last;
            }
            free(iterator);
            (queue->size)--;
            return;
        }
        last = iterator;
        iterator = iterator->next;
    }
}

int dequeue3(Queue* queue, int p){
    if(isEmpty(queue)){
        return -1;
    }
    int cnt = 0;
    Node* last = NULL;
    Node* iterator = queue->head;
    while(cnt != p){
        last = iterator;
        iterator = iterator->next;
        cnt++;
    }
    if(last){
        last->next = iterator->next;
    }
    else{
        queue->head = iterator->next;
    }
    if(iterator == queue->tail){
        queue->tail = last;
    }
    int ret = iterator->data;
    free(iterator);
    (queue->size)--;
    return ret;
}

void dropHalf(Queue* queue){
    int random_number;
    int to_drop = (int)((queue->size + 1)/2);
    for (int i = 0; i < to_drop; i++)
    {
        srand(time(NULL));
        random_number = rand() % queue->size;
        int tmp_fd = dequeue3(queue, random_number);
        Close(tmp_fd);
    }
}

void incSize(Queue* queue){
    (queue->max_size)++;
}



