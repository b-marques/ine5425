#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "system_controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum INTERVAL{
        CONSTANT = 0,
        NORMAL = 1,
        UNIFORM = 2,
        TRIANGULAR = 3,
        EXPONENTIAL = 4,
    };
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_cmb_box_tec_entity_1_activated(int index);
    void on_cmb_box_tec_entity_2_activated(int index);
    void on_cmb_box_ts_server_1_activated(int index);
    void on_cmb_box_ts_server_2_activated(int index);
    void on_cmb_box_tef_server_1_activated(int index);
    void on_cmb_box_tef_server_2_activated(int index);
    void on_cmb_box_tf_server_1_activated(int index);
    void on_cmb_box_tf_server_2_activated(int index);
    void on_start_simulation_button_clicked();
    void on_stop_simulation_button_clicked();
    void on_pause_simulation_button_clicked();

    void on_delay_slider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    SystemController* system_controller_;
};

#endif // MAINWINDOW_H
