# Moxie Engine
Moxie Engine is an ECS-based 3D game engine written in C and focused on portability and performance.

***NOTE:*** This project is heavily unstable and in an experimental phase. Please keep this in mind if you have any intentions of making use of it.

## Features
- An OpenGL-based 3D rendering system
- A collision system using Separating-Axis Theorem (SAT)
- An Entity Component System (ECS) with prebuilt components and systems for rendering and physics
- A math library which gives support for vec2/3/4, quaternions and 4x4 matrices

## Dependencies
This project vendors GLAD and requires OpenGL as it's rendering backend.

It also requires the following libraries to be installed on the system:
- [GLFW](https://www.glfw.org/download)
- [FreeType](https://freetype.org/download.html)

## Requirements
- The dependencies as listed above.
- GCC or Clang
- C11 Support
- Make

## Usage
The engine is compiled as either:
- a shared library (`.so`, `.dll`, `.dylib`)
- or a static library (`.a`, `.lib`)

The Makefile is intended to simplify this process.
### Makefile rules
#### make static
- Builds the static library in the `build` directory.
#### make shared
- Builds the shared library in the `build` directory.
#### make or make all
- Compiles both static and shared libraries.
#### make test
- Builds the test program at test/test.c using the shared library. The executable can be found at build/test executable.
- Builds the shared library if it's not present.
#### make test-run
- Runs the build/test executable.
- Builds the binary if it's not present.
#### make clean
- Deletes all the files and directories in the `build` directory.