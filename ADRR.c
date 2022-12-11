#include <stdio.h>
#include <stdlib.h>
#include "RR.h"

#define TRUE 1
#define FALSE 0

int size;

int main() {
    printf("Enter the number of processes: ");
    scanf(" %d", &size);
    Process *arr[size];
    take_input(arr);
    int quantumTime = 1, time, index = 0;
    time = arr[index]->ArrivalTime;


    PQueue *queue = (PQueue *) malloc(sizeof(PQueue));
    queue->first = 0;
    queue->last = -1;
    queue->size = 0;
    Process *queue_arr[size];

    for (int i = 0; i < size;i++)
        queue_arr[i] = NULL;

    printf("Quantum Time = %d\n", quantumTime);
    while ((index < size) && (arr[index]->ArrivalTime <= time))
        enqueue(queue_arr, queue, arr[index++]);
    Process *currentProcess;

    while (queue->size != 0){
        sort(queue_arr,queue->first, queue->first + queue->size - 1, compare_processes_BT);
        quantumTime = queue_arr[queue->first]->RemainingBurstTime;
        if (quantumTime < 20)
            quantumTime = 20;
        for (int i = 0; i < queue->size; i++){
            int RBT;
            currentProcess = dequeue(queue_arr ,queue);
            RBT = currentProcess->RemainingBurstTime;

            if (RBT >= 1.5 * quantumTime) {
                currentProcess->RemainingBurstTime = RBT - quantumTime;
                time += quantumTime;
            } else{
                time += RBT;
                currentProcess->RemainingBurstTime = 0;
            }

            while ((index < size) && (arr[index]->ArrivalTime <= time))
                enqueue(queue_arr, queue, arr[index++]);

            if (currentProcess->RemainingBurstTime == 0){
                currentProcess->CompletionTime = time;
                currentProcess->TurnaroundTime = time - currentProcess->ArrivalTime;
                currentProcess->WaitingTime = currentProcess->TurnaroundTime - currentProcess->BurstTime;
            }
            else
                enqueue(queue_arr, queue, currentProcess);

            printf("####################Time = %d####################\n", time);
            print_process(currentProcess);
        }
    }
    double averageWaitingTime = 0;
    for (int i = 0; i < size; ++i){
        print_process(arr[i]);
        averageWaitingTime += arr[i]->WaitingTime;
    }

    averageWaitingTime /= size;
    printf("\nAverage Waiting Time is: %lf\n", averageWaitingTime);

    return 0;
}

//Take Input of each process and sort them by Arrival Time
void take_input(Process *arr[]){
    printf("Enter the following for each process, seperated by ',', when asked.\n");
    printf("Process ID,Arrival Time,Burst Time\n");
    int ProcessID, ArrivalTime, BurstTime;
    for (int i = 0; i < size; ++i) {
        printf("Enter Process # %d Details:\n", i + 1);
        scanf(" %d,%d,%d", &ProcessID, &ArrivalTime, &BurstTime);
        arr[i] = (Process *) malloc(sizeof(Process));
        arr[i]->ProcessID = ProcessID;
        arr[i]->ArrivalTime = ArrivalTime;
        arr[i]->BurstTime = BurstTime;
        arr[i]->RemainingBurstTime = BurstTime;
        arr[i]->CompletionTime = 0;
    }
    sort(arr, 0, size - 1, compare_processes_AT);
}

int compare_processes_AT(void* a, void* b){
    Process *p_a= (Process *) a;
    Process *p_b= (Process *) b;
    if (p_a->ArrivalTime < p_b->ArrivalTime)
        return TRUE;
    return FALSE;
}

int compare_processes_BT(void* a, void* b){
    Process *p_a= (Process *) a;
    Process *p_b= (Process *) b;
    if (p_a->BurstTime < p_b->BurstTime)
        return TRUE;
    return FALSE;
}

//Merge Sort the process By Arrival Time
void sort(Process *arr[], int start, int end, int (*compare)(void* a, void* b)) {
    if (start < end){
        int mid = start + (end - start)/2;
        sort(arr, start, mid, compare);
        sort(arr, mid + 1, end, compare);
        merge(arr, start, mid, end, compare);
    }
}

void merge(Process *arr[], int start, int mid, int end, int (*compare)(void* a, void* b)){
    int size1 = mid - start + 1;
    int size2 = end - mid;

    Process *arrL[size1];
    Process *arrR[size2];

    for (int i = 0; i < size1; ++i)
        arrL[i] = arr[(start + i) % size];

    for (int j = 0; j < size2; ++j)
        arrR[j] = arr[(mid + 1 + j) % size];

    int i = 0, j = 0, k;
    for (k = 0; i < size1 && j < size2 ; ++k) {
        if ((*compare)((void *) arrL[i], (void *) arrR[j]))
            arr[k] = arrL[i++];
        else
            arr[k] = arrR[j++];
    }

    while (i < size1)
        arr[k++] = arrL[i++];
    while (j < size2)
        arr[k++] = arrR[j++];
}

void print_process(Process *process){
    printf("--------------------Process--------------------\n");
    printf("     Process ID = %d\n", process->ProcessID);
    printf("     Process Arrival Time = %d\n", process->ArrivalTime);
    printf("     Process Burst Time = %d\n", process->BurstTime);
    printf("     Process Remaining Burst Time = %d\n", process->RemainingBurstTime);
    printf("     Process Completion Time = %d\n", process->CompletionTime);
    if (process->CompletionTime){
        printf("     Process TurnAround Time = %d\n", process->TurnaroundTime);
        printf("     Process Waiting Time = %d\n", process->WaitingTime);
    }
    printf("--------------------Process--------------------\n");
}

void enqueue(Process *arr[], PQueue *queue, void *data) {
    queue->last = (queue->last + 1) % size;
    int index = queue->last;
    arr[index] = (Process *) data;
    queue->size++;
}

Process *dequeue(Process *arr[], PQueue *queue){
    Process *res;
    res = arr[queue->first];
    arr[queue->first] = NULL;
    queue->first = (queue->first + 1) % size;
    queue->size--;
    return res;
}
