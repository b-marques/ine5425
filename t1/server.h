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

    double average_queue_size() const;
    void average_queue_size(double average_queue_size);

    double last_queue_modified_time() const;
    void last_queue_modified_time(double last_queue_modified_time);

    double fail_time_in_working() const;
    void fail_time_in_working(double fail_time_in_working);

    std::vector<double>& available_history();
    std::vector<double>& down_time_history();

private:
    Type type_;
    bool up_;
    bool available_;

    int failures_number_;
    double average_queue_size_;
    double last_queue_modified_time_;
    double fail_time_in_working_;

    std::vector<double> available_history_;
    std::vector<double> down_time_history_;

    std::vector<std::shared_ptr<Entity>> waiting_queue_;
    std::shared_ptr<RandomFunctionsData> ts_function_;
    std::shared_ptr<RandomFunctionsData> tef_function_;
    std::shared_ptr<RandomFunctionsData> tf_function_;
};

#endif //SERVER_H
