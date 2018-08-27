#include "rationalformuladialog.h"
#include "ui_rationalformuladialog.h"
#include "runoffcoefficientdialog.h"
#include "hydrology.h"

RationalFormulaDialog::RationalFormulaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RationalFormulaDialog)
{
    ui->setupUi(this);
    connect(ui->buttonCalculate, SIGNAL(pressed()), SLOT(calculate()));
    connect(ui->buttonClear,     SIGNAL(pressed()), SLOT(clear()));
    connect(ui->streamLength,    SIGNAL(textChanged(QString)), SLOT(timeConcentration()));
    connect(ui->streamRise,      SIGNAL(textChanged(QString)), SLOT(timeConcentration()));
    connect(ui->toolButton,      SIGNAL(pressed()), SLOT(runoffCoefficient()));
    _strRational = "";
}

RationalFormulaDialog::~RationalFormulaDialog()
{
    delete ui;
}

double RationalFormulaDialog::runoff() const
{
    return _runoff;
}

void RationalFormulaDialog::setRunoff(double runoff)
{
    _runoff = runoff;
}

QString RationalFormulaDialog::strRational() const
{
    return _strRational;
}

void RationalFormulaDialog::calculate()
{
    // Get the information from dialog
    double area = ui->catchmentArea->text().toDouble();
    double intensity = ui->rainfallIntensity->text().toDouble();
    double coefficient = ui->coefficient->text().toDouble();

    // Check the input data
    if (area <= 0 || intensity <= 0 || coefficient <= 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Input error"));
        msgBox.setText(tr("Invalid input values for catchment area, rainfall intensity or runoff coefficient."));
        msgBox.setInformativeText(tr("Catchment area, rainfall intensity and runoff coefficient must be numeric values greater than zero."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    if (coefficient > 1.0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Input error"));
        msgBox.setText(tr("Runoff coefficient cannot be greater than 1"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    // Create and calculate the runoff
    _runoff = hydrology::rationalFormula(coefficient, intensity, area);

    // Set the results to the GUI
    ui->runoff->setText(QString::number(_runoff, 'f', 3));
//    ui->timeConcentration->setText(QString::number(concTime, 'f', 2));

//    _strRational = "Rational Formula\n";
//    _strRational += QString("\nSoil information\n");
//    _strRational += QString("Land use:\t%1\t%2\n").arg(ui->landUse->currentText()).arg(land_use);
//    _strRational += QString("Soil type:\t%1\t%2\n").arg(ui->soilType->currentText()).arg(soil_type);
//    _strRational += QString("\nPrecipitation\n");
//    _strRational += QString("Annual precipitation:\t%1\t%2\n").arg(precip).arg("mm");
//    _strRational += QString("Rainfall intensity:\t%1\t%2\n").arg(intensity).arg("mm/h");
//    _strRational += QString("\nWatershed\n");
//    _strRational += QString("Catchment area:\t%1\t%2\n").arg(area).arg("ha");
//    _strRational += QString("Length of main stream:\t%1\t%2\n").arg(run).arg("m");
//    _strRational += QString("Rise of main stream:\t%1\t%2\n").arg(rise).arg("m");
//    _strRational += QString("\nHydrologic properties\n");
//    _strRational += QString("Time of concentration:\t%1\t%2\n").arg(concTime).arg("min");
//    _strRational += QString("Runoff coefficient:\t%1\t%2\n").arg(coefficient).arg("");
//    _strRational += QString("Runoff (discharge):\t%1\t%2\n").arg(_runoff).arg("m^3/s");
}

void RationalFormulaDialog::clear()
{
    ui->catchmentArea->clear();
    ui->coefficient->clear();
    ui->rainfallIntensity->clear();
    ui->streamLength->clear();
    ui->streamRise->clear();
}

void RationalFormulaDialog::timeConcentration()
{
    double tc;
    double run = ui->streamLength->text().toDouble();
    double rise = ui->streamRise->text().toDouble();

    if (run > 0 && rise > 0) {
        // Use the Kirpich equation to calculate time of concentration
        // Warning! This is just to help, an accurate
        tc = hydrology::kirpich(run, rise);

        ui->timeConcentration->setText(QString::number(tc, 'f', 3));
    } else {
//        QMessageBox msgBox;
//        msgBox.setWindowTitle(tr("Input error"));
//        msgBox.setText(tr("Invalid input values for length or rise of the main stream."));
//        msgBox.setInformativeText(tr("Length and rise of the main stream must be numeric values greater than zero."));
//        msgBox.setStandardButtons(QMessageBox::Ok);
//        msgBox.setDefaultButton(QMessageBox::Ok);
//        msgBox.setIcon(QMessageBox::Critical);
//        msgBox.exec();
        return;
    }
}

void RationalFormulaDialog::runoffCoefficient()
{
    RunoffCoefficientDialog runoffDlg;
    if (runoffDlg.exec() == QDialog::Accepted) {
        // Get the calculated runoff coefficient
        double rc = runoffDlg.runoffCoefficient();

        // Check if runoff coefficient is valid
        if (rc > 0 && rc <= 1.0)
            ui->coefficient->setText(QString::number(rc, 'f', 3));
    }
}
