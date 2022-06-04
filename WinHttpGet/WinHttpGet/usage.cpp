#include <iostream>
#include "WinHttpWrapper.h"

void PrintDictionary(const std::unordered_map<std::wstring, std::wstring>& dict);

int main()
{
	using namespace std;
	const wstring domain = L"localhost";
	const wstring requestHeader = L"Content-Type: application/json";
	int port = 51654;
	bool https = false;

	using namespace WinHttpWrapper;

	HttpRequest req(domain, port, https);
	HttpResponse response;

	cout << "Action: Create Product with Id = 1" << endl;
	req.Post(L"/api/products/create", 
		requestHeader,
		R"({"Id":1, "Name":"ElectricFan","Qty":14,"Price":20.90})", 
		response);
	cout << "Returned Status:" << response.statusCode << endl;
	cout << "Content Length:" << response.contentLength << endl << endl;
	//PrintDictionary(response.GetHeaderDictionary());
	//wcout << endl << response.header << endl;
	response.Reset();

	cout << "Action: Retrieve the product with id = 1" << endl;
	req.Get(L"/api/products/1", L"", response);
	cout << "Returned Text:" << response.text << endl;
	cout << "Content Length:" << response.contentLength << endl << endl;
	response.Reset();

	cout << "Action: Update Product with Id = 1" << endl;
	req.Post(L"/api/products/1", 
		requestHeader,
		R"({"Id":1, "Name":"ElectricFan","Qty":15,"Price":29.80})", 
		response);
	cout << "Returned Status:" << response.statusCode << endl;
	cout << "Content Length:" << response.contentLength << endl << endl;
	response.Reset();

	cout << "Action: Retrieve all products" << endl;
	req.Get(L"/api/products", L"", response);
	cout << "Returned Text:" << response.text << endl;
	cout << "Content Length:" << response.contentLength << endl << endl;
	response.Reset();

	cout << "Action: Delete the product with id = 1" << endl;
	req.Delete(L"/api/products/1", L"", "", response);
	cout << "Returned Status:" << response.statusCode << endl;
	cout << "Content Length:" << response.contentLength << endl << endl;
	response.Reset();

	cout << "Action: Retrieve all products" << endl;
	req.Get(L"/api/products", L"", response);
	cout << "Returned Text:" << response.text << endl;
	cout << "Content Length:" << response.contentLength << endl << endl;
	response.Reset();

	return 0;
}

void PrintDictionary(const std::unordered_map<std::wstring, std::wstring>& dict)
{
	using namespace std;
	cout << "PrintDictionary" << endl;
	for (auto pr : dict)
	{
		wcout << pr.first << L":" << pr.second << endl;
	}
}