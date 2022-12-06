#include <stdio.h>
#include <stdlib.h>
#include "RR.h"

int main() {
    int size;
    printf("Enter the number of processes: ");
    scanf(" %d", &size);
    Process *arr[size];
    take_input(arr, size);
    int quantumTime = 1, time, index = 0;
    time = arr[index]->ArrivalTime;
    LinkedList *queue = (LinkedList *) malloc(sizeof(LinkedList));
    queue->head = (Node *) malloc(sizeof(Node));
    queue->tail = queue->head;
    queue->size = 0;
    printf("Quantum Time = %d\n", quantumTime);
    while ((index < size) && (arr[index]->ArrivalTime <= time))
        enqueue(queue, arr[index++]);
    Process *currentProcess;
    
    while (queue->head != NULL){
        int RBT;
        currentProcess = dequeue(queue)->data;
        RBT = currentProcess->RemainingBurstTime;

        if (RBT >= quantumTime) {
            currentProcess->RemainingBurstTime = RBT - quantumTime;
            time += quantumTime;
        } else{
            time += RBT;
            currentProcess->RemainingBurstTime = 0;
        }

        while ((index < size) && (arr[index]->ArrivalTime <= time))
            enqueue(queue, arr[index++]);

        if (currentProcess->RemainingBurstTime == 0){
            currentProcess->CompletionTime = time;
            currentProcess->TurnaroundTime = time - currentProcess->ArrivalTime;
            currentProcess->WaitingTime = currentProcess->TurnaroundTime - currentProcess->BurstTime;
        }
        else
            enqueue(queue, currentProcess);

        printf("####################Time = %d####################\n", time);
        print_process(currentProcess); // Comment This Line (For Debugging)
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
void take_input(Process *arr[], int size){
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
    sort_by_AT(arr, 0, size - 1);
}

//Merge Sort the process By Arrival Time
void sort_by_AT(Process *arr[], int start, int end) {
    if (start < end){
        int mid = start + (end - start)/2;
        sort_by_AT(arr, start, mid);
        sort_by_AT(arr, mid + 1, end);
        merge(arr, start, mid, end);
    }
}

void merge(Process *arr[], int start, int mid, int end){
    int size1 = mid - start + 1;
    int size2 = end - mid;

    Process *arrL[size1];
    Process *arrR[size2];

    for (int i = 0; i < size1; ++i)
        arrL[i] = arr[start + i];

    for (int j = 0; j < size2; ++j)
        arrR[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k;
    for (k = 0; i < size1 && j < size2 ; ++k) {
        if (arrL[i]->ArrivalTime < arrR[j]->ArrivalTime)
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

void enqueue(LinkedList *queue, void *data) {
    if (queue->size == 0){
        queue->head = (Node *) malloc(sizeof(Node));
        queue->head->data = (Process *) data;
        queue->head->next = NULL;
        queue->tail = queue->head;
    } else if (queue->head == queue->tail){
        queue->head->next = (Node *) malloc(sizeof(Node));
        queue->head->next->data = (Process *) data;
        queue->head->next->next = NULL;
        queue->tail = queue->head->next;
    } else{
        queue->tail->next = (Node *) malloc(sizeof(Node));
        queue->tail = queue->tail->next;
        queue->tail->data = (Process *) data;
        queue->tail->next = NULL;
    }
    queue->size++;
}

Node *dequeue(LinkedList *queue){
    Node *result = queue->head;
    if (queue->head == queue->tail)
        queue->tail = queue->head->next;
    queue->head = queue->head->next;
    queue->size--;
    return result;
}
