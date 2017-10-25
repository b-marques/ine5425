#ifndef EVENT_H
#define EVENT_H

#include "entity.h"
#include "server.h"

class Event
{
public:
    enum Type{
        ENTITY_ARRIVAL,
        ARRIVAL_AT_SERVER,
        TEF_SERVER,
        TF_SERVER,
        ENTITY_DISPOSE
    };

    Event(Type type, double time, std::shared_ptr<Entity> entity, std::shared_ptr<Server> server);
    Type type() const;
    double time() const;
    std::shared_ptr<Entity> entity() const;
    std::shared_ptr<Server> server() const;
    void server(const std::shared_ptr<Server> &server);
    void time(double time);

private:
    Type type_;
    double time_;
    std::shared_ptr<Entity> entity_;
    std::shared_ptr<Server> server_;
};

#endif // EVENT_H
