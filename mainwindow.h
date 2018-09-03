#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QStatusBar>
#include <QLabel>
#include <QGraphicsView>
#include <QDockWidget>

#include "csvdata.h"
#include "utilities.h"
#include "gabiondam.h"
#include "oropezaanalysis.h"
#include "dimensiondialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    int _dec;
    qreal _scale;

    QString _programVersion;

    GabionDam *gabionDam;
    ChannelSection channel;
    OropezaAnalysis *stability;
    Weir *tempWeir;

    DimensionDialog dimenDlg;

    QVector<Layer> dimensions;

    QVector<QPointF> points;
    QGraphicsScene* mainScene;

    QStandardItemModel *model;
    QStringList _properties;
    QStringList _values;
    QStringList _units;

    QLabel *widgetScale;
    QLabel *widgetCoordinates;
    QLabel *widgetUnits;

    QString _lengthUnit;
    QString _flowUnit;
    QString _volumeUnit;
    QString _weightUnit;
    QString _areaUnit;
    QString _sgravUnit;

    QString _strChannel;
    QString _strFlow;
    QString _strRational;
    QString _strManning;
    QString _strWeir;
    QString _strResult;
    QString _strDimensions;
    QString _strGeneralReport;

    double _flowrate;
    int _flowMethod;
    QString _strMethod;

private slots:
    void about();
    void help();
    void createStatusBar();
    void updateCoordinates();
    void openData();
    void openCsvFile();
    void prepareDesign(std::vector<std::vector<string> > &values);
    void setupChannelData(QList<QStringList> values);
    void simpleDesign();
    void performStabilityAnalysis();
    void createTextReport();
    void initializeUiTreeView();
    void updateUiProperties();
    void updatePanelCoordinates();
    void updatePanelChannel();
    void updatePanelFlow();
    void updatePanelAndTextWeir();
    void updatePanelAndTextResults();
    void updateDockPanelProperties();
    bool plotPoints();
    bool plotChannelSection();
    bool plotPointsAndChannel();
    bool plotAll();
    bool plotDimensions();
    void horizontalDimension(double xini, double xend, double y, double offset=0.5, bool top=true);
    void verticalDimension(double yini, double yend, double x, double offset=0.5, bool right=true);
    void plotChannelAnnotations();
    void showResults(QString str="", QString title="");
    void showDimensions();
    void showStabilityAnalysis();
    void exportToDXF();
    void showReport();
    void editLayers();
    void restartProject();
    void initializeStrings();

    void zoom(double scaleFactor);
    void zoomIn();
    void zoomOut();
    void zoomOriginal();
    void zoomBestFit();
    void pan(bool panMode);
    void disableNavigationButtons();
    void enableNavigationButtons();

    void pointsChangedWarning();

    QList<QStringList> convertData(std::vector<std::vector<std::string> > &values);
    QVector<QPointF> convertPoints(QList<QStringList> &stringPoints);

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // MAINWINDOW_H
