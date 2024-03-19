#include "buffer.h"
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//buffer item array that is used by both producer and consumer threads
BUFFER_ITEM buffer[BUFFER_SIZE];

//global counter variable to save how many items are in the buffer array
int counter = 0;

//used for mutual exclusion for the buffer pool 
pthread_mutex_t mutex;

//manages available resources
sem_t empty;

//manages number of resources occupied
sem_t full;

//inserts an inputted BUFFER_ITEM into the buffer array
int insert_item(BUFFER_ITEM item) {
    
    if(counter < BUFFER_SIZE) {

        buffer[counter] = item;
        counter++;
        return 0;
    }
    else {
        printf("Buffer is full, cannot insert item \n");
        return -1;
    }
}

//removes an inputted BUFFER_ITEM from the buffer array
int remove_item(BUFFER_ITEM item) {
    //boolean to check if the item was found in the array
    int itemFound = 0;

    if (counter > 0) {
        // Create a temporary array to store valid items
        BUFFER_ITEM tempArray[BUFFER_SIZE];
        int tempCounter = 0;

        //goes through buffer array and gets the checksum of each item
        for (int i = 0; i < counter; i++) {
            BUFFER_ITEM myItem = buffer[i];
            uint16_t checksum = 0;

            for (int x = 0; x < DATA_SIZE; x++) {
                checksum += myItem.data[x];
            }

            //checks if the buffer item in loop's checksum is the same as the calculated checksum a 
            if (myItem.cksum == checksum) {

                //checks if the buffer item in loop's checksum is the same as the inputte buffer item's checksum
                if (myItem.cksum != item.cksum) {
                    // This item is valid; store it in the temp array
                    tempArray[tempCounter] = myItem;
                    tempCounter++;
                } else {
                    // This item is the one to be removed, set its cksum to 0
                    myItem.cksum = 0;
                    itemFound = 1;
                }
            }
        }

        // Update the main buffer and counter
        for (int i = 0; i < tempCounter; i++) {
            buffer[i] = tempArray[i];
        }

        counter = tempCounter;

        if (itemFound == 0) {
            printf("Item not found\n");
            return -1;
        } else {
            return 0;
        }
    } else {
        printf("Buffer empty, cannot remove item\n");
        return -1;
    }
}

//function for producer threads
//creates the data and checksum for a buffer item and then uses the insert_item function to add it to buffer array
//uses the semaphores and mutex for synchronization in order to prevent race conditions
//prints the checksum of the item 
void *producer(void *param){
    
    int delay = (rand() % (atoi(param) / 2)) + 1;

    while(1) {

        BUFFER_ITEM item;
        uint16_t sum = 0;

        sleep(delay);
        for(int i = 0; i < DATA_SIZE; i++) {
            item.data[i] = (uint8_t)(rand() % 256);
            sum += (uint16_t)(item.data[i]);
        }

        item.cksum = sum;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if(insert_item(item)) {
        }
        else
            printf("producer produced %d\n", item.cksum);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

//function for consumer threads
//find random item from buffer, uses the remove_item function to remove it from buffer array, checks if caclulated checksum is equal to item checksum
//prints the checksum of item
//uses the semaphores and mutex for synchronization in order to prevent race conditions
void *consumer(void *param) {
    int delay = (rand() % (atoi(param) / 2)) + 1;

    while (1) {

        sleep(delay);

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (counter == 0) {
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            continue;
        }

        int randomPos = rand() % counter;
        BUFFER_ITEM item = buffer[randomPos];

        int remove_result = remove_item(item);

        // Call remove_item to select and remove an item
        if (remove_result == 0) {

            if (item.cksum != 0) {
                uint16_t computedChecksum = 0;
                for (int i = 0; i < DATA_SIZE; i++) {
                    computedChecksum += (uint16_t)(item.data[i]);
                }

                if (computedChecksum != item.cksum) {
                    printf("Computed Checksum (%d) did not match item checksum (%d)\n", computedChecksum, item.cksum);
                    exit(0);
                }

                printf("Consumer consumed %d\n", item.cksum);
            }
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main(int argc, char *argv[]) {

    if(argc !=4){
        printf("Usage: <delay> <num_producers> <num_consumers> \n");
        return -1;
    }

    srand(time(NULL));

    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    //initializes semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    //creates pthreads with default attributes
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    //creates the producers and consumer threads 
    for(int i = 0; i < num_producers; i++){
        pthread_create(&producers[i], &attr, producer, argv[1]);
    }

    for(int x = 0; x < num_consumers; x++){
        pthread_create(&consumers[x], &attr, consumer, argv[1]);
    }

    //time of program
    sleep(atoi(argv[1]));

    //removes the semaphores and mutex once program is completed
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}