TARGET   = Tree.out
OBJECTS := $(patsubst %.cpp,%.o, $(wildcard *.cpp))

all: $(OBJECTS)
	g++ $^ -o $(TARGET) 
	./$(TARGET)

%.o: %.cpp
	g++ -c -MD $<

include $(wildcard *.d) 