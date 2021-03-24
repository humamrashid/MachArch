# Projects:

Multiple projects are intended to be located in this repository. Currently the
following projects are included: micro86, m86asm.

## 1. **micro86**

### An emulator for a simplified model of the Intel 8086 processor.


Compiling on Unix-like systems with GCC and GLib 2.x (replace x with
your version number):

```
gcc -D M86_DEBUG=false -D M86DS_DEBUG=false `pkg-config --cflags --libs glib-2.x` -O *.c common/*.c memory/*.c -o <binary>
```

The above command assumes all repository contents are under current working directory. The "programs" directory is used to contain sample programs and their output; it is unnecessary for compilation purposes.

## 2. **m86asm**

### An assembler and C++ translator for micro86 instructions.

Code for the assembler is written in Java and should work with JDK version 7 or
above. This project may be rewritten in some other language in the future (e.g.,
C or Go). The assembler compiles to a simple predefined assembly language for
micro86 as well as functional but unconventional C++ code.
