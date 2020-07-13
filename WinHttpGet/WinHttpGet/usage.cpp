// WinHttpExample2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "WinHttpGet.h"

int main()
{
	std::wstring verb = L"GET";
	std::wstring user_agent = L"WinHttp";
	std::wstring domain = L"localhost";
	std::wstring rest_of_path = L"/Default?SearchTerm=Hello";
	std::wstring proxy_username;
	std::wstring proxy_password;
	std::wstring server_username;
	std::wstring server_password;
	int port = 54170;
	std::string output = "";
	std::wstring error;
	std::wstring header;
	std::string input_data;
	bool success = http(verb,
		user_agent, domain, rest_of_path, 
		port, false, output, error, header, input_data,
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
