CC = g++
CFLAGS = -lSDL2 -lSDL2main -L/opt/vc/lib -lbrcmGLESv2 -lm

main: loadShader
	$(CC) -o build/main -g main.cpp loadShader/loadShader.cpp sdl2/sdl2.cpp $(CFLAGS)

clean:
	rm build/main