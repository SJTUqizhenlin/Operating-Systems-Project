/* buffer.h */
typedef int buffer_item;
#define BUFFER_SIZE 5
buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty, full;
int insert_item(buffer_item item);
int remove_item(buffer_item *item);