TARGET = Vectors

CC = g++

CFLAGS = -lfreetype -lGLEW -lglfw -lSOIL -lGL -I/usr/include/freetype2 -I/usr/include/libpng16 -lfreetype -lGLU -lX11 -lpthread -lXrandr  -ldl -Wall -Wextra -g

SOURCES = main.cpp engine2d.cpp

OBJECTS = $(SOURCES: .cpp=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

