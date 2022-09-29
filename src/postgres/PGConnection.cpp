#include "PGConnection.h"

PGConnection::PGConnection(){
    conn.reset(PQsetdbLogin(host.c_str(), std::to_string(port).c_str(), nullptr, nullptr, dbName.c_str(), usrName.c_str(), usrPass.c_str()), &PQfinish);

    if (PQstatus(conn.get()) != CONNECTION_OK && PQsetnonblocking(conn.get(), 1) != 0){
       throw std::runtime_error(PQerrorMessage(conn.get()));
    }
}

std::shared_ptr<PGconn> PGConnection::connection(){
    return conn;
}
