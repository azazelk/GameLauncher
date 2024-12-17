#include "rename_dialog.h"
#include "ui_rename_dialog.h"

rename_dialog::rename_dialog(QWidget *parent): QDialog(parent), input_str(new QString), ui(new Ui::rename_dialog)
{
    ui->setupUi(this);
    ui->pushButton->setText(tr("Подтвердить"));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(input_accepted()));
}

rename_dialog::~rename_dialog()
{
    delete ui;
}

void rename_dialog::input_accepted(){
    *input_str = ui->lineEdit->text();
    this->close();
}

QString* rename_dialog::get_input(){
    return (input_str->isEmpty() ? nullptr : input_str);
}
