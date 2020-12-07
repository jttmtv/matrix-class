#include "Matrix.hpp"
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

// int main()
// {
//     Matrix a(10000, 10000, 1.32946f), b(10000, 10000, 1.23564f), c(10000, 10000);
//     auto start = system_clock::now();
//     c = a * b;
//     auto end = system_clock::now();
//     auto duration = duration_cast<microseconds>(end - start);
//     cout << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;
//     return 0;
// }

int main()
{
    Matrix a = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    Matrix b = a;
    cout << a * b << endl;
    return 0;
}