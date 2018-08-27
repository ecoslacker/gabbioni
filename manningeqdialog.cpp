#include <QPixmap>
#include "manningeqdialog.h"
#include "ui_manningeqdialog.h"
#include "roughnesscoefficientdialog.h"

ManningEqDialog::ManningEqDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManningEqDialog)
{
    ui->setupUi(this);
    connect(ui->buttonCalculate, SIGNAL(pressed()), SLOT(calculateDischarge()));
    connect(ui->toolButton,      SIGNAL(pressed()), SLOT(selectRoughnessCoefficient()));

    _strManning = "";
}

ManningEqDialog::~ManningEqDialog()
{
    delete ui;
}

ChannelSection *ManningEqDialog::channel() const
{
    return _channel;
}

void ManningEqDialog::setChannel(ChannelSection *channel)
{
    _channel = channel;
}

void ManningEqDialog::calculateDischarge()
{
    // Get the information from the GUI
    double n = ui->coefficient->text().toDouble();
    double slope = ui->slope->text().toDouble();
    double height = ui->height->text().toDouble();

    // Check the input values
    if (height <= 0 || slope <= 0 || n <= 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Input error");
        msgBox.setText(tr("Input error in roughness coefficient, slope or water height values."));
        msgBox.setInformativeText(tr("Roughness coefficient, slope and water height must be numeric values greater than zero."));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    if (height >= _channel->channelDepth()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Input error");
        msgBox.setText(tr("Input error in water height value."));
        msgBox.setInformativeText(tr("The water height cannot be equal or greather than the channel's depth."));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

//    if (slope == 0) {
//        QMessageBox msgBox;
//        msgBox.setWindowTitle("Warning in slope");
//        msgBox.setText(tr("Slope is zero."));
//        msgBox.setInformativeText(tr("Calculations will continue, but make sure the slope value is correct."));
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setStandardButtons(QMessageBox::Ok);
//        msgBox.setDefaultButton(QMessageBox::Ok);
//        msgBox.exec();
//    }

    // Calculate the discharge
    qDebug() << "Channel depth ok:" << _channel->channelDepth();
    _channel->setWaterHeight(height);
    _channel->setSlope(slope);
    _channel->setCoefficient(n);

    _channel->computeDischarge();
    _discharge = _channel->discharge();

    double hydraulicArea = _channel->hydraulicArea();
    double wettedPerimeter = _channel->wettedPerimeter();
    double hydraulicRadius = _channel->hydraulicRadius();
    double velocity = _channel->flowVelocity();

    // Set the results to the GUI
    ui->area->setText(QString::number(hydraulicArea, 'f', 2));
    ui->perimeter->setText(QString::number(wettedPerimeter, 'f', 2));
    ui->radius->setText(QString::number(hydraulicRadius, 'f', 2));
    ui->velocity->setText(QString::number(velocity, 'f', 2));
    ui->discharge->setText(QString::number(_discharge, 'f', 3));

    _strManning = "Chezy-Manning equation for runoff estimation\n";
    _strManning += QString("\n");
    _strManning += QString("Manning coefficient (n):\t%1\t%2\n").arg(n).arg("");
    _strManning += QString("Stream slope:\t%1\t%2\n").arg(slope).arg("m/m");
    _strManning += QString("Water height:\t%1\t%2\n").arg(height).arg("m");
    _strManning += QString("\nHydraulic properties\n");
    _strManning += QString("Hydraulic area:\t%1\t%2\n").arg(hydraulicArea).arg("m^2");
    _strManning += QString("Wetted perimeter:\t%1\t%2\n").arg(wettedPerimeter).arg("m");
    _strManning += QString("Hydraulic radius:\t%1\t%2\n").arg(hydraulicRadius).arg("m");
    _strManning += QString("Flow velocity:\t%1\t%2\n").arg(hydraulicRadius).arg("m/s");
    _strManning += QString("Discharge:\t%1\t%2\n").arg(_discharge).arg("m^3/s");
}

void ManningEqDialog::selectRoughnessCoefficient()
{
    RoughnessCoefficientDialog roughnessDlg;
    if (roughnessDlg.exec() == QDialog::Accepted) {
        QString coefficient = roughnessDlg.coefficient();
        if (coefficient.toDouble() != 0)
            ui->coefficient->setText(coefficient);
    }
}

double ManningEqDialog::discharge() const
{
    return _discharge;
}

void ManningEqDialog::setDischarge(double discharge)
{
    _discharge = discharge;
}

QString ManningEqDialog::strManning() const
{
    return _strManning;
}
