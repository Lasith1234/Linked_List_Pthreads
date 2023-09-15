#pragma once

typedef struct node {
	int data;
	struct node* next;
} Node;

#ifndef MAX_KEY
#define MAX_KEY 65536
#endif // MAX_KEY

#ifndef MAX_THREADS
#define MAX_THREADS 1024
#endif // MAX_THREADS


// Linked-list operations
int Member(int value, Node* head_p);
int Insert(int value, Node** head_pp);
int Delete(int value, Node** head_pp);

// Helper functions
void Insert_init(int n, Node* head_p);
void Clear_list(Node* head_p);
