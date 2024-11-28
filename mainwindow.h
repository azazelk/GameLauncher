#pragma once
#include "launchgamebutton.h"
#include <QtWidgets>

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
private:
    QScrollArea* main_scrol_wgt;
    QWidget* main_wgt;
    QGridLayout* grid_layout;
    QToolBar* tool_bar;
    QAction* add;
    QList<LaunchGameButton*>* launch_list;
    QSize* launch_button_size;
    QFile* games_paths;
    int* launch_button_spacer;
    int* rows;
    int* cols;
    void games_reset(QFile*);
    inline void cols_rows_checker();
public slots:
    void add_game();
};
