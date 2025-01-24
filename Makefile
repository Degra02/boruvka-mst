CC = mpicc
CFLAGS = -Wall -Wextra

SRC_DIR = src
INC_DIR = include

# igraph
IGRAPH_LIB_DIR = /usr/local/lib
IGRAPH_INC_DIR = /usr/local/include/igraph

MPI_LIBS = $(shell mpicc --showme:link)
LIBS = -ligraph -larpack -lm

# source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# object files
OBJS = $(SRCS:.c=.o)

TARGET = mst

# processors
NP = 1

INPUT = "graph.txt"
OUTPUT = "mst.txt"

all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CC) -o $@ $^ $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -I$(IGRAPH_INC_DIR) -L$(IGRAPH_LIB_DIR) $(LIBS) $(MPI_LIBS)

%.o: %.c
	$(CC) -c $< -o $@ -I$(INC_DIR) -I$(IGRAPH_INC_DIR)

run: $(TARGET)
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
