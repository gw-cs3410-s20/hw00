#ifndef _RING_H_
#define _RING_H_

/************************************************
 *              CSCI 3410, HW 0
 *             Ring Buffers & FIR
 *
 * Copyright (c) 2017 Phil Lopreiato
 *************************************************/


/* Data structure containing a ring buffer */
struct ring {
        unsigned int size;  // Capacity of the buffer
        unsigned int read;  // Next index to be read from
        unsigned int write; // Next index to be written to
        short *data;        // Array containing the buffer values
};

/**
 * Create a new ring buffer
 * Size parameter is the capacity of the buffer
 * Returns a pointer to the created struct, or NULL on failure
 */
struct ring * ring_create(int size);

/**
 * Returns the number of items currently in the ring buffer
 * If there is an error, return 0
 */
int ring_size(struct ring *ring);

/**
 * Enqueues an item onto the ring buffer
 * In the case of an error (invalid parameters or a full buffer), return -1
 * Return 0 on success
 */
int ring_enqueue(struct ring *ring, short data);

/**
 * Dequeues an item from the ring buffer
 * Removes an item from the buffer and stores it at the location pointed to by datap
 * In the case of an error (invalid parametres or an empty buffer), return -1
 * Returns 0 on success
 */
int ring_dequeue(struct ring *ring, short *datap);

/**
 * Computes the weighted sum of all items in the buffer
 * You may assume that the length of the weights array is the same as the buffer capacity
 * If the buffer is not full, do not compute the sum and instead return 0
 * Return 0 on an error (invalid parameters) or the weighted sum otherwise
 */
double ring_apply_fir(struct ring *ring, double *weights);

/**
 * Frees memory associated with the passed ring buffer
 */
void ring_free(struct ring *ring);

#endif /* _RING_H_ */
