#ifndef BASE64_H
#define BASE64_H
#include <Windows.h>
#include <algorithm>
#include <string>
#include <vector>

#pragma comment(lib, "Crypt32.lib")

namespace Security
{
	std::string base64_encode_raw(const PBYTE pszSource, int size) {
		std::vector<char> buffer;
		if (pszSource) {
			DWORD nDestinationSize = 0;
			if (CryptBinaryToStringA(pszSource, size, CRYPT_STRING_BASE64, nullptr, &nDestinationSize)) {
				buffer.resize(nDestinationSize);
				CryptBinaryToStringA(pszSource, size, CRYPT_STRING_BASE64, buffer.data(), &nDestinationSize);
			}
			buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
			buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
		} 
		return std::string(buffer.data(), buffer.size());
	}

	std::vector<BYTE> base64_decode_raw(LPCSTR pszSource, int size) {
		std::vector<BYTE> buffer;
   		if (pszSource) {
			DWORD nDestinationSize = 0;
			if (CryptStringToBinaryA(pszSource, size, CRYPT_STRING_BASE64, nullptr, &nDestinationSize, 0, 0)) {
				buffer.resize(nDestinationSize);
				CryptStringToBinaryA(pszSource, size, CRYPT_STRING_BASE64, buffer.data(), &nDestinationSize, 0, 0);
			}
		}  
		return buffer;
	}

	std::string base64_encode(const std::string& value) {
		return base64_encode_raw(reinterpret_cast<const PBYTE>(const_cast<char*>(value.data())), value.size());
	}

	std::string base64_decode(const std::string& value) {
		std::vector<BYTE> buffer(base64_decode_raw(value.data(), value.size()));
		return std::string(reinterpret_cast<PCHAR>(buffer.data()), buffer.size());
	}
}
#endif // !BASE64_H
