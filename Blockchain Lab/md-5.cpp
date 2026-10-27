#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <wincrypt.h>

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")

std::string md5(const std::string& input) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[16];
    DWORD hashLen = 16;

    CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
    CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);

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

    std::cout << "MD5: " << md5(text) << std::endl;

    return 0;
}

