#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib> // atoi
#include <type_traits>
#include <fstream>

using namespace std;

template <typename T>
inline void packed_args_to_string(T& arg) {
	try
	{
		cout << arg << endl;
	}
	catch (const std::exception& e)
	{
		cout << "non string arg: " << e.what() << endl;
	}
}

template <typename T, typename ...MoreT>
inline void packed_args_to_string(T& arg, MoreT& ...args) {
	cout << arg << endl;
	packed_args_to_string(args...);
}


template <typename T>
inline bool check_test(T expected, T result) {
	if (result == expected) {
		cout << "\nsuccess: ";
		return true;
	}
	cout << "\nfail: ";
	cout << "result\n" << result << "\nexpected \n" << expected << "\n";
	return false;
}

template <typename ExpectedT, typename MethodT, typename ...InT>
inline void run_test(ExpectedT& expected, MethodT& method, InT& ...in_args) {
	//cout << "test " << endl;

	clock_t start = clock();
	if (!check_test(expected, method(in_args...)))
	{
		packed_args_to_string(in_args...);
	}
	cout << "duration " << (clock() - start) / (double)CLOCKS_PER_SEC << " sec" << endl;
}

template <typename ExpectedT, typename MethodT, typename ...InT>
inline void run_test(ExpectedT&& expected, MethodT& method, InT&& ...in_args) {
	//cout << "test " << endl;

	clock_t start = clock();
	if(!check_test(expected, method(in_args...)))
	{
		packed_args_to_string(in_args...);
	}
	cout << "duration " << (clock() - start) / (double)CLOCKS_PER_SEC << " sec" << endl;
}


template <typename MethodT>
inline void run_test_from_text(MethodT& method, string&& filename) {
	ifstream file_stream(filename);
	int test_num = 1;

	try
	{
		while (!file_stream.eof())
		{
			cout << "test case " << test_num++;
			std::ostringstream out_stream;

			clock_t start = clock();

			method(file_stream, out_stream);
			string result = out_stream.str();
			string expected;
			file_stream >> expected;
			check_test(expected, result);

			cout << "duration " << (clock() - start) / (double)CLOCKS_PER_SEC << " sec" << endl;
		}

		file_stream.close();
	}
	catch (...)
	{
		file_stream.close();
		throw;
	}
}
