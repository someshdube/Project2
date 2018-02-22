// Header file that contains the processor type structure and the task type structure
// Will also contain the function definations for mode 2
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct T{
    int priority;
    int numberSubTasks; // The number of tasks the main task is broken into
    int arrivalTime; // The arrival time for the tasks
    int *subTaskDurations; // The durations of the sub tasks. Will be as long as the numberSubTasks
    double loadBalanceTask; // The load balancing factor for the task
    struct T *next; // For the purpose of queuing the data
}Task;


typedef struct {
    int numberProcessorsFree;
    int tasksLoaded; // Cannot be more than 64
    double accQueueLength; // The accumulated queue length for the code
    int GLOBAL_TIME; // Keep track of the running time in the system
    int timeUsageArray[64]; // The array with 64 elements with 0 if they are free or the time remaining on them to finish the task
    double runningUsage; // The variable to show the running usage of the processor. Will be sum of cores being used at time cycle
    double mu; // The value of mu used in the calculation of the load balancing factor
    double loadBalanceTotal; // The sum of all load balancing factors used to calc avg
    double remainingTasks; // The number of task remaing to process
    double averageWaitZero; // Avg time for 0
    double averageWaitOne; // The avg wait time for 1
    int countZero; // The count of the number of events with 0 priority
    int countOne; // The count of the number of events with 1 priority
    
}CPU;

typedef struct {
    Task *head; // The top of the queue
    Task *rear; // The end of the queue
}Queue;

// Function Declarations
bool LoadMajorQueue(Queue* superQueue, FILE *fp, double* totalTask, double* totalTime, CPU* processor);
int _CountLines(FILE* fp);
int _HelperForReadingValues(FILE* fp);
void Enqueue(Queue* superQueue, Task* toAdd);
Task* Dequeue(Queue* superQueue);
double _CalculateLoadBalancingFactor(Task* node, CPU* processor);
double LoadBalancingFactorQueue(Queue* superQueue, CPU* processor);
int _LengthQueue(Queue* object);
void Simulation(Queue* superQueue, Queue* zeroQueue, Queue* oneQueue, CPU* multiProcessor);
void Statistics(CPU* processor);