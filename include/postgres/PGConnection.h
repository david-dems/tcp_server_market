#pragma once

#include <mutex>
#include <memory>
#include <string>
#include <libpq-fe.h>

class PGConnection{
public:
    PGConnection();
    
    std::shared_ptr<PGconn> connection(); // Get connection with db

private:


    std::string host = "";
    int port = 5432;

    std::string dbName = "market";
    std::string usrName = "";
    std::string usrPass = "";

    std::shared_ptr<PGconn> conn;
};