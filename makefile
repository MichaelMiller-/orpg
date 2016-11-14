BIN = orpg
OBJ = main.o

# ./emcc tests/hello_world_sdl.cpp -o hello.html
# ./emcc tests/hello_world_file.cpp -o hello.html --preload-file tests/hello_world_file.txt

CINCS = 
CPROF = 
CFLAGS = $(CINCS) $(CPROF) -std=c++14 -O3

LINCS =
LIBS = 
LFLAGS = $(LINCS) $(CPROF)

RM = rm -f

.cpp.o:
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) $(BIN) *.o

all:
	make clean
	make 
