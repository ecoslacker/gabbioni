#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr, QString version = "");
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
    QString _version;
};

#endif // ABOUTDIALOG_H
