#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>

namespace Ui {
class TextDialog;
}

class TextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextDialog(QString fileName = "", QString title = "", QWidget *parent = nullptr);
    ~TextDialog();

private:
    Ui::TextDialog *ui;

private slots:
    void setupText(QString fileName);
};

#endif // TEXTDIALOG_H
