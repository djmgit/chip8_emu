COMPILER = cc
SOURCE_LIBS_OSX = -Iinclude/
OPT = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
OUT = -o chip8_emu
CFILES = *.c
PLATFORM := $(shell uname)

ifeq ($(PLATFORM), Darwin)
	COMPILER = clang
	SOURCE_LIBS_OSX = -Ilib/darwin -Iinclude/
	OPT = -Llib/darwin/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/darwin/libraylib.a
endif

build:
	$(COMPILER) $(SOURCE_LIBS_OSX) $(CFILES) $(OUT) $(OPT)

run:
	./chip8_emu

clean:
	rm -rf ./chip8_emu
