# DContainers

The main goal of the project is to easily express the type of multi-dimensional arrays.

Usually, arrays of multiple dimensions are represented either as one-dimensional arrays, and then accessed using some arithmetic operations, or using nested arrays, that will quickly bloat the corresponding typename.

For example, a 3x3 matrix could be expressed by one of the following types:
- `std::array<int, 9>`
- `std::array<std::array<int, 3>, 3>`

DContainers generalize the last approach, giving a shorthand way to write recursive containers.
For instance, the above example could be written using class `DArray`:
- `DArray<int, 3, 3>`

Some helper methods are then offered to easily fetch stored elements, like `arrayObject(1,2)`, which corresponds to `arrayObject.at(1).at(2)`.

## Classes implemented

- **DVector**: n-dimentional vector of arbitrary size
- **DArray**: array of fixed size for each dimension

## Code examples

```c++
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
        {4.2, 11, -1.5},
        {0.0, 1.0, 3.33}
};

// Accessing elements
d3Vector(1,1,2) = 0;
matrix(0,2) = 2.1;

// Accessing sub-containers (not necessarily by reference)
DVector<2, short>& subD3Vector = d3Vector(0);
DArray<double, 3>& subMatrix = matrix(1);

// Assigning through sub-containers
subD3Vector(1) = {24, 25, 26};
subMatrix(2) = -3.33;

// Define views on containers
DVector<3, short> view3DVector = d3Vector(Span(1), Span::all(), Span(0,1));
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
```

## Documentation

### Doxygen

Build documentation with:
```shell
doxygen docs/Doxyfile
```

> **Note**: given the abundant use of templates and variadics, having a `clang` installation is recommended.

Documentation will be accessible in `build/docs/html/index.html`.

> Custom CSS used: [doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css)

## Unit testing

Unit tests are based on framework [googletest](https://github.com/google/googletest), and are located inside `tests/`.

They can be run through executable `unit_tests`, created by CMake in the build directory.