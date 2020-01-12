/************************************************
 *              CSCI 3410, HW 0
 *             Ring Buffers & FIR
 *
 * Copyright (c) 2017 Phil Lopreiato
 *************************************************/

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "ring.h"
#include "wavfile.h"

#define FILTER_WINDOW 32
#define NUM_SAMPLES (WAVFILE_SAMPLES_PER_SECOND*5)
#define VOLUME 3200
#define FREQUENCY 440.0
#define NOISE 0.2

void
fill_sin_wave(short waveform[NUM_SAMPLES]) {
        int i;
	    for (i = 0; i < NUM_SAMPLES; i++) {
		        double t = (double) i / WAVFILE_SAMPLES_PER_SECOND;
		        waveform[i] = VOLUME * sin(FREQUENCY * t * 2 * M_PI);
	    }
}

void
add_noise(short waveform[NUM_SAMPLES]) {
        int i;
        short range, val;

        printf("Adding noise to the samples...\n");
        srand(time(NULL));
        range = (short)(NOISE * VOLUME);
        for (i = 0; i < NUM_SAMPLES; i++) {
                val = (rand() % (2*range) - range);
                waveform[i] += val;
        }
}

void
filter_sin_wave(short samples[NUM_SAMPLES], double weights[FILTER_WINDOW]) {
        struct ring *ring;
        short last;
        int sample;
        int ret;
        int i;

        /* Set up a ring buffer to use with our FIR Filter */
        ring = ring_create(FILTER_WINDOW);

        /* Until we have enough data in the buffer, just write 0s as samples */
        printf("Filter Window = %d\n", FILTER_WINDOW);
        for (i = 0; i < FILTER_WINDOW; i++) {
                ret = ring_enqueue(ring, samples[i]);
                assert(ret == 0);
                samples[i] = 0;
        }

        /* Use our ring buffer to compute the samples */
        for (; i < NUM_SAMPLES; i++) {
                sample = samples[i];
                samples[i] = (int)ring_apply_fir(ring, weights);

                /* Remove an old value from the buffer */
                ret = ring_dequeue(ring, &last);
                assert(ret == 0);

                /* Add a new value to the buffer */
                ret = ring_enqueue(ring, sample);
                assert(ret == 0);
        }

        ring_free(ring);
}

int
write_csv(const char *filename, short samples[NUM_SAMPLES]) {
        int i;
        int ret;
        size_t len;
        char line[32];

        printf("Writing sample data to %s...\n", filename);
        FILE *f = fopen(filename, "w+");
        if (!f) {
                fprintf(stderr, "couldn't open %s for writing: %s\n", filename, strerror(errno));
                return -1;
        }

        for (i = 0; i < NUM_SAMPLES / 200; i++) {
                sprintf(line, "%d, %d\n", i, samples[i]);
                len = strlen(line);
                ret = fwrite(line, sizeof(char), len, f);
                if (ret != len) {
                        fprintf(stderr, "Unable to write sample %d, skipping...\n", i);
                        continue;
                }
        }

        fclose(f);
        return 0;
}

int
write_samples(const char *filename, short samples[NUM_SAMPLES]) {
        printf("Writing tone data to %s...\n", filename);
        FILE *f = wavfile_open(filename);
        if (!f) {
                fprintf(stderr, "couldn't open %s for writing: %s\n", filename, strerror(errno));
		return -1;
        }

        wavfile_write(f, samples, NUM_SAMPLES);
        wavfile_close(f);
        return 0;
}

int main() {
	short waveform[NUM_SAMPLES];
        double fir_weights[FILTER_WINDOW];
        int i;

        printf("Building sample data...\n");
        fill_sin_wave(waveform);

        if (write_samples("clean.wav", waveform) != 0) return -1;
        if (write_csv("samples_clean.csv", waveform) != 0) return -1;

        /* Add some noise to our wave */
        add_noise(waveform);
        if (write_samples("noisy.wav", waveform) != 0) return -1;
        if (write_csv("samples_noisy.csv", waveform) != 0) return -1;

        /* Build our filter weights
         * Each weight will be 1/FILTER_WINDOW
         * This implements a moving average
         */
        printf("Building FIR Filter weights...\n");
        for (i = 0; i < FILTER_WINDOW ; i++) {
                fir_weights[i] = (1.0 / FILTER_WINDOW);
        }

        printf("Filtering samples...\n");
        filter_sin_wave(waveform, fir_weights);
        if (write_samples("filtered.wav", waveform) != 0) return -1;
        if (write_csv("samples_filtered.csv", waveform) != 0) return -1;

	return 0;
}
