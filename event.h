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

    Event(Type type, int time, Entity::Type entity_type);
    void process();
    int time() const;

private:
    Type type_;
    int time_;
    Entity::Type entity_type_;
    Server::Type server_type_;
};

#endif // EVENT_H
