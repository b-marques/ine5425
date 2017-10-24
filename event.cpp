#include "event.h"
#include "system_controller.h"

Event::Event(Type type, int time, std::shared_ptr<Entity> entity, std::shared_ptr<Server> server) :
    type_(type), time_(time), entity_(entity), server_(server)
{
}

int Event::time() const
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

            Type Event::type() const
    {
            return type_;
}
