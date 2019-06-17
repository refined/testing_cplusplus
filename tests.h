#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib> // atoi
#include <type_traits>
#include <fstream>

using namespace std;

namespace tests {
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
		if (!check_test(expected, method(in_args...)))
		{
			packed_args_to_string(in_args...);
		}
		cout << "duration " << (clock() - start) / (double)CLOCKS_PER_SEC << " sec" << endl;
	}

	inline void trim_end(string& str)
	{
		int end = str.find(" \n");
		while (end != string::npos)
		{
			str.erase(end, 1);
			end = str.find(" \n");
		}
		if (*str.rbegin() == '\n') str.erase(str.size() - 1, 1);
		if (*str.rbegin() == ' ') str.erase(str.size() - 1, 1);
	}


	inline void clearify(string& expected, string& result)
	{
		trim_end(expected);
		trim_end(result);
	}

	template <typename MethodT>
	inline bool run_test_from_text(MethodT& method, string&& filename) {
		ifstream file_stream(filename);
		int test_num = 1;
		bool res = false;

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
				string line;
				do
				{
					getline(file_stream, line);
				} while (line.empty() && !file_stream.eof());

				do
				{
					expected += line + " \n";
					if (file_stream.eof())
						break;
					getline(file_stream, line);
				} while (!line.empty());

				clearify(expected, result);

				res = check_test(expected, result);

				cout << "duration " << (clock() - start) / (double)CLOCKS_PER_SEC << " sec" << endl << endl;
			}

			file_stream.close();
			return res;
		}
		catch (...)
		{
			file_stream.close();
			throw;
		}
	}

	template <typename MethodT>
	inline void run_from_text(MethodT& method, string&& filename, string&& outfilename) {
		ifstream file_stream(filename);
		ofstream out_stream(outfilename, std::ofstream::out | std::ofstream::trunc);
		try
		{
			method(file_stream, out_stream);
			file_stream.close();
			out_stream.close();
		}
		catch (...)
		{
			file_stream.close();
			out_stream.close();
			throw;
		}
	}
}