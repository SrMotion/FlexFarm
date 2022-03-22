#pragma once
#include <string>
#include <vector>

#define RandomString RandomStringA

std::vector<std::string> ParseString(const std::string& str, std::string sep);
std::string RandomStringA(int len, std::string charset);
std::wstring RandomStringW(int len, std::wstring charset);
std::string Join(const std::vector<std::string>& strVector, std::string sep);
std::string ExtractString(const std::string& str, const std::string& start, const std::string& stop);
std::string ReplaceString(std::string target, const std::string& oldPattern, const std::string& newPattern);
std::string ReverseString(std::string toReverse);
std::string GenerateMacAddress();
bool ValidateMacAddress(const std::string& macAddr);
bool StartsWith(const std::string& toCheck, const std::string& start);
bool EndsWith(const std::string& toCheck, const std::string& end);
bool isprintable(const std::string& target);