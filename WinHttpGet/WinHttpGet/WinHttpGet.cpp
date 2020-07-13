#include "WinHttpGet.h"
#include <winhttp.h>
#pragma comment(lib, "Winhttp.lib")

// function declaration
DWORD ChooseAuthScheme(DWORD dwSupportedSchemes);

bool http_get(const std::wstring& user_agent, const std::wstring& domain, 
	const std::wstring& rest_of_path, int port, bool secure,
	std::string& output, std::wstring& error,
	const std::wstring& szProxyUsername, const std::wstring& szProxyPassword,
	const std::wstring& szServerUsername, const std::wstring& szServerPassword)
{
	DWORD dwStatusCode = 0;
	DWORD dwSupportedSchemes;
	DWORD dwFirstScheme;
	DWORD dwSelectedScheme;
	DWORD dwTarget;
	DWORD dwLastStatus = 0;
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	//LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	BOOL bDone = FALSE;
	DWORD dwProxyAuthScheme = 0;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(user_agent.c_str(),
		WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
	{
		hConnect = WinHttpConnect(hSession, domain.c_str(), port, 0);
	}
	else
	{
		error = L"WinHttpOpen fails!";
		return false;
	}

	// Create an HTTP request handle.
	if (hConnect)
	{
		DWORD flag = secure ? WINHTTP_FLAG_SECURE : 0;
		hRequest = WinHttpOpenRequest(hConnect, L"GET", rest_of_path.c_str(),
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_REFRESH | flag);
	}
	else
	{
		WinHttpCloseHandle(hSession);
		error = L"WinHttpConnect fails!";
		return false;
	}

	if (hRequest == NULL)
		bDone = TRUE;

	while (!bDone)
	{
		//  If a proxy authentication challenge was responded to, reset
		//  those credentials before each SendRequest, because the proxy  
		//  may require re-authentication after responding to a 401 or  
		//  to a redirect. If you don't, you can get into a 
		//  407-401-407-401- loop.
		if (dwProxyAuthScheme != 0 && szProxyUsername != L"")
		{
			bResults = WinHttpSetCredentials(hRequest,
				WINHTTP_AUTH_TARGET_PROXY,
				dwProxyAuthScheme,
				szProxyUsername.c_str(),
				szProxyPassword.c_str(),
				NULL);
			if (!bResults)
			{
				error = L"WinHttpSetCredentials fails!";
			}
		}

		// Send a request.
		if (hRequest)
		{
			bResults = WinHttpSendRequest(hRequest,
				WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				WINHTTP_NO_REQUEST_DATA, 0,
				0, 0);
			if (!bResults)
			{
				error = L"WinHttpSendRequest fails!";
			}
		}

		// End the request.
		if (bResults)
		{
			bResults = WinHttpReceiveResponse(hRequest, NULL);
			if (!bResults)
			{
				error = L"WinHttpReceiveResponse fails!";
			}
		}

		// Resend the request in case of 
		// ERROR_WINHTTP_RESEND_REQUEST error.
		if (!bResults && GetLastError() == ERROR_WINHTTP_RESEND_REQUEST)
			continue;

		// Check the status code.
		if (bResults)
		{
			dwSize = sizeof(dwStatusCode);
			bResults = WinHttpQueryHeaders(hRequest,
				WINHTTP_QUERY_STATUS_CODE |
				WINHTTP_QUERY_FLAG_NUMBER,
				WINHTTP_HEADER_NAME_BY_INDEX,
				&dwStatusCode,
				&dwSize,
				WINHTTP_NO_HEADER_INDEX);

			if (!bResults)
			{
				error = L"WinHttpQueryHeaders fails!";
			}

		}

		// Keep checking for data until there is nothing left.
		if (bResults)
		{
			switch (dwStatusCode)
			{
			case 200:
			{
				std::string temp;
				output = "";
				do
				{
					// Check for available data.
					dwSize = 0;
					if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
						error = L"Error in WinHttpQueryDataAvailable: ";
						error += std::to_wstring(GetLastError());

					if (dwSize == 0)
						break;

					// Allocate space for the buffer.
					temp = "";
					temp.resize(dwSize);
					// Read the data.
					ZeroMemory((void*)(&temp[0]), dwSize);
					if (!WinHttpReadData(hRequest, (LPVOID)(&temp[0]),
						dwSize, &dwDownloaded))
					{
						error = L"Error in WinHttpReadData: ";
						error += std::to_wstring(GetLastError());
					}
					else
					{
						output += temp;
					}
				} while (dwSize > 0);
			}
			bDone = TRUE;
			break;
			case 401:
				// The server requires authentication.
				//printf(" The server requires authentication. Sending credentials...\n");

				// Obtain the supported and preferred schemes.
				bResults = WinHttpQueryAuthSchemes(hRequest,
					&dwSupportedSchemes,
					&dwFirstScheme,
					&dwTarget);

				if (!bResults)
				{
					error = L"WinHttpQueryAuthSchemes in case 401 fails!";
				}

				// Set the credentials before resending the request.
				if (bResults)
				{
					dwSelectedScheme = ChooseAuthScheme(dwSupportedSchemes);

					if (dwSelectedScheme == 0)
						bDone = TRUE;
					else
					{
						bResults = WinHttpSetCredentials(hRequest,
							dwTarget,
							dwSelectedScheme,
							szServerUsername.c_str(),
							szServerPassword.c_str(),
							NULL);
						if (!bResults)
						{
							error = L"WinHttpSetCredentials in case 401 fails!";
						}
					}
				}

				// If the same credentials are requested twice, abort the
				// request.  For simplicity, this sample does not check
				// for a repeated sequence of status codes.
				if (dwLastStatus == 401)
					bDone = TRUE;

				break;

			case 407:
				// The proxy requires authentication.
				//printf("The proxy requires authentication.  Sending credentials...\n");

				// Obtain the supported and preferred schemes.
				bResults = WinHttpQueryAuthSchemes(hRequest,
					&dwSupportedSchemes,
					&dwFirstScheme,
					&dwTarget);

				if (!bResults)
				{
					error = L"WinHttpQueryAuthSchemes in case 407 fails!";
				}


				// Set the credentials before resending the request.
				if (bResults)
					dwProxyAuthScheme = ChooseAuthScheme(dwSupportedSchemes);

				// If the same credentials are requested twice, abort the
				// request.  For simplicity, this sample does not check 
				// for a repeated sequence of status codes.
				if (dwLastStatus == 407)
					bDone = TRUE;
				break;

			default:
				// The status code does not indicate success.
				error = L"Error. Status code returned:";
				error += std::to_wstring(dwStatusCode);
				bDone = TRUE;
			}
		}

		// Keep track of the last status code.
		dwLastStatus = dwStatusCode;

		// If there are any errors, break out of the loop.
		if (!bResults)
			bDone = TRUE;
	}


	// Report any errors.
	if (!bResults)
	{
		return false;
	}

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return true;
}

DWORD ChooseAuthScheme(DWORD dwSupportedSchemes)
{
	//  It is the server's responsibility only to accept 
	//  authentication schemes that provide a sufficient
	//  level of security to protect the servers resources.
	//
	//  The client is also obligated only to use an authentication
	//  scheme that adequately protects its username and password.
	//
	//  Thus, this sample code does not use Basic authentication  
	//  because Basic authentication exposes the client's username
	//  and password to anyone monitoring the connection.

	if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_NEGOTIATE)
		return WINHTTP_AUTH_SCHEME_NEGOTIATE;
	else if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_NTLM)
		return WINHTTP_AUTH_SCHEME_NTLM;
	else if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_PASSPORT)
		return WINHTTP_AUTH_SCHEME_PASSPORT;
	else if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_DIGEST)
		return WINHTTP_AUTH_SCHEME_DIGEST;
	else if (dwSupportedSchemes & WINHTTP_AUTH_SCHEME_BASIC)
		return WINHTTP_AUTH_SCHEME_BASIC;
	else
		return 0;
}
