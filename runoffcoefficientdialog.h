#ifndef RUNOFFCOEFFICIENTDIALOG_H
#define RUNOFFCOEFFICIENTDIALOG_H

#include <QDialog>

namespace Ui {
class RunoffCoefficientDialog;
}

class RunoffCoefficientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RunoffCoefficientDialog(QWidget *parent = nullptr);
    ~RunoffCoefficientDialog();

    double runoffCoefficient() const;

private:
    Ui::RunoffCoefficientDialog *ui;
    double _runoffCoefficient;

private slots:
    void calculateCoefficient();
};

#endif // RUNOFFCOEFFICIENTDIALOG_H
