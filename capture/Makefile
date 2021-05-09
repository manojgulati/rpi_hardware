CC = g++
CFLAGS = -Wall -Wno-unused-function -pthread

CXXFLAGS += `pkg-config opencv --cflags`

LDFLAGS += `pkg-config opencv --libs`

TARGET = video

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(TARGET).cpp -o $(TARGET)
	
	
clean:
	rm -f $(TARGET)

