ifdef WSL_DISTRO_NAME
	PLATFORM = wsl
else ifeq ($(OS), Windows_NT)
	PLATFORM = windows
else
	PLATFORM = linux
endif

CC = gcc
INCDIR = include
LIBDIR = lib
CCFLAGS = -I$(INCDIR) -L$(LIBDIR) -Wall -std=c99
LIBS = -lglfw3

ifeq ($(PLATFROM), windows)
	LIBS = -lglfw3
else ifeq ($(PLATFORM), wsl)
	LIBS = -lglfw -lm
endif

VENDOR_SRC = src/vendor/glad.c
CORE_SRC = src/core/core.c src/core/camera.c src/core/inputs.c src/core/platform.c src/core/time.c src/core/world.c src/core/controls.c src/core/transform.c
RENDER_SRC = src/render/mesh.c src/render/render.c src/render/shader.c src/render/shapes.c src/render/texture.c
PHYSICS_SRC = src/physics/collision.c src/physics/collision-tests.c src/physics/rigidbody.c
UTILS_SRC = src/utils/coroutine.c src/utils/gamemath.c src/utils/memory.c src/utils/vector.c
MAIN_SRC = src/main.c

MAIN_BIN = bin/main.exe
DEBUG_BIN = bin/main_debug.exe
ASAN_BIN = bin/main_asan.exe

make:
	$(CC) $(CCFLAGS) $(VENDOR_SRC) $(CORE_SRC) $(RENDER_SRC) $(PHYSICS_SRC) $(UTILS_SRC) $(MAIN_SRC) -o $(MAIN_BIN) $(LIBS)

debug:
	$(CC) $(CCFLAGS) $(VENDOR_SRC) $(CORE_SRC) $(RENDER_SRC) $(PHYSICS_SRC) $(UTILS_SRC) $(MAIN_SRC) -o $(DEBUG_BIN) $(LIBS) -g

asan:
	$(CC) $(CCFLAGS) $(VENDOR_SRC) $(CORE_SRC) $(RENDER_SRC) $(PHYSICS_SRC) $(UTILS_SRC) $(MAIN_SRC) -o $(ASAN_BIN) $(LIBS) -fsanitize=address,undefined

run: make
	./$(MAIN_BIN)

run_debug: debug
	./$(DEBUG_BIN)

run_asan: asan
	./$(ASAN_BIN)