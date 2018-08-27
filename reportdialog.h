#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDialog(QWidget *parent = nullptr, QString text = "");
    ~ReportDialog();

private:
    Ui::ReportDialog *ui;

private slots:
    void saveReport();
    void saveTextReport(QString fileName, QString delimiter);
};

#endif // REPORTDIALOG_H
