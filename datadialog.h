#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <string>
#include <vector>
#include <QDialog>
#include "csvdata.h"

namespace Ui {
class DataDialog;
}

class DataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataDialog(QWidget *parent = nullptr);
    ~DataDialog();

    std::vector<std::vector<std::string> > getData();

    QString getFileName() const;

private:
    Ui::DataDialog *ui;
    std::vector<std::vector<std::string> > _data;
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
