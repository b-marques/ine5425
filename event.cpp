#include "event.h"

Event::Event(Type type, int time, std::shared_ptr<Entity> entity, std::shared_ptr<Server> server) :
    type_(type), time_(time), entity_(entity), server_(server)
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
