# specify compiler and compiler flags
CC = mpicc
CFLAGS = -Wall -Wextra -fopenmp

# conditional inclusion of openmp flag if OMP is defined
# ifeq ($(OMP), 1)
#     CFLAGS += -fopenmp
# endif

ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

# needed directories
SRC_DIR = src
INC_DIR = include

# MPI
MPI_LIBS = $(shell mpicc --showme:link)
LIBS = -lm

# source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# object files
OBJS = $(SRCS:.c=.o)

# target executable
TARGET = mst

# n. processors
NP = 1

# input and output files, README contains more info
INPUT = "graph.txt"
OUTPUT = "mst.txt"

# some defines
GEN ?= 0
SAVE ?= 0
DEBUG ?= 0

MAX ?= 50000
MIN ?= 1


# rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(MPI_LIBS)

%.o: %.c
	$(CC) -c $< -o $@ -DGEN=$(GEN) -DMAX=$(MAX) -DMIN=$(MIN) -DSAVE=$(SAVE) -DDEBUG=$(DEBUG) $(CFLAGS)

run: clean $(TARGET)
	mpirun -np $(NP) ./$(TARGET) $(INPUT) $(OUTPUT)


debug: DEBUG=1
debug: clean $(TARGET)
	mpirun -np $(NP) ./$(TARGET) $(INPUT) $(OUTPUT)


clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
