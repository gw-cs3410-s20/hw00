/************************************************
 *          CSCI 3410, HW 0 Solution
 *             Ring Buffers & FIR
 *
 * Copyright (c) 2017 Phil Lopreiato
 *************************************************/

#include <assert.h>
#include <stdio.h>
#include "ring.h"

#define SIZE 3
#define VAL 42

#define ASSERT(st, exp, desc) \
do {                          \
        printf(desc);         \
        (st);                 \
        assert(exp);          \
        printf("OK\n");       \
} while(0)

int
main(int argc, char **argv) {
        int ret;
        int i;
        short val;
        double fir;
        struct ring *ring;

        printf("Testing Ring Buffer...\n");

        printf("\nTesting null safety...\n");
        ASSERT(ret = ring_size(NULL), ret == -1, "Testing ring_size is null safe...");
        ASSERT(ret = ring_enqueue(NULL, VAL), ret == -1, "Testing ring_enqueue is null safe...");
        ASSERT(ret = ring_dequeue(NULL, &val), ret == -1, "Testing ring_dequeue is null safe...");
        ASSERT(ret = ring_apply_fir(NULL, NULL), ret == 0, "Testing ring_apply_fir is null safe...");
        ASSERT(ring_free(NULL), 1, "Testing ring_free is null safe...");

        printf("\nTesting basic functionality...\n");
        ASSERT(ring = ring_create(SIZE), ring != NULL, "Testing we can create a ring buffer...");
        ASSERT(ret = ring_size(ring), ret == 0, "Testing size is 0...");

        ASSERT(ret = ring_dequeue(ring, &val), ret == -1, "Testing we can't dequeue when empty...");
        ASSERT(ret = ring_enqueue(ring, VAL), ret == 0, "Testing we can enqueue an item...");
        ASSERT(ret = ring_dequeue(ring, &val), ret == 0 && val == VAL, "Testing we can dequeue an item...");

        printf("\nFilling up the buffer...\n");
        for (i = 0; i < SIZE; i++) {
                ASSERT(ret = ring_enqueue(ring, VAL + i), ret == 0, "Testing we can enqueue an item...");
                ASSERT(ret = ring_size(ring), ret == i + 1, "Testing size is correct...");
        }
        ASSERT(ret = ring_enqueue(ring, VAL + SIZE), ret == -1, "Testing we can't enqueue to a full buffer...");

        printf("\nTesting FIR Calculations...\n");
        double weights1[] = {1, 0, 0};
        double weights2[] = {0, 1, 0};
        double weights3[] = {0, 0, 1};
        double weights4[] = {1, 1, 1};
        ASSERT(fir = ring_apply_fir(ring, weights1), fir == (double)(VAL + 0), "Testing we can select the first item...");
        ASSERT(fir = ring_apply_fir(ring, weights2), fir == (double)(VAL + 1), "Testing we can select the second item...");
        ASSERT(fir = ring_apply_fir(ring, weights3), fir == (double)(VAL + 2), "Testing we can select the third item...");
        ASSERT(fir = ring_apply_fir(ring, weights4), fir == (double)(3 * VAL + 3), "Testing we can sum all the items...");

        printf("\nEmptying the buffer...\n");
        for (i = 0; i < SIZE; i++) {
                ASSERT(ret = ring_size(ring), ret == SIZE - i, "Testing size is correct...");
                ASSERT(ret = ring_dequeue(ring, &val), ret == 0 && val == VAL + i, "Testing we can dequeue...");
        }
        ASSERT(ret = ring_dequeue(ring, &val), ret == -1, "Testing we can't dequeue when empty...");

        ASSERT(ring_free(ring), 1, "Testing we can free the ring...");

        printf("\nAll Ring Buffer tests passed!\n");
}
