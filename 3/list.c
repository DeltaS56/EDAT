#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "list.h"

struct _Node {
  void *info;
  struct _Node *next;
};

typedef struct _Node Node;

struct _List {
  Node *first;
  Node *last;
  int size;
};

Node *_node_new(const void *e) {
  Node *n = malloc(sizeof(Node));
  if (!n) return NULL;

  n->info = (void *)e;
  n->next = NULL;
  return n;
}

List *list_new() {
  List *pl = malloc(sizeof(List));
  if (!pl) return NULL;

  pl->first = NULL;
  pl->last = NULL;
  pl->size = 0;

  return pl;
}

Bool list_isEmpty(const List *pl) {
  if (!pl) return TRUE;

  return (pl->first == NULL) ? TRUE : FALSE;
}

Status list_pushFront(List *pl, const void *e) {
  Node *n = NULL;
  if (!pl || !e) return ERROR;

  n = _node_new(e);
  if (!n) return ERROR;

  /*Remplazamos primera posicion*/
  n->next = pl->first;
  pl->first = n;

  /*En el caso de que este vacia la lista*/
  if (pl->last == NULL) pl->last = n;

  pl->size++;
  return OK;
}

Status list_pushBack(List *pl, const void *e) {
  Node *n = NULL;
  if (!pl || !e) return ERROR;

  n = _node_new(e);
  if (!n) return ERROR;

  if (list_isEmpty(pl)) {
    pl->first = n;
    pl->last = n;
  } else {
    /*antiguo ultimo nodo tiene como siguiente al nuevo nodo*/
    pl->last->next = n;
    pl->last = n;
  }

  pl->size++;
  return OK;
}

void *list_popFront(List *pl) {
  Node *aux = NULL;
  void *e = NULL;

  if (list_isEmpty(pl)) return NULL;

  /*extraemos el primero y su info*/
  aux = pl->first;
  e = aux->info;

  /*convertimos el segundo en el primero*/
  pl->first = aux->next;

  if (pl->first == NULL) pl->last = NULL;

  free(aux);
  aux = NULL;
  pl->size--;

  return e;
}


void *list_popBack(List *pl) {
  Node *aux = NULL;
  void *e = NULL;
  if (list_isEmpty(pl)) return NULL;

  e = pl->last->info;

  if (pl->last == pl->first) {
    free(pl->first);
    pl->first = NULL;
    pl->last = NULL;
  } else { 
    /*Recorrer en O(N) desde el primero para sacar el previo al ultimo*/
    aux = pl->first;
    while (aux->next != pl->last) aux = aux->next;

    free(pl->last);
    pl->last = NULL;

    pl->last = aux;
    pl->last->next = NULL;
  }

  pl->size--;
  return e;
}

void *list_getFront(List *pl) {
  if (list_isEmpty(pl)) return NULL;

  return pl->first->info;
}

void *list_getBack(List *pl) {
  if (list_isEmpty(pl)) return NULL;

  return pl->last->info;
}

void list_free(List *pl) {
  Node *aux = NULL, *next = NULL;
  if (!pl) return;

  aux = pl->first; 
  while (aux) {
    next = aux->next;
    free(aux);
    aux = next;
  }

  free(pl);
  pl = NULL;
}

int list_size(const List *pl) {
  if (!pl) return -1;

  return pl->size;
}

int list_print(FILE *fp, const List *pl, p_list_ele_print f) {
  int printed = 0, f_char;
  Node *aux = NULL;

  if (!fp || !pl || !f) return -1;

  for (aux = pl->first; aux; aux = aux->next) {
    f_char = f(fp, aux->info);
    if (f_char < 0) return -1;
    printed += f_char;
    fprintf(fp, "\n");
  }

  return printed;
} 