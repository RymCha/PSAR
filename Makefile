#CFLAGS=-g -std=c99
CFLAGS=-g -O2 -std=c99

all: wbaes aes exec pre_compute exec2

exec : pre_compute 
	./$< > boites.h

exec2 : wbaes 
	./$<

boites.h: pre_compute
	./pre_compute > boites.h

pre_compute : pre_compute.c 
	gcc -o $@ $< 

test : test.c
	gcc -o $@ $< -lm

masked_aes: masked_aes.o
	gcc -o $@ $<


wbaes: wbaes.o
	gcc -o $@ $<

aes: aes.o
	gcc -o $@ $<
pre_compute.o: pre_compute.c
	gcc $(CFLAGS) -w -c $< -o $@

test.o: test.c
	gcc $(CFLAGS) -c $< -o $@

masked_aes.o:masked_aes.c
	gcc $(CFLAGS) -c $< -o $@

wbaes.o: wbaes.c boites.h
	gcc $(CFLAGS) -w -c $< -o $@

aes.o: aes.c
	gcc $(CFLAGS) -w -c $< -o $@


clean : 
	rm -rf  boites.h pre_compute wbaes wbaes.o mia deuxieme_max  my_code dpa cpa aes  aes_masque masked_aes tmp *~ *.o cpa_init cpa_ordre2 cpa_ordre2_maxs  cpa_ordre1 cpa_ordre2_final test test.o test_correl 0.24 cpa_plot cpa_max_plot  \#*
