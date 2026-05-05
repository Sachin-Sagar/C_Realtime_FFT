CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -O2
LDFLAGS = -lm

SRC = src/fft_f32.c src/fft_q15.c src/fft_q31.c src/fft_twiddle.c src/fft_utils.c src/fft_window.c sim/main_sim.c
OBJ = $(SRC:.c=.o)
TARGET = fft_sim

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) fft_results.csv

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
