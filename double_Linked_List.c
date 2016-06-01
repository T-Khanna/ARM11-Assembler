#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ARMasm.h"
#include <assert.h>

// Taken form the lecture notes
// this linkedlist  contains tha command as plaintext

typedef struct list_elem {
 char* value;
 struct list_elem *prev;
 struct list_elem *next;
} list_elem;

typedef struct list {
  list_elem *header;
  list_elem *footer;
} list;


typedef list_elem* list_iter;

list_elem *list_alloc_elem(void) {
  list_elem *elem = malloc(sizeof(list_elem));
  if(elem == NULL) {
    perror("list_alloc_elem");
    // exit(EXIT_FAILIURE);
  }

  return elem;
}

void list_free_elem(list_elem *elem) {
  free(elem);
}

void list_init(list *l) {
  l->header = list_alloc_elem();
  l->footer = list_alloc_elem();
  l->header->prev = NULL;
  l->footer->next = NULL;
  l->header->next = l->footer;
  l->footer->prev = l->header;
}

  list_iter list_begin(list *l) {
  return l->header->next;
}

list_iter list_end(list *l) {
  return l->footer;
}

int list_is_internal(list_iter iter) {
  return (iter->prev != NULL) && (iter->next != NULL);
}

void list_insert(list *l, list_iter iter, char* value) {
  list_elem *new_elem = list_alloc_elem();
  new_elem->value = value;

  new_elem->prev = iter->prev;
  new_elem->prev = iter;

  iter->prev->next = new_elem;
  iter->prev = new_elem;
}

void list_remove(list *l, list_iter iter) {
  // removes the elem at the iterator and moves the iterator forawrd
  // Pre, iter is not at ele or header
  assert(list_is_internal(iter));
  iter->prev->next = iter->next;
  iter->next->prev = iter->prev;

  list_elem* temp = iter->next;
  free(iter);
  iter = temp;
}

list_iter list_iter_next(list_iter iter) {
  return iter->next;
}

list_iter list_iter_prev(list_iter iter) {
  return iter->prev;
}

char* list_iter_value(list_iter iter) {
  // cannot be called on head or footer;
  assert(list_is_internal(iter));
  return iter->value;
}

void list_insert_front(list *l, char* value) {
  list_insert(l, list_begin(l), value);

}

void list_insert_back(list *l, char* value) {
  list_insert(l, list_end(l), value);
}

void list_destroy(list *l) {
  list_elem *elem = l->header;
  while (elem != NULL) {
    list_elem *next = elem->next;
    list_free_elem(elem);
    elem = next;

  }

}
