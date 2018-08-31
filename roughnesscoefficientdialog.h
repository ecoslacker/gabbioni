#ifndef ROUGHNESSCOEFFICIENTDIALOG_H
#define ROUGHNESSCOEFFICIENTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QLabel>

namespace Ui {
class RoughnessCoefficientDialog;
}

class RoughnessCoefficientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoughnessCoefficientDialog(QWidget *parent = nullptr);
    ~RoughnessCoefficientDialog();

    QString coefficient() const;

private:
    Ui::RoughnessCoefficientDialog *ui;
    QString _coefficient;

private slots:
    void populateTables();
    void populateTable(QString txtFile, QTableWidget *table, QLabel *label);
    void retrieveItemValue(QTableWidgetItem *item);
    void updateCoefficient();
};

#endif // ROUGHNESSCOEFFICIENTDIALOG_H
