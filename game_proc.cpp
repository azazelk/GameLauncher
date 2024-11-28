#include "game_proc.h"

Game_proc::Game_proc(QObject*){}

Game_proc::Game_proc(QString& path): path(new QString(path)){}

Game_proc::~Game_proc()
{
    delete path;
}

void Game_proc::start()
{
    QProcess* proc = new QProcess(this);
    proc->start(*path);
    proc->waitForFinished(-1);
    emit finished();
    delete proc;
    delete this;
}
