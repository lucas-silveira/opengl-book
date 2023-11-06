CPPFLAGS = -Wall

ifeq ($(OS),Windows_NT) 
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

ifeq ($(DETECTED_OS), Windows)
    make_build := if not exist "build" mkdir build
    remove_build := rmdir /s /q build
    CXXFLAGS += -lglfw3dll -lopengl32
endif

ifeq ($(DETECTED_OS), Linux)
    make_build := [ -d build ] || mkdir build
    remove_build := rm -rf build
    CXXFLAGS += -ldl -lglfw3
endif

# Mac OS X
ifeq ($(DETECTED_OS), Darwin)
    make_build := [ -d build ] || mkdir build
    remove_build := rm -rf build
    CXXFLAGS += -ldl -lglfw3 -framework Cocoa -framework IOKit
endif

all: build/app

build/app: main.cpp ../lib/gl.c
	$(make_build)
	g++ main.cpp ../lib/gl.c -o build/app $(CPPFLAGS) $(CXXFLAGS)

clean:
	$(remove_build)