CC ?= cc
CFLAGS += -Wall -Wpedantic -Wextra
OPT ?= -O2
LINKS += -lm -lraylib
TARGET ?= boids

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(OPT) $(TARGET).c -o $(TARGET) $(LINKS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
