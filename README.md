# Making HTTP REST Request in C++ With WinHTTP

## Table of Contents

* WinHTTP Wrapper
* Usage
* Pros and Cons of WinHTTP
* Related Article
* History


## WinHTTP Wrapper

The WinHTTP Wrapper consists of one main class that is `HttpRequest` class. In its constructor, the following parameters: `user_agent`, `proxy_username`, `proxy_password`, `server_username` and `server_password` are optional. When accessing the website through a proxy that needs logon, pass in the `proxy_username`, `proxy_password` to the constructor. Sometimes, the webserver needs to be logon as well, in that case, give `server_username` and `server_password`. For server logins, the following authentication types are supported:


* HTTP Basic Authentication
* HTTP Digest Authentication
* Passport Authentication
* NTLM Authentication
* Kerberos Authentication


When several authentication methods are available, it tries to select the most secure one first. As a word of caution, never use __HTTP Basic Authentication__, as this method sends user name and password in plaintext that means it is susceptible to Man-In-The-Middle attacks.

The `HttpRequest` class has four&nbsp;`public` functions that correspond to the 4 HTTP verbs to perform CRUD operations: `PUT`, `GET`, `POST` and `DELETE`. Each of them receives and returns an&nbsp;`HttpResponse` object about HTTP operation.

```Cpp
class HttpRequest
{
public:
    HttpRequest(
        const std::wstring& domain,
        int port,
        bool secure,
        const std::wstring& user_agent = L"WinHttpClient",
        const std::wstring& proxy_username = L"",
        const std::wstring& proxy_password = L"",
        const std::wstring& server_username = L"",
        const std::wstring& server_password = L"");

    bool Get(const std::wstring& rest_of_path,
        const std::wstring& requestHeader,
        HttpResponse& response);

    bool Post(const std::wstring& rest_of_path,
        const std::wstring& requestHeader,
        const std::string& body,
        HttpResponse& response);

    bool Put(const std::wstring& rest_of_path,
        const std::wstring& requestHeader,
        const std::string& body,
        HttpResponse& response);

    bool Delete(const std::wstring& rest_of_path,
        const std::wstring& requestHeader,
        const std::string& body,
        HttpResponse& response);
};
```

This is the `HttpResponse` class which consists of 1 `Reset` function and 4 data members that contain&nbsp;information about the HTTP operation.

```Cpp
struct HttpResponse
{
    HttpResponse() : statusCode(0) {}
    void Reset()
    {
        text = "";
        header = L"";
        statusCode = 0;
        error = L"";
    }

    std::string text;
    std::wstring header;
    DWORD statusCode;
    std::wstring error;
};
```

## Usage

Please open the `RestWebApp` solution in Visual Studio and run it with __Ctrl-F5__ before running the example code below. You will encounter an error in the web browser when running it, this is due to `RestWebApp` in&nbsp;a Web API that contains no HTML pages to be viewed on a web browser.

#### Create 1 Product

This is example code to create 1 product on the website, using HTTP `POST`:

```Cpp
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
cout << "Returned Status:" << response.statusCode << endl << endl;
response.Reset();
```

The output is below:

```
Action: Create Product with Id = 1
Returned Status:200
```

#### Retrieve 1 Product

Then we retrieve the newly created product which has the `id = 1`, using HTTP `GET`.

```Cpp
cout << "Action: Retrieve the product with id = 1" << endl;
req.Get(L"/api/products/1", L"", response);
cout << "Returned Text:" << response.text << endl << endl;
response.Reset();
```

The output is below:

```
Action: Retrieve the product with id = 1
Returned Text:{"Id":1,"Name":"ElectricFan","Qty":14,"Price":20.90}
```

#### Update 1 Product

The product is then updated with a new price, using HTTP `POST` again.

```Cpp
cout << "Action: Update Product with Id = 1" << endl;
req.Post(L"/api/products/1", 
    requestHeader,
    R"({"Id":1, "Name":"ElectricFan","Qty":15,"Price":29.80})", 
    response);
cout << "Returned Status:" << response.statusCode << endl << endl;
response.Reset();
```

The output is below:

```
Action: Update Product with Id = 1
Returned Status:200
```

#### Retrieve All Products

All products are retrieved, using HTTP `GET` to see if the new price is reflected.

```Cpp
cout << "Action: Retrieve all products" << endl;
req.Get(L"/api/products", L"", response);
cout << "Returned Text:" << response.text << endl << endl;
response.Reset();
```

The output is below:

```
Action: Retrieve all products
Returned Text:[{"Id":1,"Name":"ElectricFan","Qty":15,"Price":29.80}]
```

#### Delete 1 Product

The only one product is deleted, using HTTP `DELETE`.

```Cpp
cout << "Action: Delete the product with id = 1" << endl;
req.Delete(L"/api/products/1", L"", "", response);
cout << "Returned Status:" << response.statusCode << endl << endl;
response.Reset();
```

The output is below:

```
Action: Delete the product with id = 1
Returned Status:200
```

#### Retrieve All Products

All products are retrieved, using HTTP `GET`, to see if the product is deleted.

```Cpp
cout << "Action: Retrieve all products" << endl;
req.Get(L"/api/products", L"", response);
cout << "Returned Text:" << response.text << endl << endl;
response.Reset();
```

The output is below:

```
Action: Retrieve all products
Returned Text:[]
```

## Pros and Cons of WinHTTP

### Pros

* WinHTTP comes bundled with Windows which means you do not have to include the code in your project
* Comes with Windows NTLM and Kerberos authentication out of the box
* Comes with web proxy authentication out of the box

### Cons

* WinHTTP is tied to each Windows version. Take, for example, Windows XP which is End-Of_life, does not receive updates from Microsoft anymore, so its WinHTTP is stuck without TLS 1.3 support. It may not be a problem if you are only accessing your old intranet website whose web server is not the latest.
* Windows only. Not cross-platform.
* A minor code amendment is needed for WinHTTP on older Windows.

## Related Article


* [Making HTTP REST Request in C++ (With CPR)](https://www.codeproject.com/Articles/1244632/Making-HTTP-REST-Request-in-Cplusplus)


## History


* 30<sup>th</sup> August, 2022: Updated the source code download to support proxy detection on OSes prior to Windows 8.1.
* 28<sup>th</sup> June, 2022: Fixed the Newtonsoft.Json vulnerability reported by Github in RestWebApp project.
* 6<sup>th</sup> June, 2022: Added `GetHeaderDictionary()` and `contentLength` to `HttpResponse` in v1.0.4. `GetHeaderDictionary()` is zero overhead abstraction: It only creates the dictionary when called.
* 6<sup>th</sup> March, 2021: Updated the source code download to set the text regardless of the HTTP status in v1.0.3.
* 25<sup>th</sup> February, 2021: Fixed the handle leak caused by returning prematurely from `http()`.
* 20<sup>th</sup> September, 2020: First release
