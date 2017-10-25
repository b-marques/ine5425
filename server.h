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

    bool available() const;
    bool up() const;
    void available(bool available);
    void up(bool up);

    void reset(std::shared_ptr<RandomFunctionsData> ts_function,
               std::shared_ptr<RandomFunctionsData> tf_function,
               std::shared_ptr<RandomFunctionsData> tef_function);

    std::shared_ptr<RandomFunctionsData> ts_function() const;
    std::shared_ptr<RandomFunctionsData> tef_function() const;
    std::shared_ptr<RandomFunctionsData> tf_function() const;
    std::vector<std::shared_ptr<Entity>>& waiting_queue();
    Server::Type type() const;

    int failures_number() const;
    void failures_number(int failures_number);

    double down_time() const;
    void down_time(double down_time);

private:
    Type type_;
    bool up_;
    bool available_;

    int failures_number_;
    double down_time_;

    std::vector<std::shared_ptr<Entity>> waiting_queue_;
    std::shared_ptr<RandomFunctionsData> ts_function_;
    std::shared_ptr<RandomFunctionsData> tef_function_;
    std::shared_ptr<RandomFunctionsData> tf_function_;
};

#endif //SERVER_H
