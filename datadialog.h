#ifndef DATADIALOG_H
#define DATADIALOG_H
#include <QDialog>
#include <QList>
#include <QStringList>

namespace Ui {
class DataDialog;
}

class DataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataDialog(QWidget *parent = nullptr);
    ~DataDialog();

    QList<QStringList> getData();

    QString getFileName() const;

private:
    Ui::DataDialog *ui;
    QList<QStringList> _data;
    QString _fileName;

private slots:
    void openFile();
    void saveFile();
    void setDataToTable();
    void getDataFromTable();
    void addRow();
    void removeRow();
};

#endif // DATADIALOG_H
