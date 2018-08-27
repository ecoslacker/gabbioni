#include "runoffcoefficientdialog.h"
#include "ui_runoffcoefficientdialog.h"
#include "hydrology.h"
#include <QMessageBox>

RunoffCoefficientDialog::RunoffCoefficientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunoffCoefficientDialog)
{
    ui->setupUi(this);
    connect(ui->calculateButton, SIGNAL(pressed()), SLOT(calculateCoefficient()));
    _runoffCoefficient = 0;
}

RunoffCoefficientDialog::~RunoffCoefficientDialog()
{
    delete ui;
}

double RunoffCoefficientDialog::runoffCoefficient() const
{
    return _runoffCoefficient;
}

void RunoffCoefficientDialog::calculateCoefficient()
{
    // Get data from dialog
    double precip = ui->annualPrecipitation->text().toDouble();
    int land_use = ui->landUse->currentIndex();
    int soil_type = ui->soilType->currentIndex();

    // Check the input data
    if (precip <= 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Input error"));
        msgBox.setText(tr("Invalid input value for annual precipitation"));
        msgBox.setInformativeText(tr("Annual precipitation must be a numeric value greater than zero."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    if (soil_type < 0 || soil_type > 2) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Input error"));
        msgBox.setText(tr("Invalid soil type value."));
        msgBox.setInformativeText(tr("It should be an integer value between 0 and 2."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    if (land_use < 0 || land_use > 13) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Input error"));
        msgBox.setText(tr("Invalid land use value."));
        msgBox.setInformativeText(tr("It should be an integer value between 0 and 13."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    _runoffCoefficient = hydrology::runoffCoefficient(land_use, soil_type, precip);

    ui->runoffcoef->setText(QString::number(_runoffCoefficient, 'f', 3));
}
