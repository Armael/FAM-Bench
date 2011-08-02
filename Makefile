CC = gcc
CFLAGS = -Wall -O2 -D_REENTRANT
LIBS = -lfam

TARGET=fam_bench

all: $(TARGET)

fam_bench:	fam_bench.o
	$(CC) -o fam_bench fam_bench.o $(LIBS)

clean:
	rm *.o

mproper:	clean
	rm $(TARGET)

.c.o:
	@$(CC) $(CFLAGS) -c -o $@ $<
