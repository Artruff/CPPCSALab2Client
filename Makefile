CC = g++
CFLAGS = -std=c++11 -Wall
TARGET = myprogram
SRCS = CPPCSALab2Client.cpp MyClient.cpp
HEADERS_DIR = hv
HEADERS = $(wildcard $(HEADERS_DIR)/*.h)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean