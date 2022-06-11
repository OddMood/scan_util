#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <io.h>
#include <fcntl.h>

using std::cin;
using std::cout;
using std::wcout;
using std::endl;

struct ScanResult {
	int files;
	int js_detects;
	int cmd_detects;
	int exe_detects;
	int errors;
	std::chrono::duration<int> time;
	void Print()
	{

	}
};

ScanResult scan_directory(std::wstring path_string)
{
	const std::filesystem::path path{ path_string };
	for (auto const& dir_entry : std::filesystem::directory_iterator{ path })
	{
		wcout << dir_entry << endl;
	}
	return ScanResult{};
}

int wmain(int argc, wchar_t* argv[])
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	//std::wcout << __cplusplus << endl;
	if (argc > 2)
	{
		wcout << L"Extra parameters." << endl;
		return 0;
	}

	if (argc < 2)
	{
		wcout << L"Missing path of directory to scan" << endl;
		return 0;
	}

	std::wstring path_string(argv[1]);
	
	ScanResult result{ scan_directory(path_string) };
	
	result.Print();

	return 0;
}