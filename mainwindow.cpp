#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    launch_list = new QList<LaunchGameButton*>;
    this->setMinimumSize(800, 400);
    this->resize(800, 400);
    this->setWindowTitle("GameLauncher");
    this->setWindowIcon(QIcon(":/images/img/icon.ico"));
    main_scrol_wgt = new QScrollArea(this);
    grid_layout = new QGridLayout(main_scrol_wgt);
    tool_bar = new QToolBar(this);
    tool_bar->setMovable(false);
    tool_bar->setMinimumWidth(50);
    main_scrol_wgt->setLayout(grid_layout);
    this->setCentralWidget(main_scrol_wgt);
    add = new QAction(tr("Добавить"), tool_bar);
    tool_bar->addAction(add);
    this->addToolBar(Qt::ToolBarArea::RightToolBarArea, tool_bar);

    connect(add, SIGNAL(triggered(bool)), this, SLOT(add_game()));
}

MainWindow::~MainWindow()
{
    delete launch_list;
}

void MainWindow::add_game()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Выберите игру"), "", "*.exe");
    launch_list->append(new LaunchGameButton(path, this));
    grid_layout->addWidget(launch_list->last());
}
