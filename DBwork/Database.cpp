#include "Database.h"
#include <sqlite3.h>
#include <iostream>
#include <sstream>

Database::Database(const std::string& db_name) : db(nullptr) {
    // Open the SQLite database file
    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Create the users table if it doesn't exist
    const std::string create_table_sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            mobile_no TEXT NOT NULL,
            address TEXT NOT NULL,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            role TEXT NOT NULL
        );
    )";

    char* err_msg = nullptr;
    if (sqlite3_exec(db, create_table_sql.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::cerr << "SQL error: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::check_credentials(const std::string& username, const std::string& password) {
    const std::string select_sql = "SELECT password, role FROM users WHERE username = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const char* stored_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        // Compare the hashed password
        if (stored_password && role) {
            if (password == stored_password) {
                std::cout << "Login successful, role: " << role << std::endl;
                sqlite3_finalize(stmt);
                return true;
            }
        }
    }
    
    sqlite3_finalize(stmt);
    std::cerr << "Invalid credentials!" << std::endl;
    return false;
}

bool Database::register_user(const std::string& first_name, const std::string& last_name, const std::string& mobile_no,
                             const std::string& address, const std::string& username, const std::string& password, const std::string& role) {
    const std::string insert_sql = R"(
        INSERT INTO users (first_name, last_name, mobile_no, address, username, password, role)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, first_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, last_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, mobile_no.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, role.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to insert user: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
