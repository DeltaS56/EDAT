#include "queue.h"
#include "types.h" 
#include "stdlib.h"

#define MAX_QUEUE 2048

struct _Queue {
  void *data[MAX_QUEUE];
  void **front; /*e mas antiguo*/
  void **rear; /*lugar vacio despues del ultimo e que entra*/
};

Queue *queue_new() {
  Queue *q = malloc(1 * sizeof(Queue));
  if(!q) return NULL;

  for(int i = 0; i < MAX_QUEUE; i++) {
    q->data[i] = NULL;
  }

  /*Apuntar a base del array*/
  q->front = q->data;
  q->rear = q->data;

  return q;
}

void queue_free(Queue *q) {
  if (q) free(q);
}

Bool queue_isEmpty(const Queue *q) {
  if (!q) return TRUE;

  return (q->front == q->rear) ? TRUE : FALSE;
}

Status queue_push(Queue *q, void *ele) {
  void **prox_rear;
  if (!q || !ele) return ERROR;

  prox_rear = q->rear + 1;

  /*Si da la vuelta apuntamos a la nueva base*/
  if (prox_rear >= q->data + MAX_QUEUE) 
    prox_rear = q->data;
  if (prox_rear == q->front) 
    return ERROR;
  
  *(q->rear) = ele;
  q->rear = prox_rear;

  return OK;
}

void *queue_pop(Queue *q) {
  void *ele = NULL;
  
  if(!q || queue_isEmpty(q)) return NULL;

  ele = *(q->front);
  *(q->front) = NULL;

  q->front++;
  /*Si supera el limite vuelve a la base*/
  if (q->front >= q->data + MAX_QUEUE)
    q->front = q->data;

  return ele;
}

void *queue_getFront(const Queue *q) {
  if(!q || queue_isEmpty(q)) return NULL;

  return *(q->front);
}

void *queue_getBack(const Queue *q) {
  if(!q || queue_isEmpty(q)) return NULL;

  if(q->rear == q->data) {
    return *(q->data + MAX_QUEUE - 1);
  }
  
  return *(q->rear - 1);
}

size_t queue_size(const Queue *q) {
  if(!q || queue_isEmpty(q) == TRUE) return 0;

  return(MAX_QUEUE + (q->rear - q->front)) % MAX_QUEUE;
}

int queue_print(FILE *fp, const Queue *q, p_queue_ele_print f) {
  int t_chars = 0, n_chars;
  void **aux;
  if (!fp || !q || !f) return -1;

  aux = q->front;
  while(aux != q->rear) {
    n_chars = (f(fp, *aux));
    if (n_chars < 0) return -1;
    t_chars += n_chars;
    
    fprintf(fp, "\n");
        
    aux = q->data + (aux - q->data +1) % MAX_QUEUE;
  }

  fprintf(fp, "\n");

  return t_chars;
}