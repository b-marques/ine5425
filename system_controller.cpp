#include <iostream>
#include <algorithm>
#include <QThread>
#include "system_controller.h"

SystemController* SystemController::instance_ = nullptr;

SystemController* SystemController::instance()
{
    if (instance_ == nullptr){
        instance_ = new SystemController();
    }
    return instance_;
}

SystemController::SystemController()
{
    server_.push_back( std::unique_ptr<Server>(new Server(Server::Type::ONE)) );
    server_.push_back( std::unique_ptr<Server>(new Server(Server::Type::TWO)) );
}

void SystemController::start(Ui::MainWindow *ui)
{
    ui_ = ui;

    if(stopped_){
        reset();
    } else if(paused_) {
        resume();
    } else {
        new_run();
    }
}

void SystemController::resume()
{
    paused_ = false;
    stopped_ = false;
    run();
}

void SystemController::reset()
{
    config_initial_state();
    create_initial_events();
    run();
}

void SystemController::new_run()
{
    ui_->pause_simulation_button->setEnabled(true);
    ui_->stop_simulation_button->setEnabled(true);
    config_initial_state();
    create_initial_events();
    run();
}

void SystemController::run()
{
    print_data();
    while(!calendar_.empty() && !paused_ && !stopped_ && clock_ <= simulation_time_){
        process_events();
        refresh_data();
        QCoreApplication::processEvents();
        QThread::currentThread()->msleep(ui_->delay_slider->value());
    }
    print_data();
}

void SystemController::refresh_data()
{
    ui_->progress_bar->setValue((clock_/simulation_time_)*100);
    if(clock_ > simulation_time_)
        ui_->progress_bar->setValue(100);
}

void SystemController::print_data()
{
    //    std::cout << "Calendar size: " << calendar_.size() << std::endl;
    //    std::cout << "Entities size: " << entities_.size() << std::endl;
    //    std::cout << "Serve 1 queue size: " << server_.at(0)->waiting_queue().size() << std::endl;
    //    std::cout << "Serve 2 queue size: " << server_.at(1)->waiting_queue().size() << std::endl;

    // Entities info
    int entities_quantity = 0,
            entities_1_quantity = 0,
            entities_2_quantity = 0,
            entities_disposed = 0,
            entities_1_disposed = 0,
            entities_2_disposed = 0,
            entities_not_processed = 0,
            entities_1_not_processed = 0,
            entities_2_not_processed = 0,
            entities_change = 0,
            entities_1_change = 0,
            entities_2_change = 0;
    double total_time_spent_in_queue = 0,
            average_time_spent_in_queue = 0,
            entities_time_spent_on_system = 0,
            entities_1_time_spent_on_system = 0,
            entities_2_time_spent_on_system = 0,
            average_entities_time_spent_on_system = 0,
            average_entities_1_time_spent_on_system = 0,
            average_entities_2_time_spent_on_system = 0;


    for(auto each_entity : entities_){
        total_time_spent_in_queue += (each_entity->service_time() - each_entity->arrival_time()) < 0
                ? clock_ - each_entity->arrival_time()
                : (each_entity->service_time() - each_entity->arrival_time());
        if(each_entity->type() == Entity::ONE){
            ++entities_1_quantity;
            if(each_entity->disposed()){
                ++entities_1_disposed;
                entities_1_time_spent_on_system += each_entity->dispose_time() - each_entity->arrival_time();
            }
            if(each_entity->changed_server())
                ++entities_1_change;
        } else {
            ++entities_2_quantity;
            if(each_entity->disposed()) {
                ++entities_2_disposed;
                entities_2_time_spent_on_system += each_entity->dispose_time() - each_entity->arrival_time();
            }
            if(each_entity->changed_server())
                ++entities_2_change;
        }
    }

    entities_quantity = entities_1_quantity + entities_2_quantity;
    entities_disposed = entities_1_disposed + entities_2_disposed;
    entities_change = entities_1_change + entities_2_change;
    average_time_spent_in_queue = entities_.size() == 0 ? 0 : total_time_spent_in_queue / entities_.size();
    entities_time_spent_on_system = entities_1_time_spent_on_system + entities_2_time_spent_on_system;
    average_entities_time_spent_on_system = entities_time_spent_on_system / entities_disposed;
    average_entities_1_time_spent_on_system = entities_1_time_spent_on_system / entities_1_disposed;
    average_entities_2_time_spent_on_system = entities_2_time_spent_on_system / entities_2_disposed;

    entities_1_not_processed = entities_1_quantity - entities_1_disposed;
    entities_2_not_processed = entities_2_quantity - entities_2_disposed;
    entities_not_processed = entities_1_not_processed + entities_2_not_processed;

//    std::cout << entities_quantity << std::endl;
//    std::cout << entities_1_quantity << std::endl;
//    std::cout << entities_2_quantity << std::endl;
//    std::cout << entities_disposed << std::endl;
//    std::cout << entities_1_disposed << std::endl;
//    std::cout << entities_2_disposed << std::endl;
//    std::cout << entities_not_processed << std::endl;
//    std::cout << entities_1_not_processed << std::endl;
//    std::cout << entities_2_not_processed << std::endl;

    // Server infos
    double down_time_server_1 = server_.at(0)->down_time();
    server_.at(0)->failures_number();
    double percent_time_spent_in_failure_1 = down_time_server_1/simulation_time_;

    double down_time_server_2 = server_.at(1)->down_time();
    server_.at(1)->failures_number();
    double percent_time_spent_in_failure_2 = down_time_server_2/simulation_time_;




}

void SystemController::config_initial_state()
{
    // Extracting and setting functions types and arguments
    RandomFunctionsData::Type ftype_ts_server_1 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_1->currentIndex());
    RandomFunctionsData::Type ftype_tef_server_1 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_3->currentIndex());
    RandomFunctionsData::Type ftype_tf_server_1 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_5->currentIndex());

    server_.at(0)->reset(std::make_shared<RandomFunctionsData>( RandomFunctionsData(ftype_ts_server_1, extract_ts_server_args(Server::ONE, ftype_ts_server_1))),
                         std::make_shared<RandomFunctionsData>( RandomFunctionsData(ftype_tef_server_1, extract_tef_server_args(Server::ONE, ftype_tef_server_1))),
                         std::make_shared<RandomFunctionsData>( RandomFunctionsData(ftype_tf_server_1, extract_tf_server_args(Server::ONE, ftype_tf_server_1))));


    RandomFunctionsData::Type ftype_ts_server_2 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_2->currentIndex());
    RandomFunctionsData::Type ftype_tef_server_2 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_4->currentIndex());
    RandomFunctionsData::Type ftype_tf_server_2 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_6->currentIndex());

    server_.at(1)->reset(std::make_shared<RandomFunctionsData>( RandomFunctionsData(ftype_ts_server_2, extract_ts_server_args(Server::TWO, ftype_ts_server_2))),
                         std::make_shared<RandomFunctionsData>( RandomFunctionsData(ftype_tef_server_2, extract_tef_server_args(Server::TWO, ftype_tef_server_2))),
                         std::make_shared<RandomFunctionsData>( RandomFunctionsData(ftype_tf_server_2, extract_tf_server_args(Server::TWO, ftype_tf_server_2))));

    RandomFunctionsData::Type ftype_tec_entity_1 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_ent_1->currentIndex());
    RandomFunctionsData::Type ftype_tec_entity_2 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_ent_2->currentIndex());
    tec_function_entity_1_ = std::make_unique<RandomFunctionsData>( RandomFunctionsData( ftype_tec_entity_1, extract_tec_entity_args(Entity::ONE, ftype_tec_entity_1 )));
    tec_function_entity_2_ = std::make_unique<RandomFunctionsData>( RandomFunctionsData( ftype_tec_entity_2, extract_tec_entity_args(Entity::TWO, ftype_tec_entity_2 )));

    // Clear vectors
    calendar_.clear();
    entities_.clear();

    // Reset simulation time
    simulation_time_ = ui_->simulation_time->value();

    // Reset progess bar
    ui_->progress_bar->setValue(0);

    // Reset variables
    paused_ = false;
    stopped_ = false;
    clock_ = 0;

}

void SystemController::create_initial_events()
{
    add_event(std::make_shared<Event>(Event(Event::ENTITY_ARRIVAL,
                                            0,
                                            std::make_shared<Entity>(Entity(Entity::ONE)),
                                            server_.at(0))));

    add_event(std::make_shared<Event>(Event(Event::ENTITY_ARRIVAL,
                                            0,
                                            std::make_shared<Entity>(Entity(Entity::TWO)),
                                            server_.at(1))));

    add_event(std::make_shared<Event>(Event(Event::TEF_SERVER,
                                            0,
                                            nullptr,
                                            server_.at(0))));

    add_event(std::make_shared<Event>(Event(Event::TEF_SERVER,
                                            0,
                                            nullptr,
                                            server_.at(1))));
}

void SystemController::process_events()
{
    std::shared_ptr<Event> event = calendar_.front();

    switch (event->type()){
    case Event::ENTITY_ARRIVAL:
    {
        event->entity()->arrival_time(event->time());
        entities_.push_back(event->entity());
        add_event(std::make_shared<Event>(Event(Event::ARRIVAL_AT_SERVER, event->time(), event->entity(), event->server())));

        switch (event->entity()->type()) {
        case Entity::Type::ONE:
        {
            add_event(std::make_shared<Event>(Event(Event::ENTITY_ARRIVAL, event->time()+tec_function_entity_1_->random_value(), std::make_shared<Entity>(Entity(Entity::ONE)), server_.at(0))));
            break;
        }
        case Entity::Type::TWO:
        {
            add_event(std::make_shared<Event>(Event(Event::ENTITY_ARRIVAL, event->time()+tec_function_entity_2_->random_value(), std::make_shared<Entity>(Entity(Entity::TWO)), server_.at(1))));
            break;
        }
        default:
            break;
        }
        break;
    }
    case Event::ARRIVAL_AT_SERVER:
    {
        if(event->server()->available() && event->server()->up()) {
            event->server()->available(false);
            event->entity()->service_time(event->time());
            add_event(std::make_shared<Event>(Event(Event::ENTITY_DISPOSE, event->time() + event->server()->ts_function()->random_value(), event->entity(), event->server())));

        } else if(!event->server()->available() && event->server()->up()) {
            event->server()->waiting_queue().push_back(event->entity());

        } else if(!event->server()->up()) {
            if(event->entity()->changed_server()){
                if(event->entity()->type() == Entity::Type::ONE){
                    server_.at(0)->waiting_queue().push_back(event->entity());

                } else {
                    server_.at(1)->waiting_queue().push_back(event->entity());
                }
                break;
            }
            event->entity()->changed_server(true);
            if(event->server()->type() == Server::Type::ONE){
                event->server(server_.at(1));
            } else {
                event->server(server_.at(0));
            }
            add_event(std::make_shared<Event>(Event(Event::ARRIVAL_AT_SERVER, event->time(), event->entity(), event->server())));
        }
        break;
    }
    case Event::TEF_SERVER:
    {
        event->server()->up(true);
        add_event(std::make_shared<Event>(Event(Event::TF_SERVER, event->time() + event->server()->tef_function()->random_value(), nullptr, event->server())));
        break;
    }
    case Event::TF_SERVER:
    {
        double random_value = event->server()->tf_function()->random_value();
        event->server()->up(false);
        event->server()->down_time(event->server()->down_time() + random_value);
        event->server()->failures_number(event->server()->failures_number() + 1);

        for(auto calendar_event : calendar_){
            if(calendar_event->type() == Event::ENTITY_DISPOSE && calendar_event->server()->type() == event->server()->type())
                calendar_event->time(calendar_event->time() + random_value);
        }
        add_event(std::make_shared<Event>(Event(Event::TEF_SERVER, event->time() + random_value, nullptr, event->server())));
        break;
    }
    case Event::ENTITY_DISPOSE:
    {
        event->entity()->dispose_time(event->time());
        event->entity()->disposed(true);

        if(!event->server()->waiting_queue().empty()) {
            event->server()->waiting_queue().front()->service_time(event->time());
            add_event(std::make_shared<Event>(Event(Event::ENTITY_DISPOSE, event->time() + event->server()->ts_function()->random_value(), event->server()->waiting_queue().front(), event->server())));
            event->server()->waiting_queue().erase(event->server()->waiting_queue().begin());
        } else {
            event->server()->available(true);
        }
        break;
    }
    default:
        break;
    }

    clock_ = event->time();
    calendar_.erase(calendar_.begin());

}

bool cmp_event(std::shared_ptr<Event> event_a, std::shared_ptr<Event> event_b)
{
    return (event_a->time() < event_b->time());
}

void SystemController::add_event(std::shared_ptr<Event> event)
{
    //Adiciona o evento
    calendar_.push_back(event);
    //Reordena os eventos de acordo com o tempo
    std::sort(calendar_.begin(), calendar_.end(), cmp_event);
}

std::vector<double> SystemController::extract_tec_entity_args(Entity::Type entity_type, RandomFunctionsData::Type function_type)
{
    std::vector<double> args;
    switch (entity_type) {
    case Entity::Type::ONE:
    {
        switch (function_type) {
        case RandomFunctionsData::Type::CONSTANT:
        {
            args.push_back(ui_->const_value_ent_1->value());
            break;
        }
        case RandomFunctionsData::Type::NORMAL:
        {
            args.push_back(ui_->normal_media_ent_1->value());
            args.push_back(ui_->normal_std_dev_ent_1->value());
            break;
        }
        case RandomFunctionsData::Type::UNIFORM:
        {
            args.push_back(ui_->uniform_min_ent_1->value());
            args.push_back(ui_->uniform_max_ent_1->value());
            break;
        }
        case RandomFunctionsData::Type::TRIANGULAR:
        {
            args.push_back(ui_->triang_min_ent_1->value());
            args.push_back(ui_->triang_moda_ent_1->value());
            args.push_back(ui_->triang_max_ent_1->value());
            break;
        }
        case RandomFunctionsData::Type::EXPONENTIAL:
        {
            args.push_back(ui_->expo_media_ent_1->value());
            break;
        }
        default:
            break;
        }
        break;
    }
    case Entity::Type::TWO:
    {
        switch (function_type) {
        case RandomFunctionsData::Type::CONSTANT:
        {
            args.push_back(ui_->const_value_ent_2->value());
            break;
        }
        case RandomFunctionsData::Type::NORMAL:
        {
            args.push_back(ui_->normal_media_ent_2->value());
            args.push_back(ui_->normal_std_dev_ent_2->value());
            break;
        }
        case RandomFunctionsData::Type::UNIFORM:
        {
            args.push_back(ui_->uniform_min_ent_2->value());
            args.push_back(ui_->uniform_max_ent_2->value());
            break;
        }
        case RandomFunctionsData::Type::TRIANGULAR:
        {
            args.push_back(ui_->triang_min_ent_2->value());
            args.push_back(ui_->triang_moda_ent_2->value());
            args.push_back(ui_->triang_max_ent_2->value());
            break;
        }
        case RandomFunctionsData::Type::EXPONENTIAL:
        {
            args.push_back(ui_->expo_media_ent_2->value());
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return args;
}

std::vector<double> SystemController::extract_ts_server_args(Server::Type server_type, RandomFunctionsData::Type function_type)
{
    std::vector<double> args;
    switch (server_type) {
    case Server::Type::ONE:
    {
        switch (function_type) {
        case RandomFunctionsData::Type::CONSTANT:
        {
            args.push_back(ui_->const_value_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::NORMAL:
        {
            args.push_back(ui_->normal_media_server_1->value());
            args.push_back(ui_->normal_std_dev_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::UNIFORM:
        {
            args.push_back(ui_->uniform_min_server_1->value());
            args.push_back(ui_->uniform_max_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::TRIANGULAR:
        {
            args.push_back(ui_->triang_min_server_1->value());
            args.push_back(ui_->triang_moda_server_1->value());
            args.push_back(ui_->triang_max_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::EXPONENTIAL:
        {
            args.push_back(ui_->expo_media_server_1->value());
            break;
        }
        default:
            break;
        }
        break;
    }
    case Server::Type::TWO:
    {
        switch (function_type) {
        case RandomFunctionsData::Type::CONSTANT:
        {
            args.push_back(ui_->const_value_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::NORMAL:
        {
            args.push_back(ui_->normal_media_server_2->value());
            args.push_back(ui_->normal_std_dev_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::UNIFORM:
        {
            args.push_back(ui_->uniform_min_server_2->value());
            args.push_back(ui_->uniform_max_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::TRIANGULAR:
        {
            args.push_back(ui_->triang_min_server_2->value());
            args.push_back(ui_->triang_moda_server_2->value());
            args.push_back(ui_->triang_max_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::EXPONENTIAL:
        {
            args.push_back(ui_->expo_media_server_2->value());
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return args;
}

std::vector<double> SystemController::extract_tef_server_args(Server::Type server_type, RandomFunctionsData::Type function_type)
{
    std::vector<double> args;
    switch (server_type) {
    case Server::Type::ONE:
    {
        switch (function_type) {
        case RandomFunctionsData::Type::CONSTANT:
        {
            args.push_back(ui_->tef_const_value_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::NORMAL:
        {
            args.push_back(ui_->tef_normal_media_server_1->value());
            args.push_back(ui_->tef_normal_std_dev_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::UNIFORM:
        {
            args.push_back(ui_->tef_uniform_min_server_1->value());
            args.push_back(ui_->tef_uniform_max_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::TRIANGULAR:
        {
            args.push_back(ui_->tef_triang_min_server_1->value());
            args.push_back(ui_->tef_triang_moda_server_1->value());
            args.push_back(ui_->tef_triang_max_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::EXPONENTIAL:
        {
            args.push_back(ui_->tef_expo_media_server_1->value());
            break;
        }
        default:
            break;
        }
        break;
    }
    case Server::Type::TWO:
    {
        switch (function_type) {
        case RandomFunctionsData::Type::CONSTANT:
        {
            args.push_back(ui_->tef_const_value_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::NORMAL:
        {
            args.push_back(ui_->tef_normal_media_server_2->value());
            args.push_back(ui_->tef_normal_std_dev_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::UNIFORM:
        {
            args.push_back(ui_->tef_uniform_min_server_2->value());
            args.push_back(ui_->tef_uniform_max_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::TRIANGULAR:
        {
            args.push_back(ui_->tef_triang_min_server_2->value());
            args.push_back(ui_->tef_triang_moda_server_2->value());
            args.push_back(ui_->tef_triang_max_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::EXPONENTIAL:
        {
            args.push_back(ui_->tef_expo_media_server_2->value());
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return args;
}

std::vector<double> SystemController::extract_tf_server_args(Server::Type server_type, RandomFunctionsData::Type function_type)
{
    std::vector<double> args;
    switch (server_type) {
    case Server::Type::ONE:
    {
        switch (function_type) {
        case RandomFunctionsData::Type::CONSTANT:
        {
            args.push_back(ui_->tf_const_value_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::NORMAL:
        {
            args.push_back(ui_->tf_normal_media_server_1->value());
            args.push_back(ui_->tf_normal_std_dev_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::UNIFORM:
        {
            args.push_back(ui_->tf_uniform_min_server_1->value());
            args.push_back(ui_->tf_uniform_max_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::TRIANGULAR:
        {
            args.push_back(ui_->tf_triang_min_server_1->value());
            args.push_back(ui_->tf_triang_moda_server_1->value());
            args.push_back(ui_->tf_triang_max_server_1->value());
            break;
        }
        case RandomFunctionsData::Type::EXPONENTIAL:
        {
            args.push_back(ui_->tf_expo_media_server_1->value());
            break;
        }
        default:
            break;
        }
        break;
    }
    case Server::Type::TWO:
    {
        switch (function_type) {
        case RandomFunctionsData::Type::CONSTANT:
        {
            args.push_back(ui_->tf_const_value_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::NORMAL:
        {
            args.push_back(ui_->tf_normal_media_server_2->value());
            args.push_back(ui_->tf_normal_std_dev_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::UNIFORM:
        {
            args.push_back(ui_->tf_uniform_min_server_2->value());
            args.push_back(ui_->tf_uniform_max_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::TRIANGULAR:
        {
            args.push_back(ui_->tf_triang_min_server_2->value());
            args.push_back(ui_->tf_triang_moda_server_2->value());
            args.push_back(ui_->tf_triang_max_server_2->value());
            break;
        }
        case RandomFunctionsData::Type::EXPONENTIAL:
        {
            args.push_back(ui_->tf_expo_media_server_2->value());
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return args;
}

void SystemController::stopped(bool stopped)
{
    stopped_ = stopped;
}

void SystemController::paused(bool paused)
{
    paused_ = paused;
}
