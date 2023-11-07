#include <stdint.h>

//size of buffer array
#define BUFFER_SIZE 10

//size of data array in buffer_item
#define DATA_SIZE 30

//buffer_item struct
//contains a data array of type unint8 and a cksum of type uint16
typedef struct buffer_item {
uint8_t data[DATA_SIZE];
uint16_t cksum;
} BUFFER_ITEM;

//takes a BUFFER_ITEM input and adds it to a buffer array
int insert_item(BUFFER_ITEM item);

//takes a BUFFER_ITEM input and removes that item from the buffer array
int remove_item(BUFFER_ITEM item);

//used for producer threads
//creates data and checksum and puts it into BUFFER_ITEM struct, then uses the insert_item function to add it to buffer array
void *producer(void *param);

//used for consumer threads
//creates
void *consumer(void *param);