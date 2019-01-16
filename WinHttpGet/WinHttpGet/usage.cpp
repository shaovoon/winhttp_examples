// WinHttpExample2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "WinHttpGet.h"

int main()
{
	std::wstring user_agent = L"WinHttp";
	std::wstring domain = L"localhost";
	std::wstring rest_of_path = L"/Default?SearchTerm=Hello";
	std::wstring proxy_username = L"";
	std::wstring proxy_password = L"";
	std::wstring server_username = L"";
	std::wstring server_password = L"";
	int port = 54170;
	std::string output = "";
	std::wstring error = L"";
	bool success = http_get(
		user_agent, domain, rest_of_path, 
		port, false, output, error,
		proxy_username, proxy_password,
		server_username, server_password);
	if (success)
	{
		std::cout << "Output:" << output << std::endl;
	}
	else
	{
		std::wcout << "Error:" << error << std::endl;
	}
}
