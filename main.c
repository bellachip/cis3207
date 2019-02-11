
//Hee Won Yang
//CIS LAB 1: Discrete event simulator
// Works cited: Geeks for Geeks, http://www.lawja.io/3207/lab1/fileRead.c lab instructor: Jake Lawrence
//
//  Created by Belle on 1/21/19.
//  Copyright © 2019 Belle. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//global variables
float SEED = 0;
int INIT_TIME = 0;
int  FIN_TIME = 0;
int ARRIVE_MIN = 0;
int ARRIVE_MAX = 0;
float QUIT_PROB = 0;
int CPU_MIN = 0;
int CPU_MAX = 0;
int DISK1_MIN = 0;
int DISK1_MAX = 0;
int DISK2_MIN = 0;
int DISK2_MAX = 0;
int count =1;

#define ARRIVAL 0
#define FINISH 1
#define DISK_ARRIVAL 2
//#define DISK2_ARRIVAL 3
#define DISK_FINISH 3
//#define DISK2_FINISH 4
#define SIM_END 4
//disk cpu status
#define REST 0
#define FULL 1

#define LINE_LENGTH 1024


//prototypes
int timeassign(int INIT_TIME, int FIN_TIME);

//event struct
typedef struct event {
    int jobnum;
    int time;
    int eventtype;
    //struct node* next;
    //int pqsize;
} event;

//create node struct
typedef struct node{
    struct event job;
    struct node* next;
} node;

//FIFO Queue Function
void enQueue (node**, event, int* a);
event deQueue(node **, int* a);
//Prioirity Queue Functions
void push(node**, event, int* a);
event pop(node**, int* a);
//create event
event genEvent(int a, int b, int c);
//Handler functions
void CPUHandler(event);
void DISKHandler(event);

int cpuStatus = REST;
int disk1Status = REST;
int disk2Status = REST;

int currTime= 0;
int randTime=0;
int cpuN =0;
int disk1N =0;
int disk2N=0;
int eventN =0;
int countJobs =1;


node* cpuQ= NULL;
node* disk1Q = NULL;
node* disk2Q= NULL;
node* eventQ = NULL;
event task;


//file reader variables
const char *conf_types[12] = {"SEED", "INIT_TIME", "FIN_TIME", "ARRIVE_MIN", "ARRIVE_MAX", "QUIT_PROB", "CPU_MIN", "CPU_MAX","DISK1_MIN", "DISK1_MAX", "DISK2_MIN", "DISK2_MAX"};

int conf_values[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};



int main(){
    //File reading
    FILE *configFile = NULL;
    
    configFile = fopen("var_init.txt", "r");
    
    
    if (configFile != NULL)
    {
        char *token;
        char searchString[LINE_LENGTH];
        float searchValue;
        char line[LINE_LENGTH];
        int i;
        
        while (fgets(line, LINE_LENGTH, configFile) != NULL)
        {
            token = strtok(line, " ");
            
            if (token != NULL)
            {
                strcpy(searchString, token);
            }
            else
            {
                continue;
            }
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                searchValue = atof(token);
            }
            else
            {
                searchValue = 0;
            }
            
            for (i = 0; i < 12; i++)
            {
                if (strcmp(searchString, conf_types[i]) == 0)
                {
                    conf_values[i] = (int)searchValue;
                }
            }
        }
    }
    
    for (int k = 0; k < 12; k++)
    {
        printf("%s: %d\n", conf_types[k], conf_values[k]);
    }
    
    SEED = conf_values[0];
    INIT_TIME = conf_values[1];
    FIN_TIME = conf_values[2];
    ARRIVE_MIN = conf_values[3];
    ARRIVE_MAX = conf_values[4];
    QUIT_PROB = conf_values[5];
    CPU_MIN = conf_values[6];
    CPU_MAX = conf_values[7];
    DISK1_MIN = conf_values[8];
    DISK1_MAX = conf_values[9];
    DISK2_MIN = conf_values[10];
    DISK2_MAX = conf_values[11];
    
    srand(SEED);
    currTime = INIT_TIME;
    
    //adding two events to priority queue, job 1 arrival and simulation finished.
    push(&eventQ, genEvent(INIT_TIME, 1, ARRIVAL), &eventN);
    push(&eventQ, genEvent(FIN_TIME, 0, SIM_END), &eventN);
    
    // printf("Done.\n");
    //printf("eventNodes: %d\n" , eventNodes );
    while(eventN !=0 && currTime < FIN_TIME) {
        //printf("Job %d created, finishes at %d.\n", task.jobnum, randTime);
        task = pop(&eventQ, &eventN); //push task
        currTime = task.time;
        switch(task.eventtype){
            case ARRIVAL:
                //printf("Job %d created, arives at %d.\n", task.jobnum,  randTime);
                CPUHandler(task);
                break;
            case FINISH:
                CPUHandler(task);
                break;
                /*
            case DISK1_ARRIVAL:
                DISKHandler(task);
                break;*/
            case DISK_ARRIVAL:
                DISKHandler(task);
                break;
            case DISK_FINISH:
                DISKHandler(task);
                break;
            //case DISK2_FINISH:
              //  DISKHandler(task);
               // break;
            case SIM_END:
                printf("Simulation_end.\n");
                break;
        }
    }
    return 0;
    
}


//Timer
int timeassign (int MINT, int MAXT){
    return (rand()% (MAXT - MINT +1)) + MAXT;
}

//The function enQueue is a function that creates a linked list and adds an item in the linked list.
//FIFO Q
void enQueue(node** head, event item, int* total)
{
    // Create a new LL node
    node* new_node = (node*)malloc(sizeof(node));
    new_node -> job = item;
    new_node -> next = NULL;
    
    node *last = *head;
    (*total)++;
    if (*head == NULL){
        *head = new_node;
        return;
    }
    
    while(last->next !=NULL){
        last = last->next;
        last->next = new_node;
        return;
    }
}


// Function to remove a item from given FIFO q
event  deQueue(node** head, int* total)
{
    node *temp = *head;
    *head = (*head)-> next;
    event job = temp->job;
    free(temp);
    (*total)--;
    return job;
}

//Prioirity Queue. pops the data with the lowest priority which is time
event pop(node** head, int* total) {
    node *temp = *head;
    *head = (*head)->next;
    event job = temp ->job;
    free(temp);
    (*total)--;
    
    return job;
}

void push(node** head, event item, int* total){
    node* temp = (node*)malloc(sizeof(node));
    temp->job = item;
    temp ->next = NULL;
    int priority = temp->job.time;
    (*total)++;
    node *last = *head;
    
    if (*head == NULL){
        *head = temp;
        return;
    }else if(priority < last->job.time){
        temp->next = last;
        *head = temp;
        return;
    }
    
    while (last->next != NULL && priority >= last->next->job.time){
        last = last->next;
    }
    
    temp->next = last->next;
    last->next = temp;
    return;
}
//creates the event.
event genEvent(int time, int jobnum, int type){
    event job;
    job.time = time;
    job.jobnum = jobnum;
    job.eventtype= type;
    return job;
}

void CPUHandler(event task){
    if (task.eventtype== ARRIVAL){
        printf("[%d] Job %d ARRIVES at CPU at TIME %d.\n" , currTime, task.jobnum, currTime);
        randTime = timeassign(ARRIVE_MIN,  ARRIVE_MAX) + currTime;
        countJobs++;
        //int num=  task.jobnum +1;
        push(&eventQ, genEvent(randTime, countJobs, ARRIVAL), &eventN);
        enQueue(&cpuQ, task, &cpuN);// job sent to the CPU
        //event job = pop(&evenQ, &cpuNodes);
        
        if (cpuStatus == REST){
            task = deQueue(&cpuQ, &cpuN); //remove the task from cpuQ
            randTime = timeassign(CPU_MIN,  CPU_MAX) + currTime; //assign randTime to the new event
            push(&eventQ, genEvent(randTime, task.jobnum, FINISH), &eventN); //create new event in PQ: FINISH
            cpuStatus = REST;
            printf("CPU now running job %d finishes at %d.\n" , task.jobnum , randTime);
        }/* else if(&CpuNode == 0) {//isempty
          task = pop(&CPU, &cpuNodes);
          push(&eventQ, createEvent(randTime, task.jobnum, FINISH), &eventNodes; //create job next job
          prinf("cpu is busy)
          }*/
    }else{
        printf("[%d] CPU DONE. Job ID: %d \n" , currTime, task.jobnum);
        //printf("[%d] Job %d FINISHES at CPU at TIME %d.\n" , currentTime, task.jobnum, currentTime); //CPU DONE
        cpuStatus = REST;
        //task = deQueue(&CPU, &cpuNodes);
        int exit= rand() < QUIT_PROB * ((double)RAND_MAX +1.0);
        if (exit){
            printf("[%d] Job %d EXITS at TIME %d.\n" ,currTime, task.jobnum, currTime);
            //task= deQueue(&cpuQ, &cpuN);
            
        } else {
            randTime = timeassign(ARRIVE_MIN,ARRIVE_MAX) + currTime;
            push(&eventQ, genEvent(randTime, task.jobnum, DISK_ARRIVAL), &eventN);
            /*int send=0;
            if (disk1Nodes < disk2Nodes){
                send=1;
            }
            else if (disk1Nodes > disk2Nodes){
                send=2;
            } else  if (disk1Nodes == disk2Nodes){
                send=2;
            }
            if (send==1){
                randTime = timeassign(ARRIVE_MIN,ARRIVE_MAX) + currTime;
                push(&eventQ, createEvent(randTime, task.jobnum, DISK1_ARRIVAL), &eventNodes);
                printf("Sending Job %d to to disk1 arrives at %d\n", task.jobnum, randTime);
                
            } else if (send==2){
                randTime = timeassign(ARRIVE_MIN,ARRIVE_MAX) + currTime;
                push(&eventQ, createEvent(randTime, task.jobnum, DISK2_ARRIVAL), &eventNodes);
                printf("Sending Job %d to to disk2 arrives at %d.\n", task.jobnum, randTime);
                
                
            }*/
        }
    }
}




void DISKHandler(event task){
    
    //if (task.eventtype == DISK_ARRIVAL){
    // printf("[%d] Job %d ARRIVES at DISK1 at TIME %d.\n" ,currentTime,  task.jobnum, currentTime);
    int ident = 0;
    if (task.eventtype == DISK_ARRIVAL){
        
        printf("[%d] Job %d ARRIVES at DISK at TIME %d. \n", currTime, task.jobnum, currTime);
        if (disk1N < disk2N){
            printf("sending job %d to disk1.\n", task.jobnum);
            ident = 1;
            
        }
        else if (disk1N > disk2N){
            ident=2;
            printf("sending job %d to disk2.\n", task.jobnum);
        } else if (disk1N == disk2N){
            ident = (rand() < 0.5 *((double)RAND_MAX +1.0)) +1 ;
        }
        if (ident==1){
            
            //printf("[%d] Job %d ARRIVES at DISK at TIME %d. \n", currTime, task.jobnum, currTime);
            enQueue(&disk1Q, task, &disk1N);
            printf("Job %d is sent TO DISK1.\n", task.jobnum);
            if (disk1Status == REST){
                event job = pop(&disk1Q, &disk1N);
                randTime = timeassign(DISK1_MIN,  DISK1_MAX) + currTime;
                //printf("job finishes at %d. \n", randTime);
                push(&eventQ, genEvent(randTime, job.jobnum, DISK_FINISH), &eventN);
                disk1Status = FULL;
                //printf("debug");
            }
        } else if (ident==2){
            //printf("[%d] Job %d ARRIVES at DISK2 at TIME %d.\n",  currTime, task.jobnum, currTime);
            enQueue(&disk2Q, task, &disk2N);
            printf("Job %d is sent TO DISK2.\n", task.jobnum);
            if (disk2Status == REST){
                event job = pop(&disk2Q, &disk2N);
                randTime = timeassign(DISK2_MIN,  DISK2_MAX) + currTime;
                push(&eventQ, genEvent(randTime, job.jobnum, DISK_FINISH), &eventN);
                disk2Status = FULL;
                //printf("debug");
            }
        }
    } else {
        randTime = timeassign(ARRIVE_MIN, ARRIVE_MAX) + currTime;
        push(&eventQ, genEvent(randTime, task.jobnum, ARRIVAL), &eventN);
        int ident =0;
        if (disk1N < disk2N){
            //printf("sending job %d to disk1.\n", task.jobnum);
            ident = 1;
            
        }
        else if (disk1N > disk2N){
            ident=2;
            //printf("sending job %d to disk2.\n", task.jobnum);
        } else {
            ident = (rand() < 0.5 *((double)RAND_MAX +1.0)) +1 ;
            
        }
        
        if (ident==1){
           printf("[%d] Job %d finished I/O at Disk 1.\n",  currTime, task.jobnum );
           // deQueue(&disk1Q, &disk1N);
            disk2Status = REST;
        } else {
            printf("[%d] Job %d finished I/O at Disk 2. \n", currTime , task.jobnum);
            //deQueue(&disk2Q, &disk2N);
            disk2Status = REST;
            
        }
    }
}
/*
        
        
        else (task.eventtype == DISK1_FINISH){
            printf("[%d] Job %d FINISHES at DISK1 at TIME %d.\n" , currTime, task.jobnum, currTime);
            printf("Job %d EXITS. \n" , task.jobnum);
            
            randTime = timeassign(ARRIVE_MIN, ARRIVE_MAX) + currTime;
            push(&eventQ, createEvent(randTime, task.jobnum, ARRIVAL), &eventNodes);
            event job = deQueue(&disk2Q, &disk2Nodes);
            disk1State=REST;
            
        }
        if (task.eventtype == DISK2_FINISH){
            printf("[%d] Job %d FINISHES at DISK2 at TIME %d.\n" , currTime, task.jobnum, currTime);
            printf("Job %d EXITS. \n" , task.jobnum);
            disk2State=REST;
            randTime = timeassign(ARRIVE_MIN, ARRIVE_MAX) + currTime;
            push(&eventQ, createEvent(randTime, task.jobnum, ARRIVAL), &eventNodes);
            event job = deQueue(&disk2Q, &disk2Nodes);
            
            disk2State=REST;
            
        }
    }
 */
    
    


