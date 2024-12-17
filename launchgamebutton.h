#pragma once
#include <QtWidgets>
#include "game_proc.h"
#include "rename_dialog.h"

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
    QString* game_name;
    QAction* action_delete;
    QAction* action_set_icon;
    QAction* action_rename;
    QFile* games_list;
    int* this_button_number;
    void get_ico_path();
    void reset();
    static inline int current_button_num = 0;
    static inline bool game_launched = false;
public slots:
    void launch_game();
    void game_stoped();
    void toolmapper(QAction*);
signals:
    void reset_signal();
};

