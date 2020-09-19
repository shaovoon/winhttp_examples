// WinHttpExample2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "WinHttpWrapper.h"

int main()
{
	std::wstring domain = L"localhost";
	std::wstring rest_of_path = L"/Default?SearchTerm=Hello";
	int port = 54170;
	std::string output;
	std::wstring responseHeader;
	std::wstring error;
	std::wstring header;

	WinHttpWrapper wrapper(domain, port, false);
	bool success = wrapper.Get(
		rest_of_path, header,
		output, responseHeader, error);
	if (success)
	{
		std::cout << "Output:" << output << std::endl;
		std::wcout << L"ResponseHeader:" << responseHeader << std::endl;
	}
	else
	{
		std::wcout << "Error:" << error << std::endl;
	}
}
