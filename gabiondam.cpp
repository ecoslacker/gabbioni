#include "gabiondam.h"
#include <iostream>

const int MIN_LEVELS = 2;  // How many levels are required as minimum
const int F_ABUTMENT = 2;  // Times abutmet should be consider in each layer, (2 to consider both channel's sides)
const int BASE_LAYER = 1;

GabionDam::GabionDam(double abutment, double step, double basinLength, int stepEvery)
{
    _abutment = abutment;
    _stepLen = step;
    _basinLen = basinLength;
    _step = stepEvery;

    checkParameterValues();

    // Initialize conditions
    minLayersCondition = true;
    _weir = new Weir();
    _channelSection = new ChannelSection();
}

Weir *GabionDam::getWeir() const
{
    return _weir;
}

void GabionDam::setWeir(Weir *weir)
{
    _weir = weir;
}

ChannelSection *GabionDam::getChannelSection() const
{
    return _channelSection;
}

void GabionDam::setChannelSection(ChannelSection *channelSection)
{
    _channelSection = channelSection;
}

std::vector<Layer> GabionDam::getLayers() const
{
    return _layers;
}

void GabionDam::setLayers(const std::vector<Layer> layers)
{
    _layers = layers;
}

int GabionDam::getLevels() const
{
    return _levels;
}

void GabionDam::setLevels(int levels)
{
    _levels = levels;
}

int GabionDam::getWeirLevels() const
{
    return _weirLevels;
}

void GabionDam::setWeirLevels(int weirLevels)
{
    _weirLevels = weirLevels;
}

double GabionDam::getLeftSpillwayLimit() const
{
    return _leftSpillwayLimit;
}

void GabionDam::setLeftSpillwayLimit(double leftSpillwayLimit)
{
    _leftSpillwayLimit = leftSpillwayLimit;
}

double GabionDam::getRightSpillwayLimit() const
{
    return _rightSpillwayLimit;
}

void GabionDam::setRightSpillwayLimit(double rightSpillwayLimit)
{
    _rightSpillwayLimit = rightSpillwayLimit;
}

double GabionDam::getSpillwayLeftX() const
{
    return _spillwayLeftX;
}

void GabionDam::setSpillwayLeftX(double spillwayLeftX)
{
    _spillwayLeftX = spillwayLeftX;
}

double GabionDam::getSpillwayRightX() const
{
    return _spillwayRightX;
}

void GabionDam::setSpillwayRightX(double spillwayRightX)
{
    _spillwayRightX = spillwayRightX;
}

int GabionDam::getSpillwayBottomLevel() const
{
    return _spillwayBottomLevel;
}

void GabionDam::setSpillwayBottomLevel(int spillwayBottomLevel)
{
    _spillwayBottomLevel = spillwayBottomLevel;
}

int GabionDam::getSpillwayBottomLayer() const
{
    return _spillwayBottomLayer;
}

void GabionDam::setSpillwayBottomLayer(int spillwayBottomLayer)
{
    _spillwayBottomLayer = spillwayBottomLayer;
}

int GabionDam::getWeirLayers() const
{
    return _weirLayers;
}

void GabionDam::setWeirLayers(int weirLayers)
{
    _weirLayers = weirLayers;
}

double GabionDam::getWeirWaterHeight() const
{
    return _weir->waterHeight();
}

void GabionDam::setWeirWaterHeight(double weirWaterHeight)
{
    _weir->setWaterHeight(weirWaterHeight);
}

bool GabionDam::getMinLayersCondition() const
{
    return minLayersCondition;
}

double GabionDam::getCentroidx() const
{
    return _centroidx;
}

double GabionDam::getCentroidy() const
{
    return _centroidy;
}

double GabionDam::getCentroidz() const
{
    return _centroidz;
}

double GabionDam::getDamVolume() const
{
    return _damVolume;
}

int GabionDam::getStep() const
{
    return _step;
}

void GabionDam::setStep(int step)
{
    _step = step;
}

double GabionDam::getStepLen() const
{
    return _stepLen;
}

void GabionDam::setStepLen(double stepLen)
{
    _stepLen = stepLen;
}

double GabionDam::getBasinLen() const
{
    return _basinLen;
}

void GabionDam::setBasinLen(double basinLen)
{
    _basinLen = basinLen;
}

double GabionDam::getAbutment() const
{
    return _abutment;
}

void GabionDam::setAbutment(double abutment)
{
    _abutment = abutment;
}

void GabionDam::checkParameterValues()
{
    // Some arguments cannot be less than zero and some others less than one
    _abutment = (_abutment < 0) ? 0.5 : _abutment;
    _stepLen = (_stepLen < 0) ? 1.0 : _stepLen;
    _basinLen = (_basinLen < 0) ? 1.0 : _basinLen;
    _step = (_step < 1) ? 1 : _step;
}

bool GabionDam::checkSpillwayLimits()
{
    /* Check spillway limits
     *
     * Checks if it's possible to create the spillway inside the channel widths and dimensions.
     *
     */

    // Limits
    _spillwayBottomLevel = _levels - _weirLevels;
    _leftSpillwayLimit = _leftX.at(_spillwayBottomLevel);
    _rightSpillwayLimit = _rigthX.at(_spillwayBottomLevel);

    // Dimensions of the spillway
    _spillwayLeftX = _channelSection->streambed() - (_weir->width() / 2);
    _spillwayRightX = _spillwayLeftX + _weir->width();

    // The x-axis left coordinate of the spillway should be greather than the left limit and
    // the x-axis right coordinate of the spillway should be lower than the right limit.
    if ((_spillwayLeftX < _leftSpillwayLimit) || (_spillwayRightX > _rightSpillwayLimit))
        return false;

    return true;
}

bool GabionDam::calculateCentroid()
{
    /* Calculate centroid of the dam
     *
     * Obtain the centroidal coordinates (x, y, z) of the dam layers.
     * The centroid is used to determine the critical section of the dam.
     *
     * IMPORTANT: dimensions should be already created!
     *
     */
    _damVolume = 0;
    double _sum_vx = 0, _sum_vy = 0, _sum_vz = 0;

    // Get the centroidal coordinates and volume of each layer of the dam
    for (size_t i=0; i < _layers.size(); i++) {
        // Layer volume
        double layerVolume = _layers.at(i).height() * _layers.at(i).length() * _layers.at(i).width();
        // X-Axis coordinate of the centroid of current layer
        double x = _layers.at(i).x() + (_layers.at(i).width() / 2.0);
        // Y-Axis coordinate of the centroid of current layer
        double y = _layers.at(i).y() + (_layers.at(i).height() / 2.0);
        // Z-Axis coordinate of current layer
        double z = _layers.at(i).length() / 2.0;

        // Layer volume * coordinates
        _damVolume += layerVolume;
        _sum_vx += x * layerVolume;
        _sum_vy += y * layerVolume;
        _sum_vz += z * layerVolume;
    }

    // Compute centroids
    _centroidx = _sum_vx / _damVolume;
    _centroidy = _sum_vy / _damVolume;
    _centroidz = _sum_vz / _damVolume;

    return true;
}

bool GabionDam::createSpillway()
{
    /* Create spillway
     *
     * Modifies the existing dam dimensions to create an spillway in the top center of the dam.
     * This will split the necessary layers into two weir walls (one each side of the channel's
     * cross-section).
     *
     */

    // Check for that the dimensions exist
    if (_layers.size() <= 0)
        return false;

    // Unable to create spillway if weir levels doesn't exist
    if (_weirLevels <= 0)
        return false;

    _spillwayBottomLayer = _spillwayBottomLevel + BASE_LAYER;  // Considering the base layer

    // Spillway cannot be greather than the number of layers
    if (_spillwayBottomLayer > int(_layers.size()))
        return false;

    std::vector<Layer> rightLayers;
    for (int i=_spillwayBottomLayer; i <= _levels; i ++) {

        std::cout << "Spilway right layer: " << i << " of " << _levels << std::endl;

        // Get width of the right spillway layer, before shrink the current layer
        double w = (_layers.at(i).x() + _layers.at(i).width()) - _spillwayRightX;

        // Shrink layer's width to create the spillway's left side or abutment
        _layers.at(i).setWidth(_spillwayLeftX - _layers.at(i).x());

        // Create the other dimensions of the right side layer
        double l = _layers.at(i).length();
        double h = _layers.at(i).height();
        double x = _spillwayRightX;
        double y = _layers.at(i).y();

        Layer rightLayer(l, w, h, x, y);
        rightLayers.push_back(rightLayer);

         std::cout << l << "\t" << w << "\t" << h << "\t" << x << "\t" << y << "\t" << std::endl;
    }

    _weirLayers = _weirLevels * 2;  // Update the weir layers count

    // Append the layers of the right side of the spillway to dam dimensions
    for (size_t i=0; i < rightLayers.size(); i++ ) {
        _layers.push_back(rightLayers.at(i));
    }

    std::cout << "Spilway created successfully!" << std::endl;

    return true;
}

bool GabionDam::createDimensions()
{
    /* Create dimensions
     *
     * Generates automatic dimensions for each dam's layer using  the widths list of the channel's cross-section,
     * the layer's height, the abutment and the weir dimensions.
     *
     * This function creates the dam's dimensions following the next rules:
     * 1.- Makes sure channel cross section exists.
     *
     */

    std::cout << "========== CREATING DIMENSIONS ==========" << std::endl;

    checkParameterValues();

    // Check that channel cross-section exists
    if (_channelSection == NULL) {
        std::cout << " * Creating dimensions: ChannelSection object is null." << std::endl;
        return false;
    }

    // Check that the weir exists
    if (_weir == NULL) {
        std::cout << " * Creating dimensions: Weir object is null." << std::endl;
        return false;
    }

    // Remove any previous dimensions
    _layers.clear();

    // Extract data from channel and weir
    _levels = _channelSection->levels();
    _leftX = _channelSection->leftX();
    _rigthX = _channelSection->rightX();
    _weirLevels = _weir->levels();

    // Check that exists enough data to create MIN_LEVELS
    if (_channelSection->levels() < MIN_LEVELS) {
        std::cout << "  * Channel levels:" << _channelSection->levels() << " < " << MIN_LEVELS << std::endl;
        minLayersCondition = false;
        return false;
    }

    // Create a layer for each level

    for (int i = 0; i <= _channelSection->levels(); i++) {

        std::cout << " * Creating layer: " << i << " of " << _channelSection->levels() << std::endl;

        //double max_dim = (_levels+1) * _channelSection->layersHeight(); // WARNING! Levels should be increased by 1
        //double max_dim = (_levels + 1) * _stepLen;

        double x = _channelSection->leftX().at(i) - _abutment;
        double y = _channelSection->depthLevels().at(i);
        double h = _channelSection->layersHeight();
        double w = _channelSection->widths().at(i) + (_abutment * F_ABUTMENT);
        //double l = max_dim - (_channelSection->layersHeight() * i); // This was the old way
        //double l = max_dim - (_stepLen * i);
        double l = _channelSection->layersHeight();

//        // Add the stilling basin length to the base layer
//        if (i == 0)
//            l += _basinLen;

        //std::cout << l << "\t" << w << "\t" << h << "\t" << x << "\t" << y << "\t" << std::endl;

        // Create a new layer
        Layer _newLayer(l, w, h, x, y);

        // Add the new layer to the gabion dam's body
        _layers.push_back(_newLayer);
    }

    // Adjust length of each layer, j is a "_step" counter
    int j = 0;
    double currentLength = _channelSection->layersHeight();

    // Start from top to bottom layer
    for (int i = _channelSection->levels(); i >= 0; i--) {

        std::cout << " * Creating length for layer: " << i << std::endl;

        // Update the current length of the layer each "_step"
        std::cout << " * Current operation: " << i << "/" << _step << std::endl;
        if ((i % _step) == 0) {
            // Update current length of the layer
            currentLength = _channelSection->layersHeight() + (_stepLen * j);
            j++; // Length updated
            std::cout << " * Updating length(" << i << "/" << _step << "=" << i%_step << ") ...done" << std::endl;
        }

        // Set the length to the current layer according to step configuration
        _layers.at(i).setLength(currentLength);

        // Add the stilling basin length to the base layer
        if (i == 0)
            _layers.at(i).setLength(_channelSection->layersHeight() + (_stepLen * j) + _basinLen);
    }

    // Finally create an spillway using weir dimensions (this block is pure logic!)
    if (checkSpillwayLimits()) {
        // Try to create spillway
        if (!createSpillway()) {
            std::cout << " * Something went wrong while creating spillway!" << std::endl;
            return false; // Something went wrong while creating spillway!
        }
    } else {
        return false;
    }

    // Adjust dimensions of the bottom layers to cover at least the weir
    //std::cout << "Adjust layers from bottom to " << _layers.size() - _weirLayers << std::endl;
    for (size_t i=0; i < (_layers.size() - _weirLayers); i++) {
        if (_layers.at(i).x() > _spillwayLeftX) {
            //std::cout << "Adjust layer " << i << " from left side." << std::endl;
            double diff = abs(_layers.at(i).x() - _spillwayLeftX);
            //std::cout << "Width: " << _layers.at(i).width() << " diff: " << diff << std::endl;
            double w = _layers.at(i).width() + diff; // New width

            // Fix the x-axis coordinate and the width
            _layers.at(i).setX(_spillwayLeftX);
            _layers.at(i).setWidth(w);
        }

        if ((_layers.at(i).x() + _layers.at(i).width()) < _spillwayRightX) {
            //std::cout << "Adjust layer " << i << " from right side." << std::endl;
            double diff = abs(_spillwayRightX - (_layers.at(i).x() + _layers.at(i).width()));
            double w = _layers.at(i).width() + diff; // New width
            _layers.at(i).setWidth(w);
        }
    }

    // Get the effective height, discard bottom and weir layers
    _effectiveHeight = 0;
    for (size_t i=0; i < _layers.size() - _weirLayers; i++) {
        if (i != 0)
            _effectiveHeight += _layers.at(i).height();
    }

    // Now that is complete, we can obtain the centroid of the dam
    calculateCentroid();

    //std::cout << "Dam dimensions created successfully!" << std::endl;
    return true;
}

void GabionDam::forceUpdateDimensions()
{
    /*
     * Forces an update in the dimensions of the dam. THIS IS STILL EXPERIMENTAL!
     *
     * WARNING! This function should be used when dimensions are
     * setted by the setLayers() function.
     *
     */

    calculateCentroid();
}
