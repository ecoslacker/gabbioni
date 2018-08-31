#ifndef RATIONALFORMULADIALOG_H
#define RATIONALFORMULADIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class RationalFormulaDialog;
}

class RationalFormulaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RationalFormulaDialog(QWidget *parent = nullptr);
    ~RationalFormulaDialog();

    double runoff() const;
    void setRunoff(double runoff);

    QString strRational() const;

private:
    Ui::RationalFormulaDialog *ui;
    double _runoff;
    QString _strRational;

private slots:
    void calculate();
    void clear();
    void timeConcentration();
    void runoffCoefficient();
};

#endif // RATIONALFORMULADIALOG_H
