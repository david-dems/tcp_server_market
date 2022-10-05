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


    std::string host = "localhost";
    int port = 5432;

    std::string dbName = "market";
    std::string usrName = "server_role";
    std::string usrPass = "qwerty1234";

    std::shared_ptr<PGconn> conn;
};