**ARM Demo**

The test file is shown in **main.cpp** under the main branch.

**#1**: In the first test, the list initialization method is used to initialize the matrices A and B, and output the matrices A and B and the result of their multiplication.

**#2**: In the second test, input the matrix A and matrix B objects with the default construction, assign the result of their multiplication to the matrix C with the same default construction, and output A, B, and C.

**#3**: In the third test, input the matrix A and matrix B objects using the custom parameter construction method, assign the result of their multiplication to the default constructed matrix C, and output A, B, and C.

**#4**: In the fourth test, we verify the correctness of operator [] overloading and the standard output when two matrices do not satisfy the multiplication condition.

**#5**: In the fifth test, we verified the correctness of the large matrix assignment operation (in order to reduce the memory consumption, the large matrix was adopted shallow copy). We defined a matrix array with the size of 10000, traversed the array, and made them equal to the matrix MAT with the order of 10000, and output the number of successful assignment.

**#6**: In the sixth test, the correctness of the equal sign is verified.

**#7**: In the seventh test, the overloaded operators * are verified to be correct (constant multiplied by matrix and matrix multiplied by constant).

**#8**: In the eighth test, the correctness of the overloaded operator * (matrix-matrix multiplication) is verified, and the time required to compute the multiplication of two 10000th order matrices using OpenBLAS under ARM is given.





