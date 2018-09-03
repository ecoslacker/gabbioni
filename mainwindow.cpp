#include <cmath>
#include <QGraphicsTextItem>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "helpdialog.h"
#include "simpledesigndialog.h"
#include "analysisresultsdialog.h"
#include "reportdialog.h"
#include "editlayersdialog.h"
#include "datadialog.h"
#include "textdialog.h"
#include "dxffiles.h"

const double POINT_SIZE = 5;
const double ZOOM_DELTA = 0.1;
const int COORD_SIZE = 2;
const int MIN_DATA = 3;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->centralWidget->setMouseTracking(true);

    connect(ui->actionAbout,        SIGNAL(triggered(bool)),      SLOT(about()));
    connect(ui->actionHelp,         SIGNAL(triggered(bool)),      SLOT(help()));
    connect(ui->actionOpen,         SIGNAL(triggered(bool)),      SLOT(openCsvFile()));
    connect(ui->actionSimpleDesign, SIGNAL(triggered(bool)),      SLOT(simpleDesign()));
    connect(ui->actionZoomIn,       SIGNAL(triggered(bool)),      SLOT(zoomIn()));
    connect(ui->actionZoomOut,      SIGNAL(triggered(bool)),      SLOT(zoomOut()));
    connect(ui->actionPan,          SIGNAL(triggered(bool)),      SLOT(pan(bool)));
    connect(ui->actionZoomFit,      SIGNAL(triggered(bool)),      SLOT(zoomBestFit()));
    connect(ui->actionZoomOriginal, SIGNAL(triggered(bool)),      SLOT(zoomOriginal()));
    connect(ui->actionDimensions,   SIGNAL(triggered(bool)),      SLOT(showDimensions()));
    connect(ui->actionAnalysis,     SIGNAL(triggered(bool)),      SLOT(showStabilityAnalysis()));
    connect(ui->actionExportDXF,    SIGNAL(triggered(bool)),      SLOT(exportToDXF()));
    connect(ui->actionReport,       SIGNAL(triggered(bool)),      SLOT(showReport()));
    connect(ui->graphicsView,       SIGNAL(coordinatesChanged()), SLOT(updateCoordinates()));
    connect(ui->actionEditLayers,   SIGNAL(triggered(bool)),      SLOT(editLayers()));
    connect(ui->actionData,         SIGNAL(triggered(bool)),      SLOT(openData()));

    _programVersion = "v0.1 (Beta)";

    // Set Window icon
    QIcon winIcon("://icons/32x32/blockdevice.png");
    this->setWindowIcon(winIcon);
    this->setWindowTitle("Gabbioni - " + _programVersion);

    _scale = 1.0;

    // Start with the window maximized
    //showMaximized();

    model = new QStandardItemModel;

    initializeUiTreeView();
    disableNavigationButtons();

    ui->actionDimensions->setEnabled(false);
    ui->actionAnalysis->setEnabled(false);
    ui->actionExportDXF->setEnabled(false);
    ui->actionReport->setEnabled(false);
    ui->actionEditLayers->setEnabled(false);

    mainScene = new QGraphicsScene;
    ui->graphicsView->setScene(mainScene);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);

    // Initialize status bar and its widgets
    widgetScale = new QLabel(this);
    widgetScale->setText(QString::number(_scale, 'f', 2));
    widgetCoordinates = new QLabel(this);
    widgetCoordinates->setText(QString("x=%1, y=%2").arg(0).arg(0));
    widgetUnits = new QLabel(this);
    widgetUnits->setText("Units: SI");
    createStatusBar();

    // Add a checkable action that can be used to show or close the properties dock widget
    QIcon propertiesIcon("://icons/32x32/document-properties-2.png");
    ui->dockWidget->toggleViewAction()->setIcon(propertiesIcon);
    ui->menuView->addAction(ui->dockWidget->toggleViewAction());

    restartProject();

    // Show change log message
    TextDialog txtDlg(":/help/ChangeLog.txt", "Changelog");
    txtDlg.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QPoint scrollAmount = event->angleDelta();

    // Apply zoom.
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void MainWindow::zoom(double scaleFactor)
{
    ui->graphicsView->scale(scaleFactor, scaleFactor);
    _scale *= scaleFactor;
    qDebug() << "Scale: " << _scale;
    plotAll();
    widgetScale->setText(QString::number(_scale, 'f', 2));
}

void MainWindow::zoomIn()
{ 
    zoom(1 + ZOOM_DELTA);
}

void MainWindow::zoomOut()
{
    zoom(1 - ZOOM_DELTA);
}

void MainWindow::zoomOriginal()
{
    double xmin = utilities::minValueFrom(channel.coordX());
    double xmax = utilities::maxValueFrom(channel.coordX());
    double ymin = utilities::minValueFrom(channel.coordY());
    double ymax = utilities::maxValueFrom(channel.coordY());

    zoom(1/_scale);

    ui->graphicsView->centerOn((xmax - xmin) / 2, (ymax - ymin) / 2);
}

void MainWindow::zoomBestFit()
{
    // Use channel information
    double xmin = utilities::minValueFrom(channel.coordX());
    double xmax = utilities::maxValueFrom(channel.coordX());
    double ymin = utilities::minValueFrom(channel.coordY());
    double ymax = utilities::maxValueFrom(channel.coordY());
    double hdim = (xmax - xmin) * 1.1;
    double width = ui->graphicsView->width();
    double ratio = width/hdim;

    // Scale
    if (hdim > 0) {
        qDebug() << "Horizontal dimension: " << hdim << ", canvas width: " << width
                 << ", ratio: " << ratio << ", scale: " << ratio/_scale;
        zoom(ratio/_scale);
        ui->graphicsView->centerOn((xmax - xmin) / 2, (ymax - ymin) / 2);
    } else {
        qDebug() << "Unable to zoom at this scale.";
    }
}

void MainWindow::pan(bool panMode)
{
    if (panMode) {
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    } else {
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void MainWindow::disableNavigationButtons()
{
    ui->actionZoomIn->setEnabled(false);
    ui->actionZoomOut->setEnabled(false);
    ui->actionZoomOriginal->setEnabled(false);
    ui->actionZoomFit->setEnabled(false);
    ui->actionSimpleDesign->setEnabled(false);
}

void MainWindow::enableNavigationButtons()
{
    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);
    ui->actionZoomOriginal->setEnabled(true);
    ui->actionZoomFit->setEnabled(true);
    ui->actionSimpleDesign->setEnabled(true);
}

void MainWindow::pointsChangedWarning()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Points changed."));
    msgBox.setInformativeText(tr("The points of the dam just changed"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

QList<QStringList> MainWindow::convertData(std::vector<std::vector<string> > &values)
{
    /*
     *  Convert the data from std::strings and std::vectors to QStrings and QLists
     *
     */

    QList<QStringList> content;
    for (size_t i=0; i < values.size(); i++) {
        QStringList row;
        for (size_t j=0; j < values.at(i).size(); j++) {
            row.append(QString::fromStdString(values.at(i).at(j)));
        }
        content.append(row);
    }
    return content;
}

void MainWindow::about()
{
    AboutDialog aboutDlg(this, _programVersion);
    aboutDlg.exec();
}

void MainWindow::help()
{
    HelpDialog helpDlg;
    helpDlg.exec();
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
    statusBar()->addPermanentWidget(widgetScale);
    statusBar()->addPermanentWidget(widgetCoordinates);
    statusBar()->addPermanentWidget(widgetUnits);
}

void MainWindow::updateCoordinates()
{
    /*
     * Update the coordinates of the cursor in the statusbar
     * This function maps from global coordinates to scene coordinates
     *
     */

    // Retrieve the coordinates from the Scene, the real coordinates of the draw
    QPointF mousePosition = ui->graphicsView->mapToScene(ui->graphicsView->mapFromGlobal(QCursor::pos()));
    widgetCoordinates->setText(QString("x=%1, y=%2").arg(mousePosition.x()).arg(mousePosition.y()));
}

void MainWindow::openData()
{
    /*
     * Creates a dialog to input the channel cross-section data
     *
     */

    DataDialog dataDlg;
    if (dataDlg.exec() == QDialog::Accepted) {
        std::vector<std::vector<std::string> > inputdata;

        // Get the data from dialog, ignore emty cells
        inputdata.clear();
        bool all_empty_values = true;
        for (size_t i = 0; i < dataDlg.getData().size(); i++) {
            std::vector<std::string> pair;


            // Get the non-empty data from the dialog
            if (!dataDlg.getData().at(i).at(0).empty() && !dataDlg.getData().at(i).at(1).empty()) {
                all_empty_values = false;
                pair.push_back(dataDlg.getData().at(i).at(0));
                pair.push_back(dataDlg.getData().at(i).at(1));
            }

            // Add the pair of data
            if (pair.size() == 2)
                inputdata.push_back(pair);
        }

        // If data is empty there is nothing to do
        if (all_empty_values) {
            QMessageBox msg;
            msg.setText(tr("Invalid data detected, input values are negative, zero or empty."));
            msg.setInformativeText(tr("Please provide valid numeric data for a channel cross section."));
            msg.setIcon(QMessageBox::Warning);
            msg.exec();

            // Clear the data, just to be sure
            inputdata.clear();

            return;
        }

        // Check if there is enough data
        if (inputdata.size() < MIN_DATA) {
            // Not enough data
            QMessageBox msg;
            msg.setText(tr("Not enough data to continue."));
            msg.setInformativeText(tr("A valid channel cross section should have at least 3 pairs of coordinates"));
            msg.setIcon(QMessageBox::Warning);
            msg.exec();

            // Clear the data, just to be sure
            inputdata.clear();

            return;
        }

        // Check for negative values or if all data are zeros
        bool negative_values = false;
        bool all_zero_values = true;
        for (size_t i = 0; i < inputdata.size(); i++) {
            // Check for zero or negative values (non-numeric characters will be converted to zero)
            double x = QString::fromStdString(inputdata.at(i).at(0)).toDouble();
            double y = QString::fromStdString(inputdata.at(i).at(1)).toDouble();
            if (x < 0 || y < 0)
                negative_values = true;
            if (x > 0 || y > 0)
                all_zero_values = false;
        }

        if (negative_values || all_zero_values) {
            QMessageBox msg;
            msg.setText(tr("Input data cannot be all zeros or negative values."));
            msg.setInformativeText(tr("Please provide valid numeric data for a channel cross section."));
            msg.setIcon(QMessageBox::Warning);
            msg.exec();

            // Clear the data, just to be sure
            inputdata.clear();

            return;
        }

        // If all went OK continue to process the data and initialize the design
        prepareDesign(inputdata);

    } else {
        qDebug() << "Data imput canceled by user!";
    }
}

void MainWindow::openCsvFile()
{
    /*
     * Open a CSV text file with the channel cross-section data
     *
     */

    char delimiter = ',';

    // Get the file name from dialog
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open CSV File"), QDir::homePath(),
                                            tr("Comma separated values (*.csv);;Tab delimited (*.txt)"));

    // Check file name
    QFileInfo testFile(fileName);
    if (!testFile.exists()) {

        statusBar()->showMessage(tr("The specified file was not found or does not exist."));

        return;
    }

    // Check the extension
    if (testFile.suffix() == "txt") {
        delimiter = '\t';
    }

    // Get the data from the CSV file (this uses libcsvdata)
    std::vector<std::vector<std::string> > inputdata;
    if (!CsvData::parseFromFile(fileName.toStdString(), inputdata, delimiter)) {
        return;
    }

    // Process the data and initialize the design
    prepareDesign(inputdata);

    statusBar()->showMessage(QString("File: %1").arg(fileName));

    return;
}

void MainWindow::prepareDesign(std::vector<std::vector<string> > &values)
{
    /*
     * Carries out some actions required in order to start the design of the dam
     *
     */

    // Reinitialize the weir, gabion dam and stability analysis
    restartProject();

    // Convert the data from strings and vectors to QStrings and QLists
    QList<QStringList> content = convertData(values);
    qDebug() << "Contents:\n" << content;

    // Fill up the channel object with the data
    setupChannelData(content);

    plotPointsAndChannel();
    zoomBestFit();
}

void MainWindow::setupChannelData(QList<QStringList> values)
{
    /*
     * Extracts the channel cross-section data to initialize the ChannelSection object
     *
     */

    vector<double> x;
    vector<double> y;
    for (int i = 0; i < values.size(); i++) {

        // Check if the data has correct format
        if (values.at(i).isEmpty()) {
            continue;
        } else if (values.at(i).size() != COORD_SIZE) {

            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Incorrect format"));
            msgBox.setText(tr("Data has incorrect format or values."));
            msgBox.setInformativeText(tr("Please check data, it should be numeric data in two columns separated by comma."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();

            return;
        }

        // WARNING! String data will automatically be converted to zero values
        x.push_back(values.at(i).at(0).toDouble());
        y.push_back(values.at(i).at(1).toDouble());
    }
    qDebug() << "x: " << x;
    qDebug() << "y: " << y;

    channel.setCoordX(x);
    channel.setCoordY(y);

    // Update the coordinates in the properties panel
    updatePanelCoordinates();
}

void MainWindow::simpleDesign()
{
    // Extract the channel information (widths, depth, width, etc)
    // then pass to the design object

    qDebug() << "===============================";
    qDebug() << " Starting gabion dam design...";
    qDebug() << "===============================";

    qDebug() << " Gabion height:   " << channel.layersHeight();
    qDebug() << " Weir width:      " << tempWeir->width();
    qDebug() << " Weir coefficient:" << tempWeir->coefficient();
    qDebug() << " Basin length:    " << gabionDam->getBasinLen();
    qDebug() << " Abutment:        " << gabionDam->getAbutment();
    qDebug() << " Step length:     " << gabionDam->getStepLen();
    qDebug() << " Step:            " << gabionDam->getStep();
    qDebug() << " SG Water:        " << stability->sgWater();
    qDebug() << " SG Stone:        " << stability->sgStone();
    qDebug() << " Friction:        " << stability->friction();

    // WARNING! This dialog constructor will use the default values at first (class initialization),
    // then the user input should be remembered evey time the dialog is created again.
    SimpleDesignDialog design(_flowrate, channel.layersHeight(), tempWeir->width(), tempWeir->coefficient(),
                              gabionDam->getBasinLen(), gabionDam->getAbutment(), gabionDam->getStepLen(),
                              gabionDam->getStep(), stability->sgWater(), stability->sgStone(),
                              stability->friction(), stability->safetyFactor(), this);
    design.setChannel(&channel);  // WARNING! SETTING A CHANNEL SECTION OBJECT IS MANDATORY!
    design.initialize();          // WARNING! RUNNING THE INITIALIZE FUNCTION IS MANDATORY!

    qDebug() << "Initializing design... done.";

    if (design.exec() == QDialog::Accepted) {
        // Perform the design of the dam with the information provided

        // Get the data from the dialog
        int stepLayer = design.getStepLayer();
        double abutment = design.getAbutment();
        double stepLength = design.getStepLength();
        double basinLength = design.getBasinLength();

        tempWeir = design.weir();

        // Create gabion object and set data from design dialog
        //gabionDam = new GabionDam(abutment, stepLength, basinLength, stepLayer);
        gabionDam->setChannelSection(&channel);
        gabionDam->setWeir(design.weir());
        gabionDam->setAbutment(abutment);
        gabionDam->setStep(stepLayer);
        gabionDam->setBasinLen(basinLength);
        gabionDam->setStepLen(stepLength);

        qDebug() << "Dam object created";
        qDebug() << " Abutment:     " << abutment;
        qDebug() << " Step every:   " << stepLayer << " layers.";
        qDebug() << " Step length:  " << stepLength;
        qDebug() << " Basin length: " << basinLength;
        qDebug() << " Weir:         " << design.weir();
        qDebug() << " Channel depth:" << gabionDam->getChannelSection()->channelDepth();

        // Default values are setup automatically by the GabionDam class, just inform the user
        if ((abutment < 0) || (stepLayer <= 0) || (stepLength < 0) || (basinLength < 0)) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Invalid data"));
            msgBox.setText(tr("Cannot create dam dimensions with input values."));
            msgBox.setInformativeText(tr("Some values cannot be less or equal than zero, using default values instead."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }

        if (gabionDam->createDimensions()) {

            // Everything went OK

            qDebug() << "======================================";
            qDebug() << " Dam dimensions created successfully!";
            qDebug() << "======================================";

            // Enable dimensions action
            ui->actionDimensions->setEnabled(true);
            ui->actionAnalysis->setEnabled(true);
            ui->actionExportDXF->setEnabled(true);
            ui->actionReport->setEnabled(true);
            ui->actionEditLayers->setEnabled(true);

            // Set the dimensions to the Dimensions dialog
            dimensions = QVector<Layer>::fromStdVector(gabionDam->getLayers());
            dimenDlg.setLayers(&dimensions);
            dimenDlg.configureTable();

            // Plot dimensions
            plotAll();

            // ****** Perform stability analysis ******

            // Get the data from the dialog
            double sgWater = design.getSgWater();
            double sgStone = design.getSgStone();
            double friction = design.getFriction();
            double safetyFactor = design.getSafetyFactor();

            // Default values are setup automatically by the OropezaAnalysis class, just inform the user
            if ((sgWater <= 0) || (sgStone <= 0) || (friction <= 0) || (safetyFactor <= 0)) {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Invalid data"));
                msgBox.setText(tr("Cannot perform stability analysis with input values."));
                msgBox.setInformativeText(tr("Some values cannot be less or equal than zero, using default values instead."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
            }

            qDebug() << " SG Water:     " << sgWater;
            qDebug() << " SG Stone:     " << sgStone;
            qDebug() << " Friction:     " << friction;
            qDebug() << " Safety factor:" << safetyFactor;

            stability->setSgWater(sgWater);
            stability->setSgStone(sgStone);
            stability->setFriction(friction);
            stability->setSafetyFactor(safetyFactor);

            performStabilityAnalysis();

            // ****** Get ready the text for the report ******

            // Get the method used for flow rate (a.k.a runoff a.k.a discharge) estimation
            // Currently these methods include: a) None (direct input), b) Rational formula,
            // and c) Chezy-Manning equation.
            _strMethod = tr("None (input value)");
            _flowrate = design.getFlowrate();
            _flowMethod = design.getMethodUsed();

            _strManning = design.getStrManning();
            _strRational = design.getStrRational();

            if (_flowMethod >= 0 && _flowMethod < design.getMethods().size())
                _strMethod = design.getMethods().at(_flowMethod);

            _strFlow = "Runoff\n";
            _strFlow += QString("Method used for estimation:\t%1\t%2\n").arg(_strMethod).arg("");
            _strFlow += QString("Runoff:\t%1\t%2\n").arg(_flowrate).arg(_flowUnit);

            // ****** Update the properties dock panel ******

            updateDockPanelProperties();

            showResults(_strResult, "Stability Analysis Results");

        } else if (!gabionDam->getMinLayersCondition()) {
            // Dam doesn't have the minimum layers
            qDebug() << "Error: dam does not have minimum layers, cannot complete design!";
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Gabion height is too large"));
            msgBox.setText(tr("The gabion height selected is too large for the channel depth, please consider a lower value."));
            msgBox.setInformativeText(tr("The gabion height selected is too large for the channel depth (Minimum levels required)."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();

            restartProject();
            updatePanelChannel();

        } else {
            // Another error just raised
            qDebug() << "Error: missing data, cannot complete design!";
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Incomplete design"));
            msgBox.setText(tr("Design is not complete. Runoff, weir or other data is missing."));
            msgBox.setInformativeText(tr("Dimensions of the dam cannot be created with missing data. Provide complete data and try again."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();

            restartProject();
            updatePanelChannel();
        }
    } else {
        // Dam design was cancelled by user
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Design was cancelled"));
        msgBox.setText(tr("Design cancelled by user."));
        msgBox.setInformativeText(tr("The dam design was cancelled by the user. Please provide all the information for a new design."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void MainWindow::performStabilityAnalysis()
{
    /*
     * Perform stability analysis of the gabion dam
     *
     */

    stability->setDam(gabionDam);

    bool ok = stability->performStabilityAnalysis();

    if (ok) {
        statusBar()->showMessage(tr("Stability analysis was successful!"));
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Errors in stability analysis"));
        msgBox.setText(tr("There were some errors in the stability analysis of the current gabion dam."));
        msgBox.setInformativeText(tr("Please check all the values and try again with valid data."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void MainWindow::createTextReport()
{
    /* Create text report
     *
     * Creates a string with the data and results of each part of the gabion dam design, including:
     * - Channel cross section
     * - Channel flow
     * - Flow estimation method
     * - Weir (spillway) dimensions
     * - Gabion dam dimensions
     * - Stability analysis results
     *
     */

    // Prepare the channel cross section
    _strChannel = QString::fromStdString(channel.strChannel());

    // Prepare the gabion dam dimensions
    QString strLine;
    _strDimensions = "Dam layers dimensions\n";
    _strDimensions += "Length\tWidth\tHeight\tx-coord\ty-coord\n";
    for (int i=0; i < dimensions.size(); i++) {
        strLine = QString("%1\t%2\t%3\t%4\t%5\n").arg(dimensions.at(i).length()).arg(dimensions.at(i).width()).arg(dimensions.at(i).height()).arg(dimensions.at(i).x()).arg(dimensions.at(i).y());
        _strDimensions.append(strLine);
    }

    // Generate the general report
    _strGeneralReport = "Design of a gabion dam.\n\n";
    _strGeneralReport += _strChannel + "\n";
    _strGeneralReport += _strFlow + "\n";
    if (!_strManning.isEmpty())
        _strGeneralReport += _strManning + "\n";
    if (!_strRational.isEmpty())
        _strGeneralReport += _strRational + "\n";
    _strGeneralReport += _strWeir;
    _strGeneralReport += _strDimensions + "\n";
    _strGeneralReport += _strResult;
}

void MainWindow::initializeUiTreeView()
{
    _properties.clear();
    _values.clear();
    _units.clear();

    _properties << tr("Project name:") << tr("ID:");
    _values << "None" << "0";
    _units << "" << "";

    // Initialize the properties panel with dummy values
    updateUiProperties();
}

void MainWindow::updateUiProperties()
{
    model->clear();
    QStringList headers;
    headers << "Property" << "Value" << "Unit/Other";

    // Create rows with two columns each, first is the name and the second is the value
    for (int i = 0; i < _properties.size(); i++) {
        QList<QStandardItem*> row;
        QStandardItem *itemCol1 = new QStandardItem(_properties.at(i)); // Column 1
        QStandardItem *itemCol2 = new QStandardItem(_values.at(i));     // Column 2
        QStandardItem *itemCol3 = new QStandardItem(_units.at(i));      // Column 3
        itemCol1->setEditable(false);
        itemCol2->setEditable(true);
        itemCol2->setTextAlignment(Qt::AlignRight);
        itemCol3->setEditable(false);
        row.append(itemCol1);
        row.append(itemCol2);
        row.append(itemCol3);

        model->appendRow(row);
    }
    model->setHorizontalHeaderLabels(headers);

    ui->treeView->setModel(model);
    ui->treeView->setAlternatingRowColors(true);
    // Adjust the colum size to their content
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(2);
}

void MainWindow::updatePanelCoordinates()
{
    // Update the properties with the channel cross-section coordinates
    QString x_coord = "{";
    QString y_coord = "{";
    foreach (double xi, channel.coordX()) {
        x_coord += QString::number(xi, 'f', _dec) + ", ";
    }
    foreach (double yi, channel.coordY()) {
        y_coord += QString::number(yi, 'f', _dec) + ", ";
    }
    x_coord = x_coord.left(x_coord.size() - 2);
    y_coord = y_coord.left(y_coord.size() - 2);
    x_coord += " }";
    y_coord += " }";

    _properties << tr("X coord:") << tr("Y coord:");
    _values << x_coord << y_coord;
    _units << _lengthUnit << _lengthUnit;
    //updateUiProperties();
}

void MainWindow::updatePanelChannel()
{
    /*
     * Get the channel cross-section data from the ChannelSection object
     *
     */

    _properties << tr("Channel depth:")
                << tr("Channel width:")
                << tr("Layers height:")
                << tr("Levels:")
                << tr("Stream bed:");
    _values << QString::number(channel.channelDepth(), 'f', _dec)
            << QString::number(channel.channelWidth(), 'f', _dec)
            << QString::number(channel.layersHeight(), 'f', _dec)
            << QString::number(channel.levels(), 'f', 0)
            << QString::number(channel.streambed(), 'f', _dec);
    _units << _lengthUnit
                  << _lengthUnit
                  << _lengthUnit
                  << ""
                  << _lengthUnit;

    //updateUiProperties();
}

void MainWindow::updatePanelFlow()
{
    _properties << tr("Flow rate:")
                << tr("Method:");
    _values << QString::number(_flowrate, 'f', 4)
            << _strMethod;
    _units << _flowUnit
           << QString::number(_flowMethod, 'f', 0);

    //updateUiProperties();
}

void MainWindow::updatePanelAndTextWeir()
{

    QStringList properties;
    QStringList values;
    QStringList units;

    // This headers will be shared between text report and dock panel
    properties << tr("Weir (Spillway)\t%1\t%2\n")
               << tr("Width:\t%1\t%2\n")
               << tr("Height:\t%1\t%2\n")
               << tr("Coefficient:\t%1\t%2\n")
               << tr("Water head:\t%1\t%2\n")
               << tr("Freeboard:\t%1\t%2\n")
               << tr("Gabion layers:\t%1\t%2\n");
    values << ""
           << QString::number(gabionDam->getWeir()->width(), 'f', _dec)
           << QString::number(gabionDam->getWeir()->wallsHeight(), 'f', _dec)
           << QString::number(gabionDam->getWeir()->coefficient(), 'f', _dec)
           << QString::number(gabionDam->getWeir()->waterHeight(), 'f', _dec)
           << QString::number(gabionDam->getWeir()->wallsHeight() - gabionDam->getWeir()->waterHeight(), 'f', _dec)
           << QString::number(gabionDam->getWeir()->levels(), 'f', 0);
    units << ""
          << _lengthUnit
          << _lengthUnit
          << ""
          << _lengthUnit
          << _lengthUnit
          << "";

    for (int i=0; i < properties.size(); i++) {
        QString strLine = QString(properties.at(i)).arg(values.at(i)).arg(units.at(i));
        _strWeir += strLine;
        QStringList lineParts = strLine.split('\t');

        // Update the properties dock panel
        _properties << lineParts.at(0).trimmed();
        _values << lineParts.at(1).trimmed();
        if (lineParts.length() == 4)
            _units << lineParts.at(2).trimmed() + lineParts.at(3).trimmed();
        else
            _units << lineParts.at(2).trimmed();
    }
    //updateUiProperties();
}

void MainWindow::updatePanelAndTextResults()
{
    /*
     * Update panel and text results
     *
     * Updates the properties in the dock panel and the string containing the results and data
     * of stability analysis. Both operations were performed together because they share the
     * same information sources and in order to have the same headers.
     *
     */

    // Get the conditions as boolean text
    QString textSliding = stability->slidingCondition() ? "true" : "false";
    QString textCentral = stability->centralCondition() ? "true" : "false";
    QString textOverturn = stability->overturnCondition() ? "true" : "false";
    QString textThird = stability->thirdCondition() ? "true" : "false";


    QStringList properties;
    QStringList values;
    QStringList units;

    // These headers will be shared between text report and dock panel
    properties << tr("Dimensions\t%1\t%2\n")
               << tr("Base length (B):\t%1\t%2\n")
               << tr("Effective height (H):\t%1\t%2\n")
               << tr("Crest of the dam (b):\t%1\t%2\n")
               << tr("Water height (h'):\t%1\t%2\n")
               << tr("Unitary section volume (V):\t%1\t%2\n")
               << tr("Dam total volume (Vt):\t%1\t%2\n")
               << tr("Wet area (S):\t%1\t%2\n")
               << tr("Centroid X:\t%1\t%2\n")
               << tr("Centroid Y:\t%1\t%2\n")
               << tr("Centroid Z:\t%1\t%2\n")
               << tr("\nForces\t%1\t%2\n")
               << tr("Weight of dam's section (P):\t%1\t%2\n")
               << tr("Nappe weight (q):\t%1\t%2\n")
               << tr("Hydrostatic pressure (E):\t%1\t%2\n")
               << tr("Dam's weight lever (Zp):\t%1\t%2\n")
               << tr("Nappe lever (Xq):\t%1\t%2\n")
               << tr("Hydrostatic lever (XE):\t%1\t%2\n")
               << tr("Front dam's weight lever (Xp):\t%1\t%2\n")
               << tr("\nParameters\t%1\t%2\n")
               << tr("Spec. gravity water-sediment:\t%1\t%2\n")
               << tr("Specific gravity stone (delta):\t%1\t%2\n")
               << tr("Apparent specific gravity:\t%1\t%2\n")
               << tr("Friction factor (f):\t%1\t%2\n")
               << tr("Security factor:\t%1\t%2\n")
               << tr("\nStability conditions\t%1\t%2\n")
               << tr("Sliding condition:\t%1 :\t%2\n")
               << tr("Overturning condition:\t%1 :\t%2\n")
               << tr("Middle third condition:\t%1 :\t%2\n")
               << tr("Resulting forces:\t%1 :\t%2\n");

    values << ""
           << QString::number(stability->damBase(), 'f', _dec)
           << QString::number(stability->effectiveHeight(), 'f', _dec)
           << QString::number(stability->damCrest(), 'f', _dec)
           << QString::number(gabionDam->getWeirWaterHeight(), 'f', _dec)
           << QString::number(stability->damSectionVolume(), 'f', _dec)
           << QString::number(gabionDam->getDamVolume(), 'f', _dec)
           << QString::number(stability->wetArea(), 'f', _dec)
           << QString::number(gabionDam->getCentroidx(), 'f', _dec)
           << QString::number(gabionDam->getCentroidy(), 'f', _dec)
           << QString::number(gabionDam->getCentroidz(), 'f', _dec)
           << ""
           << QString::number(stability->damWeight(), 'f', _dec)
           << QString::number(stability->nappeWeight(), 'f', _dec)
           << QString::number(stability->hydrostatic(), 'f', _dec)
           << QString::number(stability->damLever(), 'f', _dec)
           << QString::number(stability->nappeLever(), 'f', _dec)
           << QString::number(stability->hydrostaticLever(), 'f', _dec)
           << QString::number(stability->frontLever(), 'f', _dec)
           << ""
           << QString::number(stability->sgWater(), 'f', _dec)
           << QString::number(stability->sgStone(), 'f', _dec)
           << QString::number(stability->sgStone()-stability->sgWater(), 'f', _dec)
           << QString::number(stability->friction(), 'f', _dec)
           << QString::number(stability->safetyFactor(), 'f', _dec)
           << ""
           << QString::number(stability->totalSliding(), 'f', _dec) + "\t> " + QString::number(stability->safetyFactor(), 'f', _dec)
           << QString::number(stability->totalOverturning(), 'f', _dec) + "\t> " + QString::number(stability->safetyFactor(), 'f', _dec)
           << QString::number(stability->middleThird(), 'f', _dec) + " " + _lengthUnit + "\t> " + QString::number(stability->totalDisplacement(), 'f', _dec) + " " + _lengthUnit
           << QString::number(stability->thirdForces(), 'f', _dec) + " " + _weightUnit + "\t> " + QString::number(stability->resultingForces(), 'f', _dec) + " " + _weightUnit;

    units << ""
          << _lengthUnit
          << _lengthUnit
          << _lengthUnit
          << _lengthUnit
          << _volumeUnit
          << _volumeUnit
          << _areaUnit
          << _lengthUnit
          << _lengthUnit
          << _lengthUnit
          << ""
          << _weightUnit
          << _weightUnit
          << _weightUnit
          << _lengthUnit
          << _lengthUnit
          << _lengthUnit
          << _lengthUnit
          << ""
          << _sgravUnit
          << _sgravUnit
          << _sgravUnit
          << ""
          << ""
          << ""
          << textSliding
          << textOverturn
          << textCentral
          << textThird;

    for (int i=0; i < properties.size(); i++) {
        QString strLine = QString(properties.at(i)).arg(values.at(i)).arg(units.at(i));
        _strResult += strLine;
        QStringList lineParts = strLine.split('\t');

        // Update the properties dock panel
        _properties << lineParts.at(0).trimmed();
        _values << lineParts.at(1).trimmed();
        if (lineParts.length() == 4)
            _units << lineParts.at(2).trimmed() + lineParts.at(3).trimmed();
        else
            _units << lineParts.at(2).trimmed();
    }
    //updateUiProperties();
}

void MainWindow::updateDockPanelProperties()
{
    initializeUiTreeView(); // Initialize the properties
    updatePanelCoordinates();
    updatePanelChannel();
    updatePanelFlow();
    updatePanelAndTextWeir();
    updatePanelAndTextResults();
    updateUiProperties(); // This will refresh the dock panel properties
}

bool MainWindow::plotPoints()
{
    /*!
     * \brief Plots a set of points in the canvas
     */

//    assert(channel->coordX().size() == channel->coordY().size());
    if ((channel.coordX().size() <= 0) || (channel.coordY().size() <= 0))
        return false;
    if (channel.coordX().size() != channel.coordY().size())
        return false;

    double pointSize = POINT_SIZE/_scale;

    /* Set Pen to preserve line width at zooming */
    QPen pen;
    pen.setCosmetic(true);

    // Plot all each point of the section

    for(size_t i = 0; i < channel.coordX().size(); i++) {
        double x = channel.coordX().at(i) - (pointSize / 2);
        double y = channel.coordY().at(i) - (pointSize / 2);
        mainScene->addEllipse(x, y, pointSize, pointSize, pen);
    }

    return true;
}

bool MainWindow::plotChannelSection()
{
    /*!
     * \brief Plots the stream or channel cross section
     */

    if ((channel.coordX().size() <= 0) || (channel.coordY().size() <= 0))
        return false;
    if (channel.coordX().size() != channel.coordY().size())
        return false;

    /* Set Pen to preserve line width at zooming */
    QPen pen;
    pen.setCosmetic(true);
    pen.setWidth(2);

    for (size_t i = 0; i < channel.coordX().size() - 1; i++) {
        mainScene->addLine(channel.coordX().at(i),   channel.coordY().at(i),
                           channel.coordX().at(i+1), channel.coordY().at(i+1), pen);
    }

    // TODO: measurements of the channel
//    plotChannelAnnotations();

    return true;
}

bool MainWindow::plotPointsAndChannel()
{
    /* Clear scene and view */
    mainScene->clear();
    ui->graphicsView->items().clear();

    /****** Plot everything ******/

    if (!plotPoints())
        return false;
    if (!plotChannelSection())
        return false;

    // Safely enable buttons
    enableNavigationButtons();

    return true;
}

bool MainWindow::plotAll()
{
    plotPointsAndChannel();
    plotDimensions();
    return true;
}

bool MainWindow::plotDimensions()
{
    if (dimensions.isEmpty())
        return false;

    vector<double> x_values;
    vector<double> y_values;

    // Plot the dimensions
    QPen pen;
    pen.setCosmetic(true); // Set Pen to preserve line width at zooming

    // Plot the front view
    for (int i=0; i < dimensions.size(); i++) {
        mainScene->addRect(dimensions.at(i).x(), dimensions.at(i).y(),
                           dimensions.at(i).width(), dimensions.at(i).height(), pen);
        x_values.push_back(dimensions.at(i).x() + dimensions.at(i).width());
        y_values.push_back(dimensions.at(i).y() + dimensions.at(i).height());
    }

    // Determine the x-axis origin that the lateral view should have, for the draw to be in the
    // right side of the front view
    double offset = 5;
    double x_orig = utilities::maxValueFrom(x_values) + offset;

    // Determine the y-axis origin that the top view should have for the draw to be in the
    // bottom of the front view
    double y_orig = utilities::maxValueFrom(y_values) + offset;

    // Plot the lateral and top view
    for (int i=0; i < dimensions.size(); i++) {
        // Layers in lateral view
        mainScene->addRect(x_orig, dimensions.at(i).y(),
                           dimensions.at(i).length(), dimensions.at(i).height(), pen);
        // Layers in top view
        mainScene->addRect(dimensions.at(i).x(), y_orig,
                           dimensions.at(i).width(), dimensions.at(i).length(), pen);
    }

    // Plot the water height over the weir
    QBrush brush(Qt::blue);
    double x_weir = dimensions.at(gabionDam->getSpillwayBottomLayer()).x() + dimensions.at(gabionDam->getSpillwayBottomLayer()).width();
    double y_weir = dimensions.at(gabionDam->getSpillwayBottomLayer()).y() + dimensions.at(gabionDam->getSpillwayBottomLayer()).height() - gabionDam->getWeir()->waterHeight();
    mainScene->addRect(x_weir, y_weir, gabionDam->getWeir()->width(), gabionDam->getWeir()->waterHeight(), pen, brush);
    qDebug() << " Spillway bottom layer: " << gabionDam->getSpillwayBottomLayer() << ", x: " << x_weir << ", y: " << y_weir;

    return true;
}

void MainWindow::horizontalDimension(double xini, double xend, double y, double offset, bool top)
{
    /*
     * Create an horizontal dimension
     *
     */

    double ybot, ytop;
    double ymid = y;
    double xmid = abs(xend - xini) / 2.0;
    QString label = QString::number(abs(xend - xini), 'f', _dec);

    // Define the y-axis coordinate
    if (top) {
        ymid = y - (offset * 2);  // As y-axis is inverted, we have to substract, dimension will be at the top
    } else {
        ymid = y + (offset * 2);  // Otherwise we have to add, dimension will be at bottom
    }
    ybot = ymid + offset;
    ytop = ymid - offset;

    QPen pen;
    pen.setCosmetic(true);

    QFont font;
    font.setPointSizeF(0.01);

    // Set the dimension label
    QGraphicsTextItem *dimLbl = new QGraphicsTextItem;
    dimLbl->setFont(font);
    dimLbl->setPos(xini+xmid, ymid);
    dimLbl->setPlainText(label);
    mainScene->addItem(dimLbl);

    // First half of main line
    mainScene->addLine(xini, ymid, xini + xmid - offset, ymid, pen);

    // Second half of main line
    mainScene->addLine(xini + xmid + offset, ymid, xend, ymid, pen);

    // Start vertical line
    mainScene->addLine(xini, ybot, xini, ytop, pen);

    // End vertical line
    mainScene->addLine(xend, ybot, xend, ytop, pen);
}

void MainWindow::verticalDimension(double yini, double yend, double x, double offset, bool right)
{
    /*
     * Create a vertical dimension
     *
     */
    double xleft, xright;
    double xmid = x;
    double ymid = abs(yend - yini) / 2.0;

    // Define the x-axis coordinate
    if (right) {
        xmid = x + (offset * 2);
    } else {
        xmid = x - (offset * 2);
    }
    xleft = xmid - offset;
    xright = xmid + offset;

    QPen pen;
    pen.setCosmetic(true);

    QFont font;
    font.setPointSize(1);

    // First half of main line
    mainScene->addLine(xmid, yini, xmid, yini + ymid - offset, pen);

    // Second half of main line
    mainScene->addLine(xmid, yini + ymid + offset, xmid, yend, pen);
}

void MainWindow::plotChannelAnnotations()
{
    // Channel width annotation
    // WARNING: This assumes the list of coordinates is sorted!
    double xini = channel.coordX().at(0);
    double xend = channel.coordX().at(channel.coordX().size()-1);
    double yini = channel.coordY().at(0);
    double yend = yini + utilities::maxValueFrom(channel.coordY());

    horizontalDimension(xini, xend, yini, 0.4, true);

    // Channel height annotation
    qDebug() << "xini: " << xini << " yini: " << yini << " xend: " << xend << " yend: " << yend;
    verticalDimension(yini, yend, xini, 0.4, false);
}

void MainWindow::showResults(QString str, QString title)
{
    AnalysisResultsDialog resultsDlg(str, title);
    resultsDlg.exec();
}

void MainWindow::showDimensions()
{
    dimenDlg.exec();
}

void MainWindow::showStabilityAnalysis()
{
    AnalysisResultsDialog resultsDlg(_strResult, "Stability Analysis Results");
    resultsDlg.exec();
}

void MainWindow::exportToDXF()
{
    QString dxfFile = QFileDialog::getSaveFileName(this, tr("Save DXF file"), QDir::homePath(), tr("DXF (*.dxf)"));
    qDebug() << "DXF File: " << dxfFile;

    // Create the DXF file object
    DxfFiles dxf(dxfFile.toStdString(), 1, -1);   // Set the DXF file name, x-axis scale and y-axis scale
    dxf.setX(channel.coordX());                   // Set the channel cross section coordinates,
    dxf.setY(channel.coordY());                   // x-axis and y-axis as different vectors
    dxf.setDimensions(dimensions.toStdVector());  // Set the gabion dam dimensions
    dxf.writeDxfFile();                           // Create the DXF file
}

void MainWindow::showReport()
{
    // Creates the general report of the design process
    createTextReport();

    // Opens a dialog to show the final report
    ReportDialog reportDlg(this, _strGeneralReport);
    reportDlg.exec();
}

void MainWindow::editLayers()
{
    /*
     * Allows the user to modify the layers' dimensions.
     *
     * NOTE: The dimensions vector should be converted from QVector to std::vector at the time passing the
     * argument to the dialog (constructor take care of this). Otherwise the EditLayersDialog could not modify
     * the dimensions. When returned the dimensions vector should be converted back to QVector.
     *
     */

    EditLayersDialog *EditLayersDlg = new EditLayersDialog(dimensions, this);
    if (EditLayersDlg->exec() == QDialog::Accepted) {

        int ans;
        QMessageBox msgBox;
        msgBox.setText(tr("This feature is HIGHLY EXPERIMENTAL and it's not recommended to modify dimensions by hand. Do you really wish to continue?"));
        msgBox.setWindowTitle(tr("Action not recommended"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setInformativeText(tr("Any correspondence among important values (such as abutment, layers, weir, channel, gabion height) will break! The results values will be invalid and unusable!"));
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::No);
        ans = msgBox.exec();

        switch (ans) {
        case QMessageBox::Yes: {
            // ****** This will replace the dimensions with the modifications ******

            // Update the dimensions dialog
            dimensions = QVector<Layer>::fromStdVector(EditLayersDlg->dimensions());
            dimenDlg.setLayers(&dimensions);
            dimenDlg.configureTable();

            // This will break any correspondence among important values (such as abutment, layers, weir, channel, gabion height)
            gabionDam->setLayers(EditLayersDlg->dimensions());
            gabionDam->forceUpdateDimensions();

            // Perform stability analysis and draw the dimensions again
            plotAll();
            performStabilityAnalysis();

            // ****** Update the properties dock panel ******

            updateDockPanelProperties();

            showResults(_strResult, "Stability Analysis Results");

            statusBar()->showMessage(tr("Dam's dimensions were modified by user. Results values may be INVALID by now!."));
        }
            break;
        case QMessageBox::No: {
            statusBar()->showMessage(tr("Dam's dimensions were not modified."));
        }
            break;
        default:
            // Should never be reached!
            break;
        }

    } else {
        statusBar()->showMessage(tr("Dam's dimensions were not modified."));
    }
}

void MainWindow::restartProject()
{
    /*
     * Restart or initialize the global project values such as
     *
     * flowrate
     * Weir
     * GabionDam
     * Stability
     *
     */
    initializeStrings();

    // Clear any old dimensions
    dimensions.clear();

    tempWeir = new Weir();
    gabionDam = new GabionDam();
    stability = new OropezaAnalysis();

    qDebug() << "========== RESTARTING PROJECT ==========";
    qDebug() << " Gabion height:   " << channel.layersHeight();
    qDebug() << " Weir width:      " << tempWeir->width();
    qDebug() << " Weir coefficient:" << tempWeir->coefficient();
    qDebug() << " Basin length:    " << gabionDam->getBasinLen();
    qDebug() << " Abutment:        " << gabionDam->getAbutment();
    qDebug() << " Step length:     " << gabionDam->getStepLen();
    qDebug() << " Step:            " << gabionDam->getStep();
    qDebug() << " SG Water:        " << stability->sgWater();
    qDebug() << " SG Stone:        " << stability->sgStone();
    qDebug() << " Friction:        " << stability->friction();
    qDebug() << "========== RESTARTING PROJECT ==========";

    // Initial values
    _flowrate = 0;

    //mainScene->clear();
    initializeUiTreeView();

    statusBar()->showMessage(tr("Ready."));
}

void MainWindow::initializeStrings()
{
    _dec = 2;
    _lengthUnit = "m";
    _flowUnit = "m^3/s";
    _volumeUnit = "m^3";
    _weightUnit = "t";
    _areaUnit = "m^2";
    _sgravUnit = "t/m^3";

    _strChannel = "";
    _strFlow = "";
    _strRational = "";
    _strManning = "";
    _strWeir = "";
    _strResult = "";
    _strDimensions = "";
    _strGeneralReport = "";
}

QVector<QPointF> MainWindow::convertPoints(QList<QStringList> &stringPoints)
{
    /*!
     * \brief numPoints Convert points from strings list to a double vector
     */

    // TODO: Should check the values first!

    QVector<QPointF> numPoints;
    for (int i = 0; i < stringPoints.length(); i++) {
        QPointF point;
        point.setX(stringPoints.at(i).at(0).toDouble());
        point.setY(stringPoints.at(i).at(1).toDouble());
        numPoints.append(point);
    }
    return numPoints;
}
