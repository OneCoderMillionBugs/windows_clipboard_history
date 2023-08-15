#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <windows.h>

typedef struct linked_list LinkedList;

// Create new linked list
LinkedList *ListCreate(void);

// Add an element to the linked list
BOOL ListAdd(void *data, LinkedList *list);

// Get element at some index
void *ListGet(UINT index, LinkedList *list);

// Get size of a list
INT ListSize(LinkedList *list);

#endif