CPPFLAGS = -ldl -Wall

ifeq ($(OS),Windows_NT) 
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

ifeq ($(detected_OS), Linux)
    CXXFLAGS += -lglfw3
endif

# Mac OS X
ifeq ($(detected_OS), Darwin)
    CXXFLAGS += -lglfw3 -framework Cocoa -framework IOKit
endif

all: build/app

build/app: main.cpp shader.vert shader.frag light-cube.vert light-cube.frag ../lib/gl.c
	[ -d build ] || mkdir build
	g++ main.cpp ../lib/gl.c -o build/app $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -rf build