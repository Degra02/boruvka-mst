CC = mpicc
CFLAGS = -Wall -Wextra -fopenmp

SRC_DIR = src
INC_DIR = include


MPI_LIBS = $(shell mpicc --showme:link)
LIBS = -lm

# source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# object files
OBJS = $(SRCS:.c=.o)

TARGET = mst

# processors
NP = 1

INPUT = "graph.txt"
OUTPUT = "mst.txt"

GEN ?= 0
SAVE ?= 0
DEBUG ?= 0
OMP ?= 0

MAX ?= 200
MIN ?= 5

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(MPI_LIBS)

%.o: %.c
	$(CC) -c $< -o $@ -DGEN=$(GEN) -DMAX=$(MAX) -DMIN=$(MIN) -DSAVE=$(SAVE) -DDEBUG=$(DEBUG) -DOMP=$(OMP) $(CFLAGS)

run: clean $(TARGET)
	mpirun -np $(NP) ./$(TARGET) $(INPUT) $(OUTPUT)


debug: DEBUG=1
debug: clean $(TARGET)
	mpirun -np $(NP) ./$(TARGET) $(INPUT) $(OUTPUT)


test: $(TARGET)
	@for p in {1..8}; do \
		echo "$$p processors..."; \
		echo "Command: mpirun -np $$p ./$(TARGET) $(INPUT) $(OUTPUT)"; \
		mpirun -np $$p ./$(TARGET) $(INPUT) $(OUTPUT); \
		echo; \
	done;

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
