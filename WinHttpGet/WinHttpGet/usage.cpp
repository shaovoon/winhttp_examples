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
	std::wstring requestHeader;

	using namespace WinHttpWrapper;
	HttpResponse response;

	HttpRequest req(domain, port, false);
	bool success = req.Get(
		rest_of_path, requestHeader,
		response);
	if (success)
	{
		std::cout << "StatusCode:" << response.statusCode << std::endl;
		std::cout << "Output:" << response.output << std::endl;
		std::wcout << L"ResponseHeader:" << response.header << std::endl;
	}
	else
	{
		std::cout << "StatusCode:" << response.statusCode << std::endl;
		std::wcout << "Error:" << response.error << std::endl;
	}
}
