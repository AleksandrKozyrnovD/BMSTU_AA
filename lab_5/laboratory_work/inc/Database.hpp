#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

#include "Database.h"
#include <mutex>

SQLiteDatabase::SQLiteDatabase(const std::string &dbPath)
{
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK)
    {
        // std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

SQLiteDatabase::~SQLiteDatabase()
{
    if (db)
    {
        sqlite3_close(db);
    }
}

bool SQLiteDatabase::execute_query(const std::string &query)
{
    std::unique_lock<std::mutex> lock(this->lockmutex);
    char *errorMsg = nullptr;
    int result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMsg);
    
    if (result != SQLITE_OK)
    {
        sqlite3_free(errorMsg);
        return false;
    }
    return true;
}

std::vector<std::vector<std::string>> SQLiteDatabase::execute_select_query(const std::string &query)
{
    std::vector<std::vector<std::string>> results;
    sqlite3_stmt *stmt;

    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to execute select query: " << sqlite3_errmsg(db) << std::endl;
        return results;
    }

    int columns = sqlite3_column_count(stmt);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::vector<std::string> row;
        for (int i = 0; i < columns; ++i)
        {
            const char *val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row.push_back(val ? val : "NULL");
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

#endif
