#include <gtkmm.h>
#include <iostream>
#include <sqlite3.h>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

class Database {
public:
    static sqlite3* open_db() {
        sqlite3* db;
        int rc = sqlite3_open("users.db", &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return nullptr;
        }
        return db;
    }

    static int create_table() {
        sqlite3* db = open_db();
        if (!db) return -1;

        const char* sql = R"(
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                first_name TEXT,
                last_name TEXT,
                mobile_no TEXT,
                address TEXT,
                username TEXT UNIQUE,
                password TEXT,
                role TEXT
            );
        )";

        char* errorMessage = nullptr;
        int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }

        sqlite3_close(db);
        return rc;
    }

    static int insert_user(const std::string& first_name, const std::string& last_name, const std::string& mobile_no,
                           const std::string& address, const std::string& username, const std::string& password, const std::string& role) {
        sqlite3* db = open_db();
        if (!db) return -1;

        const std::string sql = "INSERT INTO users (first_name, last_name, mobile_no, address, username, password, role) "
                                "VALUES (?, ?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return rc;
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
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return rc;
    }

    static int authenticate_user(const std::string& username, const std::string& password, std::string& role) {
        sqlite3* db = open_db();
        if (!db) return -1;

        const std::string sql = "SELECT password, role FROM users WHERE username = ?;";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return rc;
        }

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            const unsigned char* db_password = sqlite3_column_text(stmt, 0);
            const char* db_role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            // Compare hashed passwords
            if (db_password && db_role) {
                role = db_role;
                std::string hashed_input = hash_password(password);
                if (hashed_input == reinterpret_cast<const char*>(db_password)) {
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return SQLITE_OK;
                }
            }
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return SQLITE_AUTH;
    }

    static std::string hash_password(const std::string& password) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256_ctx;
        SHA256_Init(&sha256_ctx);
        SHA256_Update(&sha256_ctx, password.c_str(), password.size());
        SHA256_Final(hash, &sha256_ctx);

        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }

        return ss.str();
    }
};

class LoginPage : public Gtk::Box {
public:
    LoginPage() : Gtk::Box(Gtk::ORIENTATION_VERTICAL) {
        set_margin_top(50);
        set_margin_bottom(50);
        set_margin_left(50);
        set_margin_right(50);
        set_spacing(15);

        // Username Entry
        username_entry.set_placeholder_text("Username");
        pack_start(username_entry, Gtk::PACK_SHRINK);

        // Password Entry
        password_entry.set_placeholder_text("Password");
        password_entry.set_visibility(false);
        pack_start(password_entry, Gtk::PACK_SHRINK);

        // Login Button
        login_button.set_label("Login");
        login_button.signal_clicked().connect(sigc::mem_fun(*this, &LoginPage::on_login_clicked));
        pack_start(login_button, Gtk::PACK_SHRINK);

        // Error Label
        error_label.set_markup("<span foreground='red'></span>");
        pack_start(error_label, Gtk::PACK_SHRINK);

        // Apply CSS
        apply_css();
    }

    sigc::signal<void> signal_login_success() { return login_success_signal; }

private:
    Gtk::Entry username_entry, password_entry;
    Gtk::Button login_button;
    Gtk::Label error_label;
    sigc::signal<void> login_success_signal;

    void on_login_clicked() {
        std::string username = username_entry.get_text();
        std::string password = password_entry.get_text();

        std::string role;
        int result = Database::authenticate_user(username, password, role);
        if (result == SQLITE_OK) {
            error_label.set_markup("<span foreground='green'>Login Successful</span>");
            login_success_signal.emit();
        } else {
            error_label.set_markup("<span foreground='red'>Invalid username or password</span>");
        }
    }

    void apply_css() {
        auto css_provider = Gtk::CssProvider::create();
        css_provider->load_from_data(
            "button {"
            "  background-color: #4CAF50;"
            "  color: white;"
            "  font-weight: bold;"
            "  border-radius: 5px;"
            "  padding: 10px;"
            "}"
            "button:hover {"
            "  background-color: #45a049;"
            "}"
        );

        auto screen = Gdk::Screen::get_default();
        Gtk::StyleContext::add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
};

class UserRegistrationPage : public Gtk::Box {
public:
    UserRegistrationPage() : Gtk::Box(Gtk::ORIENTATION_VERTICAL) {
        set_margin_top(10);
        set_margin_bottom(10);
        set_margin_left(10);
        set_margin_right(10);
        set_spacing(15);

        // First Name Entry
        first_name_entry.set_placeholder_text("First Name");
        pack_start(first_name_entry, Gtk::PACK_SHRINK);

        // Last Name Entry
        last_name_entry.set_placeholder_text("Last Name");
        pack_start(last_name_entry, Gtk::PACK_SHRINK);

        // Mobile No Entry
        mobile_no_entry.set_placeholder_text("Mobile No");
        pack_start(mobile_no_entry, Gtk::PACK_SHRINK);

        // Address Entry
        address_entry.set_placeholder_text("Address");
        pack_start(address_entry, Gtk::PACK_SHRINK);

        // Username Entry
        username_entry.set_placeholder_text("Username");
        pack_start(username_entry, Gtk::PACK_SHRINK);

        // Password Entry
        password_entry.set_placeholder_text("Password");
        password_entry.set_visibility(false);
        pack_start(password_entry, Gtk::PACK_SHRINK);

        // Register Button
        register_button.set_label("Register");
        register_button.signal_clicked().connect(sigc::mem_fun(*this, &UserRegistrationPage::on_register_clicked));
        pack_start(register_button, Gtk::PACK_SHRINK);

        // Apply CSS
        apply_css();
    }

private:
    Gtk::Entry first_name_entry, last_name_entry, mobile_no_entry, address_entry, username_entry, password_entry;
    Gtk::Button register_button;

    void on_register_clicked() {
        std::string first_name = first_name_entry.get_text();
        std::string last_name = last_name_entry.get_text();
        std::string mobile_no = mobile_no_entry.get_text();
        std::string address = address_entry.get_text();
        std::string username = username_entry.get_text();
        std::string password = password_entry.get_text();

        std::string hashed_password = Database::hash_password(password);
        int result = Database::insert_user(first_name, last_name, mobile_no, address, username, hashed_password, "user");

        if (result == SQLITE_OK) {
            std::cout << "User registered successfully!" << std::endl;
        } else {
            std::cout << "Failed to register user!" << std::endl;
        }
    }

    void apply_css() {
        auto css_provider = Gtk::CssProvider::create();
        css_provider->load_from_data(
            "button {"
            "  background-color: #4CAF50;"
            "  color: white;"
            "  font-weight: bold;"
            "  border-radius: 5px;"
            "  padding: 10px;"
            "}"
            "button:hover {"
            "  background-color: #45a049;"
            "}"
        );

        auto screen = Gdk::Screen::get_default();
        Gtk::StyleContext::add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
};

// Main window
class MainWindow : public Gtk::Window {
public:
    MainWindow() {
        set_title("Admin Portal");
        set_default_size(800, 600);

        // Set up the stack and the pages
        stack.set_homogeneous(true);
        stack.add(login_page);
        stack.add(user_registration_page);

        // Connect login success signal
        login_page.signal_login_success().connect(sigc::mem_fun(*this, &MainWindow::on_login_success));

        // Add stack to the window
        add(stack);
        show_all_children();
    }

private:
    Gtk::Stack stack;
    LoginPage login_page;
    UserRegistrationPage user_registration_page;

    void on_login_success() {
        stack.set_visible_child(user_registration_page);
    }
};

int insert_admin_user() {
    std::string first_name = "Admin";
    std::string last_name = "User";
    std::string mobile_no = "9845680029";
    std::string address = "Admin Address";
    std::string username = "admin";
    std::string password = "adminpassword";  // Replace this with the actual admin password
    std::string hashed_password = Database::hash_password(password);

    return Database::insert_user(first_name, last_name, mobile_no, address, username, hashed_password, "admin");
}

// Main function
int main(int argc, char* argv[]) {
    Gtk::Main app(argc, argv);

    insert_admin_user();
    // Create and initialize database
    Database::create_table();

    MainWindow window;
    Gtk::Main::run(window);

    return 0;
}
