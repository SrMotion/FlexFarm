#include "utils.h"

std::vector<std::string> ParseString(const std::string& str, std::string sep) {
    std::vector<std::string> parsedVctr;
    if (!str.empty()) {
        std::size_t start = 0, end = 0;
        while ((end = str.find(sep, start)) != std::string::npos && !sep.empty()) {
            parsedVctr.push_back(str.substr(start, end - start));
            start = end + sep.size();
        }
        parsedVctr.push_back(str.substr(start, !sep.empty() ? end : str.size()));
    }
    return parsedVctr;
}
std::string RandomStringA(int len, std::string charset) {
    std::string buffer = "";
    for (int i = 0; i < len; i++)
        buffer += charset[rand() % (charset.length() - 1)];
    return buffer;
}

std::wstring RandomStringW(int len, std::wstring charset) {
    std::wstring buffer = L"";
    for (int i = 0; i < len; i++)
        buffer += charset[rand() % (charset.size() - 1)];
    return buffer;
}
std::string Join(const std::vector<std::string>& strVector, std::string sep) {
    std::string buffer = "";
    for (int i = 0; i < strVector.size(); i++)
        buffer.append(strVector[i] + (i < strVector.size() - 1 ? sep : ""));
    return buffer;
}

std::string ExtractString(const std::string& str, const std::string& start, const std::string& stop) {
    if (!str.empty() && !start.empty() && !stop.empty()) {
        unsigned start_pos = str.find(start), last_pos = str.find(stop), end_pos_of_start = start_pos + start.size();
        if (start_pos != std::string::npos && last_pos != std::string::npos)
            return str.substr(end_pos_of_start, last_pos - end_pos_of_start);
    }
    return "";
}

std::string ReplaceString(std::string target, const std::string& oldPattern, const std::string& newPattern) {
    size_t start_pos = target.find(oldPattern);
    if (start_pos == std::string::npos)
        return target;
    target.replace(start_pos, oldPattern.length(), newPattern);
    return target;
}

std::string ReverseString(std::string toReverse) {
    std::reverse(toReverse.begin(), toReverse.end());
    return toReverse;
}

std::string GenerateMacAddress() {
    std::vector<std::string> mac;
    for (std::size_t i = 0; i < 6; i++)
        mac.push_back(RandomString(2, "0123456789abcdef"));
    return Join(mac, ":");
}

bool ValidateMacAddress(const std::string& macAddr) {
    return (!macAddr.empty() ? (macAddr.size() == 17 ? ParseString(macAddr.c_str(), ":").size() == 6 : false) : false);
}
/*
inline bool ValidateIpAddress(const std::string& ipAddr) {
    if (!ipAddr.empty()) {
        struct sockaddr_in sa;
        return inet_pton(AF_INET, ipAddr.c_str(), &(sa.sin_addr));
    }
    return false;
}*/

bool StartsWith(const std::string& toCheck, const std::string& start) {
    return toCheck.substr(0, start.length()) == start;
}

bool EndsWith(const std::string& toCheck, const std::string& end) {
    return StartsWith(ReverseString(toCheck), ReverseString(end));
}

bool isprintable(const std::string& target) {
    return std::find_if(target.begin(), target.end(), [](char x) { return !isprint(x); }) == target.end();
}
