// WinHttpExample2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "WinHttpWrapper.h"

int main()
{
	std::wstring domain = L"localhost";
	int port = 51654;
	std::string text;
	std::wstring requestHeader;

	using namespace WinHttpWrapper;
	HttpResponse response;

	HttpRequest req(domain, port, false);

	{
		std::cout << "Action: Create Product with Id = 1" << std::endl;
		req.Post(L"/api/products/create", L"Content-Type: application/json", R"({"Id":1, "Name":"ElectricFan","Qty":14,"Price":20.90})", response);
		std::cout << "Returned Status:" << response.statusCode << std::endl;
	}
	response.Reset();
	{
		std::cout << "Action: Retrieve the product with id = 1" << std::endl;
		req.Get(L"/api/products/1", L"", response);
		std::cout << "Returned Text:" << response.text << std::endl;
	}
	response.Reset();
	{
		std::cout << "Action: Update Product with Id = 1" << std::endl;
		req.Post(L"/api/products/1", L"Content-Type: application/json", R"({"Id":1, "Name":"ElectricFan","Qty":15,"Price":29.80})", response);
		std::cout << "Returned Status:" << response.statusCode << std::endl;
	}
	response.Reset();
	{
		std::cout << "Action: Retrieve all products" << std::endl;
		req.Get(L"/api/products", L"", response);
		std::cout << "Returned Text:" << response.text << std::endl;
	}
	response.Reset();
	{
		std::cout << "Action: Delete the product with id = 1" << std::endl;
		req.Delete(L"/api/products/1", L"", "", response);
		std::cout << "Returned Status:" << response.statusCode << std::endl;
	}
	response.Reset();
	{
		std::cout << "Action: Retrieve all products" << std::endl;
		req.Get(L"/api/products", L"", response);
		std::cout << "Returned Text:" << response.text << std::endl;
	}
	response.Reset();

	return 0;
}
