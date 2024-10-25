#pragma once
#include <QtWidgets>

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
};

