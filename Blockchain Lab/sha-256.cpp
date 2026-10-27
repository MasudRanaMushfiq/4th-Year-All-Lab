#define _WIN32_WINNT 0x0600
#define WINVER 0x0600

#include <windows.h>
#include <wincrypt.h>

#ifndef CALG_SHA_256
#define CALG_SHA_256 (ALG_ID)0x800c
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")

std::string sha256(const std::string& input) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[32];
    DWORD hashLen = 32;

    CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
    CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash);

    std::vector<BYTE> buf(input.begin(), input.end());
    CryptHashData(hHash, buf.data(), static_cast<DWORD>(buf.size()), 0);

    CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0);

    std::stringstream ss;
    for (DWORD i = 0; i < hashLen; i++) {
        ss << std::hex
           << std::setw(2)
           << std::setfill('0')
           << (int)hash[i];
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return ss.str();
}

int main() {
    std::string text;
    std::cout << "Enter text: ";
    std::getline(std::cin, text);

    std::cout << "SHA-256: " << sha256(text) << std::endl;

    return 0;
}


