#ifndef __QUEUE_H_
#define __QUEUE_H_

typedef struct queue_node {
    char hash[41];
    struct queue_node* next;
} queue_node_t;

typedef struct queue {
    int size;
    queue_node_t* head;
} queue_t;

void my_init(queue_t* queue) {
    if (queue == NULL) { exit(1); }
    queue->size = 0;
    queue->head = NULL;
}

void my_push(queue_t* queue, const char* hash) {
    queue_node_t* new_node = malloc(sizeof(queue_node_t));
    if (new_node == NULL) { printf("enqueue malloc fail.\n"); exit(1); }
    memset(new_node, 0, sizeof(queue_node_t));

    memcpy(new_node->hash, hash, strlen(hash));
    new_node->next = NULL;

    if (queue->head == NULL) {
        queue->head = new_node;
        printf("222\n");
    } else {
        printf("333\n");
        queue_node_t* tmp = queue->head;
        while (tmp->next != NULL) {
            if (strcmp(tmp->hash, hash) == 0) { free(new_node); return; }
            tmp = tmp->next;
        }
        printf("444\n");
        tmp->next = new_node;
    }
    queue->size += 1;
    
}

void my_pop(queue_t* queue) {
    if (queue->head == NULL) { return; }
    queue->size -= 1;
    queue_node_t* tmp = queue->head;
    queue->head = queue->head->next;
    free(tmp);
}

void my_top(queue_t* queue, char* hash) {
    if (hash == NULL) { exit(1); }
    if (queue->head == NULL) { return; }
    memcpy(hash, queue->head->hash, HASH_LENGTH);
    hash[HASH_LENGTH] = '\0';
}

int my_empty(queue_t* queue) {
    return queue->size == 0;
}

#endif