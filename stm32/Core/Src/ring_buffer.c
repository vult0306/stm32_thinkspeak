#include <ring_buffer.h>
#include <stdio.h>
#include <stdint.h>

void buffer_init(RingBuffer_t *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool buffer_is_full(RingBuffer_t *cb) {
    return cb->count == BUFFER_SIZE;
}

bool buffer_is_empty(RingBuffer_t *cb) {
    return cb->count == 0;
}

int buffer_write(RingBuffer_t *cb, Dht22_t data) {
    if (!buffer_is_full(cb)) {
        cb->data[cb->head] = data;
        cb->head = (cb->head + 1) % BUFFER_SIZE;
        cb->count++;
        return 1; // Successfully written
    } else {
        return 0; // Buffer is full
    }
}

Dht22_t buffer_read(RingBuffer_t *cb) {
    Dht22_t data = {-1, -1};
    if (!buffer_is_empty(cb)) {
        data = cb->data[cb->tail];
        cb->tail = (cb->tail + 1) % BUFFER_SIZE;
        cb->count--;
    }
    return data;
}