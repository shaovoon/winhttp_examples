#pragma once

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool http_get(
	const std::wstring& user_agent, const std::wstring& domain, 
	const std::wstring& rest_of_path, int port, bool secure,
	std::string& output, std::wstring& pp_error,
	const std::wstring& szProxyUsername, const std::wstring& szProxyPassword, 
	const std::wstring& szServerUsername, const std::wstring& szServerPassword);




