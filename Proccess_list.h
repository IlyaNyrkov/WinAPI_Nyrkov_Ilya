#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <list>
#include <algorithm>
#include <string>
#include <cctype>
#include <sstream>
#include <iostream>
#include <string>

bool findStringIC(const std::wstring& strHaystack, const std::wstring& strNeedle);

int ProcessInfo(DWORD processID, std::wstring& result);

int show_procceses(std::wstring& result); 