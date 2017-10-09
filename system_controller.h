#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "server.h"
#include <list>

class SystemController
{
  public:
    SystemController();
    ~SystemController();

  private:
    int random_arrival_time();

    int _clock;
    std::list<Entity> _entity;  
    Server _server[2];
}

#endif //SYSTEM_CONTROLLER_H
