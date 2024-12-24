#pragma once

#include <QDialog>

namespace Ui {
class rename_dialog;
}

class rename_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit rename_dialog(QWidget *parent = nullptr);
    ~rename_dialog();
    QString* get_input();
private:
    Ui::rename_dialog *ui;
    QString* input_str;
public slots:
    void input_accepted();
};

