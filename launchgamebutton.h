#pragma once
#include <QtWidgets>
#include "game_proc.h"

namespace Ui {
class LaunchGameButton;
}

class LaunchGameButton : public QWidget
{
    Q_OBJECT
public:
    explicit LaunchGameButton(QString&, QWidget* parent = nullptr);
    ~LaunchGameButton();
private:
    Ui::LaunchGameButton *ui;
    QString* path;
    QThread* game_thread;
    QString* game_ico_path;
    void get_ico_path();
    static inline bool game_launched = false;
public slots:
    void launch_game();
    void game_stoped();
};

