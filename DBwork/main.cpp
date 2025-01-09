#include <gtkmm.h>
#include "MainWindow.h"
#include "Database.h"
#include <iostream>

// Function to initialize the SQLite database
void initialize_database() {
    sqlite3* db;
    char* err_msg = nullptr;

    // Open (or create) the database file
    if (sqlite3_open("users.db", &db)) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // SQL query to create the users table if it doesn't already exist
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "first_name TEXT NOT NULL, "
        "last_name TEXT NOT NULL, "
        "mobile_no TEXT NOT NULL, "
        "address TEXT NOT NULL, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL, "
        "role TEXT NOT NULL"
        ");";

    if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    } else {
        std::cout << "Database initialized successfully." << std::endl;
    }

    sqlite3_close(db);
}

int main(int argc, char* argv[]) {
    // Initialize the database and create the table if it doesn't exist
    initialize_database();

    auto app = Gtk::Application::create(argc, argv, "com.example.adminpanel");

    Database db("users.db");
    // Create and show the main window
    MainWindow main_window(db);

    // Run the GTKmm main loop
    return app->run(main_window);
}
