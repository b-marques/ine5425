#include "server.h"

Server::Server(Type type) :
    type_(type), up_(true), failures_number_(0), down_time_(0)
{
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

void Server::reset(std::shared_ptr<RandomFunctionsData> ts_function,
                   std::shared_ptr<RandomFunctionsData> tf_function,
                   std::shared_ptr<RandomFunctionsData> tef_function)
{
    available(true);
    up(true);
    ts_function_ = ts_function;
    tf_function_ = tf_function;
    tef_function_ = tef_function;
    waiting_queue_.clear();
    failures_number_ = 0;
    down_time_ = 0;
}

std::shared_ptr<RandomFunctionsData> Server::ts_function() const
{
    return ts_function_;
}

std::shared_ptr<RandomFunctionsData> Server::tef_function() const
{
    return tef_function_;
}

std::shared_ptr<RandomFunctionsData> Server::tf_function() const
{
    return tf_function_;
}

std::vector<std::shared_ptr<Entity>>& Server::waiting_queue()
{
    return waiting_queue_;
}

Server::Type Server::type() const
{
    return type_;
}

int Server::failures_number() const
{
    return failures_number_;
}

void Server::failures_number(int failures_number)
{
    failures_number_ = failures_number;
}

double Server::down_time() const
{
    return down_time_;
}

void Server::down_time(double down_time)
{
    down_time_ = down_time;
}
