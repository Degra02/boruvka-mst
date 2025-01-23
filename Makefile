CC = mpicc
CFLAGS = -Wall -Wextra -lm

SRC_DIR = src
INC_DIR = include

# source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# object files
OBJS = $(SRCS:.c=.o)

TARGET = mst

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ -I$(INC_DIR)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
