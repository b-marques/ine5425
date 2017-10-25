#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include <vector>
#include <memory>

#include "server.h"
#include "entity.h"
#include "event.h"
#include "ui_main_window.h"
#include "random_functions_data.h"

class SystemController
{
public:
    static SystemController* instance();
    ~SystemController();
    void start(Ui::MainWindow *ui);
    void config_initial_state();
    void create_initial_events();
    void process_events();
    void resume();
    void reset();
    void new_run();
    void run();
    void refresh_data();
    void print_data();
    void add_event(std::shared_ptr<Event> event);
    std::vector<double> extract_tec_entity_args(Entity::Type entity_type, RandomFunctionsData::Type function_type);
    std::vector<double> extract_ts_server_args(Server::Type server_type, RandomFunctionsData::Type function_type);
    std::vector<double> extract_tef_server_args(Server::Type server_type, RandomFunctionsData::Type function_type);
    std::vector<double> extract_tf_server_args(Server::Type server_type, RandomFunctionsData::Type function_type);

    void paused(bool paused);
    void stopped(bool stopped);

private:
    static SystemController* instance_;
    SystemController();

    std::vector<std::shared_ptr<Entity>> entities_;
    std::vector<std::shared_ptr<Server>> server_;
    std::vector<std::shared_ptr<Event>> calendar_;

    std::unique_ptr<RandomFunctionsData> tec_function_entity_1_;
    std::unique_ptr<RandomFunctionsData> tec_function_entity_2_;

    double clock_;

    bool paused_ = false;
    bool stopped_ = false;

    double simulation_time_;

    Ui::MainWindow* ui_;
};

#endif //SYSTEM_CONTROLLER_H
