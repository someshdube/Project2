// Somesh Dube
// dube1@purdue.edu
// Partner- Shreya (PURDUE EMAIL)

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include "part2.h"


bool LoadMajorQueue(Queue* superQueue, FILE *fp, double* totalTask, double* totalTime, CPU* processor){ // superQueue holds the head of the queue and calculates mu
    // Reset file pointer to start of file
    fseek(fp, 0, SEEK_SET);
    int numLines = _CountLines(fp);
    processor->remainingTasks = numLines;
    // Reset file pointer
    fseek(fp, 0, SEEK_SET);
    for(int i = 0; i < numLines; i++){
        int arrivalTime = _HelperForReadingValues(fp);
        int priority = _HelperForReadingValues(fp);
        if (priority == 1){
            processor->countOne++;
        }
        else{
            processor->countZero++;
        }
        int numberSubTasks = _HelperForReadingValues(fp);
        totalTask += numberSubTasks; // Add the running total of subtasks
        int * arrayDuarations = malloc(sizeof(int) * numberSubTasks);
        // Allocating memory for the task in the queue
        Task* node  = malloc(sizeof(Task));
        // Malloc for the array in Task
        node->subTaskDurations = malloc(sizeof(int)*numberSubTasks);
        for(int k = 0; k < numberSubTasks; k++){
            node->subTaskDurations[k] = _HelperForReadingValues(fp);
            totalTime += node->subTaskDurations[k]; // Add the running total of the durations of each task
        }
        // Adding the node to the queue
        Enqueue(superQueue, node);
    }
     // This would have built the super node with all the arrival times and everything
}

int _CountLines(FILE* fp){ // Function to count the number of lines in a file and return it
    int counter = 0;
    fseek(fp, 0, SEEK_SET);
    char charIn = getc(fp);
    while (charIn != EOF){
        if (charIn == '\n'){ // End of line
            counter++;
        }
        charIn = fgetc(fp);
    }
    return counter;
}

int _HelperForReadingValues(FILE* fp){ // The fucntion will return an int from the file and goes through the file
    int pos = 0;
    char readTheComma = fgetc(fp);
    
    while( (readTheComma != ' ') && (readTheComma != '\n')){
        pos++;
        readTheComma = fgetc(fp);
    }
    pos++;
    char *stringReturn = malloc((pos+1)*sizeof(char*));
    fseek(fp, -(pos), SEEK_CUR);
    fread(stringReturn, pos, 1, fp);
    int index = pos - 1;
    stringReturn[index] = 0;
    return atoi(stringReturn);
}

void Enqueue(Queue* superQueue, Task* toAdd){
    if (superQueue->head == NULL && superQueue->rear == NULL){
        superQueue->head = superQueue->rear = toAdd;
        return;
    }
    Task* temp = superQueue->rear;
    temp->next = toAdd;
    superQueue->rear = toAdd;
}

Task* Dequeue(Queue* superQueue){
    if (superQueue->head == NULL){
        return NULL;
    }
    if (superQueue->rear == superQueue->head){ // Only one node
        Task* temp = superQueue->head;
        superQueue->head = superQueue->rear = NULL;
        return temp;
    }
    else {
        Task* temp = superQueue->head;
        superQueue->head = superQueue->head->next;
        return temp;
    }
}

double _CalculateLoadBalancingFactor(Task* node, CPU* processor){ // Function to calc the load balancing factor for the node
    double smallestTime = -1;
    double largestTime = -1;
    double muMax = 1;
    double muMin = 1;
    for (int i = 0; i < node->numberSubTasks; i++){
        if (node->subTaskDurations[i] <= muMin && node->subTaskDurations[i != 0 ]){
            muMin = node->subTaskDurations[i];
        }
        if (node->subTaskDurations[i] >= muMax){
            muMax = node->subTaskDurations[i];
        }
    }
    smallestTime = 1 / muMax;
    largestTime = 1/ muMin;
    return ((largestTime - smallestTime) / (1 / processor->mu));
}

double LoadBalancingFactorQueue(Queue* superQueue, CPU* processor){ // Function assigns value of load factor to all the variables
    Task* headCopy = superQueue->head;
    while(headCopy != NULL){ // Go through the nodes one by one and calculate the load factor
        headCopy->loadBalanceTask = _CalculateLoadBalancingFactor(headCopy, processor);
        headCopy = headCopy->next;
    }
}
int _LengthQueue(Queue* object){ // Function gives the length of the queue
    int len = 0;
    Task* temp = object->head;
    while(temp != NULL){
        len++;
        temp = temp->next;
    }
}
void Simulation(Queue* superQueue, Queue* zeroQueue, Queue* oneQueue, CPU* multiProcessor){
    while(multiProcessor->remainingTasks != 0){
        // Time is updated at the end
        // Seeing if any event has arrived at the present time
        // If event has arrived at this time then send it to the respective queue
        Task* tempNode = superQueue->head;
        while(tempNode != NULL){
            if(tempNode->arrivalTime == multiProcessor->GLOBAL_TIME){
            if(tempNode->priority == 0){
                Enqueue(zeroQueue, tempNode);
                Dequeue(superQueue);
            }
            else {
                Enqueue(oneQueue, tempNode);
                Dequeue(superQueue);
            }
            tempNode = tempNode->next;
        }
        }
        
        // Check for events in the 0 queue and if they have requirements that can be met by the avalible resources
        Task* tempZeroNode = zeroQueue->head;
        while (multiProcessor->numberProcessorsFree != 0 && tempZeroNode != NULL){
            // Check the 0 queue for elements to add to the queue           
            if(tempZeroNode->numberSubTasks <= multiProcessor->numberProcessorsFree){ // The task can be added to the queue
                multiProcessor->tasksLoaded = tempZeroNode->numberSubTasks;
                multiProcessor->numberProcessorsFree -= multiProcessor->tasksLoaded;
                int indexControl = 0;
                
                for (int i = 0; i < tempZeroNode->numberSubTasks; i++){ // Setup the timeUsage array
                    while(multiProcessor->timeUsageArray[indexControl] != 0 && indexControl < 64){
                        indexControl++;
                    }
                    multiProcessor->timeUsageArray[indexControl] = tempZeroNode->subTaskDurations[i];
                }
                multiProcessor->loadBalanceTotal += tempZeroNode->loadBalanceTask;
                multiProcessor->remainingTasks--;
                multiProcessor->averageWaitZero += multiProcessor->GLOBAL_TIME - tempZeroNode->arrivalTime;
                Dequeue(zeroQueue);
            }
            tempZeroNode = tempZeroNode->next;
        }

        // Once you are done with the 0 you run through queue 1 in the same manner
        Task* tempOneNode = oneQueue->head;
        while (multiProcessor->numberProcessorsFree != 0 && tempZeroNode != NULL){
            // Check the 1 queue for elements to add to the queue           
            if(tempOneNode->numberSubTasks <= multiProcessor->numberProcessorsFree){ // The task can be added to the queue
                multiProcessor->tasksLoaded = tempOneNode->numberSubTasks;
                multiProcessor->numberProcessorsFree -= multiProcessor->tasksLoaded;
                int indexControl = 0;
                
                for (int i = 0; i < tempOneNode->numberSubTasks; i++){ // Setup the timeUsage array
                    while(multiProcessor->timeUsageArray[indexControl] != 0 && indexControl < 64){
                        indexControl++;
                    }
                    multiProcessor->timeUsageArray[indexControl] = tempOneNode->subTaskDurations[i];
                }
                multiProcessor->loadBalanceTotal += tempZeroNode->loadBalanceTask;
                multiProcessor->remainingTasks--;
                multiProcessor->averageWaitOne += multiProcessor->GLOBAL_TIME - tempOneNode->arrivalTime;
                Dequeue(oneQueue);
            }
            tempOneNode = tempOneNode->next;
        }

        // Update the system and all the processor values
        for (int i = 0; i < 64; i++){ // Update the running usage calculator
            if (multiProcessor->timeUsageArray[i]> 0){
                multiProcessor->runningUsage++;
            }
        }
        // Update the queue length
        multiProcessor->accQueueLength += _LengthQueue(zeroQueue) + _LengthQueue(oneQueue);
        multiProcessor->GLOBAL_TIME++;
        // Reduce time on the array to keep track of changing values
        for (int i = 0; i < 64; i++){ // Update the running usage calculator
            if (multiProcessor->timeUsageArray[i]> 0){
                if((multiProcessor->timeUsageArray[i] - 1) >= 0){
                    multiProcessor->timeUsageArray[i]--;
                }
            }
        }
        int processorNewCount = 0;
        for (int i = 0; i < 64; i++){ // Update the running usage calculator
            if (multiProcessor->timeUsageArray[i]== 0){
                processorNewCount++;
            }
        }
        multiProcessor->numberProcessorsFree = processorNewCount;
        
    }   
}

void Statistics(CPU* processor){ // Function to calculate the statistics
    FILE* fpOut = fopen("output.txt", "w");
    fprintf(fpOut, "Avg wait time for task with priority 0 is %lf\n", (processor->averageWaitZero / processor->countZero));
    fprintf(fpOut, "Avg wait time for task with priority 1 is %lf\n", (processor->averageWaitOne / processor->countOne));
    fprintf(fpOut, "Avg queue length is %lf\n", (processor->accQueueLength / (processor->countOne + processor->countZero)));
    fprintf(fpOut, "Avg load balance is %lf \n", processor->loadBalanceTotal / (processor->countOne + processor->countZero));
    fprintf(fpOut, "Avg CPU Usage is is %lf \n", processor->runningUsage / 64);
}