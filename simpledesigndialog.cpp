#include <QDebug>
#include <QPixmap>
#include "simpledesigndialog.h"
#include "ui_simpledesigndialog.h"

const int DEC_PREC = 2;

SimpleDesignDialog::SimpleDesignDialog(double runoff, double gabionHeight, double weirWidth, double weirCoeff, double basinLength, double abutment,
                                       double stepLength, int stepLayer, double sgWater, double sgStone, double friction, double safetyFactor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimpleDesignDialog)
{
    ui->setupUi(this);
    // this->setWindowFlags(Qt::Window); // This adds the minimize and maximize buttons

    // Set default values before making the connection of signal/slot to avoid overwriting the values
    _flowrate = runoff;
    _abutment = abutment;
    _stepLayer = stepLayer;
    _stepLength = stepLength;
    _gabionHeight = gabionHeight;
    _weirWidth = weirWidth;
    _weirCoeff = weirCoeff;
    _basinLength = basinLength;
    _sgWater = sgWater;
    _sgStone = sgStone;
    _friction = friction;
    _safetyFactor = safetyFactor;

    // Set default values
    qDebug() << "Setting default values to design dialog";
    qDebug() << " Abutment:         " << QString::number(_abutment, 'f', DEC_PREC);
    qDebug() << " Weir coefficient: " << QString::number(_weirCoeff, 'f', DEC_PREC);
    qDebug() << " Gabion height:    " << QString::number(_gabionHeight, 'f', 2);
    qDebug() << " Step each:        " << _stepLayer;

    ui->runoff->setText(QString::number(_flowrate, 'f'));
    ui->weirCoefficient->setText(QString::number(_weirCoeff, 'f', DEC_PREC));
    ui->weirWidth->setText(QString::number(_weirWidth, 'f', DEC_PREC));
    ui->basinLength->setText(QString::number(_basinLength, 'f', DEC_PREC));
    ui->sgWater->setText(QString::number(_sgWater, 'f', DEC_PREC));
    ui->sgStone->setText(QString::number(_sgStone, 'f', DEC_PREC));
    ui->friction->setText(QString::number(_friction, 'f', DEC_PREC));
    ui->safetyFactor->setText(QString::number(_safetyFactor, 'f', DEC_PREC));

    // WARNING! This decimal precision should not change. This assummes the text format is: 1.00, 0.50, etc.
    ui->gabionHeight->setCurrentText(QString::number(_gabionHeight, 'f', 2));

    // These values should be setted after the gabionHeight ComboBox, otherwise they will be overwritten
    ui->abutment->setText(QString::number(_abutment, 'f', DEC_PREC));
    ui->stepLength->setText(QString::number(_stepLength, 'f', DEC_PREC));
    ui->applyStepEvery->setValue(_stepLayer);

    connect(ui->buttonRational,   SIGNAL(pressed()),                   SLOT(rationalFormula()));
    connect(ui->buttonManning,    SIGNAL(pressed()),                   SLOT(manningEquation()));
    connect(ui->weirWidth,        SIGNAL(textChanged(QString)),        SLOT(designWeir()));
    connect(ui->weirCoefficient,  SIGNAL(textChanged(QString)),        SLOT(designWeir()));
    connect(ui->runoff,           SIGNAL(textChanged(QString)),        SLOT(designWeir()));
    connect(ui->gabionHeight,     SIGNAL(currentIndexChanged(int)),    SLOT(updateChannel()));
    connect(ui->gabionHeight,     SIGNAL(currentTextChanged(QString)), SLOT(updateAbutment(QString)));
    connect(ui->gabionHeight,     SIGNAL(currentTextChanged(QString)), SLOT(updateStepLength(QString)));
    connect(ui->abutment,         SIGNAL(textChanged(QString)),        SLOT(updateAbutmentValue()));
    connect(ui->applyStepEvery,   SIGNAL(valueChanged(int)),           SLOT(updateStepLayer(int)));

//    connect(ui->stepLength,       SIGNAL(textChanged(QString)),        SLOT(updateStepLengthValue()));
//    connect(ui->basinLength,      SIGNAL(textChanged(QString)),        SLOT(updateBasinLength(QString)));
//    connect(ui->sgWater,          SIGNAL(textChanged(QString)),        SLOT(updateSGWater(QString)));
//    connect(ui->sgStone,          SIGNAL(textChanged(QString)),        SLOT(updateSGStone(QString)));
//    connect(ui->friction,         SIGNAL(textChanged(QString)),        SLOT(updateFriction(QString)));
//    connect(ui->safetyFactor,     SIGNAL(textChanged(QString)),        SLOT(updateSafetyFactor(QString)));
    connect(this,                 SIGNAL(accepted()),                  SLOT(setValuesFromUI()));

    // Method used for the estimation of runoff
    _methodUsed = 0;
    _methods << "None" << "Rational formula" << "Chezy-Manning equation";
    _weir = NULL;
}

SimpleDesignDialog::~SimpleDesignDialog()
{
    delete ui;
}

ChannelSection *SimpleDesignDialog::channel() const
{
    return _channel;
}

void SimpleDesignDialog::setChannel(ChannelSection *channel)
{
    _channel = channel;
}

Weir *SimpleDesignDialog::weir() const
{
    return _weir;
}

void SimpleDesignDialog::setWeir(Weir *weir)
{
    _weir = weir;
}

void SimpleDesignDialog::initialize()
{
    // WARNING! Be sure to set channel first, use setChannel()
    //ui->gabionHeight->setCurrentIndex(1); // This will call updateChannel()
    updateChannel();
}

double SimpleDesignDialog::getAbutment() const
{
    return _abutment;
}

void SimpleDesignDialog::setAbutment(double abutment)
{
    _abutment = abutment;
}

QString SimpleDesignDialog::getStrRational() const
{
    return _strRational;
}

QString SimpleDesignDialog::getStrManning() const
{
    return _strManning;
}

int SimpleDesignDialog::getMethodUsed() const
{
    return _methodUsed;
}

QStringList SimpleDesignDialog::getMethods() const
{
    return _methods;
}

int SimpleDesignDialog::getStepLayer() const
{
    return _stepLayer;
}

void SimpleDesignDialog::setStepLayer(int stepLayer)
{
    _stepLayer = stepLayer;
}

double SimpleDesignDialog::getStepLength() const
{
    return _stepLength;
}

void SimpleDesignDialog::setStepLength(double stepLength)
{
    _stepLength = stepLength;
}

double SimpleDesignDialog::getBasinLength() const
{
    return _basinLength;
}

void SimpleDesignDialog::setBasinLength(double basinLength)
{
    _basinLength = basinLength;
}

double SimpleDesignDialog::getGabionHeight() const
{
    return _gabionHeight;
}

double SimpleDesignDialog::getFriction() const
{
    return _friction;
}

double SimpleDesignDialog::getSgWater() const
{
    return _sgWater;
}

double SimpleDesignDialog::getSafetyFactor() const
{
    return _safetyFactor;
}

double SimpleDesignDialog::getSgStone() const
{
    return _sgStone;
}

double SimpleDesignDialog::getFlowrate() const
{
    return _flowrate;
}

void SimpleDesignDialog::rationalFormula()
{
    RationalFormulaDialog rationalDlg(this);
    if (rationalDlg.exec() == QDialog::Accepted) {
        _flowrate = rationalDlg.runoff();
        ui->runoff->setText(QString::number(_flowrate, 'f', 3));
        _methodUsed = 1;
        _strRational = rationalDlg.strRational();
        _strManning = "";
    }
}

void SimpleDesignDialog::manningEquation()
{
    ManningEqDialog manningDlg(this);
    manningDlg.setChannel(_channel);  // Pass channel object to allow the runoff calculation based on geometry
    if (manningDlg.exec() == QDialog::Accepted) {
        _flowrate = manningDlg.discharge();
        ui->runoff->setText(QString::number(_flowrate, 'f', 3));
        _methodUsed = 2;
        _strManning = manningDlg.strManning();
        _strRational = "";
    }
}

void SimpleDesignDialog::designWeir()
{
    // Get the information from the graphic interface
    _weirWidth = ui->weirWidth->text().toDouble();
    _weirCoeff = ui->weirCoefficient->text().toDouble();
    _gabionHeight = ui->gabionHeight->currentText().toDouble();
    _flowrate = ui->runoff->text().toDouble();

    if (_gabionHeight <= 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error in input values"));
        msgBox.setText(tr("Invalid gabion height for the weir."));
        msgBox.setInformativeText(tr("The gabion height for the weir should be a numeric value greater than zero."));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        // Reset the weir in case it was created with previous values
        _weir = NULL;

        return;
    }

    if ((_flowrate > 0) && (_weirWidth > 0) && (_weirCoeff > 0)) {

        // Create and design the weir
        _weir = new Weir(_weirWidth, 0, _gabionHeight, _weirCoeff);
        _weir->setDischarge(_flowrate);
        _weir->designBroadCrestedRectangular();

        // Set the results to the graphic interface
        ui->waterHeight->setText(QString::number(_weir->waterHeight(), 'f', 2));
        ui->weirHeight->setText(QString::number(_weir->wallsHeight(), 'f', 2));
        ui->freeboard->setText(QString::number(_weir->wallsHeight() - _weir->waterHeight(), 'f', 2));
        ui->weirLayers->setText(QString::number(_weir->levels()));

        qDebug() << "Weir designed successfuly!";
    } else {
        ui->waterHeight->clear();
        ui->weirHeight->clear();
        ui->freeboard->clear();
        ui->weirLayers->clear();

        // Reset the weir in case it was created with previous values
        _weir = NULL;

        qDebug() << "Not enough information to design weir!";
    }

    // Get ready some values
    //updateAbutmentValue();
}

void SimpleDesignDialog::updateChannel()
{
    /*
     * When the gabion's height changes it's time to update the widths
     * of the channel's cross section
     *
     */
//    QMessageBox msgBox;
//    msgBox.setText("Gabion height changed to " + ui->gabionHeight->currentText());
//    msgBox.exec();

    double height = ui->gabionHeight->currentText().toDouble();
    _channel->setLayersHeight(height);
    _channel->extractInformation();
    qDebug() << "Update gabions height to: " << height;
    qDebug() << "Recalculating channel widths.";

    QVector<double> depth = QVector<double>::fromStdVector(_channel->depthLevels());
    QVector<double> widths = QVector<double>::fromStdVector(_channel->widths());
    QVector<double> rightX = QVector<double>::fromStdVector(_channel->rightX());
    QVector<double> leftX = QVector<double>::fromStdVector(_channel->leftX());
    qDebug() << " Depth:  " << depth;
    qDebug() << " Width:  " << widths;
    qDebug() << " RightX: " << rightX;
    qDebug() << " LeftX:  " << leftX;

    // Set the maximum value to apply step to the channel levels
    int maxStep = _channel->levels() - 1;
    qDebug() << " Maximum step value: " << maxStep;
    if (maxStep <= 0)
        maxStep = 1;
    ui->applyStepEvery->setMinimum(1);
    ui->applyStepEvery->setMaximum(maxStep);

    // Design weir
    designWeir();
}

void SimpleDesignDialog::updateAbutment(QString value)
{
    // Updates the abutment value in the dialog's text box
    ui->abutment->setText(value);
}

void SimpleDesignDialog::updateAbutmentValue()
{
    // Updates the abutment value
    _abutment = ui->abutment->text().toDouble();
}

void SimpleDesignDialog::updateStepLength(QString value)
{
    // Updates the step length value in the dialog's text box
    ui->stepLength->setText(value);
}

void SimpleDesignDialog::updateStepLengthValue()
{
    // Updates the step length value
    _stepLength = ui->stepLength->text().toDouble();
}

void SimpleDesignDialog::updateBasinLength(QString value)
{
    _basinLength = value.toDouble();
}

void SimpleDesignDialog::updateStepLayer(int value)
{
    _stepLayer = value;
}

void SimpleDesignDialog::updateSGWater(QString value)
{
    _sgWater = value.toDouble();
}

void SimpleDesignDialog::updateSGStone(QString value)
{
    _sgStone = value.toDouble();
}

void SimpleDesignDialog::updateSafetyFactor(QString value)
{
    _safetyFactor = value.toDouble();
}

void SimpleDesignDialog::updateFriction(QString value)
{
    _friction = value.toDouble();
}

void SimpleDesignDialog::setValuesFromUI()
{
    /*
     * Sets all the values of the design from the user interface
     * getting the values from its corresponding widgets.
     *
     */

    _flowrate = ui->runoff->text().toDouble();
    qDebug() << "Flow rate (from UI):" << _flowrate;

    _abutment = ui->abutment->text().toDouble();
    _basinLength = ui->basinLength->text().toDouble();
    _stepLength = ui->stepLength->text().toDouble();
    _stepLayer = ui->applyStepEvery->value();

    _weirWidth = ui->weirHeight->text().toDouble();
    _weirCoeff = ui->weirCoefficient->text().toDouble();

    _sgWater = ui->sgWater->text().toDouble();
    _sgStone = ui->sgStone->text().toDouble();
    _safetyFactor = ui->safetyFactor->text().toDouble();
    _friction = ui->friction->text().toDouble();
}
