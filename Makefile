CC = gcc
CXX = g++
CFLAGS = -Iinclude -Wall -Wextra -O2
CXXFLAGS = -Iinclude -Wall -Wextra -O2 -std=c++17
LDFLAGS = -lm

C_SRC = src/fft_f32.c src/fft_q15.c src/fft_q31.c src/fft_twiddle.c src/fft_utils.c src/fft_window.c
C_OBJ = $(C_SRC:.c=.o)
SIM_SRC = sim/main_sim.cpp
SIM_OBJ = $(SIM_SRC:.cpp=.o)

TARGET = fft_sim
SWEEP_TARGET = sweep_test

all: $(TARGET) $(SWEEP_TARGET)

$(TARGET): $(C_OBJ) $(SIM_OBJ)
	$(CXX) $(C_OBJ) $(SIM_OBJ) -o $(TARGET) $(LDFLAGS)

$(SWEEP_TARGET): $(C_OBJ) sim/sweep_test.o
	$(CXX) $(C_OBJ) sim/sweep_test.o -o $(SWEEP_TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(C_OBJ) $(SIM_OBJ) sim/sweep_test.o $(TARGET) $(SWEEP_TARGET) 
	rm -rf results/*.dat results/*.png results/*.txt

run: $(TARGET)
	mkdir -p results
	./$(TARGET)

sweep: $(SWEEP_TARGET)
	mkdir -p results
	./$(SWEEP_TARGET)

.PHONY: all clean run sweep
