#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
public:
    enum Type{
        ONE = 0,
        TWO = 1
    };

    Entity();
    ~Entity();

private:
    Type type_;
    int arrival_time_;
    int service_time_;
    int dispose_time_;

};

#endif //ENTITY_H
