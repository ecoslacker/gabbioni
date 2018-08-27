#include "dimensiondialog.h"
#include "ui_dimensiondialog.h"

#include <QTableWidgetItem>
#include <QDebug>

DimensionDialog::DimensionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DimensionDialog)
{
    ui->setupUi(this);
}

DimensionDialog::~DimensionDialog()
{
    delete ui;
}

QVector<Layer> *DimensionDialog::getLayers() const
{
    return layers;
}

void DimensionDialog::setLayers(QVector<Layer> *value)
{
    layers = value;
}

void DimensionDialog::configureTable()
{
    ui->tableWidget->setRowCount(layers->size());
    for (int i = 0; i < layers->size(); i++) {
        double l = layers->at(i).length();
        double w = layers->at(i).width();
        double h = layers->at(i).height();
        double x = layers->at(i).x();
        double y = layers->at(i).y();

//        qDebug() << "Length:  " << l;
//        qDebug() << "Width:   " << w;
//        qDebug() << "Height:  " << h;
//        qDebug() << "Coord X: " << x;
//        qDebug() << "Coord Y: " << y;

        QTableWidgetItem *litem = new QTableWidgetItem(QString::number(l, 'f', 2));
        litem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        litem->setFlags(litem->flags() &  ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 0, litem);

        QTableWidgetItem *witem = new QTableWidgetItem(QString::number(w, 'f', 2));
        witem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        witem->setFlags(witem->flags() &  ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 1, witem);

        QTableWidgetItem *hitem = new QTableWidgetItem(QString::number(h, 'f', 2));
        hitem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        hitem->setFlags(hitem->flags() &  ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 2, hitem);

        QTableWidgetItem *xitem = new QTableWidgetItem(QString::number(x, 'f', 2));
        xitem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        xitem->setFlags(xitem->flags() &  ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 3, xitem);

        QTableWidgetItem *yitem = new QTableWidgetItem(QString::number(y, 'f', 2));
        yitem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        yitem->setFlags(yitem->flags() &  ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 4, yitem);

    }
}
