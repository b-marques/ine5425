#include "event.h"

Event::Event(Type type, int time, Entity::Type entity_type) :
    type_(type), time_(time), entity_type_(entity_type), server_type_(static_cast<Server::Type>(entity_type))
{
}

void Event::process()
{
    switch (type_){
    case ENTITY_ARRIVAL:
        break;

    case ARRIVAL_AT_SERVER:
        break;

    case TEF_SERVER:
        break;

    case TF_SERVER:
        break;

    case ENTITY_DISPOSE:
        break;

    default:
        break;
    }
}

int Event::time() const
{
    return time_;
}
