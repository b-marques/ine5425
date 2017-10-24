#include <vector>
#include <memory>

#include "main_window.h"
#include "ui_main_window.h"
#include "random_functions_data.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Modelagem e Simulação - T1"));
    system_controller_ = SystemController::instance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmb_box_tec_entity_1_activated(int index)
{
    ui->stacked_widget_ent_1->setCurrentIndex(index);
}

void MainWindow::on_cmb_box_tec_entity_2_activated(int index)
{
    ui->stacked_widget_ent_2->setCurrentIndex(index);
}

void MainWindow::on_cmb_box_ts_server_1_activated(int index)
{
    ui->stacked_widget_server_1->setCurrentIndex(index);
}

void MainWindow::on_cmb_box_ts_server_2_activated(int index)
{
    ui->stacked_widget_server_2->setCurrentIndex(index);
}

void MainWindow::on_cmb_box_tef_server_1_activated(int index)
{
    ui->stacked_widget_server_3->setCurrentIndex(index);
}

void MainWindow::on_cmb_box_tef_server_2_activated(int index)
{
    ui->stacked_widget_server_4->setCurrentIndex(index);
}

void MainWindow::on_cmb_box_tf_server_1_activated(int index)
{
    ui->stacked_widget_server_5->setCurrentIndex(index);
}

void MainWindow::on_cmb_box_tf_server_2_activated(int index)
{
    ui->stacked_widget_server_6->setCurrentIndex(index);
}

void MainWindow::on_start_simulation_button_clicked()
{
    system_controller_->start(ui);
}
