#pragma once

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


class WinHttpWrapper
{
public:
	WinHttpWrapper(
		const std::wstring& domain,
		int port, 
		bool secure,
		const std::wstring& user_agent = L"WinHttpClient",
		const std::wstring& proxy_username = L"", 
		const std::wstring& proxy_password = L"", 
		const std::wstring& server_username = L"", 
		const std::wstring& server_password = L"")
		: m_Domain(domain)
		, m_Port(port)
		, m_Secure(secure)
		, m_UserAgent(user_agent)
		, m_ProxyUsername(proxy_username)
		, m_ProxyPassword(proxy_password)
		, m_ServerUsername(server_username)
		, m_ServerPassword(server_password)
	{}

	bool Get(const std::wstring& rest_of_path,
		const std::wstring& requestHeader,
		std::string& output,
		std::wstring& responseHeader,
		std::wstring& pp_error);
	bool Post(const std::wstring& rest_of_path,
		const std::wstring& requestHeader,
		const std::string& input_data,
		std::string& output,
		std::wstring& responseHeader,
		std::wstring& pp_error);
	bool Put(const std::wstring& rest_of_path,
		const std::wstring& requestHeader,
		const std::string& input_data,
		std::string& output,
		std::wstring& responseHeader,
		std::wstring& pp_error);
	bool Delete(const std::wstring& rest_of_path,
		const std::wstring& requestHeader,
		const std::string& input_data,
		std::string& output,
		std::wstring& responseHeader,
		std::wstring& pp_error);

private:
	// Request is wrapper around http()
	bool Request(
		const std::wstring& verb,
		const std::wstring& rest_of_path,
		const std::wstring& requestHeader,
		const std::string& input_data,
		std::string& output,
		std::wstring& responseHeader,
		std::wstring& pp_error);
	static bool http(
		const std::wstring& verb, const std::wstring& user_agent, const std::wstring& domain,
		const std::wstring& rest_of_path, int port, bool secure,
		const std::wstring& requestHeader, const std::string& input_data,
		std::string& output, std::wstring& responseHeader, std::wstring& pp_error,
		const std::wstring& szProxyUsername, const std::wstring& szProxyPassword,
		const std::wstring& szServerUsername, const std::wstring& szServerPassword);

	static DWORD ChooseAuthScheme(DWORD dwSupportedSchemes);

	std::wstring m_Domain;
	int m_Port;
	bool m_Secure;
	std::wstring m_UserAgent;
	std::wstring m_ProxyUsername;
	std::wstring m_ProxyPassword;
	std::wstring m_ServerUsername;
	std::wstring m_ServerPassword;




};

