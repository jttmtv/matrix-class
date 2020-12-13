#include "Matrix.h"
#include <iostream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

int main()
{
	int test_cases = 8;
	while (test_cases--)
	{
		switch (test_cases)
		{
		case 7:
		{
			cout << "TestCase #1:" << endl;
			Matrix A = { {1, 2, 3}, {1, 3, 2}, {2, 1, 3} };
			Matrix B = { {2, 3, 1}, {3, 1, 2}, {3, 2, 1} };
			cout << "Matrix A:\n" << A << endl;
			cout << "Matrix B:\n" << B << endl;
			cout << "The result of the multiplication of matrix A and matrix B is:\n" << A * B << endl;
		}
		break;
		case 6:
		{
			cout << "TestCase #2:" << endl;
			Matrix A, B, C;
			cin >> A >> B;
			C = A * B;
			cout << "Matrix A:\n" << A << endl;
			cout << "Matrix B:\n" << B << endl;
			cout << "The result of the multiplication of matrix A and matrix B is:\n" << C << endl;
		}
		break;
		case 5:
		{
			cout << "TestCase #3:" << endl;
			Matrix A(2, 3), B(3, 2), C;
			cin >> A >> B;
			C = A * B;
			cout << "Matrix A:\n" << A << endl;
			cout << "Matrix B:\n" << B << endl;
			cout << "The result of the multiplication of matrix A and matrix B is:\n" << C << endl;
		}
		break;
		case 4:
		{
			cout << "TestCase #4:" << endl;
			Matrix A, B;
			A.set(6, 9, 1.2);
			B.set(8, 6);
			cout << "Original Matrix A:\n" << A << endl;
			cout << "Original Matrix B:\n" << B << endl;
			for (int i = 0; i < A.rows(); ++i)
				for (int j = 0; j < A.cols(); ++j)
					A[i][j] = sqrt(i + 2 * j);
			for (int i = 0; i < B.rows(); ++i)
				for (int j = 0; j < B.cols(); ++j)
					B[i][j] = sqrt(2 * i + j);
			cout << "Modified Matrix A:\n" << setiosflags(ios::fixed) << setprecision(3) << A << endl;
			cout << "Modified Matrix B:\n" << setprecision(3) << B << endl;
			cout << "The result of the multiplication of matrix A and matrix B is:\n" << A * B << endl;
		}
		break;
		case 3:
		{
			cout << "TestCase #5:" << endl;
#define CNT 10000		
			Matrix Mat;
			Matrix Clone_Mat[CNT];
			int success_times = 0;
			Mat.set(10000, 10000, 3.1415927);
			for (int i = 0; i < CNT; i++)
			{
				Clone_Mat[i] = Mat;
				++success_times;
			}
			cout << "The copy operation was successfully executed " << success_times << " times!\n" << endl;
		}
		break;
		case 2:
		{
			cout << "TestCase #6:" << endl;
			Matrix A(1000, 1000, 6.0 / 5.0);
			Matrix B(1000, 1000, 1.2);
			if (A == B)
				cout << "A is equal to B\n" << endl;
			if (A != B)
				cout << "A is not equal to B\n" << endl;
		}
		break;
		case 1:
		{
			cout << "TestCase #7:" << endl;
			Matrix A, B, C;
			A.set(6, 8, 1.234);
			C = B = A;
			cout << "Matrix A:\n" << A << endl;
			cout << "Original Matrix B:\n" << B << endl;
			cout << "Original Matrix C:\n" << C << endl;
			B = 2 * A;
			C = A * 2;
			cout << "Modified Matrix B:\n" << B << endl;
			cout << "Modified Matrix C:\n" << C << endl;
		}
		break;
		case 0:
		{
			cout << "TestCase #8:" << endl;
			Matrix A(10000, 10000, 1.32946f), B(10000, 10000, 1.23564f), C(10000, 10000);
			auto start = system_clock::now();
			C = A * B;
			auto end = system_clock::now();
			auto duration = duration_cast<microseconds>(end - start);
			cout << "It took " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s to multiply two matrices of order 10000 on ARM Development Board!" << endl;
		}
		break;
		default:
			break;
		}
	}
	return 0;
}
