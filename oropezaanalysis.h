#ifndef OROPEZAANALYSIS_H
#define OROPEZAANALYSIS_H

#include "layer.h"
#include "gabiondam.h"

class OropezaAnalysis
{
public:
    OropezaAnalysis(double specGravWater = 1.2, double specGravStone = 2.4, double friction = 0.75, double securityFactor = 1.0);

    double friction() const;
    void setFriction(double friction);

    double damBase() const;
    void setDamBase(double damBase);

    double damCrest() const;
    void setDamCrest(double damCrest);

    double effectiveHeight() const;
    void setEffectiveHeight(double effectiveHeight);

    double damWeight() const;
    void setDamWeight(double damWeight);

    double damLever() const;
    void setDamLever(double damLever);

    double frontLever() const;
    void setFrontLever(double frontLever);

    bool slidingCondition() const;
    void setSlidingCondition(bool slidingCondition);

    bool overturnCondition() const;
    void setOverturnCondition(bool overturnCondition);

    bool centralCondition() const;
    void setCentralCondition(bool centralCondition);

    vector<Layer> dimensions() const;
    void setDimensions(const vector<Layer> &dimensions);

    double damSectionVolume() const;
    void setDamSectionVolume(double damSectionVolume);

    double nappeWeight() const;
    void setNappeWeight(double nappeWeight);

    double hydrostatic() const;
    void setHydrostatic(double hydrostatic);

    double hydrostaticLever() const;
    void setHydrostaticLever(double hydrostaticLever);

    bool performStabilityAnalysis();

    double wetArea() const;
    void setWetArea(double wetArea);

    double nappeLever() const;
    void setNappeLever(double nappeLever);

    double totalSliding() const;
    void setTotalSliding(double totalSliding);

    double totalOverturning() const;
    void setTotalOverturning(double totalOverturning);

    double resultingForces() const;
    void setResultingForces(double resultingForces);

    double middleThird() const;
    void setMiddleThird(double middleThird);

    double totalDisplacement() const;
    void setTotalDisplacement(double totalDisplacement);

    double thirdForces() const;
    void setThirdForces(double thirdForces);

    bool thirdCondition() const;
    void setThirdCondition(bool thirdCondition);

    GabionDam *dam() const;
    void setDam(GabionDam *dam);

    double safetyFactor() const;
    void setSafetyFactor(double safetyFactor);

    double sgStone() const;
    void setSgStone(double sgStone);

    double sgWater() const;
    void setSgWater(double sgWater);

private:
    double _damBase;           // (B) The length of the bottom layer
    double _damCrest;          // (b) The length of the top weir layer
    double _damSectionVolume;  // (V) The volume of the dam's uniraty section
    double _volumeSum;         // ( ) An auxiliary variable
    double _effectiveHeight;   // (H) Exclude the weir layers and the base
    double _damWeight;         // (P) Weight of the unitary section
    double _damLever;          // (Zp)
    double _nappeWeight;       // (q)
    double _nappeLever;        // (Xq)
    double _hydrostatic;       // (E)
    double _hydrostaticLever;  // (XE)
    double _frontLever;        // (Xp = B - Zp)
    double _wetArea;           // (Sm)

    bool _slidingCondition;    // The sliding condition
    bool _overturnCondition;   // The overturning condition
    bool _centralCondition;    // The middle third resulting forces
    bool _thirdCondition;      // The location of resulting forces should be in the middle third

    double _totalSliding;      // Total sliding of the dam
    double _totalOverturning;  // Overturning value of the dam
    double _resultingForces;   // Resulting vector of all the forces
    double _totalDisplacement; // Total displacement of the dam
    double _middleThird;       // The value of the middle third
    double _thirdForces;

    double _friction;          // Friction coefficient
    double _sgStone;           // Specific gravity of stone [t/m^3]
    double _sgWater;           // Specific gravity of water with sediment [t/m^3]
    double _safetyFactor;      // Safety factor

    vector<Layer> _dimensions;
    GabionDam *_dam;

    void checkParameterValues();
    void unitarySectionVolume();
    void calculateNappeWeight();
    void calculateNappeLever();
    void calculateDamSectionWeight();
    void calculateDamSectionLever();
    void calculateHydrostatic();
    void calculateHydrostaticLever();

    bool checkSligindCondition();
    bool checkOverturnCondition();
    bool checkCentralCondition();
    bool verifyMiddleThird();
};

#endif // OROPEZAANALYSIS_H
