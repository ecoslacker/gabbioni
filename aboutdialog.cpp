#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent, QString version) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    _version = version;
    ui->labelVersion->setText(_version);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
