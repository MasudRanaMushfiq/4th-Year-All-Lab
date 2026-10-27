#include <iostream>
#include <windows.h>
#include <wincrypt.h>

#ifndef CALG_SHA_256
#define CALG_SHA_256 0x0000800c
#endif

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "advapi32.lib")

// Helper: print error
void printError(const char* msg) {
    std::cout << msg << " failed\n";
}

int main() {
    HCRYPTPROV hProv = 0;
    HCRYPTKEY hKey = 0;
    HCRYPTHASH hHash = 0;

    // 1. Get crypto provider
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        printError("AcquireContext");
        return 1;
    }

    // 2. Generate RSA key pair (private + public)
    if (!CryptGenKey(hProv, AT_SIGNATURE, 0, &hKey)) {
        printError("GenKey");
        return 1;
    }

    std::string message = "Bangladesh is beautiful";

    // 3. Create SHA-256 hash object
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        printError("CreateHash");
        return 1;
    }

    // 4. Hash the message
    CryptHashData(hHash,
        (BYTE*)message.c_str(),
        message.size(),
        0
    );

    // 5. Sign the hash (PRIVATE KEY)
    BYTE signature[256];
    DWORD sigLen = sizeof(signature);

    if (!CryptSignHashA(hHash, AT_SIGNATURE, NULL, 0, signature, &sigLen)) {
        printError("SignHash");
        return 1;
    }

    std::cout << "Signature created!\n";

    // 6. VERIFY signature (PUBLIC KEY)
    BOOL ok = CryptVerifySignatureA(
        hHash,
        signature,
        sigLen,
        hKey,
        NULL,
        0
    );

    if (ok) {
        std::cout << "Signature VALID\n";
    } else {
        std::cout << "Signature INVALID\n";
    }

    // cleanup
    CryptDestroyHash(hHash);
    CryptDestroyKey(hKey);
    CryptReleaseContext(hProv, 0);

    return 0;
}

