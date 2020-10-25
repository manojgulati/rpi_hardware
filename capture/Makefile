CC = g++
CFLAGS = -Wall -Wno-unused-function -pthread

CXXFLAGS += `pkg-config opencv --cflags`

LDFLAGS += `pkg-config opencv --libs`

TARGET = capture_t

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(TARGET).c -o $(TARGET)
	
	
clean:
	rm -f $(TARGET)

