#include "Procceess_list.h"

/// Try to find in the Haystack the Needle - ignore case
bool findStringIC(const std::wstring& strHaystack, const std::wstring& strNeedle)
{
	auto it = std::search(
		strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
	);
	return (it != strHaystack.end());
}

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

int ProcessInfo(DWORD processID, std::wstring& result)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess) {
		return 1;
	}
	WCHAR procWChar[MAX_PATH];
	DWORD namelen = GetProcessImageFileName(hProcess, procWChar, sizeof(procWChar) / sizeof(*procWChar));
	if (0 == namelen)
	{
		result = L"Name was empty, skiping";
		return 1;
	}
	std::wstring procName = std::wstring(procWChar);
	size_t lastPath = procName.find_last_of(L"\\");
	procName = procName.substr(lastPath + 1, procName.length() - lastPath - 1);
	result += L"Process: " + procName + L"\ ";
	CloseHandle(hProcess);
	return 0;
}

int show_procceses(std::wstring& result) {
	DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD cProcesses;
	unsigned int i;
	// Get the list of process identifiers.
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return 1;
	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);
	// Print the names of each process.
	for (i = 0; i < cProcesses; i++)
	{
		ProcessInfo(aProcesses[i], result);
		if (i % 3 == 0) {
			result += L"\n";
		}
	}
}