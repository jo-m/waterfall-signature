// gcc -o waterfall waterfall.c && rec -t s32 -r 44100 - 2> /dev/null | ./waterfall
// gcc -o waterfall waterfall.c && ./waterfall
 
#include <limits.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <string.h>

// taken from http://rosettacode.org/wiki/Fast_Fourier_transform#C

typedef double complex cplx;
 
void _fft_rec(cplx buf[], cplx out[], int n, int step)
{
    if (step < n) {
        _fft_rec(out, buf, n, step * 2);
        _fft_rec(out + step, buf + step, n, step * 2);
 
        for (int i = 0; i < n; i += 2 * step) {
            cplx t = cexp(-I * M_PI * i / n) * out[i + step];
            buf[i / 2]     = out[i] + t;
            buf[(i + n)/2] = out[i] - t;
        }
    }
}
 
void fft(cplx buf[], int n)
{
    cplx out[n];
    memcpy(out, buf, sizeof(out));
 
    _fft_rec(buf, out, n, 1);
}

// show approx. SHOW_RATE/s updates
#define SHOW_RATE 60

// input samplerate
#define RATE 44100

// which maximal frequency to show.
// will be RATE * 0.5 * SPECTRUM_CUTOFF
#define SPECTRUM_CUTOFF 1/3

#define STDIN 0
#define MAX_FRAMES_PER_TAKE RATE / SHOW_RATE
#define FRAME_TYPE int
#define CHANNELS 2
// show only every SKIP-th frame
#define SKIP 4

int get_frames_per_take() {
    int frames_per_take = 1;
    while(frames_per_take * 2 < MAX_FRAMES_PER_TAKE)
        frames_per_take *= 2;
    return frames_per_take;
}

void copy_frames(FRAME_TYPE *in, cplx *out, int num_frames) {
    // take every CHANNELS-th sample (leave one channel out)
    for(int i = 0; i < num_frames; i++) {
        out[i] = ((double)in[CHANNELS * i]) / INT_MAX / 4;
    }
}

void window(cplx *buf, const int num_frames) {
    for(int i = 0; i < num_frames; i++) {
        float win = 0.5*(1-cos(2 * M_PI * i / (num_frames-1)));
        buf[i] = buf[i] * win;
    }
}

int main(int argc, char **argv)
{
    // how many frames to record in one take
    int frames_per_take = get_frames_per_take();

    FRAME_TYPE in[frames_per_take * CHANNELS];
    cplx buf[frames_per_take];
    printf("frames_per_take=%d data_size=%d\n", frames_per_take, sizeof(in));
    printf("exact_rate=%f\n", (double)RATE/frames_per_take);
    printf("max_freq_displayed=%f\n", (double) RATE / 2 * SPECTRUM_CUTOFF);

    int skip = 0;
    while(1) {
        int i = 0;
        // fill whole block
        while(i < sizeof(in)) {
            int r = read(STDIN, ((void *)in) + i, sizeof(in) - i);
            if(r == 0) {
                return 0;
            }
            i += r;
        }
        copy_frames(in, buf, frames_per_take);
        window(buf, frames_per_take);
        fft(buf, frames_per_take);

        // skip display?
        skip = (skip+1) % SKIP;
        if(skip) {
            continue;
        }

        printf("|");
        // spectrum is symmetric so only go to frames_per_take/2
        for(int i = 0; i < frames_per_take/2*SPECTRUM_CUTOFF; i++) {
            int val = (int)(cabs(buf[i]) * 10);
            if(val > 10) val = 10;
            char *scale = " .,~+<zoZO8";
            printf("%c", scale[val]);
        }
        printf("|\n");
    }
     
    return 0;
}
