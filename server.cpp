#include "server.h"

Server::Server(Type type) :
    type_(type)
{
    up_ = true;
}

Server::~Server()
{
}

bool Server::available() const
{
    return available_;
}

void Server::available(bool available)
{
    available_ = available;
}

bool Server::up() const
{
    return up_;
}

void Server::up(bool up)
{
    up_ = up;
}

void Server::reset(std::unique_ptr<RandomFunctionsData> ts_function,
                   std::unique_ptr<RandomFunctionsData> tf_function,
                   std::unique_ptr<RandomFunctionsData> tef_function)
{
    available(true);
    up(true);
    ts_function_ = (std::move(ts_function));
    tf_function_ = (std::move(tf_function));
    tef_function_ = (std::move(tef_function));
}
