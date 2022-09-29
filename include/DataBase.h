#pragma once

#include "PGConnectionPool.h"

class DataBase {
public:
    static DataBase* getDB();
    PGConnectionPool& Pool();
private:
    DataBase(){};
    DataBase(const DataBase&);
    DataBase& operator=(const DataBase&);
    static DataBase *DB;

    PGConnectionPool pool;

};