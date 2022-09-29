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


    std::string host;
    int port;

    std::string dbName;
    std::string usrName;
    std::string usrPass;

    std::shared_ptr<PGconn> conn;
};