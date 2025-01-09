#include <iostream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

std::string hash_password(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.size(), hash);

    std::ostringstream hashed_password;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hashed_password << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return hashed_password.str();
}

int main() {
    std::string admin_password = "admin";
    std::string user_password = "user";

    std::cout << "Hash for 'admin': " << hash_password(admin_password) << std::endl;
    std::cout << "Hash for 'user': " << hash_password(user_password) << std::endl;

    return 0;
}
