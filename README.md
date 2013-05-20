Simple matrix toolchain
=======================
Feel free to contribute your own tools in whatever language you choose (though please no NodeJS or similar weird stuff).
The only thing fixed is the file format (as found in vectors.h).

Building
--------
Just run 'make', it'll create a './bin/' with the executables.
If you add a tool, please also add it to the makefile :)


Using
-----
To create a matrix, use 'mkmatrix' (you could also create your own with a hex editor. It's that simple!).
Now you could do fun stuff such as
mult A 5 | gauss | det or
gauss A | mult B | tee C | lsmatrix