CC=gcc

CFLAGS=-c -Wall

all: pagetable

pagetable: main.o pageFunctions.o helpers.o byu_tracereader.o output_mode_helpers.o
	$(CC) main.o pageFunctions.o helpers.o byu_tracereader.o output_mode_helpers.o $(LFLAGS) -o pagetable

main.o: main.c
	$(CC) $(CFLAGS) main.c

pageFunctions.o: pageFunctions.c
	$(CC) $(CFLAGS) pageFunctions.c

helpers.o: helpers.c
	$(CC) $(CFLAGS) helpers.c

byu_tracereader.o: byu_tracereader.c
	$(CC) $(CFLAGS) byu_tracereader.c
	
output_mode_helpers.o: output_mode_helpers.c
	$(CC) $(CFLAGS) output_mode_helpers.c

clean:
	$(RM) *.o pagetable