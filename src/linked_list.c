#include "linked_list.h"

typedef struct list_node ListNode;

struct list_node
{
    void *data;
    ListNode *next;
};

struct linked_list
{
    ListNode *head;
    UINT size;
};

LinkedList *ListCreate(void)
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->size = 0;

    return list;
}

BOOL ListAdd(void *data, LinkedList *list)
{
    if (list == NULL)
    {
        return FALSE;
    }

    ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
    newNode->data = data;
    newNode->next = NULL;

    ListNode *temp = list->head;
    newNode->next = temp;
    list->head = newNode;

    list->size++;

    return TRUE;
}

void *ListGet(UINT index, LinkedList *list)
{
    if (list == NULL)
    {
        return NULL;
    }

    ListNode *current = list->head;
    UINT counter = 0;

    while (current != NULL && counter < index)
    {
        current = current->next;
        counter++;
    }

    if (current != NULL)
    {
        return current->data;
    }

    return NULL;
}

INT ListSize(LinkedList *list)
{
    if (list == NULL)
    {
        return -1;
    }
    return list->size;
}