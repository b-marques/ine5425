#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <vector>
#include "entity.h"
#include "random_functions_data.h"

class Server
{
public:
    enum Type{
        ONE = 0,
        TWO = 1
    };
    Server(Type type);
    ~Server();
    void processing_end();

    bool available() const;
    void available(bool available);

    bool up() const;
    void up(bool up);

    void reset(std::unique_ptr<RandomFunctionsData> ts_function,
               std::unique_ptr<RandomFunctionsData> tf_function,
               std::unique_ptr<RandomFunctionsData> tef_function);

private:
    Type type_;
    bool up_;
    bool available_;
    std::vector<Entity> waiting_queue_;
    std::unique_ptr<RandomFunctionsData> ts_function_;
    std::unique_ptr<RandomFunctionsData> tef_function_;
    std::unique_ptr<RandomFunctionsData> tf_function_;
};

#endif //SERVER_H
