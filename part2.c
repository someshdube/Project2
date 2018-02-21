// Somesh Dube
// dube1@purdue.edu
// Partner- Shreya (PURDUE EMAIL)

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>

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
    return; // This would have built the super node with all the arrival times and everything
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
    Queue* temp = malloc(sizeof(temp));
    temp->value = toAdd;
    temp->next = NULL;
    if (superQueue->front == NULL && superQueue->rear == NULL){
        superQueue->front = superQueue->rear = temp;
        return;
    }
    rear->next = temp;
    rear = temp;
}

Task* Dequeue((Queue* superQueue){
    if (superQueue->front == NULL){
        return NULL;
    }
    if (superQueue->rear == superQueue->front){ // Only one node
        Task* temp = superQueue->front;
        superQueue->front = superQueue->rear = NULL;
        return temp;
    }
    else {
        Task* temp = superQueue->front;
        superQueue->front = superQueue->front->next;
        return temp;
    }
}

double _CalculateLoadBalancingFactor(Task* node, CPU* processor){ // Function to calc the load balancing factor for the node
    double smallestTime = -1;
    double largestTime = -1;
    double muMax = 0;
    double muMin = 0;
    for (int i = 0; i < node->numberSubTasks; i++){
        if (node->subTaskDurations[i] <= muMin){
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

double LoadBalancingFactorQueue(Queue* superQueue){ // Function assigns value of load factor to all the variables
    Task* headCopy = superQueue->head;
    while(headCopy != NULL){ // Go through the nodes one by one and calculate the load factor
        headCopy->loadBalanceTask = _CalculateLoadBalancingFactor(headCopy, processor);
        headCopy = headCopy->next;
    }
}

void Simulation(Queue* superQueue, Queue* zeroQueue, Queue* oneQueue, CPU* multiProcessor){
    while(multiProcessor->remainingTasks != 0){
        // Time is updated at the end
        // Seeing if any event has arrived at the present time
        // If event has arrived at this time then send it to the respective queue
        Task* tempNode = superQueue->head;
        while(tempNode->arrivalTime <= multiProcessor->GLOBAL_TIME){
            if(tempNode->)
        }
    }
}