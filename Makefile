# Makefile to build the project

CC = g++
CFLAGS = -Wall -I hv
LDFLAGS = -L lib -lCatch2
LIBS = -lhv
OUTDIR = build/
TESTDIR = test/
PACKAGEDIR = package/usr/bin/

all: clean CPPCSALab2Client Test

CPPCSALab2Client: CPPCSALab2Client.o MyClient.o
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUTDIR)CPPCSALab2Client CPPCSALab2Client.o MyClient.o $(LIBS)
	cp $(OUTDIR)CPPCSALab2Client $(PACKAGEDIR)CPPCSALab2Client

CPPCSALab2Client.o: src/CPPCSALab2Client.cpp
	$(CC) $(CFLAGS) -c src/CPPCSALab2Client.cpp

MyClient.o: src/MyClient.cpp
	$(CC) $(CFLAGS) -c src/MyClient.cpp

Test: Test.o
	mkdir -p $(TESTDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TESTDIR)Test Test.o $(LIBS)

Test.o: src/Test.cpp
	$(CC) $(CFLAGS) -c src/Test.cpp

clean:
	rm -f *.o *.deb $(OUTDIR)CPPCSALab2Client $(PACKAGEDIR)CPPCSALab2Client $(TESTDIR)Test
	rmdir -p $(OUTDIR) $(TESTDIR)