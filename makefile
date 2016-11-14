BIN = orpg
OBJ = main.o

CC = g++
CINCS = 
CPROF = 
CFLAGS = $(CINCS) $(CPROF) -std=c++14 -O3

LINCS =
LIBS = 
LFLAGS = $(LINCS) $(CPROF)

RM = rm

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(LFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) $(BIN) *.o

all:
	make clean
	make 
