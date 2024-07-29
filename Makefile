CC=g++
CXX_FLAGS=-Wall -Werror -std=c++17 -g

TARGET= calc.out


$(TARGET):
	$(CC) $(CXX_FLAGS) src/*.cpp -o $(TARGET)

all:
	$(TARGET)
	
clean:
	rm *.out

