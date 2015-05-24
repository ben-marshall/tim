
T.I.M
=================================

This is project TIM (tiny instruction machine). The aim is to build a fully functioning CPU core
using hardware design languages (VHDL in this case) and a compiler stack to go with it. The core
will be implemented on an FPGA for hardware testing, but will also come with a full software
simulation testbench.

The idea is to create a barebones CPU core people can use in their own project, for educational
purposes, or for whatever else they can think of!

Current documentation can be found for the [ISA](http://ben-marshall.github.io/tim/doc/isa/index.html)
on the projects website. Software and hardware module documentation will follow once the specification
is complete.


Getting Started
---------------------------------

Documentation is built using [Doxygen](http://www.doxygen.org) and the rest of the build system
relies on [CMake](http://www.cmake.org/cmake). Starting from the root directory, run the project
setup script like so:

```sh
$> source project.sh
```

The `source` command is important!

This will create a folder called `build` in the root of your project directory and run CMake from
that folder. This stops CMake cluttering the source folders with all of its auxiliary files.
From that folder you can then run `make docs` to built all of the documentation which will appear
in `build/docs/`.
