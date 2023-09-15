#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"

int Member(int value, Node* head_p)
{
	Node* cur_p = head_p;
    
	while (cur_p != NULL && cur_p->data < value)
	{
		cur_p = cur_p->next;
	}

	if (cur_p == NULL || cur_p->data > value)
	{
        //printf("%d is not a member in linked-list\n", value); 
		return 0;
	}
	else 
	{
        //printf("%d is a member in linked-list\n", value); 
		return 1;
	}
}

int Insert(int value, Node** head_pp)
{
    Node* cur_p = *head_pp;
    Node* prev_p = NULL;
    Node* temp_p;
    
    while (cur_p != NULL && cur_p->data < value) 
    {
        prev_p = cur_p;
        cur_p = cur_p->next;
    }

    if (cur_p == NULL || cur_p->data > value) 
    {
        temp_p = malloc(sizeof(Node));
        temp_p->data = value;
        temp_p->next = cur_p;

        if (prev_p == NULL)
        {
            *head_pp = temp_p;
        }
        else
        {
            prev_p->next = temp_p;
        }

        //printf("%d is inserted into the linked-list\n", value); 
        return 1; 
    }
    else 
    {
        //printf("%d already exsists in the linked-list\n", value);
        return 0;
    }
}

int Delete(int value, Node** head_pp)
{
    Node* cur_p = *head_pp;
    Node* prev_p = NULL;

    while (cur_p != NULL && cur_p->data < value) 
    {
        prev_p = cur_p;
        cur_p = cur_p->next;
    }

    if (cur_p != NULL && cur_p->data == value) 
    {
        if (prev_p == NULL) 
        {
            *head_pp = cur_p->next;
            free(cur_p);
        }
        else 
        {
            prev_p->next = cur_p->next;
            free(cur_p);
        }

        //printf("%d is deleted from the linked-list\n", value); 
        return 1;
    }
    else 
    {
        //printf("%d is not in the linked-list\n", value);
        return 0;
    }
}

void Insert_init(int n, Node* head_p) 
{
    for (int i = 0; i < n; i++)
    {
        int r = rand() % MAX_KEY;

        if (!Insert(r, &head_p))
        {
            i--;
        }
    }
}

void Clear_list(Node* head_p) 
{
    Node* cur_p;
    Node* next_p;

    if (head_p == NULL) return;

    cur_p = head_p;
    next_p = cur_p->next;

    while (next_p != NULL) 
    {
        free(cur_p);
        cur_p = next_p;
        next_p = cur_p->next;
    }

    free(cur_p);
}
