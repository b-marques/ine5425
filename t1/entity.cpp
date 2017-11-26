#include "entity.h"

Entity::Entity(Entity::Type type) :
    type_(type),
    arrival_time_(0),
    service_time_(0),
    dispose_time_(0),
    changed_server_(false),
    disposed_(false)
{
}

Entity::~Entity()
{
}

void Entity::arrival_time(double arrival_time)
{
    arrival_time_ = arrival_time;
}

void Entity::service_time(double service_time)
{
    service_time_ = service_time;
}

void Entity::dispose_time(double dispose_time)
{
    dispose_time_ = dispose_time;
}

Entity::Type Entity::type() const
{
    return type_;
}

bool Entity::changed_server() const
{
    return changed_server_;
}

void Entity::changed_server(bool changed_server)
{
    changed_server_ = changed_server;
}

double Entity::arrival_time() const
{
    return arrival_time_;
}

double Entity::service_time() const
{
    return service_time_;
}

double Entity::dispose_time() const
{
    return dispose_time_;
}

bool Entity::disposed() const
{
    return disposed_;
}

void Entity::disposed(bool disposed)
{
    disposed_ = disposed;
}
