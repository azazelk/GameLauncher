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
    QGridLayout* grid_layout;
    QToolBar* tool_bar;
    QAction* add;
    QList<LaunchGameButton*>* launch_list;
public slots:
    void add_game();
};
