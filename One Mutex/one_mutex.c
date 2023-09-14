#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct Node
{
    int data;
    struct Node* next;
};

long thread_count;
void *thread_task(void *rank);

int member(int value);
int insert(int value);
int delete(int value);

struct Node* nodes_list_head = NULL;
pthread_mutex_t com_mutex, mutex1;
pthread_t* thread_handles;

float mMember;   
float mInsert;   
float mDelete;

int tot_operations;
int pop_values;
int MAX_KEY = 65535;
clock_t start, end;

int mem_operations;
int ins_operations;
int del_operations;


int main(int argc, char const *argv[])
{
     
    double time_elapsed;
    thread_count = strtol(argv[1], NULL, 10);
    pop_values = strtol(argv[2], NULL, 10);
    tot_operations = strtol(argv[3], NULL, 10);
    
    mMember = (float) atof(argv[4]);
    mInsert = (float) atof(argv[5]);
    mDelete = (float) atof(argv[6]);

    mem_operations = tot_operations * mMember;
    ins_operations = tot_operations * mInsert;
    del_operations = tot_operations * mDelete;

    pthread_mutex_init(&com_mutex, NULL);
    pthread_mutex_init(&mutex1, NULL);
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    int i;
    for(i=0; i<pop_values; i++){   
        int rdm = rand()%(65535);
        if(!insert(rdm)){
            i--;    
        }
    }
    struct Node* current_node =  nodes_list_head;
    
    long thread;        
    start = clock();
    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, thread_task, (void*) thread);
    }
    printf("Main Thread");
    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }
    
    end = clock();
    // free(thread_handles); 
    // return 0;
    
    time_elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("execution time is : %f\n", time_elapsed);
    // printf("%.10f\n", time_elapsed);
    
    // Clear_Memory();
    pthread_mutex_destroy(&com_mutex);
    pthread_mutex_destroy(&mutex1);
    free(thread_handles);
    return 0;
}

int member(int value){

    struct Node* current_node =  nodes_list_head;
    while(nodes_list_head != NULL && current_node->data< value)
        current_node = current_node->next;

    if (current_node != NULL && current_node->data == value)
    {
        // printf("Node of value %d exists!\n", value);
        return 1;
    }else{
        // printf("Node of value %d does not exist!\n", value);
        return 0;
    }
}

int insert(int value){

    struct Node* current_node =  nodes_list_head;
    struct Node* temp_node = NULL;
    struct Node* change_node = NULL;

    while (current_node != NULL && current_node->data < value)
    {
        change_node = current_node;
        current_node = current_node->next;
    }

    if(current_node == NULL || current_node -> data > value){
        
        temp_node = malloc(sizeof(struct Node));
        temp_node -> data = value;
        temp_node -> next = current_node;
        
        if(change_node == NULL)
            nodes_list_head = temp_node;
        else{
            change_node -> next = temp_node;
            }
        // printf("Inserting Node of Value: %d\n", value);
        return 1;
    }else{
        return 0;
    }    
}

int delete(int value){
    struct Node* current_node =  nodes_list_head;
    struct Node* temp_node;
    struct Node* change_node;

    while (current_node != NULL && current_node->data < value)
    {
        change_node = current_node;
        current_node = current_node->next;
    }

    if (current_node != NULL && current_node ->data == value)
    {
        if(change_node == NULL){
            nodes_list_head = current_node -> next;
        }else{
            change_node ->next = current_node ->next;
        }
        free(current_node);
        // printf("Deleting Node of Value: %d\n", value);
        return 1;
    }
    return 0;
}


void* thread_task(void* rank) {
    int i, val;
    int ops_per_thread = tot_operations/thread_count;

    for (i = 0; i < ops_per_thread; i++) {

        pthread_mutex_lock( &mutex1 ); 
        // printf("OPCOUNT IS:%d thread:%d\n", OPcount, rank);

        pthread_mutex_unlock( &mutex1 );

        float operation_choice = ((float)rand() / RAND_MAX) * (1.0);
        val = rand()%MAX_KEY;
       
        if (operation_choice < mMember) {
            pthread_mutex_lock(&com_mutex);
            member(val);
            pthread_mutex_unlock(&com_mutex);
        } else if (operation_choice < mMember + mInsert) {
            pthread_mutex_lock(&com_mutex);
            insert(val);
            pthread_mutex_unlock(&com_mutex);
        } else {
            pthread_mutex_lock(&com_mutex);
            delete(val);
            pthread_mutex_unlock(&com_mutex);
        }
    }
    return NULL;
}