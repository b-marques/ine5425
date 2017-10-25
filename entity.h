#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
public:
    enum Type{
        ONE = 0,
        TWO = 1
    };

    Entity(Entity::Type type);
    ~Entity();

    void arrival_time(double arrival_time);
    void service_time(double service_time);
    void dispose_time(double dispose_time);
    void changed_server(bool changed_server);

    Type type() const;
    bool changed_server() const;
    double arrival_time() const;
    double service_time() const;
    double dispose_time() const;

    bool disposed() const;
    void disposed(bool disposed);

private:
    Type type_;
    double arrival_time_;
    double service_time_;
    double dispose_time_;
    bool changed_server_;
    bool disposed_;
};

#endif //ENTITY_H
