#pragma once

#include <mutex>
#include <memory>
#include <string>
#include <libpq-fe.h>
#include "ServerConfig.h"

class PGConnection{
public:
    PGConnection();
    
    std::shared_ptr<PGconn> connection(); // Get connection with db

private:


    std::string host = DBHOST;
    int port = DBPORT;

    std::string dbName = DBNAME;
    std::string usrName = ROLE;
    std::string usrPass = PASS;

    std::shared_ptr<PGconn> conn;
};