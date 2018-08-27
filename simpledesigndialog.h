#ifndef SIMPLEDESIGNDIALOG_H
#define SIMPLEDESIGNDIALOG_H

#include <QDialog>
#include "rationalformuladialog.h"
#include "manningeqdialog.h"
#include "channelsection.h"
#include "weir.h"

namespace Ui {
class SimpleDesignDialog;
}

class SimpleDesignDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SimpleDesignDialog(double runoff, double gabionHeight, double weirWidth, double weirCoeff, double basinLength, double abutment,
                                 double stepLength, int stepLayer, double sgWater, double sgStone, double friction, double safetyFactor, QWidget *parent = 0);
    ~SimpleDesignDialog();

    ChannelSection *channel() const;
    void setChannel(ChannelSection *channel);

    Weir *weir() const;
    void setWeir(Weir *weir);

    void initialize();

    bool getMinLayersCondition() const;

    double getAbutment() const;
    void setAbutment(double abutment);

    QString getStrRational() const;

    QString getStrManning() const;

    int getMethodUsed() const;

    QStringList getMethods() const;

    int getStepLayer() const;
    void setStepLayer(int stepLayer);

    double getStepLength() const;
    void setStepLength(double stepLength);

    double getBasinLength() const;
    void setBasinLength(double basinLength);

    double getGabionHeight() const;

    double getFriction() const;

    double getSgWater() const;

    double getSafetyFactor() const;

    double getSgStone() const;

    double getFlowrate() const;

private:
    Ui::SimpleDesignDialog *ui;
    int _methodUsed;
    int _stepLayer;
    double _flowrate;
    double _abutment;
    double _stepLength;
    double _basinLength;
    double _weirWidth;
    double _weirCoeff;
    double _gabionHeight;
    double _sgStone;
    double _friction;
    double _sgWater;
    double _safetyFactor;

    QString _strRational;
    QString _strManning;
    QStringList _methods;

    ChannelSection *_channel;
    Weir *_weir;

private slots:
    void rationalFormula();
    void manningEquation();
    void designWeir();
    void updateChannel();
    void updateAbutment(QString value);
    void updateAbutmentValue();
    void updateStepLength(QString value);
    void updateStepLengthValue();
    void updateBasinLength(QString value);
    void updateStepLayer(int value);
    void updateSGWater(QString value);
    void updateSGStone(QString value);
    void updateSafetyFactor(QString value);
    void updateFriction(QString value);

    void setValuesFromUI();
};

#endif // SIMPLEDESIGNDIALOG_H
