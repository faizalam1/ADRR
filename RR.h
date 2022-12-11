#ifndef CRR_RR_H
#define CRR_RR_H

typedef struct Process{
    int ProcessID;
    int ArrivalTime;
    int BurstTime;
    int RemainingBurstTime;
    int CompletionTime;
    int TurnaroundTime;
    int WaitingTime;
} Process;

typedef struct ProcessQueue{
    int first;
    int last;
    int size;
} PQueue;

void print_process(Process *process);

void enqueue(Process *arr[], PQueue *queue, void *data);

Process *dequeue(Process *arr[], PQueue *queue);

int compare_processes_AT(void* a, void* b);

int compare_processes_BT(void* a, void* b);

void merge(Process *arr[], int start, int mid, int end, int (*compare)(void* a, void* b));

void sort(Process *arr[], int start, int end, int (*compare)(void* a, void* b));

void take_input(Process *arr[]);

#endif //CRR_RR_H
