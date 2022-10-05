#include "PGConnectionPool.h"

PGConnectionPool::PGConnectionPool(){
    createPool();
}

void PGConnectionPool::createPool(){
    std::lock_guard<std::mutex> locker(mutex);

    for (int i = 0; i < pool_size; i++){
        connections.emplace(std::make_shared<PGConnection>());
    }
}

std::shared_ptr<PGConnection> PGConnectionPool::getConnection(){
    std::unique_lock lock(mutex);

    while(connections.empty())
        condition.wait(lock);
    auto connection = connections.front();
    connections.pop();

    return connection;    
}

void PGConnectionPool::freeConnection(std::shared_ptr<PGConnection> conn){
    std::unique_lock<std::mutex> lock(mutex);
    connections.push(conn);
    lock.unlock();
    condition.notify_one();
}
