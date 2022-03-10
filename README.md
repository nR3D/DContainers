# Templatized Matrices

- **DMatrix**: n-dimentional matrix of arbitrary size
- **SMatrix**: matrix of fixed size for each dimension

## Code examples

```c++
// Construction
DMatrix<3, short> d3Matrix = {
        {
            {1, 2, 3},
            {4, 5, 6, 7}
        },
        {
            {8, 9},
            {10, 11, 12, 13, 14}
        },
};

SMatrix<double, 2, 3> sMatrix = {
        {4.2, 11, -1.5},
        {0.0, 1.0, 3.33}
};

// Accessing elements
d3Matrix(1,1,2) = 0;
sMatrix(0,2) = 2.1;

// Accessing submatrices (not necessarily by reference)
DMatrix<2, short>& subD3Matrix = d3Matrix(0);
SMatrix<double, 3>& subSMatrix = sMatrix(1);

// Assigning through submatrices
subD3Matrix(1) = {24, 25, 26};
subSMatrix(2) = -3.33;
```

### Printing
```
std::cout << d3Matrix << std::endl;

>    DMatrix<3>[2,2,3]{
>    |1, 2, 3|
>    |24, 25, 26|,
>    
>    |8, 9|
>    |10, 11, 0, 13, 14|
>    }
```

```
std::cout << d3Matrix << std::endl;

>    |4.2, 11.1, 2.1|
>    |0, 1, -3.33|
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