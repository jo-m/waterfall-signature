# Waterfall diagram on terminal
Another thing I found on my hard drive: following old ideals, I also wanted a cool C signature. Sadly, mine is not entirely independent, and it needs `sox`'s `rec` binary.

`sig.c` contains the compressed C code, `waterfall.c` the original.

## Compile and run
    brew install sox
    make
    rec -t s32 -r 44100 -q - | ./sig
    
![image](screenshot.png)
