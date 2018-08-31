#include "editlayersdialog.h"
#include "ui_editlayersdialog.h"
#include <QMessageBox>
#include <QDebug>

const double MIN_LENGTH = 0.5;
const double MIN_HEIGHT = 0.25; // This value has not justification

EditLayersDialog::EditLayersDialog(QVector<Layer> dimensionsList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditLayersDialog)
{
    ui->setupUi(this);

    // Set the dimensions and configure spin box
    _dimensions = dimensionsList.toStdVector();
    ui->spinBox->setMaximum(int(_dimensions.size()));

    connect(ui->spinBox,             SIGNAL(valueChanged(int)),    SLOT(setupDimensions(int)));
    connect(ui->doubleSpinBoxLength, SIGNAL(valueChanged(double)), SLOT(updateLength(double)));
    connect(ui->doubleSpinBoxWidth,  SIGNAL(valueChanged(double)), SLOT(updateWidth(double)));
    connect(ui->doubleSpinBoxHeight, SIGNAL(valueChanged(double)), SLOT(updateHeight(double)));
    connect(ui->doubleSpinBoxX,      SIGNAL(valueChanged(double)), SLOT(updateX(double)));
    connect(ui->doubleSpinBoxY,      SIGNAL(valueChanged(double)), SLOT(updateY(double)));
    connect(ui->safeDimensions,      SIGNAL(toggled(bool)),        SLOT(toogleRestrictionLimits(bool)));

    // Use safe dimensions restrictions by default
    if (ui->safeDimensions->isChecked())
        setupSafeLimits();
    else
        setupUnsafeLimits();

    // Set the first layer as default
    setupDimensions(1);

    // Configure the steps
    ui->doubleSpinBoxHeight->setSingleStep(0.1);
    ui->doubleSpinBoxLength->setSingleStep(0.1);
    ui->doubleSpinBoxWidth->setSingleStep(0.1);
    ui->doubleSpinBoxX->setSingleStep(0.1);
    ui->doubleSpinBoxY->setSingleStep(0.1);
}

EditLayersDialog::~EditLayersDialog()
{
    delete ui;
}

std::vector<Layer> EditLayersDialog::dimensions() const
{
    return _dimensions;
}

void EditLayersDialog::setupDimensions(int layer)
{
    layer--;
    ui->doubleSpinBoxHeight->setValue(_dimensions.at(layer).height());
    ui->doubleSpinBoxWidth->setValue(_dimensions.at(layer).width());
    ui->doubleSpinBoxLength->setValue(_dimensions.at(layer).length());
    ui->doubleSpinBoxX->setValue(_dimensions.at(layer).x());
    ui->doubleSpinBoxY->setValue(_dimensions.at(layer).y());

    if (ui->safeDimensions->isChecked())
        setupSafeLimits();
    else
        setupUnsafeLimits();

    updateLimits();
}

void EditLayersDialog::setupSafeLimits()
{
    _minLength.clear();
    _minHeight.clear();

    for (size_t i=0; i < _dimensions.size(); i++) {
        // Set minimum possible length of the layers
        if (i == (_dimensions.size() - 1))
            _minLength.push_back(MIN_LENGTH);
        else
            _minLength.push_back(_dimensions.at(i+1).length());

        // Set minimum height
        _minHeight.push_back(MIN_HEIGHT);
    }

    for (std::vector<double>::iterator it=_minLength.begin(); it != _minLength.end(); ++it)
        qDebug() << ' ' << *it;
}

void EditLayersDialog::setupUnsafeLimits()
{
    _minHeight.clear();
    _minLength.clear();

    // Although safe dimension limits are disabled, length and height cannot be less than zero! :-)
    for (size_t i=0; i < _dimensions.size(); i++) {
            _minLength.push_back(0);
            _minHeight.push_back(0);
    }
}

void EditLayersDialog::updateLimits()
{
    int layer = ui->spinBox->value() - 1;
    ui->doubleSpinBoxLength->setMinimum(_minLength.at(layer));
    ui->doubleSpinBoxHeight->setMinimum(_minHeight.at(layer));
    ui->doubleSpinBoxX->setMinimum(-999);
    ui->doubleSpinBoxY->setMinimum(-999);
}

void EditLayersDialog::updateLength(double value)
{
    int i = ui->spinBox->value() - 1;
    ui->doubleSpinBoxLength->setMinimum(_minLength.at(i));
    _dimensions.at(i).setLength(value);
}

void EditLayersDialog::updateWidth(double value)
{
    int i = ui->spinBox->value() - 1;
    _dimensions.at(i).setWidth(value);
}

void EditLayersDialog::updateHeight(const double value)
{
    int i = ui->spinBox->value() - 1;
    _dimensions.at(i).setHeight(value);
}

void EditLayersDialog::updateX(double value)
{
    int i = ui->spinBox->value() - 1;
    _dimensions.at(i).setX(value);
}

void EditLayersDialog::updateY(double value)
{
    int i = ui->spinBox->value() - 1;
    _dimensions.at(i).setY(value);
}

void EditLayersDialog::toogleRestrictionLimits(bool value)
{
    // If option is checked, set safe dimension limits
    if (value) {
        setupSafeLimits();
    } else {
        // If option is unchecked, allow unsafe dimension limits
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Action not recommended"));
        msgBox.setText(tr("Disablig safe restrictions is not recommended because it allows to use meaningless dimensions (like zeros or negative numbers)."));
        msgBox.setInformativeText(tr("Use this option with care."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();

        setupUnsafeLimits(); // Adjust limits
    }
    updateLimits();  // Setup limits to GUI
}
