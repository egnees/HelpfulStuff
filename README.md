# Some helpful things I use in practice.
There are only `C++` implementations right now.

There are usage examples for most of implemented structures. 

It is not guranted that this structures`s implementation is optimally, but most of them were used in practice and work good.

## Primitives
Primitives contains some useful structures not included in STL yet. For example, there is lightweight function reference class.

+ ```function_ref.h``` allows you to create refference to any function.
+ ```fdymlib.c``` is an example of loading dynamic library and calling some function from this dynamic library. The point is that you know only name, return value and signature of this function. In example, if you execute the program like this: ```./a libc.so.6 printf vsiid 'hello %d %d im %lf' 3 2 12.2```, then *printf* will be executed with corresponding parametrs, also *v* means *void* and it is the return values type, *s* means *string* – the first argument of *printf*, *i* means *int* – the second argument and etc. For realizations I used alloca method to allocate enough bytes on stack and then call the function by the prototype. Use the following command to compile: ```gcc -m32 fdymlib.c -o a```. If you want work in 64 bit mode, then you can compile like this: ```gcc fdymlib.c -o a``` and change *SIZESTR* constant to 8 (this value is equal to `sizeof(void*)`). There is only Linux and OS X support right now.  

## Matrix Template
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
There are basic matrix operators such as 
+ Sum
+ Substraction
+ Multiplication
+ Taking the inverse matrix
+ Matrix power
```C++
C = A + B; // sum
C = A - B; // sub
C = A * B; // mult
C = A.inv(); // inverse, works only for square invertible matrices
C = A ^ p; // matrix power, O(p n^3) implementation.
```

One can access to the matrix element in the $i$-th row and $j$-th column like that
```C++
A[i][j] = 5;
```

Also matrix template supports such functions as 
+ Row echelon form of matrix computation
+ Determinant computation
+ Printing the characteristic polynomial of matrix
+ Taking the basis of matrix rows
+ Rank computation
```C++
B = A.USV();
std::cout << B; // row echelon form of matrix computation and printing

std::cout << A.det() << '\n'; // matrix determinant, O(n^3) implementation

std::vector<double> char_poly = A.charPoly(); // characteristic polynomial, O(2^n n^2) implementation
A.printCharPoly(); // prints characteristic polynomial

std::vector<Matrix<double>> fss = A.FSS(); // fundamental system of solutions, or rows basis
A.printFSS(); // prints fss

std::cout << A.rk() << '\n'; // matrix rank, O(n^3)
```

Also you can find such useful file as ```frac.h```, this is rational numbers implementation.
In ```examples``` folder you can find more examples of matrix template and ```frac.h``` usage.
