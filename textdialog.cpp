#include "textdialog.h"
#include "ui_textdialog.h"
#include <QFile>
#include <QTextStream>

TextDialog::TextDialog(QString fileName, QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    setupText(fileName);
}

TextDialog::~TextDialog()
{
    delete ui;
}

void TextDialog::setupText(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString content = in.readAll();

    ui->plainTextEdit->setPlainText(content);
}
