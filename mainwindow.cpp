#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    QDir current_dir(QDir::current());
    games_paths = new QFile(current_dir.absoluteFilePath("games_paths.txt"));

    launch_list = new QList<LaunchGameButton*>;
    launch_button_size = new QSize(102, 142);
    rows = new int(0);
    cols = new int(0);
    this->setMinimumSize(845, 400);
    this->resize(845, 400);
    this->setWindowTitle("GameLauncher");
    this->setWindowIcon(QIcon(":/images/img/icon.ico"));
    main_scrol_wgt = new QScrollArea(this);
    main_scrol_wgt->setWidgetResizable(true);
    main_wgt = new QWidget(main_scrol_wgt);
    grid_layout = new QGridLayout(main_wgt);

    grid_layout->setColumnStretch(0, 104);
    grid_layout->setColumnStretch(1, 104);
    grid_layout->setColumnStretch(2, 104);
    grid_layout->setColumnStretch(3, 104);
    grid_layout->setColumnStretch(4, 104);
    grid_layout->setColumnStretch(5, 104);
    grid_layout->setColumnStretch(6, 104);
    grid_layout->setRowStretch(0, 140);
    grid_layout->setRowStretch(1, 140);
    grid_layout->setRowStretch(2, 140);

    main_wgt->setLayout(grid_layout);
    main_scrol_wgt->setWidget(main_wgt);
    main_scrol_wgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    main_wgt->resize(main_scrol_wgt->size());
    main_wgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tool_bar = new QToolBar(this);
    tool_bar->setMovable(false);
    tool_bar->setMinimumWidth(50);
    add = new QAction(tr("Добавить"), tool_bar);
    tool_bar->addAction(add);
    this->setCentralWidget(main_scrol_wgt);
    this->addToolBar(Qt::ToolBarArea::RightToolBarArea, tool_bar);

    games_reset(games_paths);

    connect(add, SIGNAL(triggered(bool)), this, SLOT(add_game()));
}

MainWindow::~MainWindow()
{
    delete launch_list;
    delete launch_button_size;
    delete rows;
    delete cols;
}

void MainWindow::add_game()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Выберите игру"), "", "*.exe");
    auto path_cheker = [=](){
        games_paths->open(QIODevice::ReadWrite);
        QList<QByteArray> path_from_file = games_paths->readAll().split('\n');
        QString path_ff;
        games_paths->close();
        int i = 0;
        while(i < path_from_file.size()){
            path_ff = path_from_file[0];
            if (path == path_ff){
                return false;
            }
            i++;
        }
        return true;
    };
    if (!path.isEmpty() && path_cheker()){
        launch_list->append(new LaunchGameButton(path, this));
        grid_layout->addWidget(launch_list->last(), *rows, *cols);
        games_paths->open(QIODevice::Append);
        QTextStream out(games_paths);
        out << path << '\n';
        games_paths->close();
        (*cols)++;
    } else {
        QMessageBox alert;
        alert.setText(tr("Игра уже добавлена!"));
        alert.setWindowTitle(tr("Предупреждение"));
        alert.setWindowIcon(QIcon(":/images/img/warning.ico"));
        alert.exec();
    }
    cols_rows_checker();
}

void MainWindow::games_reset(QFile* games_paths){
    games_paths->open(QIODevice::ReadOnly);
    if (games_paths->isOpen()){
        QList<QByteArray> games_from_file = games_paths->readAll().split('\n');
        QString game_path;
        int i = 0;
        while (i < games_from_file.size()-1){
            game_path = games_from_file.at(i);
            launch_list->append(new LaunchGameButton(game_path, this));
            grid_layout->addWidget(launch_list->last(), *rows, *cols);
            (*cols)++;
            cols_rows_checker();
            i++;
        }
    } else {
        games_paths->open(QIODevice::WriteOnly);
    }
    games_paths->close();
}

void MainWindow::cols_rows_checker(){
    if (*cols == 7){
        (*cols) = 0;
        (*rows)++;
    }
}
