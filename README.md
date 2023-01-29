# Some helpful things I use in practice.
There are only `C++` implementations right now.

There are usage examples for most of implemented structures. 

It is not guranted that this structures`s implementation is optimally, but most of them were used in practice and work good.

# Primitives
Primitives contains some useful structures not included in STL yet. For example, there is lightweight function reference class.

# Matrix Template
Matrix Template is a matrix structure supports general matrix functions from classic linear algebra course.

For matrix instantiation one can type 
```C++
Matrix<double> M({
  {1, 2, 1.5},
  {0, 0, 1.5},
  {2.3, -13, 0.25}
  {5, 5, -144, 0}
});
```
There are basic matrix operators such as sum, substraction, multiplication, taking the inverse matrix
```C++
A + B // sum
A - B // sub
A * B // mult
A.inv() // inverse, works only for square invertible matrices
```
