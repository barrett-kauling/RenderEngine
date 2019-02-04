BIN=main
OUTDIR=build/
SRCDIR=src/
INCDIR=include/
RSCDIR=resource/

MKDIR=mkdir -p

CC=/usr/bin/g++
CFLAGS=-I $(SRCDIR) -I $(INCDIR) -I $(RSCDIR)
LDFLAGS=-std=c++17 -fPIC -lstdc++ -lGL -lglut -lGLEW -lpng -lm -lgcc_s -lc

SOURCES= \
	$(shell find -name '*.cpp' -type f -printf '%f ')
#	$(BIN).cpp \
#	$(notdir $(wildcard $(SRCDIR)*.cpp))

OBJS=$(SOURCES:.cpp=.o)

all: build $(BIN)

build: $(OUTDIR)

$(OUTDIR):
	$(MKDIR) $(OUTDIR)

%.o: $(SRCDIR)%.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) -c $< -o $(OUTDIR)$@

%.o: %.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) -c $< -o $(OUTDIR)$@

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(BIN) $(OUTDIR)*.o

clean:
	-rm $(OUTDIR)*.o
	-rm $(BIN)