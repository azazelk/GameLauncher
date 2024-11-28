#include "launchgamebutton.h"
#include "ui_launchgamebutton.h"

LaunchGameButton::LaunchGameButton(QString& path, QWidget* parent): QWidget(parent),
    ui(new Ui::LaunchGameButton), path(new QString(path)), game_thread(new QThread), game_ico_path(new QString)
{
    ui->setupUi(this);
    this->setLayout(ui->vlayout);
    get_ico_path();
    ui->pushButton->setStyleSheet(QString("background: url(" + *game_ico_path + ") center center no-repeat;"));
}

LaunchGameButton::~LaunchGameButton()
{
    delete ui;
    delete path;
    delete game_thread;
    delete game_ico_path;
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
    ico_name += ".png";
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
    game_image.save(dir.absoluteFilePath(QString("games_pixmaps")) + QDir::separator() + ico_name, "png");
    *game_ico_path = dir.absoluteFilePath(QString("games_pixmaps/")) + ico_name;
}
