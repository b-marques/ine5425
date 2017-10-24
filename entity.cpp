#include "entity.h"

Entity::Entity(Entity::Type type) :
    type_(type)
{
}

Entity::~Entity()
{
}

void Entity::arrival_time(int arrival_time)
{
    arrival_time_ = arrival_time;
}

void Entity::service_time(int service_time)
{
    service_time_ = service_time;
}

void Entity::dispose_time(int dispose_time)
{
    dispose_time_ = dispose_time;
}

