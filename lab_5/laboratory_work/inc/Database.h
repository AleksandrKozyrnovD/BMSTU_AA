#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <mutex>

class SQLiteDatabase
{
public:
    SQLiteDatabase(const std::string &dbPath);

    ~SQLiteDatabase();

    // (INSERT, UPDATE, DELETE)
    bool execute_query(const std::string &query);

    // (SELECT)
    std::vector<std::vector<std::string>> execute_select_query(const std::string &query);

private:
    sqlite3 *db;
    std::mutex lockmutex;
};

#include "Database.hpp"

#endif
