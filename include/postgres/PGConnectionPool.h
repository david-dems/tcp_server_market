#pragma once 

#include "PGConnection.h"
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

class PGConnectionPool{
public:
    PGConnectionPool();
    std::shared_ptr<PGConnection> getConnection();
    void freeConnection(std::shared_ptr<PGConnection> conn);

private:
    void createPool();
    
    const int pool_size = 10;
    std::queue<std::shared_ptr<PGConnection>> connections;
    std::mutex mutex;
    std::condition_variable condition;
};
