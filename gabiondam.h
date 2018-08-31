#ifndef GABIONDAM_H
#define GABIONDAM_H

#include <vector>
#include "utilities.h"
#include "layer.h"
#include "channelsection.h"
#include "weir.h"

class GabionDam
{
public:
    GabionDam(double abutment = 0.5, double step = 1.0, double basinLength = 1.0, int stepEvery = 1);

    Weir *getWeir() const;
    void setWeir(Weir *weir);

    ChannelSection *getChannelSection() const;
    void setChannelSection(ChannelSection *channelSection);

    bool createDimensions();
    void forceUpdateDimensions();

    std::vector<Layer> getLayers() const;
    void setLayers(const std::vector<Layer> layers);

    int getLevels() const;
    void setLevels(int levels);

    int getWeirLevels() const;
    void setWeirLevels(int weirLevels);

    double getLeftSpillwayLimit() const;
    void setLeftSpillwayLimit(double leftSpillwayLimit);

    double getRightSpillwayLimit() const;
    void setRightSpillwayLimit(double rightSpillwayLimit);

    double getSpillwayLeftX() const;
    void setSpillwayLeftX(double spillwayLeftX);

    double getSpillwayRightX() const;
    void setSpillwayRightX(double spillwayRightX);

    int getSpillwayBottomLevel() const;
    void setSpillwayBottomLevel(int spillwayBottomLevel);

    int getSpillwayBottomLayer() const;
    void setSpillwayBottomLayer(int spillwayBottomLayer);

    int getWeirLayers() const;
    void setWeirLayers(int weirLayers);

    double getWeirWaterHeight() const;
    void setWeirWaterHeight(double weirWaterHeight);

    bool getMinLayersCondition() const;

    double getCentroidx() const;

    double getCentroidy() const;

    double getCentroidz() const;

    double getDamVolume() const;

    int getStep() const;
    void setStep(int step);

    double getStepLen() const;
    void setStepLen(double stepLen);

    double getBasinLen() const;
    void setBasinLen(double basinLen);

    double getAbutment() const;
    void setAbutment(double abutment);

private:
    bool minLayersCondition;

    int _levels;
    int _weirLevels;
    int _weirLayers;
    int _spillwayBottomLevel;
    int _spillwayBottomLayer;
    int _step;

    double _stepLen;
    double _basinLen;
    double _abutment;
    double _leftSpillwayLimit;
    double _rightSpillwayLimit;
    double _spillwayLeftX;
    double _spillwayRightX;
    double _centroidx;
    double _centroidy;
    double _centroidz;
    double _effectiveHeight;
    double _damVolume;

    std::vector<Layer> _layers; // The dam dimensions as Layer objects
    std::vector<double> _leftX;
    std::vector<double> _rigthX;

    Weir *_weir;
    ChannelSection *_channelSection;

    void checkParameterValues();
    bool createSpillway();
    bool checkSpillwayLimits();
    bool calculateCentroid();
};

#endif // GABIONDAM_H
