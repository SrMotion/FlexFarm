#ifndef CRYPTO_H
#define CRYPTO_H
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "CryptoPP\\md5.h"
#include "CryptoPP\\aes.h"
#include "CryptoPP\\sha.h"
#include "CryptoPP\\gcm.h"
#include "CryptoPP\\hex.h"
#include "CryptoPP\\modes.h"
#include "CryptoPP\\filters.h"
#include "CryptoPP\\pwdbased.h"
#include "base64.h"

namespace Security
{
	CryptoPP::SecByteBlock DeriveSHA256KeyHash(const std::string& key) {
		int iter = 2000;
		std::for_each(key.begin(), key.end(), [&iter](int chr) { iter += chr; });
		CryptoPP::SecByteBlock derived(44);
		CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> kdf;
		kdf.DeriveKey(derived.data(), derived.size(), 0, reinterpret_cast<PBYTE>(const_cast<char*>(key.data())), key.size(), 0, 0, iter);
		return derived;
	}

	std::string EncryptString(const std::string& plainText, const std::string& key) {
		if (!plainText.empty() && !key.empty()) {
			std::vector<BYTE> cipherBlock(plainText.size() + CryptoPP::AES::BLOCKSIZE);
			try {
				CryptoPP::SecByteBlock derived(DeriveSHA256KeyHash(key));
				CryptoPP::GCM<CryptoPP::AES>::Encryption encryptor;
				encryptor.SetKeyWithIV(derived.data(), 32, derived.data() + 32, 12);
				CryptoPP::ArraySink arraySink(cipherBlock.data(), cipherBlock.size());
				CryptoPP::ArraySource arraySource(reinterpret_cast<PBYTE>(const_cast<char*>(plainText.data())), plainText.size(), true, new CryptoPP::AuthenticatedEncryptionFilter(encryptor, new CryptoPP::Redirector(arraySink), false, 16));
				cipherBlock.resize(arraySink.TotalPutLength());
			}
			catch (...) { }
			return base64_encode_raw(cipherBlock.data(), cipherBlock.size());
		}
		return "";
	}

	std::string DecryptString(const std::string& cipherText, const std::string& key) {
		if (!cipherText.empty() && !key.empty()) {
			std::string recovered;
			try {
				if (std::vector<BYTE> cipherBlock(base64_decode_raw(cipherText.data(), cipherText.size())); cipherBlock.size() > 16) {
					CryptoPP::SecByteBlock derived(DeriveSHA256KeyHash(key));
					CryptoPP::GCM<CryptoPP::AES>::Decryption decryptor;
					decryptor.SetKeyWithIV(derived.data(), 32, derived.data() + 32, 12);
					CryptoPP::AuthenticatedEncryptionFilter decryptionFilter(decryptor, new CryptoPP::StringSink(recovered), false, 16);
					decryptionFilter.Put(cipherBlock.data(), cipherBlock.size() - 16);
					decryptionFilter.MessageEnd();
					if (!recovered.empty()) return std::string(recovered.data(), recovered.size() - 16 > 0 ? recovered.size() - 16 : recovered.size());
				}
			}
			catch (...) { }
		}
		return "";
	}

	std::string MD5(const PBYTE value, int size) {
		std::string buffer = "";
		if (value && size) {
			BYTE digest[CryptoPP::Weak::MD5::DIGESTSIZE];
			CryptoPP::Weak::MD5 hash;
			hash.CalculateDigest(digest, value, size);
			CryptoPP::HexEncoder encoder;
			encoder.Attach(new CryptoPP::StringSink(buffer));
			encoder.Put(digest, CryptoPP::Weak::MD5::DIGESTSIZE);
			encoder.MessageEnd();
			std::transform(buffer.begin(), buffer.end(), buffer.begin(), static_cast<int(*)(int)>(std::tolower));
		}
		return buffer;
	}

	std::string MD5(const std::string& value) {
		return MD5(reinterpret_cast<const PBYTE>(const_cast<char*>(value.data())), value.size());
	}
} using namespace Security;
#endif
