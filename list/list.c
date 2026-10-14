// list/list.c
// 
// Implementation for linked list.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

node_t *getNode(elem value) {
  node_t *mynode = (node_t *)malloc(sizeof(node_t));
  mynode->value = value;
  mynode->next = NULL;
  return mynode;
}

list_t *list_alloc() { 
  list_t *mylist = (list_t *)malloc(sizeof(list_t)); 
  mylist->head = NULL;
  return mylist;
}

void list_free(list_t *l) {
  if (l == NULL) return;
  node_t *curr = l->head;
  node_t *next = NULL;
  while (curr != NULL) {
    next = curr->next;
    free(curr);
    curr = next;
  }
  free(l);
}

void list_print(list_t *l) {
  if (l == NULL) return;
  node_t *curr = l->head;
  while (curr != NULL) {
    printf("%d->", curr->value);
    curr = curr->next;
  }
  printf("NULL\n");
}

char *listToString(list_t *l) {
  char *buf = (char *)malloc(sizeof(char) * 10024);
  buf[0] = '\0';
  char tbuf[20];
  if (l == NULL) return buf;

  node_t *curr = l->head;
  while (curr != NULL) {
    sprintf(tbuf, "%d->", curr->value);
    strcat(buf, tbuf);
    curr = curr->next;
  }
  strcat(buf, "NULL");
  return buf;
}

int list_length(list_t *l) {
  if (l == NULL) return 0;
  int count = 0;
  node_t *curr = l->head;
  while (curr != NULL) {
    count++;
    curr = curr->next;
  }
  return count;
}

void list_add_to_back(list_t *l, elem value) {
  if (l == NULL) return;
  node_t *new_node = getNode(value);
  if (l->head == NULL) {
    l->head = new_node;
    return;
  }
  node_t *curr = l->head;
  while (curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = new_node;
}

void list_add_to_front(list_t *l, elem value) {
  if (l == NULL) return;
  node_t *new_node = getNode(value);
  new_node->next = l->head;
  l->head = new_node;
}

void list_add_at_index(list_t *l, elem value, int index) {
  if (l == NULL || index < 1) return;
  if (index == 1) {
    list_add_to_front(l, value);
    return;
  }
  node_t *curr = l->head;
  int count = 1;
  while (curr != NULL && count < index - 1) {
    curr = curr->next;
    count++;
  }
  if (curr == NULL) return;
  node_t *new_node = getNode(value);
  new_node->next = curr->next;
  curr->next = new_node;
}

elem list_remove_from_back(list_t *l) {
  if (l == NULL || l->head == NULL) return -1;
  if (l->head->next == NULL) {
    elem val = l->head->value;
    free(l->head);
    l->head = NULL;
    return val;
  }
  node_t *curr = l->head;
  while (curr->next->next != NULL) {
    curr = curr->next;
  }
  elem val = curr->next->value;
  free(curr->next);
  curr->next = NULL;
  return val;
}

elem list_remove_from_front(list_t *l) {
  if (l == NULL || l->head == NULL) return -1;
  node_t *temp = l->head;
  elem val = temp->value;
  l->head = l->head->next;
  free(temp);
  return val;
}

elem list_remove_at_index(list_t *l, int index) {
  if (l == NULL || l->head == NULL || index < 1) return -1;
  if (index == 1) {
    return list_remove_from_front(l);
  }
  node_t *curr = l->head;
  int count = 1;
  while (curr->next != NULL && count < index - 1) {
    curr = curr->next;
    count++;
  }
  if (curr->next == NULL) return -1;
  node_t *to_delete = curr->next;
  elem val = to_delete->value;
  curr->next = to_delete->next;
  free(to_delete);
  return val;
}

bool list_is_in(list_t *l, elem value) {
  if (l == NULL) return false;
  node_t *curr = l->head;
  while (curr != NULL) {
    if (curr->value == value) return true;
    curr = curr->next;
  }
  return false;
}

elem list_get_elem_at(list_t *l, int index) {
  if (l == NULL || index < 1) return -1;
  node_t *curr = l->head;
  int count = 1;
  while (curr != NULL) {
    if (count == index) return curr->value;
    curr = curr->next;
    count++;
  }
  return -1;
}

int list_get_index_of(list_t *l, elem value) {
  if (l == NULL) return -1;
  node_t *curr = l->head;
  int count = 1;
  while (curr != NULL) {
    if (curr->value == value) return count;
    curr = curr->next;
    count++;
  }
  return -1;
}