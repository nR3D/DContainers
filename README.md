# DContainers

[![CMake Unit Tests](https://github.com/nR3D/DContainers/actions/workflows/cmake-tests.yml/badge.svg)](https://github.com/nR3D/DContainers/actions/workflows/cmake-tests.yml)
[![Documentation](https://img.shields.io/badge/docs-Doxygen-blue)](#Documentation)

Easily define multi-dimensional containers at compile-time.

Usually, containers of multiple dimensions are represented either as a one-dimensional container, and then accessed through arithmetic operations, or using nested containers, that will quickly bloat the corresponding typename.

For example, a 3x3 matrix could be expressed by one of the following types:
- `std::array<double, 9>`
- `std::array<std::array<double, 3>, 3>`

This library generalizes the last approach, giving a shorthand way to write recursive containers.

For instance, the above example could be written using class `DArray`, where the size of each dimension is expressed in its type:
- `DArray<double, 3, 3>`

Similarly, `DVector` could be used to represent a general 2-dimensional matrix:
- `DVector<2, double>`

Some helper methods are then offered to easily fetch stored elements, like `container.at(1,2)`, which corresponds to `container.at(1).at(2)`, or views using `Span` class.

## Code examples

```c++
using mdc::DVector, mdc::DArray, mdc::Span;

// Construction
DVector<3, short> d3Vector = {
        {
            {1, 2, 3},
            {4, 5, 6, 7}
        },
        {
            {8, 9},
            {10, 11, 12, 13, 14}
        },
};

DArray<double, 2, 3> matrix = {
        {4.2, 11.0, -1.5},
        {0.0, 1.0, 3.33}
};

// Accessing elements
d3Vector.at(1,1,2) = 0;
matrix.at(0,2) = 2.1;

// Accessing sub-containers (not necessarily by reference)
DVector<2, short>& subD3Vector = d3Vector.at(0);
DArray<double, 3>& subMatrix = matrix.at(1);

// Assigning through sub-containers
subD3Vector.at(1) = {24, 25, 26};
subMatrix.at(2) = -3.33;

// Define views on containers
DVector<3, short> view3DVector = d3Vector.at(Span::of(1), Span::all(), Span::of(0,1));
DArray<double, 2, 1> viewMatrix = matrix.at(Span::all(), Span::of<0>());
```

### Printing
```c++
std::cout << d3Vector << std::endl;
std::cout << "\nTotal: " << d3Vector.total() << " elements";


>    DVector<3>{
>    |1, 2, 3|
>    |24, 25, 26|,
>    
>    |8, 9|
>    |10, 11, 0, 13, 14|
>    }
>
>    Total: 13 elements


std::cout << matrix << std::endl;
std::cout << "\nTotal: " << matrix.total() << " elements";


>    |4.2, 11.1, 2.1|
>    |0, 1, -3.33|
>
>    Total: 6 elements


std::cout << view3DVector << std::endl;
std::cout <<"\nTotal: " << view3DVector.total() << " elements";


>    DVector<3>{
>    |8, 9|
>    |10, 11|
>    }
>
>    Total: 4 elements

std::cout << viewMatrix << std::endl;
std::cout <<"\nTotal: " << viewMatrix.total() << " elements";


>    |4.2|
>    |0.0|
>
>    Total: 2 elements
```

## Documentation

### Doxygen

Build documentation with:
```shell
doxygen docs/Doxyfile
```

> **Note**: given the abundant use of templates and variadics, having a `clang` installation is recommended.

Documentation will be accessible in `docs/html/index.html`.

## Usage

### Build

```shell
cmake -B build
cmake --build build
```

### Install

```shell
sudo cmake --build build -- install
```

Alternatively, specify a custom installation directory:

```shell
cmake -B build -DCMAKE_INSTALL_PREFIX=<custom_install_dir>
cmake --build build -- install
```

### Unit tests

Unit tests are based on framework [googletest](https://github.com/google/googletest), and are located inside `test/unit/`.

Run `test` target to execute unit tests:

```shell
cmake --build build -- test
```

### Uninstall

```shell
sudo xargs rm < build/install_manifest.txt
```

### Include inside your project

`CMakeLists.txt`:
```cmake
find_package(DContainers 0.1 REQUIRED)
target_link_libraries(yourTarget DContainers::DContainers)
```

C++ file:
```c++
#include <DContainers/DVector.hpp>
#include <DContainers/DArray.hpp>
#include <DContainers/Span.hpp>
```


## Acknowledgments

- Custom Doxygen CSS: [doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css)
- CMake best practices: [It's Time To Do CMake Right](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)
