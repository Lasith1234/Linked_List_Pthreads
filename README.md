
# Concurrent Programming - Lab 01

## Linked List Implementation Using Pthreads

To compile the program, run the following command from the directory where the files are located.  
`gcc -o main  main.c operations.c -lpthread -lm`

To execute the compiled program run the following command.  
**(Provide an integer as the THREAD_COUNT)**  
`./main THREAD_COUNT`

*The fractions `m_member`, `m_insert`, and `m_delete` defined in `main.c` holds the execution ratios of each Linked List Operation. Those can be updated accordingly to change the number of times each Linked List Operation is executed for a test case.*
