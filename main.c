# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include "part2.h"

// Main file for the project part 2

int main(int argc, char **argv){
    if (argc == 2){ // The second mode of operation
        // Somesh Dube's Code
        // Open file
        FILE *fpRead = fopen(argv[1], "r");
        if (fpRead == NULL){ // Cannot open file
            printf("Unable to open file.\n");
            return EXIT_FAILURE;
        }

        // Setup CPU and Set up individual Queues
        CPU * multiProcessor;
        multiProcessor = malloc(sizeof(CPU));
        // Initial values for the processor
        multiProcessor->numberProcessorsFree = 0;
        multiProcessor->tasksLoaded = 0;
        multiProcessor->accQueueLength = 0;
        multiProcessor->GLOBAL_TIME = 0;
        for(int i = 0; i < 64; i++){
            multiProcessor->timeUsageArray[i] = 0;
        }
        multiProcessor->runningUsage = 0;
        multiProcessor->mu = 0;
        multiProcessor->loadBalanceTask = 0;
        multiProcessor->remainingTasks = 0
        
        // Setup of the queue
        Queue* zeroQueue;
        zeroQueue->head = NULL;
        zeroQueue->rear = NULL;
        Queue* oneQueue = NULL;
        oneQueue->head = NULL;
        oneQueue->rear = NULL;
        
        // Create super queue and make it empty
        // Load the queue and get values required to calculate mu
        double tasksSum = 0;
        double timeSum = 0;
        double *totalSubTasks = tasksSum;
        double *totalSubTime = timeSum;
        
        Queue * superQueue;
        superQueue->head = NULL;
        superQueue->rear = NULL;
        LoadMajorQueue(superQueue, fp, totalSubTasks, totalSubTime, multiProcessor);

        
        // Calculate MU constant based on the contents of the queue
        double MU = tasksSum / timeSum; // Value of MU for calculation of the load balancing factor
        multiProcessor->mu = MU; // Giving the value to the processor

        // Calculate load balancing factor for all the elements in the superQueue
        LoadBalancingFactorQueue(superQueue);
    }
}