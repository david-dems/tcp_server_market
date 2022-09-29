#include "DataBase.h"

DataBase* DataBase::getDB(){
    if (!DB)
        DB = new DataBase;
    return DB;
}

DataBase* DataBase::DB = 0;



PGConnectionPool& DataBase::Pool(){
    return pool;
}