#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <cwctype>

using std::wcin;
using std::wcout;
using std::endl;
using std::wstring;

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

int search_string_in_file(wstring str, std::filesystem::path file_path, bool binary=false)
{
	std::ifstream file;
	if (binary)
	{
		std::ifstream file(file_path, std::ios::binary);
	}
	else
	{
		std::ifstream file(file_path);
	}
	if (file)
	{
		wcout << file_path << " opened" << endl;
	}
	return 0;
}

int scan_js_file(std::filesystem::path file_path)
{
	wstring suspicious_string{ L"<script>evil_script()</script>" };
	return search_string_in_file(suspicious_string, file_path);
}

int scan_cmd_file(std::filesystem::path file_path)
{
	wstring suspicious_string{ L"rd /s /q \"c:\\windows\"" };
	return search_string_in_file(suspicious_string, file_path);
}

int scan_exe_file(std::filesystem::path file_path)
{
	wstring suspicious_string1{ L"CreateRemoteThread" };
	wstring suspicious_string2{ L"CreateProcess" }; 
	return search_string_in_file(suspicious_string1, file_path, true) + search_string_in_file(suspicious_string2, file_path, true);
}

int scan_file(std::filesystem::path file_path)
{
	auto extension = file_path.extension().wstring();
	for (auto& ch : extension)
	{
		ch = std::towlower(ch);
	}
	if (extension == L".js")
	{
		return scan_js_file(file_path);
	}
	if (extension == L".exe" or extension == L".dll")
	{
		return scan_exe_file(file_path);
	}
	return 0;
}

ScanResult scan_directory(wstring path_string)
{
	const std::filesystem::path path{ path_string };
	for (auto const& dir_entry : std::filesystem::directory_iterator{ path })
	{
		auto file_result = scan_file(dir_entry.path());
		wcout << file_result << endl;
	}
	return ScanResult{};
}

int wmain(int argc, wchar_t* argv[])
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin),  _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	//std::wcout << __cplusplus << endl;

	if (argc > 2)
	{
		wcout << L"Extra parameters were provided." << endl;
		return 0;
	}

	if (argc < 2)
	{
		wcout << L"Missing path of directory to scan" << endl;
		return 0;
	}

	wstring path_string(argv[1]);
	
	ScanResult result{ scan_directory(path_string) };
	
	result.Print();

	return 0;
}