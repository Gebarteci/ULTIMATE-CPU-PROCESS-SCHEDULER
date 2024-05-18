#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

#define MAX_PROCESSES 25
#define RAM_SIZE 2048
#define HIGH_PRIORITY_RAM 512
#define USER_PROCESS_RAM (RAM_SIZE - HIGH_PRIORITY_RAM)
#define QUANTUM_TIME_2 8
#define QUANTUM_TIME_3 16
#define MAX_CPU0 100
#define MAX_CPU1 100

int count = 0; //clock,milisecond


// c = current
int cProcesses, cUser, cHigh, cCPU0,cCPU1, cRam = 0;





typedef struct {
    char process_id[2];
    int arrival_time;
    int priority;
    int burst_time;
    int ram;
    int cpu_rate;
    int r_time; //remaining time
} Process;

Process processes[MAX_PROCESSES];
int n = 0; //Process number,for assigning the variables into processes[]

// Structure for a queue node
typedef struct Node {
    Process data;
    struct Node* next;
} Node;

// Structure for the queue
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Queue q_fcfs;
Queue q_sjf;
Queue q_rr8;
Queue q_rr16;

//queue functions
void initializeQueue(Queue* queue);
int isEmpty(Queue* queue);
void enqueue(Queue* queue, Process process);
Process dequeue(Queue* queue);
void displayQueue(Queue* queue);



// Functions
int readFile(char* filename);
void scheduleProcesses();
void fcfsAlgorithm(Process process);
void sjfAlgorithm(Process process);
void roundRobinAlgorithm(Process process, int quantum_time);
bool checkResources(Process process);
void assignToCPU1(Process process);
void assignToCPU2(Process process);
void printOutputFile();


int main(int argc, char* argv[]) {

    initializeQueue(&q_fcfs);
    initializeQueue(&q_sjf);
    initializeQueue(&q_rr8);
    initializeQueue(&q_rr16);

    
    readFile("input.txt");


    while (1) {

        
        scheduleProcesses();
        count++;
        
    }
    

    

    return 0;
}

bool checkResources(Process process) {

    if (process.priority == 0) {

        if ((cHigh + process.ram) <= HIGH_PRIORITY_RAM && (cCPU0 + process.cpu_rate) <= MAX_CPU0) {
            
            printf("resources are enough\n" );
            return 1;
        }
        else {
            printf("resources are not enough\n");

            return 0;
        }

    }
    else {

        if (cUser + process.ram <= USER_PROCESS_RAM && cCPU1 + process.cpu_rate <= MAX_CPU1) {
           
            printf("resources are enough\n");
            return 1;
        }
        else {
            printf("resources are not enough\n");
            return 0;
        }

    }


};



// Function to read processes from file
int readFile(char* filename) {

    

    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        return 1;
    }

    char line[100]; // input uzunluguna gore ayarlanir

    while (fgets(line, sizeof(line), fp) != NULL) {
        
        if (n == -1) {
            n++;
            continue;
        }

        char processName[2]; //ilk elemanı doğru okumuyor
        char* token = strtok(line, ",");
        if (token != NULL) {
            strncpy(processName, token, sizeof(processName) - 1);
            processName[sizeof(processName) - 1] = '\0'; // Ensure null termination????????
        }
        
        processes[n].arrival_time = atoi(strtok(NULL, ","));
        processes[n].priority = atoi(strtok(NULL, ","));
        processes[n].burst_time = atoi(strtok(NULL, ","));
        processes[n].ram = atoi(strtok(NULL, ","));
        processes[n].cpu_rate = atoi(strtok(NULL, ","));
        processes[n].r_time = 0;


        for (int i = 0; i < 2; i++) {
            processes[n].process_id[i] = processName[i]; // Format process ID as P1, P2, etc.
        }

        n++;

           
        


        
        //printf("%d", processes[n].cpu_rate); //deneme icin

       /* for (int i = 0; i < 2; ++i) {
            printf("processName[%d] = %c\n", i, processName[i]);
        }*/
        

        
    }
     

    fclose(fp);

    return 0;
}



void scheduleProcesses() {

    for (int i = 0; i <= n; i++) {

        if (processes[i].arrival_time == count) {

            switch (processes[i].priority)
            {
            case 0:
                fcfsAlgorithm(processes[i]);
                break;
            case 1:
                sjfAlgorithm(processes[i]);
                break;
            case 2:
                roundRobinAlgorithm(processes[i], QUANTUM_TIME_2);
                break;
            case 3:
                roundRobinAlgorithm(processes[i], QUANTUM_TIME_3);
                break;
            default:
                break;
            }

        }


    }
    
    //Scheduling code using FCFS, SJF, and Round Robin algorithms
}


void fcfsAlgorithm(Process process) {

    if (checkResources(process) == 1) {
        printf("high\n\n");
        cHigh = cHigh + process.ram;
        cCPU0 = cCPU0 + process.cpu_rate;
    }
    else {
        printf("high\n\n");
    }
   
};

void sjfAlgorithm(Process process) {
    if (checkResources(process) == 1) {
        printf("sjf\n\n");
        cUser = cUser + process.ram;
        cCPU1 = cCPU1 + process.cpu_rate;
    }
    else {
        printf("sjf\n\n");
    }
};

void roundRobinAlgorithm(Process process,int quantum_time) {

    if (checkResources(process) == 1) {
        printf("robin\n\n");
        cUser = cUser + process.ram;
        cCPU1 = cCPU1 + process.cpu_rate;
    }
    else {
        printf("robin\n\n");
    }
};



void assignToCPU1(Process process) {};
void assignToCPU2(Process process) {};
void printOutputFile() {};

//-------------------------queue functions

void initializeQueue(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

int isEmpty(Queue* queue) {
    return queue->front == NULL;
}

void enqueue(Queue* queue, Process process) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = process;
    newNode->next = NULL;

    if (isEmpty(queue)) {
        queue->front = queue->rear = newNode;
    }
    else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

Process dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        exit(1);  // Or handle the error appropriately
    }

    Node* temp = queue->front;
    Process process = temp->data;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return process;
}

void displayQueue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }

    Node* temp = queue->front;
    while (temp != NULL) {
        printf("Process ID: %s, Arrival Time: %d, Priority: %d, Burst Time: %d, RAM: %d, CPU Rate: %d, Remaining Time: %d\n",
            temp->data.process_id, temp->data.arrival_time, temp->data.priority, temp->data.burst_time,
            temp->data.ram, temp->data.cpu_rate, temp->data.r_time);
        temp = temp->next;
    }
    printf("\n");
}

