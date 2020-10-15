CC = g++
CFLAGS = -Wall -Wno-unused-function

CXXFLAGS += `pkg-config opencv --cflags`

LDFLAGS += `pkg-config opencv --libs`

TARGET = capture


$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(TARGET).c -o $(TARGET)
	
	
clean:
	rm -f $(TARGET)

