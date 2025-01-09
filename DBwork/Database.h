#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include<sqlite3.h>

class Database {
public:
    Database(const std::string& db_name);
    ~Database();

    bool check_credentials(const std::string& username, const std::string& password);
    bool register_user(const std::string& first_name, const std::string& last_name, const std::string& mobile_no,
                       const std::string& address, const std::string& username, const std::string& password, const std::string& role);

private:
    sqlite3* db;
};

#endif // DATABASE_H
