#include <iostream>
#include <algorithm>
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
    config_initial_state();
    create_initial_events();
}

void SystemController::config_initial_state()
{
    // Extracting and setting functions types and arguments
    RandomFunctionsData::Type ftype_ts_server_1 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_1->currentIndex());
    RandomFunctionsData::Type ftype_tef_server_1 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_3->currentIndex());
    RandomFunctionsData::Type ftype_tf_server_1 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_5->currentIndex());

    server_.at(0)->reset(std::make_unique<RandomFunctionsData>( RandomFunctionsData(ftype_ts_server_1, extract_ts_server_args(Server::ONE, ftype_ts_server_1))),
                         std::make_unique<RandomFunctionsData>( RandomFunctionsData(ftype_tef_server_1, extract_tef_server_args(Server::ONE, ftype_tef_server_1))),
                         std::make_unique<RandomFunctionsData>( RandomFunctionsData(ftype_tf_server_1, extract_tf_server_args(Server::ONE, ftype_tf_server_1))));


    RandomFunctionsData::Type ftype_ts_server_2 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_2->currentIndex());
    RandomFunctionsData::Type ftype_tef_server_2 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_4->currentIndex());
    RandomFunctionsData::Type ftype_tf_server_2 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_server_6->currentIndex());

    server_.at(1)->reset(std::make_unique<RandomFunctionsData>( RandomFunctionsData(ftype_ts_server_2, extract_ts_server_args(Server::TWO, ftype_ts_server_2))),
                         std::make_unique<RandomFunctionsData>( RandomFunctionsData(ftype_tef_server_2, extract_tef_server_args(Server::TWO, ftype_tef_server_2))),
                         std::make_unique<RandomFunctionsData>( RandomFunctionsData(ftype_tf_server_2, extract_tf_server_args(Server::TWO, ftype_tf_server_2))));

    RandomFunctionsData::Type ftype_tec_entity_1 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_ent_1->currentIndex());
    RandomFunctionsData::Type ftype_tec_entity_2 = static_cast<RandomFunctionsData::Type>(ui_->stacked_widget_ent_2->currentIndex());
    tec_function_entity_1_ = std::make_unique<RandomFunctionsData>( RandomFunctionsData( ftype_tec_entity_1, extract_tec_entity_args(Entity::ONE, ftype_tec_entity_1 )));
    tec_function_entity_2_ = std::make_unique<RandomFunctionsData>( RandomFunctionsData( ftype_tec_entity_2, extract_tec_entity_args(Entity::TWO, ftype_tec_entity_2 )));

    // Clear vectors
    calendar_.clear();
    entities_.clear();

    clock_ = 0;
}

void SystemController::create_initial_events()
{
    add_event(std::make_shared<Event>(Event(Event::ENTITY_ARRIVAL, 0, Entity::ONE)));
    add_event(std::make_shared<Event>(Event(Event::ENTITY_ARRIVAL, 0, Entity::TWO)));
    add_event(std::make_shared<Event>(Event(Event::ARRIVAL_AT_SERVER, 0, Entity::ONE)));
    add_event(std::make_shared<Event>(Event(Event::ARRIVAL_AT_SERVER, 0, Entity::TWO)));
    add_event(std::make_shared<Event>(Event(Event::TEF_SERVER, 0, Entity::ONE)));
    add_event(std::make_shared<Event>(Event(Event::TEF_SERVER, 0, Entity::TWO)));
}

void SystemController::next_event()
{

}

bool cmp_event(std::shared_ptr<Event> event_a, std::shared_ptr<Event> event_b)
{
    return (event_a->time() < event_b->time());
}

void SystemController::add_event(std::shared_ptr<Event> event)
{
    calendar_.push_back(event);
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

