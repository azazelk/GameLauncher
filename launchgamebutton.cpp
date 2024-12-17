#include "launchgamebutton.h"
#include "ui_launchgamebutton.h"

LaunchGameButton::LaunchGameButton(QString& path, QWidget* parent): QWidget(parent),
    ui(new Ui::LaunchGameButton), path(new QString(path)), game_thread(new QThread), game_ico_path(new QString), game_name(nullptr),
    action_delete(new QAction(tr("Удалить"),this)), action_set_icon(new QAction(tr("Изменить ярлык"),this)),
    action_rename(new QAction(tr("Переименовать"), this)), this_button_number(new int)
{
    ui->setupUi(this);
    this->setLayout(ui->vlayout);

    QDir current_dir = QDir::current();
    games_list = new QFile(current_dir.absoluteFilePath("games_list.txt"));
    reset();
    get_ico_path();
    ui->pushButton->setStyleSheet(QString("background: url(" + *game_ico_path + ") center center no-repeat;"));
    action_rename->setData(0);
    action_set_icon->setData(1);
    action_delete->setData(2);
    QList<QAction*> action_list = {
        action_rename,
        action_set_icon,
        action_delete
    };
    ui->toolButton->setPopupMode(QToolButton::InstantPopup);
    ui->toolButton->addActions(action_list);
    ui->toolButton->setText(*game_name);
    connect(ui->toolButton, SIGNAL(triggered(QAction*)), this, SLOT(toolmapper(QAction*)));
}

LaunchGameButton::~LaunchGameButton()
{
    delete ui;
    delete path;
    delete game_thread;
    delete game_ico_path;
    delete game_name;
    delete action_delete;
    delete action_set_icon;
    delete action_rename;
    delete games_list;
    delete this_button_number;
}

void LaunchGameButton::launch_game(){
    if (!game_launched){
        Game_proc* game_proc = new Game_proc(*path);
        game_proc->moveToThread(game_thread);
        connect(game_thread, SIGNAL(started()), game_proc, SLOT(start()));
        connect(game_proc, SIGNAL(finished()), this, SLOT(game_stoped()));
        game_thread->start();
        game_launched = true;
    }
}

void LaunchGameButton::game_stoped(){
    game_thread->quit();
    game_launched = false;
}

void LaunchGameButton::get_ico_path(){
    if (game_name != nullptr && *game_name != ""){
        return;
    }
    game_name = new QString();
    QString ico_name;
    QRegularExpression regexp("(/+[a-zA-Z0-9]+\.exe)");
    QRegularExpressionMatch mreg = regexp.match(*path);
    if (mreg.hasMatch()){
        ico_name = mreg.captured();
        ico_name.removeFirst();
        QString::Iterator it = std::find(ico_name.begin(), ico_name.end(), '.');
        ico_name.erase(it, ico_name.end());
    } else {
        return;
    }
    *game_name = ico_name;
    games_list->open(QIODevice::Append);
    QTextStream out(games_list);
    out << *game_name << "\n";
    games_list->close();
    QDir dir = QDir::current();
    dir.mkdir("games_pixmaps");
    QDirIterator dir_iter(dir.absoluteFilePath(QString("games_pixmaps")), QStringList() << "*.png");
    while (dir_iter.hasNext()) {
        if (dir_iter.next() == ico_name){
            return;
        }
    }
    QFileSystemModel fmodel;
    QFileInfo fin(*path);
    fmodel.setRootPath(*path);
    QImage game_image = fmodel.iconProvider()->icon(fin).pixmap(32, 32).toImage();
    game_image = game_image.scaled(104, 140);
    ico_name += ".png";
    game_image.save(dir.absoluteFilePath(QString("games_pixmaps")) + "/" + ico_name, "png");
    *game_ico_path = dir.absoluteFilePath(QString("games_pixmaps")) + "/" + ico_name;
}

void LaunchGameButton::toolmapper(QAction* act){
    int act_data = act->data().toInt();
    QDir dir = QDir::current();
    if (act_data == 0){
        rename_dialog dlg;
        QString old_name = *game_name;
        dlg.exec();
        if (dlg.get_input() != nullptr){
            game_name = dlg.get_input();
            ui->toolButton->setText(*game_name);
            games_list->open(QIODevice::ReadOnly);
            QList<QByteArray> names = games_list->readAll().split('\n');
            games_list->close();
            games_list->open(QIODevice::WriteOnly);
            QTextStream out(games_list);
            for (int i = 0; i < names.size(); i++){
                if (i == *this_button_number){
                    out << *game_name << "\n";
                } else {
                    out << names.at(i) << "\n";
                }
            }
            games_list->close();
        }
        QFile::rename("games_pixmaps/" + old_name + ".png", "games_pixmaps/" + *game_name + ".png");
        *game_ico_path = dir.absoluteFilePath(QString("games_pixmaps")) + QDir::separator() + *game_name + ".png";
    } else if (act_data == 1){
        QString ico_path = QFileDialog::getOpenFileName(this, tr("Выберите ярлык"), "", "*.png, *.jpg");
        if (ico_path.isEmpty()){
            return;
        }
        QFile::remove(dir.absoluteFilePath(QString("games_pixmaps")) + QDir::separator() + *game_name + ".png");
        QImage pic(ico_path);
        QFile::remove("games_pixmaps/" + *game_name + ".png");
        pic.scaled(100, 140).save("games_pixmaps/" + *game_name + ".png", "png");
        ui->pushButton->setStyleSheet(QString("background: url(" + *game_ico_path + ") center center no-repeat;"));
    } else if (act_data == 2){
        this->hide();
        games_list->open(QIODevice::ReadOnly);
        QList<QByteArray> games_list_copy = games_list->readAll().split('\n');
        QFile paths_list("games_paths.txt");
        paths_list.open(QIODevice::ReadOnly);
        QList<QByteArray> paths_list_copy = paths_list.readAll().split('\n');
        paths_list.close();
        games_list->close();
        paths_list.open(QIODevice::WriteOnly);
        games_list->open(QIODevice::WriteOnly);
        games_list_copy.erase(games_list_copy.end()-1);
        paths_list_copy.erase(paths_list_copy.end()-1);
        QTextStream out1(games_list);
        QTextStream out2(&paths_list);
        for (int i = 0; i < games_list_copy.size(); i++){
            if (i == *this_button_number){
                continue;
            } else {
                out1 << games_list_copy.at(i) << "\n";
                out2 << paths_list_copy.at(i) << "\n";
            }
        }
        games_list->close();
        paths_list.close();
        QFile::remove("games_pixmaps/" + *game_name + ".png");
        emit reset_signal();
        delete this;
    }
}

void LaunchGameButton::reset()
{
    games_list->open(QIODevice::ReadWrite);
    QList<QByteArray> names = games_list->readAll().split('\n');
    names.erase(names.end()-1);
    games_list->close();
    if (names.length() > current_button_num){
        game_name = new QString();
        *game_name = names.at(current_button_num);
        current_button_num++;
        *this_button_number = current_button_num-1;
        QDir dir = QDir::current();
        *game_ico_path = dir.absoluteFilePath(QString("games_pixmaps")) + "/" + *game_name + ".png";
    } else {
        current_button_num++;
        *this_button_number = current_button_num-1;
    }

}
