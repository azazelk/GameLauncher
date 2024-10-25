#include "launchgamebutton.h"
#include "ui_launchgamebutton.h"

LaunchGameButton::LaunchGameButton(QString& path, QWidget* parent): QWidget(parent),
    ui(new Ui::LaunchGameButton), path(new QString(path))
{
    ui->setupUi(this);
}

LaunchGameButton::~LaunchGameButton()
{
    delete ui;
}
