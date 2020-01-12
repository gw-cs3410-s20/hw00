/************************************************
 *              CSCI 3410, HW 0
 *             Ring Buffers & FIR
 *************************************************/

#include <stdlib.h>
#include "ring.h"

struct ring *
ring_create(int size) {
        /* TODO: implement this function */
        return NULL;
}

int
ring_size(struct ring *ring) {
        /* TODO: implement this function */
        return -1;
}

int
ring_enqueue(struct ring *ring, short data) {
        /* TODO: implement this function */
        return -1;
}

int
ring_dequeue(struct ring *ring, short *datap) {
        /* TODO: implement this function */
        return -1;
}

double
ring_apply_fir(struct ring *ring, double *weights) {
        /* TODO: implement this function */
        return -1;
}

void
ring_free(struct ring *ring) {
        /* TODO: implement this function */
}
