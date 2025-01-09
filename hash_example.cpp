#include <openssl/sha.h>
#include <iostream>
#include <iomanip>
#include <sstream>

std::string hash_password(const std::string& password) {
    // Create a SHA-256 hash context
    unsigned char hash[SHA256_DIGEST_LENGTH];

    // Perform SHA-256 hash
    SHA256_CTX sha256_ctx;
    if (!SHA256_Init(&sha256_ctx)) {
        std::cerr << "SHA256_Init failed" << std::endl;
        return "";
    }

    // Update the hash with the password data
    if (!SHA256_Update(&sha256_ctx, password.c_str(), password.length())) {
        std::cerr << "SHA256_Update failed" << std::endl;
        return "";
    }

    // Finalize the hash
    if (!SHA256_Final(hash, &sha256_ctx)) {
        std::cerr << "SHA256_Final failed" << std::endl;
        return "";
    }

    // Convert the hash to a hex string
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)hash[i];
    }

    return ss.str();
}

int main() {
    std::string password = "Embiot@01";
    std::string hashed_password = hash_password(password);

    std::cout << "SHA-256 hash: " << hashed_password << std::endl;
    return 0;
}
