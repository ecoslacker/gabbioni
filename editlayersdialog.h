#ifndef EDITLAYERSDIALOG_H
#define EDITLAYERSDIALOG_H

#include <QDialog>
#include <QVector>
#include <vector>
#include "layer.h"

namespace Ui {
class EditLayersDialog;
}

class EditLayersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditLayersDialog(QVector<Layer> dimensionsList = {}, QWidget *parent = nullptr);
    ~EditLayersDialog();

    std::vector<Layer> dimensions() const;

private:
    Ui::EditLayersDialog *ui;
    std::vector<Layer> _dimensions;
    std::vector<double> _minLength;
    std::vector<double> _minHeight;
    std::vector<double> _maxHeight;

private slots:
    void setupDimensions(int layer);
    void setupSafeLimits();
    void setupUnsafeLimits();
    void updateLimits();
    void updateLength(double value);
    void updateWidth(double value);
    void updateHeight(const double value);
    void updateX(double value);
    void updateY(double value);
    void toogleRestrictionLimits(bool value);
};

#endif // EDITLAYERSDIALOG_H
