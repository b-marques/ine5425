#ifndef SYSTEM_CORE_H
#define SYSTEM_CORE_H

#include "employee.h"
#include <list>

class SystemCore 
{
  public:
    SystemCore();
    ~SystemCore();

  private:
    int random_arrive_time();

    int _clock;
    std::list<Client> clients;  
    Employee _employee[2];
}

#endif //SYSTEM_CORE_H