#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include "json.hpp"
#include "Common.hpp"
#include "HandlerFactory.h"
#include "RegisterHandler.h"
#include "HelloHandler.h"
#include "OrderHandler.h"
#include "ActiveOrdersHandler.h"
#include "BalanceHandler.h"
#include "SignInHandler.h"
#include "HistoryHandler.h"
#include "DealsHandler.h"
#include "DeleteHandler.h"
#include "QuotationsHandler.h"
#include "Engine.h"

using boost::asio::ip::tcp;

class session
{
public:
    session(boost::asio::io_service& io_service)
        : socket_(io_service)
    {
        handlerFactory.addHandler<HelHandler>("HelHandler");
        handlerFactory.addHandler<RegHandler>("RegHandler");
        handlerFactory.addHandler<OrderHandler>("OrderHandler");
        handlerFactory.addHandler<ActiveOrderHandler>("ActiveOrdersHandler");
        handlerFactory.addHandler<BalanceHandler>("BalanceHandler");
        handlerFactory.addHandler<SignInHandler>("SignInHandler");
        handlerFactory.addHandler<HistoryOrderHandler>("HistoryOrderHandler");
        handlerFactory.addHandler<DealsHandler>("DealsHandler");
        handlerFactory.addHandler<DeleteHandler>("DeleteHandler");
        handlerFactory.addHandler<QuotationsHandler>("QuotationsHandler");
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&session::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    // Обработка полученного сообщения.
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred)
    {
        if (!error)
        {
            data_[bytes_transferred] = '\0';

            std::string reply;

            try{ 
                // Парсим json, который пришёл нам в сообщении.
                auto j = nlohmann::json::parse(data_);
                if (j["ReqType"] == nullptr){
                    throw std::logic_error("Bad request type");
                }
                std::string reqType = j["ReqType"];

                //Создаем необходимый для обработки реквеста колбэк 
                RequestHandler *h = handlerFactory.make(reqType + "Handler");
                reply = h->makeReply(std::move(j));

                // после использования нужно удалить хендлер из-за new в фабрике
                delete h; 

            } catch (std::logic_error const& e) {
                nlohmann::json rep;
                rep["Error"] = "Bad ReqType";
                std::cerr << e.what();
                reply = rep.dump();
            } catch(std::exception const& e) {
                nlohmann::json rep;
                rep["Error"] = "Server Error";
                std::cerr << e.what();
                reply = rep.dump();
            }
            
            reply += "\n";

            boost::asio::async_write(socket_, 
                boost::asio::buffer(reply, reply.size()),
                boost::bind(&session::handle_write, this,
                    boost::asio::placeholders::error));
        }
        else
        {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                boost::bind(&session::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            delete this;
        }
    }

private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    HandlerFactory handlerFactory;

};

class server
{
public:
    server(boost::asio::io_service& io_service)
        : io_service_(io_service),
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        std::cout << "Server started! Listen " << port << " port" << std::endl;

        session* new_session = new session(io_service_);
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this, new_session,
                boost::asio::placeholders::error));
    }

    void handle_accept(session* new_session,
        const boost::system::error_code& error)
    {
        if (!error)
        {
            new_session->start();
            new_session = new session(io_service_);
            acceptor_.async_accept(new_session->socket(),
                boost::bind(&server::handle_accept, this, new_session,
                    boost::asio::placeholders::error));
        }
        else
        {
            delete new_session;
        }
    }

private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

int main()
{
    try
    {
        MatchingEngine engine;
        engine.run();

        boost::asio::io_service io_service;

        server s(io_service);

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}