#include <iostream>
#include "sqlite3.h"
#include <string>

// Function to execute an SQL command
void executeSQL(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "SQL executed successfully" << std::endl;
    }
}

// Callback function to display query results
int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

int main() {
    sqlite3* db;
    int rc;

    // Open a database connection
    rc = sqlite3_open("mydatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    else {
        std::cout << "Opened database successfully" << std::endl;
    }

    // Create a table
    std::string createTableSQL = "CREATE TABLE IF NOT EXISTS activation_keys ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "key TEXT NOT NULL UNIQUE);";
    executeSQL(db, createTableSQL);

    // Insert data into the table
    std::string insertDataSQL = "INSERT INTO activation_keys (key) VALUES ('exampleKey1');";
    executeSQL(db, insertDataSQL);

    // Query data from the table
    std::string querySQL = "SELECT * FROM activation_keys;";
    rc = sqlite3_exec(db, querySQL.c_str(), callback, 0, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    // Close the database connection
    sqlite3_close(db);
    return 0;
}
