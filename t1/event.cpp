#include "event.h"
#include "system_controller.h"

Event::Event(Type type, double time, std::shared_ptr<Entity> entity, std::shared_ptr<Server> server) :
    type_(type), time_(time), entity_(entity), server_(server)
{
}

double Event::time() const
{
    return time_;
}

std::shared_ptr<Entity> Event::entity() const
{
    return entity_;
}

std::shared_ptr<Server> Event::server() const
{
    return server_;
}

void Event::server(const std::shared_ptr<Server> &server)
{
    server_ = server;
}

void Event::time(double time)
{
    time_ = time;
}

Event::Type Event::type() const
{
    return type_;
}
