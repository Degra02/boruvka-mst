# specify compiler and compiler flags
CC = mpicc
CFLAGS = -Wall -Wextra -fopenmp -std=c99

# needed directories
SRC_DIR = src
INC_DIR = include

# MPI
# MPI_LIBS = $(shell mpicc --showme:link)
# LIBS = -lm

# source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# object files
OBJS = $(SRCS:.c=.o)

# target executable
TARGET = mst

# n. processors
NP = 1

# some defines
# GEN ?= 0
# SAVE ?= 0
# DEBUG ?= 0
#
# MAX ?= 50000
# MIN ?= 1


# rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) 

#$(MPI_LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# -DGEN=$(GEN) -DMAX=$(MAX) -DMIN=$(MIN) -DSAVE=$(SAVE) -DDEBUG=$(DEBUG)

# run: $(TARGET)
# 	mpirun -np $(NP) ./$(TARGET) -i $(INPUT) -o $(OUTPUT)
#
#
# debug: $(TARGET)
# 	mpirun -np $(NP) ./$(TARGET) $(INPUT) $(OUTPUT) -v


clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
