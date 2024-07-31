CC=g++
CXX_FLAGS=-Wall -Werror -std=c++17 -O2

TARGET= calc.out


	
all:
	rm -rf build/ && mkdir build/
	$(CC) $(CXX_FLAGS) src/*.cpp -o build/$(TARGET)

debug:
	rm -rf build_debug/ && mkdir build_debug/
	$(CC) $(CXX_FLAGS) -DTOKEN_DEBUG src/*.cpp -o build_debug/$(TARGET)

clean:
	rm *.out

