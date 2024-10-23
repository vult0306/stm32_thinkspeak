#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include <stdbool.h>

#define BUFFER_SIZE 10

typedef struct {
    float Tempt;
    float Humid;
} Dht22_t;

typedef struct {
    Dht22_t data[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} RingBuffer_t;

extern void buffer_init(RingBuffer_t *cb);
extern bool buffer_is_full(RingBuffer_t *cb);
extern bool buffer_is_empty(RingBuffer_t *cb);
extern int buffer_write(RingBuffer_t *cb, Dht22_t data);
extern Dht22_t buffer_read(RingBuffer_t *cb);

#endif
