CC = g++
CFLAGS = -lSDL2 -lSDL2main -L/opt/vc/lib -lbrcmGLESv2 -lm

build/main: loadShader
	@ mkdir -p build
	$(CC) -o build/main -g main.cpp loadShader/loadShader.cpp sdl2/sdl2.cpp $(CFLAGS)

run: build/main
	@ ./build/main

clean:
	rm -r build