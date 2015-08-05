default: sig waterfall

waterfall: waterfall.c
	gcc -o waterfall waterfall.c

sig: sig.c
	gcc -o sig sig.c

run: waterfall
	rec -t s32 -q -r 44100 - | ./waterfall
