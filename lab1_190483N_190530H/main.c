#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "helper.h"

// cd Debug; ./lab1_190483N_190530H 10; cd ../


int n = 1000;   //Initial population element count
int m = 10000;  //Total random operations count

float m_member = 0.5;   //Fraction of Member 
float m_insert = 0.25;   //Fraction of Insert
float m_delete = 0.25;   //Fraction of Delete

int thread_count = 1;   //Default thread count is 1
int thread_count_in;    //Thread count input from cmd line
int test_count = 100;
float serial_times[100];
float parallel_times[100];

pthread_t* thread_handles;
pthread_mutex_t com_mutex;
pthread_rwlock_t rwlock;
pthread_mutex_t count_mutex;

/* Shared variables */
Node* head_p = NULL;

double start_time, finish_time, time_elapsed;

int member_count = 0; //Member function call count
int insert_count = 0; //Insert function call count
int delete_count = 0; //Delete function call count


float Execute_serial(int test_id);
void* Thread_serial();
float Execute_parallel_mutex(int thread_count_in, int test_id);
void* Thread_mutex(void* rank);
float Execute_parallel_rw(int thread_count_in, int test_id);
void* Thread_rw_lock(void* rank);
void Print_op_summary();
float calculate_std(float data[], float mean);


int main(int argc, char* argv[])
{
    if (argc != 2) 
    {
        fprintf(stderr, "please provide a command line argument for thread count less than %d\n", MAX_THREADS);
        exit(0);
    }

    thread_count_in = strtol(argv[1], NULL, 10);
    if (thread_count_in <= 0 || thread_count_in > MAX_THREADS)
    {
        fprintf(stderr, "please provide a command line argument for thread count less than %d\n", MAX_THREADS);
        exit(0);
    }

    pthread_mutex_init(&com_mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);
    pthread_mutex_init(&count_mutex, NULL);

    /*########### Serial Block ###########*/

    float serial_time_sum = 0;
    float serial_time = 0;
    float avg_time_serial = 0;
    float std_time_serial = 0;

    for (int i = 1; i <= test_count; i++)
    {
        serial_time = Execute_serial(i);
        serial_time_sum += serial_time;
        serial_times[i - 1] = serial_time;
    }

    avg_time_serial = serial_time_sum / test_count;
    std_time_serial = calculate_std(serial_times, avg_time_serial);

    /*########### End of Serial Block ###########*/

    /*########### Parallel Block - Mutex ###########*/

    float parallel_time_mutex_sum = 0;
    float parallel_time_mutex = 0;
    float avg_time_parallel_mutex = 0;
    float std_time_parallel_mutex = 0;

    for (int i = 1; i <= test_count; i++)
    {
        parallel_time_mutex = Execute_parallel_mutex(thread_count_in, i);
        parallel_time_mutex_sum += parallel_time_mutex;
        parallel_times[i - 1] = parallel_time_mutex;
    }

    avg_time_parallel_mutex = parallel_time_mutex_sum / test_count;
    std_time_parallel_mutex = calculate_std(parallel_times, avg_time_parallel_mutex);

    /*########### End of Parellel Block - Mutex ###########*/

    /*########### Parallel Block - RW Lock ###########*/

    float parallel_time_rw_sum = 0;
    float parallel_time_rw = 0;
    float avg_time_parallel_rw = 0;
    float std_time_parallel_rw = 0;

    for (int i = 1; i <= test_count; i++)
    {
        parallel_time_rw = Execute_parallel_rw(thread_count_in, i);
        parallel_time_rw_sum += parallel_time_rw;
        parallel_times[i - 1] = parallel_time_rw;
    }

    avg_time_parallel_rw = parallel_time_rw_sum / test_count;
    std_time_parallel_rw = calculate_std(parallel_times, avg_time_parallel_rw);

    /*########### End of Parellel Block - RW Lock ###########*/

    printf("##### Overall execution summary #####\n\n");

    printf("Total time elapsed for %d serial executions = %.10f seconds\n", test_count, serial_time_sum);
    printf("Average Time Elapsed for %d serial executions  = %.10f seconds\n", test_count, avg_time_serial);
    printf("Standard deviation of Time Elapsed for %d serial executions  = %.10f seconds\n\n", test_count, std_time_serial);

    printf("Total time elapsed for %d parellel executions (Mutex) = %.10f seconds\n", test_count, parallel_time_mutex_sum);
    printf("Average Time Elapsed for %d parellel executions (Mutex)  = %.10f seconds\n", test_count, avg_time_parallel_mutex);
    printf("Standard deviation of Time Elapsed for %d parellel executions (Mutex)  = %.10f seconds\n\n", test_count, std_time_parallel_mutex);

    printf("Total time elapsed for %d parellel executions (RW Lock) = %.10f seconds\n", test_count, parallel_time_rw_sum);
    printf("Average Time Elapsed for %d parellel executions (RW Lock)  = %.10f seconds\n", test_count, avg_time_parallel_rw);
    printf("Standard deviation of Time Elapsed for %d parellel executions (RW Lock)  = %.10f seconds\n\n", test_count, std_time_parallel_rw);

    pthread_mutex_destroy(&com_mutex);
    pthread_rwlock_destroy(&rwlock);
    pthread_mutex_destroy(&count_mutex);

	return 0;
}


float Execute_serial(int test_id)
{
    head_p = NULL;
    Insert_init(n, head_p);

    start_time = clock();
    Thread_serial();
    finish_time = clock();

    printf("* Serial Execution %d *\n", test_id);
    Print_op_summary();

    time_elapsed = (finish_time - start_time) / CLOCKS_PER_SEC;
    printf("Time Elapsed for serial execution %d  = %.10f seconds\n\n", test_id, time_elapsed);

    Clear_list(head_p);

    return time_elapsed;
}


void* Thread_serial()
{
    int i, val;

    int my_member = 0;
    int my_insert = 0;
    int my_delete = 0;
    
    for (i = 0; i < m; i++) 
    {
        float operation_choice = (rand() % 3 + 1);
        val = rand() % MAX_KEY;
        
        if (operation_choice == 1)
        {
            if (my_member < m * m_member)
            {
                Member(val, head_p);
                my_member++;
            }
            else
            {
                i--;
            }
        }
        else if (operation_choice == 2)
        {
            if (my_insert < m * m_insert)
            {
                Insert(val, &head_p);
                my_insert++;
            }
            else
            {
                i--;
            }
        }
        else 
        {
            if (my_delete < m * m_delete)
            {
                Delete(val, &head_p);
                my_delete++;
            }
            else
            {
                i--;
            }
        }
    }

    member_count += my_member;
    insert_count += my_insert;
    delete_count += my_delete;

    return NULL;
}


float Execute_parallel_mutex(int thread_count_in, int test_id)
{
    thread_count = thread_count_in;
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    head_p = NULL;
    Insert_init(n, head_p);

    int i;

    start_time = clock();

    for (i = 0; i < thread_count; i++)
        pthread_create(&thread_handles[i], NULL, Thread_mutex, (void*)i);

    for (i = 0; i < thread_count; i++)
        pthread_join(thread_handles[i], NULL);

    finish_time = clock();

    printf("* Parallel (Mutex) Execution %d *\n", test_id);
    Print_op_summary();

    time_elapsed = (finish_time - start_time) / CLOCKS_PER_SEC;
    printf("Time Elapsed for parallel (Mutex) execution %d  = %.10f seconds\n\n", test_id, time_elapsed);

    Clear_list(head_p);
    free(thread_handles);

    return time_elapsed;
}


void* Thread_mutex(void* rank) 
{
    printf("Running Thread %1d of %d\n\n", rank, thread_count);

    int i, val;

    int my_member = 0;
    int my_insert = 0;
    int my_delete = 0;

    int ops_per_thread = m / thread_count;

    for (i = 0; i < ops_per_thread; i++) {

        float operation_choice = (rand() % 3 + 1);
        val = rand() % MAX_KEY;

        if (operation_choice == 1) {

            if (my_member < ops_per_thread * m_member) {
                pthread_mutex_lock(&com_mutex);
                Member(val, head_p);
                pthread_mutex_unlock(&com_mutex);
                my_member++;
            }
            else {
                i--;
            }

        }
        else if (operation_choice == 2) {

            if (my_insert < ops_per_thread * m_insert) {
                pthread_mutex_lock(&com_mutex);
                Insert(val, &head_p);
                pthread_mutex_unlock(&com_mutex);
                my_insert++;
            }
            else {
                i--;
            }
        }
        else {

            if (my_delete < ops_per_thread * m_delete) {
                pthread_mutex_lock(&com_mutex);
                Delete(val, &head_p);
                pthread_mutex_unlock(&com_mutex);
                my_delete++;
            }
            else {
                i--;
            }

        }
    }
    return NULL;
}


float Execute_parallel_rw(int thread_count_in, int test_id)
{
    thread_count = thread_count_in;
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    head_p = NULL;
    Insert_init(n, head_p);

    int i;

    start_time = clock();

    for (i = 0; i < thread_count; i++)
        pthread_create(&thread_handles[i], NULL, Thread_rw_lock, (void*)i);

    for (i = 0; i < thread_count; i++)
        pthread_join(thread_handles[i], NULL);

    finish_time = clock();

    printf("* Parallel (RW Lock) Execution %d *\n", test_id);
    Print_op_summary();

    time_elapsed = (finish_time - start_time) / CLOCKS_PER_SEC;
    printf("Time Elapsed for parallel (RW Lock) execution %d  = %.10f seconds\n\n", test_id, time_elapsed);

    Clear_list(head_p);
    free(thread_handles);

    return time_elapsed;
}


void* Thread_rw_lock(void* rank)
{
    printf("Running Thread %1d of %d\n\n", rank, thread_count);

    int i, val;

    int my_member = 0;
    int my_insert = 0;
    int my_delete = 0;

    int ops_per_thread = m / thread_count;

    for (i = 0; i < ops_per_thread; i++)
    {
        int operation_choice = (rand() % 3 + 1);
        val = rand() % MAX_KEY;

        if (operation_choice == 1)
        {
            if (my_member < ops_per_thread * m_member)
            {
                pthread_rwlock_rdlock(&rwlock);
                Member(val, head_p);
                pthread_rwlock_unlock(&rwlock);
                my_member++;
            }
            else
            {
                i--;
            }
        }
        else if (operation_choice == 2)
        {
            if (my_insert < ops_per_thread * m_insert)
            {
                pthread_rwlock_wrlock(&rwlock);
                Insert(val, &head_p);
                pthread_rwlock_unlock(&rwlock);
                my_insert++;
            }
            else
            {
                i--;
            }
        }
        else
        {
            if (my_delete < ops_per_thread * m_delete)
            {
                pthread_rwlock_wrlock(&rwlock);
                Delete(val, &head_p);
                pthread_rwlock_unlock(&rwlock);
                my_delete++;
            }
            else
            {
                i--;
            }
        }
    }

    pthread_mutex_lock(&count_mutex);
    member_count += my_member;
    insert_count += my_insert;
    delete_count += my_delete;
    pthread_mutex_unlock(&count_mutex);

    return NULL;
}


void Print_op_summary()
{
    printf("No. of Member operations  = %d\n", member_count);
    printf("No. of Insert operations  = %d\n", insert_count);
    printf("No. of Delete operations  = %d\n", delete_count);

    int total_op_count = member_count + insert_count + delete_count;

    printf("Total No. of operations  = %d\n", total_op_count);

    member_count = 0;
    insert_count = 0;
    delete_count = 0;
}


float calculate_std(float data[], float mean) 
{
    float std = 0;

    int i;
    for (i = 0; i < test_count; ++i)
    {
        std += pow(data[i] - mean, 2);
    }

    return sqrt(std / test_count);
}
