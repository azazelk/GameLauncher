#pragma once
#include <QtWidgets>

class Game_proc: public QObject{
    Q_OBJECT
public:
    Game_proc(QObject* obj = nullptr);
    Game_proc(QString&);
    ~Game_proc();
private:
    QString* path;
public slots:
    void start();
signals:
    void finished();
};
