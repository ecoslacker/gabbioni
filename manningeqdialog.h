#ifndef MANNINGEQDIALOG_H
#define MANNINGEQDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include "channelsection.h"

namespace Ui {
class ManningEqDialog;
}

class ManningEqDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManningEqDialog(QWidget *parent = nullptr);
    ~ManningEqDialog();

    ChannelSection *channel() const;
    void setChannel(ChannelSection *channel);

    double discharge() const;
    void setDischarge(double discharge);

    QString strManning() const;

private:
    Ui::ManningEqDialog *ui;
    ChannelSection *_channel;
    double _discharge;

    QString _strManning;

private slots:
    void calculateDischarge();
    void selectRoughnessCoefficient();
};

#endif // MANNINGEQDIALOG_H
